#include <gtest/gtest.h>
#include <serialization.hpp>
#include <template_utilities.hpp>
#include <net.hpp>
#include <source_location>

namespace arch::net::graphics::exceptions::serialization {
template<class... T>
class multitype_template {};
}

TEST(Serialization, NameOfTypeFormatting) {
	// kurna jak to sformatuje to wszystko sformatuje
	//std::cout << arch::tmpl_utils::nameoftype_v<arch::net::graphics::exceptions::serialization::multitype_template<ct_str_t(u8"amogus"), long double, size_t, float, __int8>> << '\n';
	constexpr auto name_msvc = R"raw_name(class arch::net::graphics::exceptions::serialization::multitype_template<struct __ct_str_t::__ct_str_to_t<char,struct __ct_str_t::ct_str<char,7>{char{97,109,111,103,117,115,0}},struct __ct_str_t::v_str<char,struct __ct_str_t::ct_c<char,97>,struct __ct_str_t::ct_c<char,109>,struct __ct_str_t::ct_c<char,111>,struct __ct_str_t::ct_c<char,103>,struct __ct_str_t::ct_c<char,117>,struct __ct_str_t::ct_c<char,115>,struct __ct_str_t::ct_c<char,0> > >,long double,unsigned __int64,float,char>)raw_name";
	constexpr auto name_gcc = R"raw_name(arch::net::graphics::exceptions::serialization::multitype_template<__ct_str_t::__ct_str_to_t<char, __ct_str_t::ct_str<char, 7>{"amogus"}, __ct_str_t::v_str<char, __ct_str_t::ct_c<char, 'a'>, __ct_str_t::ct_c<char, 'm'>, __ct_str_t::ct_c<char, 'o'>, __ct_str_t::ct_c<char, 'g'>, __ct_str_t::ct_c<char, 'u'>, __ct_str_t::ct_c<char, 's'>, __ct_str_t::ct_c<char, '\000'> > >, long double, long long unsigned int, float, char>)raw_name";
	constexpr auto name_clang = R"raw_name(arch::net::graphics::exceptions::serialization::multitype_template<__ct_str_t::__ct_str_to_t<char, __ct_str_t::ct_str<char, 7>{"amogus"}, __ct_str_t::v_str<char, __ct_str_t::ct_c<char, 'a'>, __ct_str_t::ct_c<char, 'm'>, __ct_str_t::ct_c<char, 'o'>, __ct_str_t::ct_c<char, 'g'>, __ct_str_t::ct_c<char, 'u'>, __ct_str_t::ct_c<char, 's'>, __ct_str_t::ct_c<char, '\x00'>>>, long double, unsigned long long, float, char>)raw_name";

	auto fmted_name_msvc = arch::tmpl_utils::nameoftype_fmt(name_msvc);
	auto fmted_name_gcc = arch::tmpl_utils::nameoftype_fmt(name_gcc);
	auto fmted_name_clang = arch::tmpl_utils::nameoftype_fmt(name_clang);

	//std::cout << fmted_name_msvc << '\n';
	//std::cout << fmted_name_gcc << '\n';
	//std::cout << fmted_name_clang << '\n';

	EXPECT_EQ(fmted_name_msvc, fmted_name_gcc);
	EXPECT_EQ(fmted_name_msvc, fmted_name_clang);
	EXPECT_EQ(fmted_name_gcc, fmted_name_clang);
}

template<bool Which>
struct foo1 {
	template<class T1>
	struct foo2 {
		template<class T2>
		struct bar {
			using type = std::conditional_t<Which, T1, T2>;
		};
	};
};
TEST(Serialization, TypeDescriptor) {
	using f2 = foo1<true>::foo2<int>;
	using b1 = f2::bar<long double>;
	using b2 = foo1<false>::foo2<long>::bar<long double>;

	auto&& bar_td1 = typedesc(b1::type);
	auto&& bar_td2 = typedesc(b2::type);

	EXPECT_EQ(bar_td1.name(), "int");
	EXPECT_EQ(bar_td2.name(), "long double");

	EXPECT_EQ(bar_td1.hash(), arch::ser::hash("int"));
	EXPECT_EQ(bar_td2.hash(), arch::ser::hash("long double"));
}

struct foo : public arch::ser::ISerializable {
	float val1;
	bool val2;
	int val3;

	foo() = default;
	foo(float v1, bool v2, int v3) {
		val1 = v1;
		val2 = v2;
		val3 = v3;
	}
	bool operator==(const foo& x) const {
		return val1 == x.val1 and val2 == x.val2 and val3 == x.val3;
	}

	DEFAULT_SERIALIZE_DECL();
};

DEFAULT_SERIALIZE_OUTSIDE(foo);

DEFAULT_DESERIALIZE(foo);

TEST(Serialization, Concepts) {
	constexpr bool serializable = arch::ser::Serializable<foo>;
	constexpr bool deserializable = arch::ser::Deserializable<foo>;
	constexpr bool fully_serializable = arch::ser::FullySerializable<foo>;

	EXPECT_TRUE(serializable);
	EXPECT_TRUE(deserializable);
	EXPECT_TRUE(fully_serializable);
}

TEST(Serialization, RegisterDeserializer) {
	//std::cout << typedesc(foo).name() << '\n';
	EXPECT_EQ(arch::ser::deserializer_map::get(typedesc(foo)), &arch::ser::deserialize<foo>);
}

TEST(Serialization, SerializeDeserialize) {
	foo f(13.37f, false, 420);
	auto s = f.serialize();
	auto ds = s.deserialize_to<foo>();
	EXPECT_EQ(f, *ds);
}