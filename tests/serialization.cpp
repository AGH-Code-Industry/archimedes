#include <gtest/gtest.h>
#include <serialization.hpp>
#include <template_utilities.hpp>
#include <net.hpp>
#include <source_location>

TEST(Serialization, NameOfTypeFormatting) {
	// kur³a jak to sformatuje to wszystko sformatuje
	constexpr auto name_msvc = R"raw_name(struct __ct_str_t::__ct_str_to_t<char,struct __ct_str_t::ct_str<char,7>{char{97,109,111,103,117,115,0}},struct __ct_str_t::v_str<char,struct __ct_str_t::ct_c<char,97>,struct __ct_str_t::ct_c<char,109>,struct __ct_str_t::ct_c<char,111>,struct __ct_str_t::ct_c<char,103>,struct __ct_str_t::ct_c<char,117>,struct __ct_str_t::ct_c<char,115>,struct __ct_str_t::ct_c<char,0> > >)raw_name";
	constexpr auto name_gcc = R"raw_name(__ct_str_t::__ct_str_to_t<char, __ct_str_t::ct_str<char, 7>{"amogus"}, __ct_str_t::v_str<char, __ct_str_t::ct_c<char, 'a'>, __ct_str_t::ct_c<char, 'm'>, __ct_str_t::ct_c<char, 'o'>, __ct_str_t::ct_c<char, 'g'>, __ct_str_t::ct_c<char, 'u'>, __ct_str_t::ct_c<char, 's'>, __ct_str_t::ct_c<char, '\000'> > >)raw_name";
	constexpr auto name_clang = R"raw_name(__ct_str_t::__ct_str_to_t<char, __ct_str_t::ct_str<char, 7>{"amogus"}, __ct_str_t::v_str<char, __ct_str_t::ct_c<char, 'a'>, __ct_str_t::ct_c<char, 'm'>, __ct_str_t::ct_c<char, 'o'>, __ct_str_t::ct_c<char, 'g'>, __ct_str_t::ct_c<char, 'u'>, __ct_str_t::ct_c<char, 's'>, __ct_str_t::ct_c<char, '\x00'>>>)raw_name";

	auto fmted_name_msvc = arch::tmpl_utils::nameoftype_fmt(name_msvc);
	auto fmted_name_gcc = arch::tmpl_utils::nameoftype_fmt(name_gcc);
	auto fmted_name_clang = arch::tmpl_utils::nameoftype_fmt(name_clang);
	
	EXPECT_EQ(fmted_name_msvc, fmted_name_gcc);
	EXPECT_EQ(fmted_name_msvc, fmted_name_clang);
	EXPECT_EQ(fmted_name_gcc, fmted_name_clang);
}