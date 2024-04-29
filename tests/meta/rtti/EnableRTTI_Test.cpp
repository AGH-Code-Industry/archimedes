#include <gtest/gtest.h>
#include <meta/Rtti.h>

class FooBase {
	THIS_T(FooBase);
	EnableRTTI;

public:

	virtual ~FooBase() noexcept = default;
};

class FooBase2 {
	THIS_T(FooBase2);
	// EnableRTTI;

public:

	virtual ~FooBase2() noexcept = default;
};

class FooRTTIEnabled: public FooBase, public FooBase2 {
	THIS_T(FooRTTIEnabled);
	EnableRTTI;

public:

	virtual ~FooRTTIEnabled() noexcept = default;
};

class FooRTTIDisabled: public FooBase, public FooBase2 {
	THIS_T(FooRTTIDisabled);
	// EnableRTTI;

public:

	virtual ~FooRTTIDisabled() noexcept = default;
};

TEST(RTTITest, AttributeChecking) {
	using arch::meta::rtti::RTTIEnabled;

	EXPECT_TRUE(RTTIEnabled<FooBase>);
	EXPECT_FALSE(RTTIEnabled<FooBase2>);
	EXPECT_TRUE(RTTIEnabled<FooRTTIEnabled>);
	EXPECT_FALSE(RTTIEnabled<FooRTTIDisabled>);
}
