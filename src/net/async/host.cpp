#include <net/async/Host.h>
#include <net/Host.h>
#include <net/Exception.h>
#include <thread>

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
std::future<Host::FromResult> Host::localhost(bool update, TimeoutMs timeout) {
	if (update) {
		return std::async(std::launch::async, [t = timeout]() -> Host::FromResult {
			Host host = localhost();
			auto updateFuture = host.update(t);
			auto updateResult = updateFuture.get();
			return {host, updateResult};
		});
	}
	else {
		return std::async(std::launch::async, []() -> Host::FromResult {
			return {localhost(), UpdateResult::none};
		});
	}
}

std::future<Host::FromResult> Host::fromIp(IPv4 ip, bool update, TimeoutMs timeout) {
	if (update) {
		return std::async(std::launch::async, [_ip = ip, t = timeout]() -> Host::FromResult {
			Host host{_ip};
			auto updateFuture = host.update(t);
			auto updateResult = updateFuture.get();
			return {host, updateResult};
		});
	}
	else {
		return std::async(std::launch::async, [_ip = ip]() -> Host::FromResult {
			return {Host(_ip), UpdateResult::none};
		});
	}
}
std::future<Host::FromResult> Host::fromHostname(std::string_view hostname, TimeoutMs timeout) {
	return std::async(std::launch::async, [h = hostname, t = timeout]() -> Host::FromResult {
		Host host;
		host._hostname = h;
		auto update_future = host.update(t);
		auto update_result = update_future.get();
		return {host, UpdateResult::none};
	});
}

const IPv4& Host::ip() const {
	return _ips[0];
}
const std::vector<IPv4>& Host::ips() const {
	return _ips;
}
bool Host::hasIp(IPv4 address) const {
	for (auto&& ip : _ips) if (ip == address) {
		return true;
	}

	return false;
}
const std::string& Host::hostname() const {
	return _hostname;
}
std::string Host::_updateHostname() {
	std::string nodeName;
	nodeName.reserve(1025);
	memset(nodeName.data(), 0, 1025);

	if (not _hostname.empty()) { // hostname avalible
		nodeName = _hostname;
	}
	else if (_ips[0] == IPv4::localhost) {
		char hostname[NI_MAXHOST]{};
		gethostname(hostname, NI_MAXHOST);
		_hostname = hostname;
		nodeName = _hostname;
	}
	else { // update hostname
		sockaddr_in sa;
		memset(&sa, 0, sizeof(sa));
		sa.sin_addr = _ips[0];
		sa.sin_family = AF_INET;
		sa.sin_port = htons(0);

		char hostname[NI_MAXHOST]{};
		static char servInfo[NI_MAXSERV];

		int result = getnameinfo((sockaddr*)&sa, sizeof(sockaddr), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, 0);
		if (result != 0) {
			throw NetException(gai_strerror(net_errno(result)));
		}
		else {
			_hostname = hostname;
		}

		nodeName = _ips[0].str();
	}

	return nodeName;
}
std::future<Host::UpdateResult> Host::update(TimeoutMs timeout) {
	return std::async(std::launch::async, [t = std::chrono::milliseconds(timeout), this]() mutable -> Host::UpdateResult {
		std::unique_lock update_lock(_updateMutex, std::defer_lock);
		auto timeNow = std::chrono::high_resolution_clock::now();
		if (not update_lock.try_lock_for(t)) {
			return UpdateResult::timeout;
		}

		t -= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timeNow);

		std::mutex m;
		std::condition_variable cv;
		Host::UpdateResult updateResult = Host::UpdateResult::none;
		std::exception_ptr exception;

		std::jthread update_thread([&, this]() {
			std::string nodeName;

			// transports exception back to async
			try {
				nodeName = _updateHostname();
			}
			catch (...) {
				exception = std::current_exception();
				cv.notify_one();
				return;
			}
				
			addrinfo hints;
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			addrinfo* data;

			int result = getaddrinfo(nodeName.c_str(), nullptr, &hints, &data);

			if (result != 0) {
				updateResult = Host::UpdateResult::failure;

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

			updateResult = Host::UpdateResult::success;
			cv.notify_one();
			return;
		});

		std::unique_lock<std::mutex> lock(m);
		auto wres = cv.wait_for(lock, t);
		if (wres == std::cv_status::timeout) {
			return Host::UpdateResult::timeout;
		}
		else {
			if (exception) { // rethrows exception from update_thread
				std::rethrow_exception(exception);
			}
			return updateResult;
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