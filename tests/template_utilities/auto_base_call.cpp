/*
#include <gtest/gtest.h>
#include <utilities/auto_base_call.hpp>
#include <sstream>
#include <string>

namespace u {

std::stringstream stream;

class base {
public:
	virtual ~base() = default;
	virtual void update() {
		stream << "base\n";
	}
};
class baseC {
public:
	virtual ~baseC() = default;
	virtual void update() {
		stream << "baseC\n";
	}
};
class baseD {
public:
	virtual ~baseD() = default;
	virtual void update() {
		stream << "baseD\n";
	}
};
class derivedA : public arch::utils::register_base<base>, public base {
public:
	virtual ~derivedA() = default;

	UPWARD_CALL(update, () {
		stream << "derivedA\n";
	})
};
class derivedB : public arch::utils::register_base<derivedA>, public derivedA {
public:
	virtual ~derivedB() = default;

	UPWARD_CALL(update, () {
		stream << "derivedB\n";
	})
};
class derivedC : public arch::utils::register_base<baseC>, public baseC {
public:
	virtual ~derivedC() = default;

	UPWARD_CALL(update, () {
		stream << "derivedC\n";
	})
};
class derivedD : public arch::utils::register_base<baseD>, public baseD {
public:
	virtual ~derivedD() = default;

	UPWARD_CALL(update, () {
		stream << "derivedD\n";
	})
};

class derived : public arch::utils::register_base<derivedB, derivedD, derivedC>, public derivedB, public derivedD, public derivedC {
public:
	virtual ~derived() = default;

	UPWARD_CALL(update, () {
		stream << "derived\n";
	})
};
}

TEST(AutoBaseCall, CallUpwards) {
	u::base* d = new u::derived();
	d->update();
	delete d;

	EXPECT_EQ(u::stream.str(), std::string(R"raw_str(base
derivedA
derivedB
baseD
derivedD
baseC
derivedC
derived
)raw_str"));
	u::stream.str("");

	u::derived d2;
	d2.update();

	EXPECT_EQ(u::stream.str(), std::string(R"raw_str(base
derivedA
derivedB
baseD
derivedD
baseC
derivedC
derived
)raw_str"));
}

namespace d {

std::stringstream stream;

class base {
public:
	virtual ~base() = default;
	virtual void update() {
		stream << "base\n";
	}
};
class baseC {
public:
	virtual ~baseC() = default;
	virtual void update() {
		stream << "baseC\n";
	}
};
class baseD {
public:
	virtual ~baseD() = default;
	virtual void update() {
		stream << "baseD\n";
	}
};
class derivedA : public arch::utils::register_base<base>, public base {
public:
	virtual ~derivedA() = default;

	DOWNWARD_CALL(update, () {
		stream << "derivedA\n";
	})
};
class derivedB : public arch::utils::register_base<derivedA>, public derivedA {
public:
	virtual ~derivedB() = default;

	DOWNWARD_CALL(update, () {
		stream << "derivedB\n";
	})
};
class derivedC : public arch::utils::register_base<baseC>, public baseC {
public:
	virtual ~derivedC() = default;

	DOWNWARD_CALL(update, () {
		stream << "derivedC\n";
	})
};
class derivedD : public arch::utils::register_base<baseD>, public baseD {
public:
	virtual ~derivedD() = default;

	DOWNWARD_CALL(update, () {
		stream << "derivedD\n";
	})
};

class derived : public arch::utils::register_base<derivedB, derivedD, derivedC>, public derivedB, public derivedD, public derivedC {
public:
	virtual ~derived() = default;

	DOWNWARD_CALL(update, () {
		stream << "derived\n";
	})
};
}

TEST(AutoBaseCall, CallDownwards) {
	d::base* d = new d::derived();
	d->update();
	delete d;

	EXPECT_EQ(d::stream.str(), std::string(R"raw_str(derived
derivedB
derivedA
base
derivedD
baseD
derivedC
baseC
)raw_str"));
	d::stream.str("");

	d::derived d2;
	d2.update();

	EXPECT_EQ(d::stream.str(), std::string(R"raw_str(derived
derivedB
derivedA
base
derivedD
baseD
derivedC
baseC
)raw_str"));
}
*/