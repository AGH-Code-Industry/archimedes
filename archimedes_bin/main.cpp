#include <bitset>
#include <entt/entt.hpp>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <unordered_map>

#include <Ecs.h>
#include <Engine.h>
#include <Logger.h>
#include <TUtils.h>
#include <meta.h>
#include <net.h>

#pragma region iter_tests

struct iter1 {
	static inline constexpr size_t pageSize = 1'024;

	inline iter1(std::vector<std::vector<size_t>>* _vec, size_t _offset) noexcept: vec{ _vec }, offset{ _offset } {}

	inline iter1(std::vector<std::vector<size_t>>& x) noexcept: iter1(&x, 0) {}

	inline static iter1 begin(std::vector<std::vector<size_t>>& x) noexcept { return iter1(x); }

	inline static iter1 end(std::vector<std::vector<size_t>>& x) noexcept { return iter1(&x, 1'024 * x.size()); }

	inline size_t& operator*() noexcept { return (*vec)[offset >> 10][offset & ((size_t)1'024 - (size_t)1)]; }

	inline iter1& operator++() noexcept {
		++offset;
		return *this;
	}

	std::vector<std::vector<size_t>>* vec;
	size_t offset;
};

struct iter2 {
	static inline constexpr size_t pageSize = 1'024;

	inline iter2(std::vector<size_t>* _vec, size_t _offset) noexcept: vec{ _vec }, offset{ _offset } {}

	inline iter2(std::vector<std::vector<size_t>>& x) noexcept: iter2(&x.front(), 0) {}

	inline static iter2 begin(std::vector<std::vector<size_t>>& x) noexcept { return iter2(x); }

	inline static iter2 end(std::vector<std::vector<size_t>>& x) noexcept { return iter2(&x.back() + 1, 0); }

	inline size_t& operator*() noexcept { return (*vec)[offset]; }

	inline iter2& operator++() noexcept {
		/*++offset;
		if (offset & pageSize) {
			offset = 0;
		}*/
		//++offset;
		if ((++offset) == pageSize) {
			offset = 0;
		}
		if (not offset) {
			++vec;
		}
		return *this;
	}

	std::vector<size_t>* vec;
	size_t offset;
};

struct iter3 {
	static inline constexpr size_t pageSize = 1'024;

	inline iter3(std::vector<size_t>* _vec, size_t _offset) noexcept: vec{ _vec }, offset{ _offset } {}

	inline iter3(std::vector<std::vector<size_t>>& x) noexcept: iter3(&x.front(), 0) {}

	inline static iter3 begin(std::vector<std::vector<size_t>>& x) noexcept { return iter3(x); }

	inline static iter3 end(std::vector<std::vector<size_t>>& x) noexcept { return iter3(&x.back() + 1, 0); }

	inline size_t& operator*() noexcept { return (*vec)[offset]; }

	inline iter3& operator++() noexcept {
		offset = (offset + 1) & ((size_t)1'023);
		if (not offset) {
			++vec;
		}
		return *this;
	}

	std::vector<size_t>* vec;
	size_t offset;
};

inline bool operator==(const iter1& one, const iter1& other) noexcept {
	return one.vec == other.vec && one.offset == other.offset;
}

inline bool operator==(const iter2& one, const iter2& other) noexcept {
	return one.vec == other.vec && one.offset == other.offset;
}

inline bool operator==(const iter3& one, const iter3& other) noexcept {
	return one.vec == other.vec && one.offset == other.offset;
}

struct XD {
	// static inline constexpr bool in_place_delete = true;
	int x;

	XD() = default;
	XD(int _x): x{ _x } {};

	~XD() {
		//
	}

	XD(const XD& xd) {
		x = xd.x;
		std::cout << std::format("copy: {} -> {}\n", (size_t)&xd, (size_t)this);
	}

	XD(XD&& xd) {
		x = xd.x;
		xd.x = 0;
		std::cout << std::format("move: {} -> {}\n", (size_t)&xd, (size_t)this);
	}

	XD& operator=(const XD& other) {
		x = other.x;
		std::cout << std::format("copy: {} -> {}\n", (size_t)&other, (size_t)this);
		return *this;
	}

	XD& operator=(XD&& other) {
		x = other.x;
		other.x = 0;
		std::cout << std::format("move: {} -> {}\n", (size_t)&other, (size_t)this);
		return *this;
	}
};

struct XD2 {
	int x;

	XD2() = default;
	XD2(int _x): x{ _x } {};
	XD2(const XD2&) = default;
	XD2(XD2&&) = default;

	XD2& operator=(const XD2& other) {
		x = other.x;
		return *this;
	}

	XD2& operator=(XD2&& other) {
		x = other.x;
		other.x = 0;
		return *this;
	}
};

// using traits = entt::sparse_set::traits_type;

#pragma endregion

namespace arch::ecs {

enum e16 : u16;
enum e8 : u8;

template<>
struct EntitySpecs<e16>: AutoEntitySpecs<e16, u8, 12> {};

template<>
struct EntitySpecs<e8>: AutoEntitySpecs<e8, u8, 5> {};

} // namespace arch::ecs

struct NonMoveable {
	static inline constexpr bool in_place_delete = true;

	int val = 0;

	NonMoveable(){};
	NonMoveable(int v): val{ v } {};
	NonMoveable(const NonMoveable&) = default;
	NonMoveable(NonMoveable&&) = default;

	NonMoveable& operator=(int v) {
		val = v;
		return *this;
	}

	NonMoveable& operator=(const NonMoveable&) = default;
	NonMoveable& operator=(NonMoveable&&) = default;
};

struct Empty {};

struct NoUniqueAddress {
	int x;
	float d;
	size_t mogus;
	int sus;
	[[no_unique_address]] Empty empty;
	int xd3;
};

using clk = std::chrono::high_resolution_clock;

namespace ecs = arch::ecs;

struct Flag {
	// static inline constexpr bool inPlaceComponent = true;
	static inline constexpr bool flagComponent = true;
};

struct Flag2 {
	// static inline constexpr bool flagComponent = true;
};

// template<class E, class... Ts>
// bool hasAll(const ecs::Domain<E>& domain, const E entity, arch::TypeString<Ts...> = arch::TypeString{}) {
//	return (domain.hasComponent<Ts>(entity) && ...);
// }

#include <execution>

int main() {
	/*entt::registry{}.view<Flag, Flag2>().get((entt::entity)0);
	auto xd = entt::registry{}.view<Flag>();*/

	arch::Logger::init(arch::LogLevel::info);

	ecs::Domain<ecs::e32> domain;

	auto e0 = domain.newEntity();
	auto e1 = domain.newEntity();
	auto e2 = domain.newEntity();
	auto e3 = domain.newEntity();
	auto e4 = domain.newEntity();
	auto e5 = domain.newEntity();
	auto e6 = domain.newEntity();
	auto e7 = domain.newEntity();
	auto e8 = domain.newEntity();
	auto e9 = domain.newEntity();
	auto e10 = domain.newEntity();

	domain.addComponent<Flag>(e0);
	domain.addComponent<Flag>(e1);
	domain.addComponent<Flag>(e2);
	domain.addComponent<Flag>(e3);
	domain.addComponent<Flag>(e4);
	domain.addComponent<Flag>(e5);
	domain.addComponent<Flag2>(e6);
	domain.addComponent<Flag2>(e2);
	domain.addComponent<Flag>(e6);

	domain.kill(e3);

	auto view = domain.view<Flag, Flag2>();

	view.forEach([](auto&& entity, auto&& flag, auto&& flag2) {
		//
		std::cout << std::format("{}\n", (uint32_t)entity);
	});

#pragma region OldTests

	/*arch::ecs::EntityPool<arch::ecs::e8, 4> pool;
	using Traits = decltype(pool)::Traits;

	auto&& [sparse, dense] = pool.debug();

	for (auto i = 0; i != 10; ++i) {
		auto e = pool.newEntity();
	}

	auto e2 = Traits::Entity::fromParts(2, 0);

	pool.kill(e2);

	for (auto&& e : pool) {
		std::cout << std::format("{}|{}\n", Traits::Id::part(e), Traits::Version::part(e));
	}

	std::cout << '\n';

	for (auto&& e : *dense) {
		std::cout << std::format("{}|{}\n", Traits::Id::part(e), Traits::Version::part(e));
	}

	std::cout << '\n';

	std::cout << std::format("{}\n", pool.alive(Traits::Entity::fromParts(2, 1)));
	std::cout << std::format("{}\n", pool.alive(Traits::Entity::fromParts(2, 0)));
	std::cout << std::format("{}\n", pool.contains(2));

	std::cout << '\n';

	e2 = pool.recycleEntity(Traits::Entity::fromParts(2, 254));

	for (auto&& e : pool) {
		std::cout << std::format("{}|{}\n", Traits::Id::part(e), Traits::Version::part(e));
	}

	std::cout << '\n';

	std::cout << std::format("{}\n", pool.alive(e2));
	std::cout << std::format("{}\n", pool.alive(Traits::Entity::fromParts(2, 0)));
	std::cout << std::format("{}\n", pool.contains(2));*/

	/*entt::registry reg;

	auto e1 = reg.create();
	auto e2 = reg.create();
	auto e3 = reg.create();
	auto e4 = reg.create();

	reg.emplace<NonMoveable>(e1, 420);
	reg.emplace<NonMoveable>(e2, 1'337);
	reg.emplace<NonMoveable>(e3, 360);
	reg.emplace<NonMoveable>(e4, 69);

	reg.view<NonMoveable>().each([](const auto e, auto& c) { std::cout << c.val << '\n'; });

	reg.sort<NonMoveable>([](const NonMoveable& lhs, const NonMoveable& rks) { return lhs.val < rks.val; });
	reg.view<NonMoveable>().each([](const auto e, auto& c) { std::cout << c.val << '\n'; });*/

	/*entt::entity x = traits::construct(0, std::numeric_limits<uint16_t>::max() - 1);

	entt::sparse_set set{ entt::deletion_policy::swap_only };

	set.push(x);

	for (auto&& e : set) {
		std::cout << std::format("{}|{}\n", traits::to_entity(e), traits::to_version(e));
	}

	set.erase(x);

	std::cout << set.contains(x) << '\n';

	for (auto&& e : set) {
		std::cout << std::format("{}|{}\n", traits::to_entity(e), traits::to_version(e));
	}*/

	/*entt::sparse_set set{ entt::deletion_policy::swap_only };

	set.push(traits::construct(0, 0));*/

	/*entt::registry reg;

	auto e0 = reg.create();
	auto e1 = reg.create();
	auto e2 = reg.create();
	auto e3 = reg.create();
	auto e4 = reg.create();

	reg.emplace<XD>(e0, 0);
	reg.emplace<XD>(e1, 1);
	reg.emplace<XD>(e2, 2);
	reg.emplace<XD>(e3, 3);
	reg.emplace<XD>(e4, 4);

	{
		auto view = reg.view<XD>();

		view.each([](const auto entity, auto&& xd) {
			std::cout << std::format(
				"{}|{} : {}->{}\n",
				traits::to_entity(entity),
				traits::to_version(entity),
				(size_t)&xd,
				xd.x
			);
		});
	}
	std::cout << '\n';

	reg.erase<XD>(e2);

	{
		auto view = reg.view<XD>();

		view.each([](const auto entity, auto&& xd) {
			std::cout << std::format(
				"{}|{} : {}->{}\n",
				traits::to_entity(entity),
				traits::to_version(entity),
				(size_t)&xd,
				xd.x
			);
		});
	}

	reg.emplace<XD>(e2, 2);
	std::cout << std::format("\n");

	{
		auto view = reg.view<XD>();

		view.each([](const auto entity, auto&& xd) {
			std::cout << std::format(
				"{}|{} : {}->{}\n",
				traits::to_entity(entity),
				traits::to_version(entity),
				(size_t)&xd,
				xd.x
			);
		});
	}*/

	/*entt::registry reg;

	auto entt1 = reg.create();
	auto entt2 = reg.create();
	auto entt3 = reg.create();
	auto entt4 = reg.create();
	reg.emplace<XD>(entt1, 1);
	reg.emplace<XD>(entt2, 2);
	reg.emplace<XD>(entt3, 3);
	reg.emplace<XD>(entt4, 4);

	auto view = reg.view<XD>();
	view.each([](auto& xd) { std::cout << std::format("({})->x = {}\n", (void*)&xd, xd.x); });

	reg.erase<XD>(entt3);

	view = reg.view<XD>();
	view.each([](auto& xd) { std::cout << std::format("({})->x = {}\n", (void*)&xd, xd.x); });*/

	/*namespace chr = std::chrono;
	namespace rg = std::ranges;
	namespace vw = std::views;
	using clk = chr::high_resolution_clock;

	constexpr size_t pageSize = 10;

	std::array<std::array<char, pageSize>, pageSize> map;
	for (auto&& row : map) {
		for (auto&& val : row) {
			val = '#';
		}
	}
	map[0][0] = 'X';
	std::cout << "\033[2J\033[1;1H";
	for (auto&& row : map) {
		for (auto&& val : row) {
			std::cout << val;
		}
		std::cout << '\n';
	}

	size_t x = 0;
	size_t y = 0;
	for (;;) {
		long long dx;
		std::cin >> dx;

		map[y][x] = '#';

		x += dx;
		if (dx < 0) {
			if (x > pageSize) {
				x = (size_t)0 - x;
				y -= 1 + (x / pageSize);
				x = pageSize - (x) % pageSize;
			}
		} else {
			y += x / pageSize;
			x %= pageSize;
		}
		std::cout << y << ' ' << x << '\n';
		getchar();
		getchar();

		map[y][x] = 'X';

		std::cout << "\033[2J\033[1;1H";
		for (auto&& row : map) {
			for (auto&& val : row) {
				std::cout << val;
			}
			std::cout << '\n';
		}
	}

	return 0;*/

	/*entt::basic_storage<XD> storage;

	for (size_t i = 0; i != 1'000'000; ++i) {
		storage.emplace((entt::entity)i, (int)i);
	}*/

	/*{
		auto start = clk::now();
		for (auto&& o : storage) {
			++o.x;
		}
		auto end = clk::now();
		std::cout << std::format("{}\n", end - start);
	}*/
	/*{
		using alloc_traits = std::allocator_traits<decltype(storage)::allocator_type>;
		using traits_type = entt::component_traits<XD>;
		auto start = clk::now();
		auto _begin = storage.rbegin();
		auto containter = *(std::vector<
							typename alloc_traits::pointer,
							typename alloc_traits::template rebind_alloc<typename alloc_traits::pointer>>**)&_begin;
		for (auto&& o : *containter) {
			auto end = o + traits_type::page_size;
			for (auto i = o; i != end; ++i) {
				++(i->x);
			}
		}
		auto end = clk::now();
		std::cout << std::format("{}\n", end - start);
	}*/

	// size_t iters = 5 * 10'000;

	//{
	//	std::vector<std::vector<size_t>> nums;
	//	for (size_t i = 0; i != 1'024; ++i) {
	//		nums.emplace_back();
	//		for (size_t j = 0; j != 1'024; ++j) {
	//			nums.back().emplace_back(i * 1'024 + j);
	//		}
	//		// std::cout << i << ": " << nums.back().size() << '\n';
	//	}
	//	decltype(clk::now() - clk::now()) avg = {};
	//	for (size_t n = 0; n != iters; ++n) {
	//		if (n % (iters / 10) == 0) {
	//			std::cout << std::format("n = {}\n", n);
	//		}

	//		// std::cout << nums.size() << '\n';

	//		{
	//			auto start = clk::now();
	//			for (auto i = iter1::begin(nums), end = iter1::end(nums); i != end; ++i) {
	//				++(*i);
	//			}
	//			auto end = clk::now();
	//			avg += end - start;
	//		}
	//	}
	//	avg /= iters;
	//	std::cout << std::format("custom iter1: {}\n", avg);
	//}

	/// ITERS TEST
	/// ITERS TEST
	/// ITERS TEST
	/// ITERS TEST
	/// ITERS TEST

	// size_t iters = 5 * 10'000;

	//{
	//	std::vector<std::vector<size_t>> nums;
	//	for (size_t i = 0; i != 1'024; ++i) {
	//		nums.emplace_back();
	//		for (size_t j = 0; j != 1'024; ++j) {
	//			nums.back().emplace_back(i * 1'024 + j);
	//		}
	//		// std::cout << i << ": " << nums.back().size() << '\n';
	//	}
	//	decltype(clk::now() - clk::now()) avg = {};
	//	for (size_t n = 0; n != iters; ++n) {
	//		if (n % (iters / 10) == 0) {
	//			std::cout << std::format("n = {}\n", n);
	//		}
	//		// std::cout << nums.size() << '\n';

	//		{
	//			auto start = clk::now();
	//			for (auto i = iter2::begin(nums), end = iter2::end(nums); i != end; ++i) {
	//				++(*i);
	//			}
	//			auto end = clk::now();
	//			avg += end - start;
	//		}
	//	}
	//	avg /= iters;
	//	std::cout << std::format("custom iter2: {}\n", avg);
	//}

	//{
	//	std::vector<std::vector<size_t>> nums;
	//	for (size_t i = 0; i != 1'024; ++i) {
	//		nums.emplace_back();
	//		for (size_t j = 0; j != 1'024; ++j) {
	//			nums.back().emplace_back(i * 1'024 + j);
	//		}
	//		// std::cout << i << ": " << nums.back().size() << '\n';
	//	}
	//	decltype(clk::now() - clk::now()) avg = {};
	//	for (size_t n = 0; n != iters; ++n) {
	//		if (n % (iters / 10) == 0) {
	//			std::cout << std::format("n = {}\n", n);
	//		}
	//		// std::cout << nums.size() << '\n';

	//		{
	//			auto start = clk::now();
	//			for (auto i = iter3::begin(nums), end = iter3::end(nums); i != end; ++i) {
	//				++(*i);
	//			}
	//			auto end = clk::now();
	//			avg += end - start;
	//		}
	//	}
	//	avg /= iters;
	//	std::cout << std::format("custom iter3: {}\n", avg);
	//}

	//{
	//	std::vector<std::vector<size_t>> nums;
	//	for (size_t i = 0; i != 1'024; ++i) {
	//		nums.emplace_back();
	//		for (size_t j = 0; j != 1'024; ++j) {
	//			nums.back().emplace_back(i * 1'024 + j);
	//		}
	//		// std::cout << i << ": " << nums.back().size() << '\n';
	//	}
	//	decltype(clk::now() - clk::now()) avg = {};
	//	for (size_t n = 0; n != iters; ++n) {
	//		if (n % (iters / 10) == 0) {
	//			std::cout << std::format("n = {}\n", n);
	//		}
	//		// std::cout << nums.size() << '\n';

	//		{
	//			auto start = clk::now();
	//			for (size_t i = 0; i != nums.size(); ++i) {
	//				for (size_t j = 0; j != nums[i].size(); ++j) {
	//					++nums[i][j];
	//				}
	//			}
	//			auto end = clk::now();
	//			avg += end - start;
	//		}
	//	}
	//	avg /= iters;
	//	std::cout << std::format("custom iter3: {}\n", avg);
	//}

	//{
	//	std::vector<std::vector<size_t>> nums;
	//	for (size_t i = 0; i != 1'024; ++i) {
	//		nums.emplace_back();
	//		for (size_t j = 0; j != 1'024; ++j) {
	//			nums.back().emplace_back(i * 1'024 + j);
	//		}
	//		// std::cout << i << ": " << nums.back().size() << '\n';
	//	}
	//	decltype(clk::now() - clk::now()) avg = {};
	//	for (size_t n = 0; n != iters; ++n) {
	//		if (n % (iters / 10) == 0) {
	//			std::cout << std::format("n = {}\n", n);
	//		}
	//		// std::cout << nums.size() << '\n';

	//		{
	//			auto start = clk::now();
	//			for (auto&& v : nums) {
	//				for (auto&& o : v) {
	//					++o;
	//				}
	//			}
	//			auto end = clk::now();
	//			avg += end - start;
	//		}
	//	}
	//	avg /= iters;
	//	std::cout << std::format("custom iter4: {}\n", avg);
	//}

	//{
	//	std::vector<std::vector<size_t>> nums;
	//	for (size_t i = 0; i != 1'024; ++i) {
	//		nums.emplace_back();
	//		for (size_t j = 0; j != 1'024; ++j) {
	//			nums.back().emplace_back(i * 1'024 + j);
	//		}
	//		// std::cout << i << ": " << nums.back().size() << '\n';
	//	}
	//	decltype(clk::now() - clk::now()) avg = {};
	//	for (size_t n = 0; n != iters; ++n) {
	//		if (n % (iters / 10) == 0) {
	//			std::cout << std::format("n = {}\n", n);
	//		}
	//		// std::cout << nums.size() << '\n';

	//		{
	//			auto start = clk::now();
	//			auto&& view = nums | vw::join;
	//			for (auto&& o : view) {
	//				++o;
	//			}
	//			auto end = clk::now();
	//			avg += end - start;
	//		}
	//	}
	//	avg /= iters;
	//	std::cout << std::format("custom iter5: {}\n", avg);
	//}

	//{
	//	std::vector<std::vector<size_t>> nums;
	//	for (size_t i = 0; i != 1'024; ++i) {
	//		nums.emplace_back();
	//		for (size_t j = 0; j != 1'024; ++j) {
	//			nums.back().emplace_back(i * 1'024 + j);
	//		}
	//		// std::cout << i << ": " << nums.back().size() << '\n';
	//	}
	//	auto&& view = nums | vw::join;
	//	decltype(clk::now() - clk::now()) avg = {};
	//	for (size_t n = 0; n != iters; ++n) {
	//		if (n % (iters / 10) == 0) {
	//			std::cout << std::format("n = {}\n", n);
	//		}
	//		// std::cout << nums.size() << '\n';

	//		{
	//			auto start = clk::now();
	//			for (auto&& o : view) {
	//				++o;
	//			}
	//			auto end = clk::now();
	//			avg += end - start;
	//		}
	//	}
	//	avg /= iters;
	//	std::cout << std::format("custom iter6: {}\n", avg);
	//}

	// avg = {};
	// for (size_t n = 0; n != 1'000; ++n) {
	//	if (n % 100 == 0) {
	//		std::cout << std::format("n = {}\n", n);
	//	}
	//	std::vector<std::vector<size_t>> nums;
	//	for (size_t i = 0; i != 1'024; ++i) {
	//		nums.emplace_back();
	//		for (size_t j = 0; j != 1'024; ++j) {
	//			nums.back().emplace_back(i * 1'024 + j);
	//		}
	//		// std::cout << i << ": " << nums.back().size() << '\n';
	//	}
	//	// std::cout << nums.size() << '\n';

	//	{
	//		auto start = clk::now();
	//		for (size_t i = 0; i != nums.size(); ++i) {
	//			for (size_t j = 0; j != nums[i].size(); ++j) {
	//				++nums[i][j];
	//			}
	//		}
	//		auto end = clk::now();
	//		avg += end - start;
	//	}
	//}
	// avg /= 1'000;
	// std::cout << std::format("loop:            {}\n", avg);

	// avg = {};
	// for (size_t n = 0; n != 1'000; ++n) {
	//	if (n % 100 == 0) {
	//		std::cout << std::format("n = {}\n", n);
	//	}
	//	std::vector<std::vector<size_t>> nums;
	//	for (size_t i = 0; i != 1'024; ++i) {
	//		nums.emplace_back();
	//		for (size_t j = 0; j != 1'024; ++j) {
	//			nums.back().emplace_back(i * 1'024 + j);
	//		}
	//		// std::cout << i << ": " << nums.back().size() << '\n';
	//	}
	//	// std::cout << nums.size() << '\n';

	//	{
	//		auto start = clk::now();
	//		for (auto&& v : nums) {
	//			for (auto&& o : v) {
	//				++o;
	//			}
	//		}
	//		auto end = clk::now();
	//		avg += end - start;
	//	}
	//}
	// avg /= 1'000;
	// std::cout << std::format("iterator:        {}\n", avg);

#pragma endregion
}
