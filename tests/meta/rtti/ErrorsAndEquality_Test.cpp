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

	EXPECT_EQ(typedesc(fooRTTIEnabled), typedesc(FooRTTIEnabled));
	std::cout << "expected message:\n";
	EXPECT_EQ(typedesc(fooRTTIDisabled), typedesc(FooRTTIDisabled));

	FooBase& fooBaseRTTIEnabled = fooRTTIEnabled;
	FooBase& fooBaseRTTIDisabled = fooRTTIDisabled;
	FooBase* fooBasePtrRTTIEnabled = &fooRTTIEnabled;
	FooBase* fooBasePtrRTTIDisabled = &fooRTTIDisabled;

	EXPECT_EQ(typedesc(fooBaseRTTIEnabled), typedesc(FooRTTIEnabled));
	std::cout << "expected message:\n";
	EXPECT_NE(typedesc(fooBaseRTTIDisabled), typedesc(FooRTTIDisabled));
	std::cout << "expected message:\n";
	EXPECT_EQ(typedesc(fooBaseRTTIDisabled), typedesc(FooBase));

	EXPECT_EQ(typedesc(*fooBasePtrRTTIEnabled), typedesc(FooRTTIEnabled));
	std::cout << "expected message:\n";
	EXPECT_NE(typedesc(*fooBasePtrRTTIDisabled), typedesc(FooRTTIDisabled));
	std::cout << "expected message:\n";
	EXPECT_EQ(typedesc(*fooBasePtrRTTIDisabled), typedesc(FooBase));
}

TEST(RTTITest, Typedesc2) {
	arch::Logger::init(arch::LogLevel::debug);

	FooRTTIEnabled fooRTTIEnabled;
	FooRTTIDisabled fooRTTIDisabled;

	EXPECT_EQ(typedesc(fooRTTIEnabled), typedesc(FooRTTIEnabled));
	std::cout << "expected message:\n";
	EXPECT_EQ(typedesc(fooRTTIDisabled), typedesc(FooRTTIDisabled));

	FooBase2& fooBaseRTTIEnabled = fooRTTIEnabled;
	FooBase2& fooBaseRTTIDisabled = fooRTTIDisabled;
	FooBase2* fooBasePtrRTTIEnabled = &fooRTTIEnabled;
	FooBase2* fooBasePtrRTTIDisabled = &fooRTTIDisabled;

	EXPECT_EQ(typedesc(fooBaseRTTIEnabled), typedesc(FooRTTIEnabled));
	std::cout << "expected message:\n";
	EXPECT_NE(typedesc(fooBaseRTTIDisabled), typedesc(FooRTTIDisabled));
	std::cout << "expected message:\n";
	EXPECT_EQ(typedesc(fooBaseRTTIDisabled), typedesc(FooBase2));

	EXPECT_EQ(typedesc(*fooBasePtrRTTIEnabled), typedesc(FooRTTIEnabled));
	std::cout << "expected message:\n";
	EXPECT_NE(typedesc(*fooBasePtrRTTIDisabled), typedesc(FooRTTIDisabled));
	std::cout << "expected message:\n";
	EXPECT_EQ(typedesc(*fooBasePtrRTTIDisabled), typedesc(FooBase2));
}
