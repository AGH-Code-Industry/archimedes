#include <print>

#include <Ecs.h>
#include <gtest/gtest.h>

using namespace arch;

// Component type that tracks constructions, copies, moves and destructions
class TrackerComponent {
public:
	TrackerComponent() noexcept { ++_constructions; }

	TrackerComponent(const TrackerComponent& other) noexcept: _val{ other._val } {
		++_constructions;
		++_copies;
	}

	TrackerComponent(TrackerComponent&& other) noexcept: _val{ std::move(other._val) } {
		++_constructions;
		++_moves;
	}

	TrackerComponent& operator=(const TrackerComponent& other) noexcept {
		_val = other._val;
		++_copies;

		return *this;
	}

	TrackerComponent& operator=(TrackerComponent&& other) noexcept {
		_val = std::move(other._val);
		++_moves;

		return *this;
	}

	~TrackerComponent() noexcept { ++_destructions; }

	static int constructions() noexcept { return _constructions; }

	static int destructions() noexcept { return _destructions; }

	static int copies() noexcept { return _copies; }

	static int moves() noexcept { return _moves; }

	static void reset() noexcept {
		_constructions = 0;
		_destructions = 0;
		_copies = 0;
		_moves = 0;
	}

private:
	inline static int _constructions = 0;
	inline static int _destructions = 0;
	inline static int _copies = 0;
	inline static int _moves = 0;

	int _val = 123;
};

// same as TrackerComponent, but in-place
class InPlaceTrackerComponent {
public:
	static constexpr bool inPlaceComponent = true;

	InPlaceTrackerComponent() noexcept { ++_constructions; }

	InPlaceTrackerComponent(const InPlaceTrackerComponent& other) noexcept: _val{ other._val } {
		++_constructions;
		++_copies;
	}

	InPlaceTrackerComponent(InPlaceTrackerComponent&& other) noexcept: _val{ std::move(other._val) } {
		++_constructions;
		++_moves;
	}

	InPlaceTrackerComponent& operator=(const InPlaceTrackerComponent& other) noexcept {
		_val = other._val;
		++_copies;

		return *this;
	}

	InPlaceTrackerComponent& operator=(InPlaceTrackerComponent&& other) noexcept {
		_val = std::move(other._val);
		++_moves;

		return *this;
	}

	~InPlaceTrackerComponent() noexcept { ++_destructions; }

	static int constructions() noexcept { return _constructions; }

	static int destructions() noexcept { return _destructions; }

	static int copies() noexcept { return _copies; }

	static int moves() noexcept { return _moves; }

	static void reset() noexcept {
		_constructions = 0;
		_destructions = 0;
		_copies = 0;
		_moves = 0;
	}

private:
	inline static int _constructions = 0;
	inline static int _destructions = 0;
	inline static int _copies = 0;
	inline static int _moves = 0;

	int _val = 123;
};

// flag component, all trackers should report 0
class FlagComponent {
public:
	static constexpr bool flagComponent = true;

	FlagComponent() noexcept { ++_constructions; }

	FlagComponent(const FlagComponent& other) noexcept {
		++_constructions;
		++_copies;
	}

	FlagComponent(FlagComponent&& other) noexcept {
		++_constructions;
		++_moves;
	}

	FlagComponent& operator=(const FlagComponent& other) noexcept {
		++_copies;

		return *this;
	}

	FlagComponent& operator=(FlagComponent&& other) noexcept {
		++_moves;

		return *this;
	}

	~FlagComponent() noexcept { ++_destructions; }

	static int constructions() noexcept { return _constructions; }

	static int destructions() noexcept { return _destructions; }

	static int copies() noexcept { return _copies; }

	static int moves() noexcept { return _moves; }

	static void reset() noexcept {
		_constructions = 0;
		_destructions = 0;
		_copies = 0;
		_moves = 0;
	}

private:
	inline static int _constructions = 0;
	inline static int _destructions = 0;
	inline static int _copies = 0;
	inline static int _moves = 0;
};

// FlagComponent is empty
static_assert(std::is_empty_v<FlagComponent>);

void reset() noexcept {
	TrackerComponent::reset();
	InPlaceTrackerComponent::reset();
}

TEST(ECS, Component_Create) {
	reset();

	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	// all entities should have all components
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<TrackerComponent>(entity);
		domain.addComponent<InPlaceTrackerComponent>(entity);
		domain.addComponent<FlagComponent>(entity);

		// does entity have all the components
		ASSERT_TRUE(domain.hasComponent<TrackerComponent>(entity));
		ASSERT_TRUE(domain.hasComponent<InPlaceTrackerComponent>(entity));
		ASSERT_TRUE(domain.hasComponent<FlagComponent>(entity));
	}

	// are there entityCount components?
	ASSERT_EQ(domain.count<TrackerComponent>(), entityCount);
	ASSERT_EQ(domain.count<InPlaceTrackerComponent>(), entityCount);
	ASSERT_EQ(domain.count<FlagComponent>(), entityCount);

	// were all components properly constructed?
	ASSERT_EQ(TrackerComponent::constructions(), entityCount);
	ASSERT_EQ(InPlaceTrackerComponent::constructions(), entityCount);

	// were flags constructed?
	ASSERT_EQ(FlagComponent::constructions(), 0);
}

