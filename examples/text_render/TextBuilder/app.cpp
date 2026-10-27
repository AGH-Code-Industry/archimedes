#include "app.h"

#include <chrono>
#include <print>

#include <archimedes/Text.h>
#include <unilbrk.h>
#include <unistr.h>

using namespace arch;

class EndException: public arch::Exception {
public:
	EndException(std::string_view message, const std::stacktrace& stacktrace = std::stacktrace::current()):
		Exception("Nothing", message, stacktrace) {
	}
};

void TestApp::init() {
	// text::TextBuilder builder;

	// auto&& fontDB = font::FontDB::get();
	// fontDB["Arial"]->regular()->assure();

	constexpr const char32_t test[] = U"Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
									  "Praesent commodo cursus magna, vel scelerisque nisl consectetur et. "
									  "Nullam id dolor id nibh ultricies vehicula ut id elit. "
									  "Integer posuere erat a ante venenatis dapibus posuere velit aliquet.\n"
									  "\n"
									  "Cras mattis consectetur purus sit amet fermentum. "
									  "Donec sed odio dui. Etiam porta sem malesuada magna mollis euismod. "
									  "Aenean lacinia bibendum nulla sed consectetur.\n"
									  "\n"
									  "Niektóre słowa zawierają polskie znaki: zażółć gęślą jaźń, "
									  "Pchnąć w tę łódź jeża lub ośm skrzyń fig. "
									  "To pozwala sprawdzić obsługę znaków spoza podstawowego ASCII.\n"
									  "\n"
									  "Unicode: café, naïve, coöperate, résumé, München, français, "
									  "Ελληνικά, Русский, 中文, 日本語, 한국어, العربية, עברית.\n"
									  "\n"
									  "Spacje: pojedyncza,  podwójna,   potrójna. "
									  "Tabulator:\tmiędzy słowami. "
									  "Spacja nierozdzielająca: foo bar. "
									  "Zero-width space: foo​bar. "
									  "Soft hyphen: antidisestab­lishmentarian.\n"
									  "\n"
									  "Interpunkcja: słowo,słowo; słowo:słowo! "
									  "Czy pytanie? Tak — odpowiedź. "
									  "(nawiasy) [kwadratowe] {klamrowe} <ostre>. "
									  "(begin end) [begin end] {begin end} <begin end>. "
									  "\"Cudzysłów\" 'apostrof' … wielokropek — pauza – półpauza.\n"
									  "\n"
									  "Liczby: 123 456, 3.14159, 1,000.00, 2026-09-16, "
									  "12:34:56, 100%, €99.99, $42.50, 10², H₂O.\n"
									  "\n"
									  "Łączenie: state-of-the-art, well-known, mother-in-law, "
									  "foo_bar, snake_case, camelCase, C++, C#, UTF-8, U+00A0.\n"
									  "\n"
									  "Znaki specjalne: © ® ™ § ¶ † ‡ • ° ± × ÷ ≠ ≤ ≥ ∞ "
									  "→ ← ↑ ↓ ⇒ ⇐ ↔.\n"
									  "\n"
									  "Brak spacji: ToJestDługiCiągSłówKtóryNieZawieraŻadnychSpacji "
									  "iPowinienByćTraktowanyJakoJedenCiągZnaków.\n"
									  "\n"
									  "Koniec tekstu.";

	std::array<char, sizeof(test) / sizeof(char32_t)> breaks{};
	u32_possible_linebreaks(reinterpret_cast<const uint32_t*>(test), breaks.size(), "UTF-32", breaks.data());
	for (auto&& [i, c] : std::views::enumerate(test)) {
		auto xd = text::convertTo<char>(std::u32string_view(&c, 1));

		if (i < breaks.size() && breaks[i + 1] == UC_BREAK_PROHIBITED) {
			std::print("{}", xd);
		} else {
			std::println("{}", xd);
		}
	}
	std::println();

	// clang-format off
	/*builder
		.face(fontDB["Arial"]->regular())
		.text(test);
	auto xd = builder
		._processText();*/

	// clang-format on

	throw EndException("nothing", {});
}

void TestApp::update() {
}
