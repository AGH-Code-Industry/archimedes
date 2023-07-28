/*
#include <net/async/host.hpp>
#include <net/__net_init.hpp>
#include <net/host.hpp>

namespace arch::net::async {
	Host::Host(IPv4 ip) {
		_ips[0] = ip;
	}

	Host Host::localhost() {
		return Host(IPv4::localhost);
	}
	std::future<Host> Host::localhost(bool update, timeout_t timeout) {
		if (update) {
			return std::async(std::launch::async, [t = timeout]()->Host {
				Host host = localhost();
				auto update_future = host.update(t);
				update_future.wait();
				return host;
			});
		}
		else {
			return std::async(std::launch::async, []()->Host {
				return localhost();
			});
		}
	}

	std::future<Host> Host::from_ip(IPv4 ip, bool update, timeout_t timeout) {
		if (update) {
			return std::async(std::launch::async, [_ip = ip, t = timeout]()->Host {
				Host host{_ip};
				auto update_future = host.update(t);
				update_future.wait();
				return host;
			});
		}
		else {
			return std::async(std::launch::async, [_ip = ip]()->Host {
				return Host(_ip);
			});
		}
	}
	std::future<Host> Host::from_hostname(std::string_view hostname, timeout_t timeout) {
		return std::async(std::launch::async, [h = hostname, t = timeout]()->Host {
			Host host;
			host._hostname = h;
			auto update_future = host.update(t);
			update_future.wait();
			return host;
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
	std::future<Host::update_result> Host::update(timeout_t timeout) {
		return std::async(std::launch::async, [t = std::chrono::milliseconds(timeout), this]()->Host::update_result {
			std::mutex m;
			std::condition_variable cv;
			Host::update_result _result = Host::update_result::none;

			std::jthread update_thread([&, this]() {
				if (not __net_auto.initialized) {
					// log error or smth
					{
						std::lock_guard<std::mutex> l(m);
						_result = Host::update_result::failure;
					}
					cv.notify_one();
					return;
				}

				std::string node_name;
				node_name.reserve(1025);
				memset(node_name.data(), 0, 1025);

				if (not _hostname.empty()) {
					node_name = _hostname;
				}
				else if (_ips[0] == IPv4::localhost) {
					char hostname[NI_MAXHOST]{};
					gethostname(hostname, NI_MAXHOST);
					_hostname = hostname;
					node_name = _hostname;
				}
				else {
					sockaddr_in sa;
					memset(&sa, 0, sizeof(sa));
					sa.sin_addr = _ips[0];
					sa.sin_family = AF_INET;
					sa.sin_port = htons(0);

					char hostname[NI_MAXHOST];
					char serv_info[NI_MAXSERV];

					int result = getnameinfo((sockaddr*)&sa, sizeof(sockaddr), hostname, NI_MAXHOST, serv_info, NI_MAXSERV, 0);
					if (result != 0) {
						// log error or smth WSAGetLastError()
						{
							std::lock_guard<std::mutex> l(m);
							_result = Host::update_result::failure;
						}
						cv.notify_one();
						return;
					}
					else {
						_hostname = hostname;
					}

					node_name = _ips[0].str();
				}

				static addrinfo hints;
				memset(&hints, 0, sizeof(hints));
				hints.ai_family = AF_INET;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;

				addrinfo* data;

				int result = getaddrinfo(node_name.c_str(), nullptr, &hints, &data);

				if (result != 0) {
					// log error or smth gai_strerror(result)
					{
						std::lock_guard<std::mutex> l(m);
						_result = Host::update_result::failure;
					}
					cv.notify_one();
					return;
				}

				for (auto i = data; i != nullptr; i = i->ai_next) {
					if (((sockaddr_in*)i->ai_addr)->sin_addr != _ips[0]) {
						_ips.emplace_back(((sockaddr_in*)i->ai_addr)->sin_addr);
					}
				}

				freeaddrinfo(data);

				{
					std::lock_guard<std::mutex> l(m);
					_result = Host::update_result::success;
				}
				cv.notify_one();
				return;
			});

			std::unique_lock<std::mutex> lock(m);
			auto wres = cv.wait_for(lock, t);
			if (wres == std::cv_status::timeout) {
				return Host::update_result::timeout;
			}
			else {
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
}*/