TEST(ECS, Component_Remove) {
	reset();

	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<TrackerComponent>(entity);
		domain.addComponent<InPlaceTrackerComponent>(entity);
		domain.addComponent<FlagComponent>(entity);
	}

	for (auto [i, entity] : std::views::enumerate(domain.entities())) {
		// did ECS remove all the components?
		ASSERT_TRUE(domain.removeComponent<TrackerComponent>(entity));
		ASSERT_TRUE(domain.removeComponent<InPlaceTrackerComponent>(entity));
		ASSERT_TRUE(domain.removeComponent<FlagComponent>(entity));

		// was TrackerComponent destroyed and had some other moved in its place?
		ASSERT_EQ(TrackerComponent::destructions(), i + 1);
		ASSERT_EQ(TrackerComponent::moves(), i + 1);
		ASSERT_EQ(TrackerComponent::copies(), 0);

		// was InPlaceTrackerComponent only destroyed?
		ASSERT_EQ(InPlaceTrackerComponent::destructions(), i + 1);
		ASSERT_EQ(InPlaceTrackerComponent::moves(), 0);
		ASSERT_EQ(InPlaceTrackerComponent::copies(), 0);

		// was FlagComponent not destroyed, since it doesn't exist?
		ASSERT_EQ(FlagComponent::destructions(), 0);
		ASSERT_EQ(FlagComponent::moves(), 0);
		ASSERT_EQ(FlagComponent::copies(), 0);

		// do component counts match the expected values?
		ASSERT_EQ(
			domain.count<TrackerComponent>(),
			TrackerComponent::constructions() - TrackerComponent::destructions()
		);
		ASSERT_EQ(
			domain.count<InPlaceTrackerComponent>(),
			InPlaceTrackerComponent::constructions() - InPlaceTrackerComponent::destructions()
		);
		ASSERT_EQ(domain.count<InPlaceTrackerComponent>(), entityCount - i - 1);
	}
}

TEST(ECS, Component_IgnoreSecondAdd) {
	reset();

	ecs::Domain domain;

	constexpr int entityCount = 1'000;
	constexpr int expectedValue = 12'345;
	constexpr int otherValue = 54'321;

	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();

		// first added int
		int& intFirstAdd = domain.addComponent<int>(entity, expectedValue);
		// second added int
		int& intSecondAdd = domain.addComponent<int>(entity, otherValue);

		// is actual value the expected one?
		ASSERT_EQ(domain.getComponent<int>(entity), expectedValue);
		// is there only one int?
		ASSERT_EQ(&intFirstAdd, &intSecondAdd);
	}
}

TEST(ECS, Component_MovingRemove) {
	reset();

	ecs::Domain domain;

	constexpr int entityCount = 1'000;
	using Vec = std::vector<int>;

	auto expectedVec = std::views::iota(0, entityCount) | std::ranges::to<std::vector>();

	// each entity has vector [0; entityCount) and trackers
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<Vec>(entity) = expectedVec;
		domain.addComponent<TrackerComponent>(entity);
		domain.addComponent<InPlaceTrackerComponent>(entity);
	}

	for (auto [i, entity] : std::views::enumerate(domain.entities())) {
		// get data pointer of old vector
		auto oldVecData = domain.getComponent<Vec>(entity).data();
		// move vector from domain
		auto vec = domain.removeComponent<Vec>(entity, moveFlag);

		// move trackers from domain
		auto tracker = domain.removeComponent<TrackerComponent>(entity, moveFlag);
		auto inPlaceTracker = domain.removeComponent<InPlaceTrackerComponent>(entity, moveFlag);

		// did move removed vector?
		ASSERT_EQ(domain.count<Vec>(), entityCount - i - 1);
		// did data moved correctly?
		ASSERT_EQ(vec, expectedVec);
		// did data moved efficiently?
		ASSERT_EQ(oldVecData, vec.data());

		// was tracker moved twice? (to return + to moved-from)
		ASSERT_EQ(TrackerComponent::moves(), (i + 1) * 2);
		// was in-place tracker moved once?
		ASSERT_EQ(InPlaceTrackerComponent::moves(), i + 1);

		// were Tracker components copied?
		ASSERT_EQ(TrackerComponent::copies(), 0);
		ASSERT_EQ(InPlaceTrackerComponent::copies(), 0);

		// were Tracker components properly destroyed? (second moved-from + previous tc/iptc)
		ASSERT_EQ(TrackerComponent::destructions(), i * 2 + 1);
		ASSERT_EQ(InPlaceTrackerComponent::destructions(), i * 2 + 1);
	}
}

TEST(ECS, Component_PageSize) {
	reset();

	ecs::Domain domain;

	constexpr int pageCount = 10;
	constexpr int pageSize = ecs::ComponentSpecs<TrackerComponent>::pageSize;

	// makes 10 pages for components
	for (int i = 0; i != pageSize * pageCount; ++i) {
		domain.addComponent<TrackerComponent>(domain.newEntity());
	}

	for (int p = 0; p != pageCount; ++p) {
		// is each page continous?
		auto trackerPagePtr = &*std::next(domain.components<TrackerComponent>().begin(), p * pageSize);
		for (auto&& [i, tracker] : domain.components<TrackerComponent>()
				 | std::views::drop(p * pageSize)
				 | std::views::take(pageSize)
				 | std::views::enumerate) {
			ASSERT_EQ(&tracker, trackerPagePtr + i);
		}
	}
}

TEST(ECS, Component_TryGet) {
	reset();

	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	// half of entities have no components
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		if (i % 2) {
			domain.addComponent<TrackerComponent>(entity);
		}
	}

	// does OptRef hold correct value?
	for (auto [i, entity] : std::views::enumerate(domain.entities())) {
		ASSERT_EQ(domain.tryGetComponent<TrackerComponent>(entity).hasValue(), i % 2);
	}
}

TEST(ECS, Component_EmptyCPool) {
	reset();

	ecs::Domain domain;

	// no components -> begin == end
	ASSERT_EQ(
		std::as_const(domain).components<TrackerComponent>().begin(),
		std::as_const(domain).components<TrackerComponent>().end()
	);
	ASSERT_EQ(domain.components<TrackerComponent>().begin(), domain.components<TrackerComponent>().end());
}
