#include <Ecs.h>
#include <gtest/gtest.h>

using namespace arch;

// Component type that tracks constructions, copies, moves and destructions
class TrackingComponent {
public:
	TrackingComponent() noexcept { ++_constructions; }

	TrackingComponent(const TrackingComponent& other) noexcept: _val{ other._val } {
		++_constructions;
		++_copies;
	}

	TrackingComponent(TrackingComponent&& other) noexcept: _val{ std::move(other._val) } {
		++_constructions;
		++_moves;
	}

	TrackingComponent& operator=(const TrackingComponent& other) noexcept {
		_val = other._val;
		++_copies;

		return *this;
	}

	TrackingComponent& operator=(TrackingComponent&& other) noexcept {
		_val = std::move(other._val);
		++_moves;

		return *this;
	}

	~TrackingComponent() noexcept { ++_destructions; }

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

// same as TrackingComponent, but in-place
class InPlaceTrackingComponent {
public:
	static constexpr bool inPlaceComponent = true;

	InPlaceTrackingComponent() noexcept { ++_constructions; }

	InPlaceTrackingComponent(const InPlaceTrackingComponent& other) noexcept: _val{ other._val } {
		++_constructions;
		++_copies;
	}

	InPlaceTrackingComponent(InPlaceTrackingComponent&& other) noexcept: _val{ std::move(other._val) } {
		++_constructions;
		++_moves;
	}

	InPlaceTrackingComponent& operator=(const InPlaceTrackingComponent& other) noexcept {
		_val = other._val;
		++_copies;

		return *this;
	}

	InPlaceTrackingComponent& operator=(InPlaceTrackingComponent&& other) noexcept {
		_val = std::move(other._val);
		++_moves;

		return *this;
	}

	~InPlaceTrackingComponent() noexcept { ++_destructions; }

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
	TrackingComponent::reset();
	InPlaceTrackingComponent::reset();
}

TEST(ECS, Component_Create) {
	reset();

	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	// all entities should have all components
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<TrackingComponent>(entity);
		domain.addComponent<InPlaceTrackingComponent>(entity);
		domain.addComponent<FlagComponent>(entity);

		// does entity have all the components
		ASSERT_TRUE(domain.hasComponent<TrackingComponent>(entity));
		ASSERT_TRUE(domain.hasComponent<InPlaceTrackingComponent>(entity));
		ASSERT_TRUE(domain.hasComponent<FlagComponent>(entity));
	}

	// are there entityCount components?
	ASSERT_EQ(domain.count<TrackingComponent>(), entityCount);
	ASSERT_EQ(domain.count<InPlaceTrackingComponent>(), entityCount);
	ASSERT_EQ(domain.count<FlagComponent>(), entityCount);
	// were all components properly constructed?
	ASSERT_EQ(TrackingComponent::constructions(), entityCount);
	ASSERT_EQ(InPlaceTrackingComponent::constructions(), entityCount);
	// were flags constructed?
	ASSERT_EQ(FlagComponent::constructions(), 0);
}

TEST(ECS, Component_Remove) {
	reset();

	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<TrackingComponent>(entity);
		domain.addComponent<InPlaceTrackingComponent>(entity);
		domain.addComponent<FlagComponent>(entity);
	}

	for (auto [i, entity] : std::views::enumerate(domain.entities())) {
		// did ECS remove all the components?
		ASSERT_TRUE(domain.removeComponent<TrackingComponent>(entity));
		ASSERT_TRUE(domain.removeComponent<InPlaceTrackingComponent>(entity));
		ASSERT_TRUE(domain.removeComponent<FlagComponent>(entity));

		// was TrackingComponent destroyed and had some other moved in its place?
		ASSERT_EQ(TrackingComponent::destructions(), i + 1);
		ASSERT_EQ(TrackingComponent::moves(), i + 1);
		ASSERT_EQ(TrackingComponent::copies(), 0);

		// was InPlaceComponent only destroyed?
		ASSERT_EQ(InPlaceTrackingComponent::destructions(), i + 1);
		ASSERT_EQ(InPlaceTrackingComponent::moves(), 0);
		ASSERT_EQ(InPlaceTrackingComponent::copies(), 0);

		// was FlagComponent not destroyed, since it doesn't exist?
		ASSERT_EQ(FlagComponent::destructions(), 0);
		ASSERT_EQ(FlagComponent::moves(), 0);
		ASSERT_EQ(FlagComponent::copies(), 0);

		// do component counts match the expected values?
		ASSERT_EQ(
			domain.count<TrackingComponent>(),
			TrackingComponent::constructions() - TrackingComponent::destructions()
		);
		ASSERT_EQ(
			domain.count<InPlaceTrackingComponent>(),
			InPlaceTrackingComponent::constructions() - InPlaceTrackingComponent::destructions()
		);
		ASSERT_EQ(domain.count<InPlaceTrackingComponent>(), entityCount - i - 1);
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

	// each entity has vector [0; entityCount)
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<Vec>(entity) = expectedVec;
	}

	for (auto [i, entity] : std::views::enumerate(domain.entities())) {
		// get data pointer of old vector
		auto oldVecData = domain.getComponent<Vec>(entity).data();
		// move vector from domain
		auto vec = domain.removeComponent<Vec>(entity, moveFlag);

		// did move removed vector?
		ASSERT_EQ(domain.count<Vec>(), entityCount - i - 1);
		// did data moved correctly?
		ASSERT_EQ(vec, expectedVec);
		// did data moved efficiently?
		ASSERT_EQ(oldVecData, vec.data());
	}
}
