#include <utilities/nameoftype/nameoftype_fmt.hpp>

namespace arch::utils {
enum token_type {
	white, // spaces
	str, // string literals
	str_end, // end of string literal
	ch, // character literals
	ch_end, // end of character literal
	force_str, // force char in string literal
	force_ch, // force char in char literal
	alpha, // symbols
	num, // numerical literals
	flt_exp, // exponential float
	punct, // punctuation	
};

const char* token_to_str(token_type t) {
	switch (t) {
		case white:		return "white"; break;
		case str:		return "str"; break;
		case str_end:	return "str_end"; break;
		case ch:		return "ch"; break;;
		case ch_end:	return "ch_end"; break;
		case force_str:	return "force_str"; break;
		case force_ch:	return "force_ch"; break;
		case alpha:		return "alpha"; break;
		case num:		return "num"; break;
		case punct:		return "punct"; break;
		default:		return ""; break;
	}
};

// returns type of token given character belongs to within given context
// return values: white, ch, ch_end, force_ch, str, str_end, force_str, num, alpha, punct
token_type get_token_type(char c, token_type context) {
	if (isspace(c)) {
		// 1. separator of tokens
		// 2. inside char literal
		// 3. inside string literal
		if (context == white or context == alpha or context == num or context == punct) { // 1
			return white;
		}
		else if (context == force_ch) { // 2
			return ch;
		}
		else if (context == force_str) { // 3
			return str;
		}
		else { // 2, 3
			return context;
		}
	}
	else if (ispunct(c) and c != '_') {
		if (c == '"') { // double quote character
			// 1. beginning of string literal
			// 2. ending of string literal
			// 3. forced char in string literal
			// 4. char in char literal
			if (context == ch or context == force_ch) { // 4
				return ch;
			}
			else if (context == str) { // 2
				return str_end;
			}
			else { // 1, 3
				return str;
			}
		}
		else if (c == '\'') { // quote character
			// 1. beginning of char literal
			// 2. ending of char literal
			// 3. forced char in char literal
			// 4. char in string literal
			// 5. separator in numerical literal
			if (context == str or context == force_str) { // 4
				return str;
			}
			else if (context == ch) { // 2
				return ch_end;
			}
			else if (context == num) { // 5
				return num;
			}
			else { // 1, 3
				return ch;
			}
		}
		else if (c == '\\') {
			if (context == str) {
				return force_str;
			}
			else if (context == ch) {
				return force_ch;
			}
			else if (context == force_str) {
				return str;
			}
			else if (context == force_ch) {
				return ch;
			}
		}
		else if (c == '+' or c == '-' and context == flt_exp) {
			return num;
		}
		if (c == '.' and context == num) {
			return num;
		}
		else if (context == white or context == alpha or context == punct) {
			return punct;
		}
		else if (context == str or context == force_str) {
			return str;
		}
		else if (context == ch or context == force_ch) {
			return ch;
		}
		else {
			return white;
		}
	}
	else if (isalpha(c) or c == '_') {
		// 1. inside string
		// 2. inside char
		// 3. numerical literal
		// 4. symbols
		if (context == str or context == force_str) { // 1
			return str;
		}
		else if (context == ch or context == force_ch) { // 2
			return ch;
		}
		else if (context == num) { // 3
			if (c == 'e' or c == 'E' or c == 'p' or c == 'P') {
				return flt_exp;
			}
			return num;
		}
		else { // 4
			return alpha;
		}
	}
	else if (isdigit(c)) {
		// 1. in string
		// 2. in char
		// 3. in symbol
		// 4. in numerical string
		if (context == str or context == force_str) { // 1
			return str;
		}
		else if (context == ch or context == force_ch) { // 2
			return ch;
		}
		else if (context == alpha) {
			return alpha;
		}
		else {
			return num;
		}
	}
	else {
		return white;
	}
}

struct current_token_t { size_t begin; size_t count; };

void parse_tokens(std::string_view& name, std::vector<std::string_view>& tokens) {
	token_type context = white;
	current_token_t current_token{0, 0};

	auto save_token = [&]() {
		tokens.emplace_back(name.substr(current_token.begin, current_token.count));
		current_token.begin += current_token.count;
		current_token.count = 0;
		context = white;
	};

	for (size_t i = 0; i != name.length(); ++i) {
		token_type type = get_token_type(name[i], context);
		switch (context) { // context -> symbol
			case white: // between tokens
				switch (type) { // new symbol
					case alpha:
						context = alpha;
						current_token.begin = i;
						current_token.count = 1;
						break;
					case punct:
						context = punct;
						current_token.begin = i;
						current_token.count = 1;
						break;
					case str:
						context = str;
						current_token.begin = i;
						current_token.count = 1;
						break;
					case ch:
						context = ch;
						current_token.begin = i;
						current_token.count = 1;
						break;
					case num:
						context = num;
						current_token.begin = i;
						current_token.count = 1;
						break;
					case white:
					default:
						break;
				}
				break;
			case str:
				switch (type) {
					case str:
						++current_token.count;
						break;
					case str_end:
						++current_token.count;
						save_token();
						break;
					case force_str:
						++current_token.count;
						context = force_str;
						break;
				}
				break;
			case ch:
				switch (type) {
					case ch:
						++current_token.count;
						break;
					case ch_end:
						++current_token.count;
						save_token();
						break;
					case force_ch:
						++current_token.count;
						context = force_ch;
						break;
				}
				break;
			case force_str:
				++current_token.count;
				context = str;
				break;
			case force_ch:
				++current_token.count;
				context = ch;
				break;
			case num:
				switch (type) {
					case num:
						++current_token.count;
						break;
					case flt_exp:
						++current_token.count;
						context = flt_exp;
						break;
					default:
						save_token();
						--i;
						break;
				}
				break;
			case flt_exp:
				switch (type) {
					case num:
						++current_token.count;
						context = num;
						break;
				}
			case punct:
				switch (type) {
					case punct:
						++current_token.count;
						break;
					default:
						save_token();
						--i;
						break;
				}
				break;
			case alpha:
				switch (type) {
					case alpha:
						++current_token.count;
						break;
					default:
						save_token();
						--i;
						break;
				}
				break;
		}
	}
	if (current_token.count != 0) {
		save_token();
	}
}

bool specifier_erase_pred(const std::string_view& x) {
	return x == "struct" or x == "class" or x == "enum" or x == "constexpr" or x == "constinit" or x == "consteval";
}

std::string_view multitoken_to_token(std::multiset<std::string_view>& multitoken) {
	if (multitoken.contains("char")) {
		if (multitoken.contains("unsigned")) {
			return "unsigned char";
		}
		else if (multitoken.contains("signed")) {
			return "signed char";
		}
		else {
			return "char";
		}
	}
	else if (multitoken.contains("short")) {
		if (multitoken.contains("unsigned")) {
			return "unsigned short";
		}
		else {
			return "short";
		}
	}
	else if (multitoken.contains("long")) {
		if (multitoken.count("long") == 1) {
			if (multitoken.contains("double")) {
				return "long double";
			}
			else if (multitoken.contains("unsigned")) {
				return "unsigned long";
			}
			else {
				return "long";
			}
		}
		else { // count == 2
			if (multitoken.contains("unsigned")) {
				return "unsigned long long";
			}
			else {
				return "long long";
			}
		}
	}
	else {
		if (multitoken.contains("unsigned")) {
			return "unsigned int";
		}
		else {
			return "int";
		}
	}
}

void fmt_numeric(std::string_view& token, std::string& to_return) {
	std::string fmted_token;
	if (token.find('.') != token.npos) { // floating point literal
		if (token.ends_with('f') or token.ends_with('F')) { // float
			fmted_token = token;
			fmted_token = std::to_string(std::stof(fmted_token));
		}
		else if (token.ends_with('l') or token.ends_with('L')) { // long double
			fmted_token = token;
			fmted_token = std::to_string(std::stold(fmted_token));
		}
		else { // double literal
			fmted_token = token;
			fmted_token = std::to_string(std::stod(fmted_token));
		}
	}
	else { // integer literal
		fmted_token = token;
		fmted_token = std::to_string(std::stoull(fmted_token));
	}

	to_return += fmted_token;
}

void fmt_string(std::vector<std::string_view>& tokens, size_t i, std::string_view& token, std::string& to_return) {
	if (i != 0) {
		if (tokens[i - 1] == "L") {
			to_return.pop_back();
			to_return += "wchar_t{";
		}
		else if (tokens[i - 1] == "u8") {
			to_return.pop_back();
			to_return.pop_back();
			to_return += "char8_t{";
		}
		else if (tokens[i - 1] == "u") {
			to_return.pop_back();
			to_return += "char16_t{";
		}
		else if (tokens[i - 1] == "U") {
			to_return.pop_back();
			to_return += "char32_t{";
		}
		else {
			goto __add_char;
		}
	}
	else {
	__add_char:
		to_return += "char{";
	}
	for (size_t j = 1; j != token.length() - 1; ++j) {
		if (token[j] == '\\') {
			std::string temp;
			size_t processed;
			switch (token[j + 1]) {
				case '\'':
					to_return += std::to_string(0x27) + ',';
					break;
				case '\"':
					to_return += std::to_string(0x22) + ',';
					break;
				case '?':
					to_return += std::to_string(0x3f) + ',';
					break;
				case '\\':
					to_return += std::to_string(0x5c) + ',';
					break;
				case 'a':
					to_return += std::to_string(0x07) + ',';
					break;
				case 'b':
					to_return += std::to_string(0x08) + ',';
					break;
				case 'f':
					to_return += std::to_string(0x0c) + ',';
					break;
				case 'n':
					to_return += std::to_string(0x0a) + ',';
					break;
				case 'r':
					to_return += std::to_string(0x0d) + ',';
					break;
				case 't':
					to_return += std::to_string(0x09) + ',';
					break;
				case 'v':
					to_return += std::to_string(0x0b) + ',';
					break;
				case 'e':
					to_return += std::to_string(0x1b) + ',';
					break;
				case 'x': // hex literal
					temp = token.substr(j + 2);
					to_return += std::to_string(std::stoull(temp, &processed, 16)) + ',';
					j += processed + 1;
					break;
				case 'u': // unicode 4 digits
					temp = token.substr(j + 2, 4);
					to_return += std::to_string(std::stoull(temp, nullptr, 16)) + ',';
					j += 4 + 1;
					break;
				case 'U': // unicode 8 digits
					temp = token.substr(j + 2, 8);
					to_return += std::to_string(std::stoull(temp, nullptr, 16)) + ',';
					j += 8 + 1;
					break;
				default: // octal literal
					temp = token.substr(j + 1, 3);
					to_return += std::to_string(std::stoull(temp, &processed, 8)) + ',';
					j += processed;
					break;
			}
			++j;
		}
		else {
			to_return += std::to_string((int)token[j]) + ',';
		}
	}
	to_return += "0}";
}

void fmt_char(std::string_view& token, std::string& to_return) {
	for (size_t j = 1; j < token.length() - 1; ++j) {
		if (token[j] == '\\') {
			std::string temp;
			size_t processed;
			switch (token[j + 1]) {
				case '\'':
					to_return += 0x27;
					break;
				case '\"':
					to_return += 0x22;
					break;
				case '?':
					to_return += 0x3f;
					break;
				case '\\':
					to_return += 0x5c;
					break;
				case 'a':
					to_return += 0x07;
					break;
				case 'b':
					to_return += 0x08;
					break;
				case 'f':
					to_return += 0x0c;
					break;
				case 'n':
					to_return += 0x0a;
					break;
				case 'r':
					to_return += 0x0d;
					break;
				case 't':
					to_return += 0x09;
					break;
				case 'v':
					to_return += 0x0b;
					break;
				case 'e':
					to_return += 0x1b;
					break;
				case 'x': // hex literal
					temp = token.substr(j + 2);
					to_return += std::to_string(std::stoull(temp, &processed, 16));
					j += processed + 1;
					break;
				case 'u': // unicode 4 digits
					temp = token.substr(j + 2, 4);
					to_return += std::to_string(std::stoull(temp, nullptr, 16));
					j += 4 + 1;
					break;
				case 'U': // unicode 8 digits
					temp = token.substr(j + 2, 8);
					to_return += std::to_string(std::stoull(temp, nullptr, 16));
					j += 8 + 1;
					break;
				default: // octal literal
					temp = token.substr(j + 1, 3);
					to_return += std::to_string(std::stoull(temp, &processed, 8));
					j += processed;
					break;
			}
			++j;
		}
		else {
			to_return += std::to_string((int)token[j]);
		}
	}
}

std::string final_fmt(std::vector<std::string_view>& tokens) {
	std::string to_return;
	std::multiset<std::string_view> multitoken;
	for (size_t i = 0; i != tokens.size(); ++i) { // format literals
		auto&& token = tokens[i];
		if (isdigit(token[0])) { // format numerical literal
			fmt_numeric(token, to_return);
		}
		else if (token[0] == '"') { // string literal
			fmt_string(tokens, i, token, to_return);
		}
		else if (token[0] == '\'') { // char literal
			fmt_char(token, to_return);
		}
		else if (token == "true") {
			to_return += '1';
		}
		else if (token == "false") {
			to_return += '0';
		}
		else if (token == "signed" or
				 token == "unsigned" or
				 token == "char" or
				 token == "short" or
				 token == "int" or
				 token == "long" or
				 token == "double") {

			multitoken.insert(token);
		}
#ifdef _MSC_VER // integer aliases used by MSVC
		else if (token == "__int8") {
			multitoken.insert("char");
		}
		else if (token == "__int16") {
			multitoken.insert("short");
		}
		else if (token == "__int32") {
			multitoken.insert("int");
		}
		else if (token == "__int64") {
			multitoken.insert("long");
			multitoken.insert("long");
		}
#endif
		else {
			if (not multitoken.empty()) {
				to_return += multitoken_to_token(multitoken);
				multitoken.clear();
			}
			to_return += token;
		}
	}
	if (not multitoken.empty()) {
		to_return += multitoken_to_token(multitoken);
		multitoken.clear();
	}

	return to_return;
}

std::string nameoftype_fmt(std::string_view name) {
	std::vector<std::string_view> tokens;

	parse_tokens(name, tokens);

	std::erase_if(tokens, specifier_erase_pred);

	return final_fmt(tokens);
}
}