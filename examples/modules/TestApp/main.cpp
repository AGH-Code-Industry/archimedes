import archimedes;

using arch::utils::UUID;

#include <print>

int main() {
	std::println("dupa {}", UUID::random().toString());
}
