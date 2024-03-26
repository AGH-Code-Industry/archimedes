#include <thread>

#include <net/Host.h>
#include <net/NetException.h>
#include <net/async/Host.h>

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
		return std::async(std::launch::async, [t = timeout]() -> FromResult {
			Host host = localhost();
			auto updateFuture = host.update(t);
			auto updateResult = updateFuture.get();
			return {host, updateResult};
		});
	}
	return std::async(std::launch::async, []() -> FromResult { return {localhost(), UpdateResult::none}; });
}

std::future<Host::FromResult> Host::fromIp(IPv4 ip, bool update, TimeoutMs timeout) {
	if (update) {
		return std::async(std::launch::async, [ip = ip, t = timeout]() -> FromResult {
			Host host{ip};
			auto updateFuture = host.update(t);
			auto updateResult = updateFuture.get();
			return {host, updateResult};
		});
	}
	return std::async(std::launch::async, [ip = ip]() -> FromResult { return {Host(ip), UpdateResult::none}; });
}

std::future<Host::FromResult> Host::fromHostname(std::string_view hostname, TimeoutMs timeout) {
	return std::async(std::launch::async, [h = hostname, t = timeout]() -> FromResult {
		Host host;
		host._hostname = h;
		auto updateFuture = host.update(t);
		[[maybe_unused]] auto updateResult = updateFuture.get();
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
	for (auto&& ip : _ips) {
		if (ip == address) {
			return true;
		}
	}

	return false;
}

const std::string& Host::hostname() const {
	return _hostname;
}

std::string Host::_updateHostname() {
	std::string nodeName;
	nodeName.reserve(1'025);
	memset(nodeName.data(), 0, 1'025);

	if (not _hostname.empty()) { // hostname avalible
		nodeName = _hostname;
	} else if (_ips[0] == IPv4::localhost) {
		char hostname[NI_MAXHOST]{};
		gethostname(hostname, NI_MAXHOST);
		_hostname = hostname;
		nodeName = _hostname;
	} else { // update hostname
		sockaddr_in sa = {};
		sa.sin_addr = _ips[0];
		sa.sin_family = AF_INET;
		sa.sin_port = htons(0);

		char hostname[NI_MAXHOST]{};
		static char servInfo[NI_MAXSERV];

		int result = getnameinfo((sockaddr*)&sa, sizeof(sockaddr), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, 0);
		if (result != 0) {
			throw NetException(gai_strerror(netErrno(result)));
		}
		_hostname = hostname;

		nodeName = _ips[0].str();
	}

	return nodeName;
}

std::future<Host::UpdateResult> Host::update(TimeoutMs timeout) {
	return std::async(std::launch::async, [t = std::chrono::milliseconds(timeout), this]() mutable -> UpdateResult {
		std::unique_lock updateLock(_updateMutex, std::defer_lock);
		auto timeNow = std::chrono::high_resolution_clock::now();
		if (not updateLock.try_lock_for(t)) {
			return UpdateResult::timeout;
		}

		t -= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timeNow);

		std::mutex m;
		std::condition_variable cv;
		UpdateResult updateResult = UpdateResult::none;
		std::exception_ptr exception;

		std::jthread updateThread([&, this]() {
			std::string nodeName;

			// transports exception back to async
			try {
				nodeName = _updateHostname();
			} catch (...) {
				exception = std::current_exception();
				cv.notify_one();
				return;
			}

			addrinfo hints = {};
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			addrinfo* data;

			int result = getaddrinfo(nodeName.c_str(), nullptr, &hints, &data);

			if (result != 0) {
				updateResult = UpdateResult::failure;

				// transports exception back to async
				try {
					throw NetException(gai_strerror(netErrno(result)));
				} catch (...) {
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

			updateResult = UpdateResult::success;
			cv.notify_one();
		});

		std::unique_lock lock(m);
		auto wres = cv.wait_for(lock, t);
		if (wres == std::cv_status::timeout) {
			return UpdateResult::timeout;
		}
		if (exception) { // rethrows exception from update_thread
			std::rethrow_exception(exception);
		}
		return updateResult;
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

} // namespace arch::net::async
