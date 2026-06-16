#include <archimedes/Logger.h>
#include <archimedes/meta/Rtti.h>
#include <gtest/gtest.h>

class FooBase {
	THIS_T(FooBase);
	EnableRTTI;

public:

	virtual ~FooBase() noexcept = default;
};

class FooBase2 {
	THIS_T(FooBase2);
	EnableRTTI;

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

TEST(RTTITest, Typedesc) {
	arch::Logger::init(arch::LogLevel::debug);

	FooRTTIEnabled fooRTTIEnabled;
	FooRTTIDisabled fooRTTIDisabled;

	std::cout << std::format(
		"1st: {}\n2nd: {}\n",
		arch::typedesc(fooRTTIEnabled).name,
		arch::typedesc<FooRTTIEnabled>().name
	);
	EXPECT_EQ(arch::typedesc(fooRTTIEnabled), arch::typedesc<FooRTTIEnabled>());
	std::cout << "expected message:\n";
	EXPECT_EQ(arch::typedesc(fooRTTIDisabled), arch::typedesc<FooRTTIDisabled>());

	FooBase& fooBaseRTTIEnabled = fooRTTIEnabled;
	FooBase& fooBaseRTTIDisabled = fooRTTIDisabled;
	FooBase* fooBasePtrRTTIEnabled = &fooRTTIEnabled;
	FooBase* fooBasePtrRTTIDisabled = &fooRTTIDisabled;

	EXPECT_EQ(arch::typedesc(fooBaseRTTIEnabled), arch::typedesc<FooRTTIEnabled>());
	std::cout << "expected message:\n";
	EXPECT_NE(arch::typedesc(fooBaseRTTIDisabled), arch::typedesc<FooRTTIDisabled>());
	std::cout << "expected message:\n";
	EXPECT_EQ(arch::typedesc(fooBaseRTTIDisabled), arch::typedesc<FooBase>());

	EXPECT_EQ(arch::typedesc(*fooBasePtrRTTIEnabled), arch::typedesc<FooRTTIEnabled>());
	std::cout << "expected message:\n";
	EXPECT_NE(arch::typedesc(*fooBasePtrRTTIDisabled), arch::typedesc<FooRTTIDisabled>());
	std::cout << "expected message:\n";
	EXPECT_EQ(arch::typedesc(*fooBasePtrRTTIDisabled), arch::typedesc<FooBase>());
}

TEST(RTTITest, Typedesc2) {
	arch::Logger::init(arch::LogLevel::debug);

	FooRTTIEnabled fooRTTIEnabled;
	FooRTTIDisabled fooRTTIDisabled;

	EXPECT_EQ(arch::typedesc(fooRTTIEnabled), arch::typedesc<FooRTTIEnabled>());
	std::cout << "expected message:\n";
	EXPECT_EQ(arch::typedesc(fooRTTIDisabled), arch::typedesc<FooRTTIDisabled>());

	FooBase2& fooBaseRTTIEnabled = fooRTTIEnabled;
	FooBase2& fooBaseRTTIDisabled = fooRTTIDisabled;
	FooBase2* fooBasePtrRTTIEnabled = &fooRTTIEnabled;
	FooBase2* fooBasePtrRTTIDisabled = &fooRTTIDisabled;

	EXPECT_EQ(arch::typedesc(fooBaseRTTIEnabled), arch::typedesc<FooRTTIEnabled>());
	std::cout << "expected message:\n";
	EXPECT_NE(arch::typedesc(fooBaseRTTIDisabled), arch::typedesc<FooRTTIDisabled>());
	std::cout << "expected message:\n";
	EXPECT_EQ(arch::typedesc(fooBaseRTTIDisabled), arch::typedesc<FooBase2>());

	EXPECT_EQ(arch::typedesc(*fooBasePtrRTTIEnabled), arch::typedesc<FooRTTIEnabled>());
	std::cout << "expected message:\n";
	EXPECT_NE(arch::typedesc(*fooBasePtrRTTIDisabled), arch::typedesc<FooRTTIDisabled>());
	std::cout << "expected message:\n";
	EXPECT_EQ(arch::typedesc(*fooBasePtrRTTIDisabled), arch::typedesc<FooBase2>());
}
