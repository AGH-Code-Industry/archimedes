#include <net/async/host.hpp>
#include <net/host.hpp>
#include <net/exception.hpp>

namespace arch::net::async {
Host::Host(IPv4 ip) {
	_ips[0] = ip;
}
Host::Host(const Host& other) {
	this->_hostname = other._hostname;
	this->_ips = other._ips;
}

Host Host::localhost() {
	return Host(IPv4::localhost);
}
std::future<std::pair<Host, Host::update_result>> Host::localhost(bool update, timeout_t timeout) {
	if (update) {
		return std::async(std::launch::async, [t = timeout]()->std::pair<Host, Host::update_result> {
			Host host = localhost();
			auto update_future = host.update(t);
			auto update_result = update_future.get();
			return std::make_pair(host, update_result);
		});
	}
	else {
		return std::async(std::launch::async, []()->std::pair<Host, Host::update_result> {
			return std::make_pair(localhost(), update_result::none);
		});
	}
}

std::future<std::pair<Host, Host::update_result>> Host::from_ip(IPv4 ip, bool update, timeout_t timeout) {
	if (update) {
		return std::async(std::launch::async, [_ip = ip, t = timeout]()->std::pair<Host, Host::update_result> {
			Host host{_ip};
			auto update_future = host.update(t);
			auto update_result = update_future.get();
			return std::make_pair(host, update_result);
		});
	}
	else {
		return std::async(std::launch::async, [_ip = ip]()->std::pair<Host, Host::update_result> {
			return std::make_pair(Host(_ip), update_result::none);
		});
	}
}
std::future<std::pair<Host, Host::update_result>> Host::from_hostname(std::string_view hostname, timeout_t timeout) {
	return std::async(std::launch::async, [h = hostname, t = timeout]()->std::pair<Host, Host::update_result> {
		Host host;
		host._hostname = h;
		auto update_future = host.update(t);
		auto update_result = update_future.get();
		return std::make_pair(host, update_result::none);
	});
}

const IPv4& Host::ip() const {
	return _ips[0];
}
const std::vector<IPv4>& Host::ips() const {
	return _ips;
}
bool Host::has_ip(IPv4 address) const {
	for (auto&& ip : _ips) if (ip == address) {
		return true;
	}

	return false;
}
const std::string& Host::hostname() const {
	return _hostname;
}
std::string Host::_update_hostname() {
	std::string node_name;
	node_name.reserve(1025);
	memset(node_name.data(), 0, 1025);

	if (not _hostname.empty()) { // hostname avalible
		node_name = _hostname;
	}
	else if (_ips[0] == IPv4::localhost) {
		char hostname[NI_MAXHOST]{};
		gethostname(hostname, NI_MAXHOST);
		_hostname = hostname;
		node_name = _hostname;
	}
	else { // update hostname
		sockaddr_in sa;
		memset(&sa, 0, sizeof(sa));
		sa.sin_addr = _ips[0];
		sa.sin_family = AF_INET;
		sa.sin_port = htons(0);

		char hostname[NI_MAXHOST]{};
		char serv_info[NI_MAXSERV];

		int result = getnameinfo((sockaddr*)&sa, sizeof(sockaddr), hostname, NI_MAXHOST, serv_info, NI_MAXSERV, 0);
		if (result != 0) {
			throw NetException(gai_strerror(net_errno(result)));
		}
		else {
			_hostname = hostname;
		}

		node_name = _ips[0].str();
	}

	return node_name;
}
std::future<Host::update_result> Host::update(timeout_t timeout) {
	return std::async(std::launch::async, [t = std::chrono::milliseconds(timeout), this]()mutable->Host::update_result {
		std::unique_lock update_lock(_update_mutex, std::defer_lock);
		auto time_now = std::chrono::high_resolution_clock::now();
		if (not update_lock.try_lock_for(t)) {
			return update_result::timeout;
		}

		t -= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time_now);

		std::mutex m;
		std::condition_variable cv;
		Host::update_result _result = Host::update_result::none;
		std::exception_ptr exception;

		std::jthread update_thread([&, this]() {
			std::string node_name;

			// transports exception back to async
			try {
				node_name = _update_hostname();
			}
			catch (...) {
				exception = std::current_exception();
				cv.notify_one();
				return;
			}
				
			static addrinfo hints;
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			addrinfo* data;

			int result = getaddrinfo(node_name.c_str(), nullptr, &hints, &data);

			if (result != 0) {
				_result = Host::update_result::failure;

				// transports exception back to async
				try {
					throw NetException(gai_strerror(net_errno(result)));
				}
				catch (...) {
					exception = std::current_exception();
					cv.notify_one();
					return;
				}
			}

			for (auto i = data; i != nullptr; i = i->ai_next) {
				if (((sockaddr_in*)i->ai_addr)->sin_addr != _ips[0]) {
					_ips.emplace_back(((sockaddr_in*)i->ai_addr)->sin_addr);
				}
			}

			freeaddrinfo(data);

			_result = Host::update_result::success;
			cv.notify_one();
			return;
		});

		std::unique_lock<std::mutex> lock(m);
		auto wres = cv.wait_for(lock, t);
		if (wres == std::cv_status::timeout) {
			return Host::update_result::timeout;
		}
		else {
			if (exception) { // rethrows exception from update_thread
				std::rethrow_exception(exception);
			}
			return _result;
		}
	});
}

net::Host Host::sync() const {
	net::Host host;
	host._ips = this->_ips;
	host._hostname = this->_hostname;
	return host;
}

Host::Host() {
	_ips[0] = IPv4();
}
}