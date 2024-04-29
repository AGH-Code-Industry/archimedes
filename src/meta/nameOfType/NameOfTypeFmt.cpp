#include <set>
#include <string>
#include <vector>

#include <meta/nameOfType/NameOfTypeFmt.h>

namespace arch::meta::nameOfType {

/// @brief Enum with types of tokens present in typeOf<T>() result
enum TokenType {
	white, // spaces
	str, // string literals
	strEnd, // end of string literal
	ch, // character literals
	chEnd, // end of character literal
	forceStr, // force char in string literal
	forceCh, // force char in char literal
	alpha, // symbols
	num, // numerical literals
	fltExp, // exponential float
	punct, // punctuation
};

/// @brief Returns TokenType of character c, depending on context
/// @param c - character to check
/// @param context - current TokenType
TokenType getTokenType(char c, TokenType context) {
	if (isspace(c)) {
		// 1. separator of tokens
		// 2. inside char literal
		// 3. inside string literal
		if (context == white or context == alpha or context == num or context == punct) { // 1
			return white;
		} else if (context == forceCh) { // 2
			return ch;
		} else if (context == forceStr) { // 3
			return str;
		} else { // 2, 3
			return context;
		}
	} else if (ispunct(c) and c != '_') {
		if (c == '"') { // double quote character
			// 1. beginning of string literal
			// 2. ending of string literal
			// 3. forced char in string literal
			// 4. char in char literal
			if (context == ch or context == forceCh) { // 4
				return ch;
			} else if (context == str) { // 2
				return strEnd;
			} else { // 1, 3
				return str;
			}
		} else if (c == '\'') { // quote character
			// 1. beginning of char literal
			// 2. ending of char literal
			// 3. forced char in char literal
			// 4. char in string literal
			// 5. separator in numerical literal
			if (context == str or context == forceStr) { // 4
				return str;
			} else if (context == ch) { // 2
				return chEnd;
			} else if (context == num) { // 5
				return num;
			} else { // 1, 3
				return ch;
			}
		} else if (c == '\\') {
			if (context == str) {
				return forceStr;
			} else if (context == ch) {
				return forceCh;
			} else if (context == forceStr) {
				return str;
			} else if (context == forceCh) {
				return ch;
			}
		} else if (c == '+' or c == '-' and context == fltExp) {
			return num;
		}
		if (c == '.' and context == num) {
			return num;
		} else if (context == white or context == alpha or context == punct) {
			return punct;
		} else if (context == str or context == forceStr) {
			return str;
		} else if (context == ch or context == forceCh) {
			return ch;
		} else {
			return white;
		}
	} else if (isalpha(c) or c == '_') {
		// 1. inside string
		// 2. inside char
		// 3. numerical literal
		// 4. symbols
		if (context == str or context == forceStr) { // 1
			return str;
		} else if (context == ch or context == forceCh) { // 2
			return ch;
		} else if (context == num) { // 3
			if (c == 'e' or c == 'E' or c == 'p' or c == 'P') {
				return fltExp;
			}
			return num;
		} else { // 4
			return alpha;
		}
	} else if (isdigit(c)) {
		// 1. in string
		// 2. in char
		// 3. in symbol
		// 4. in numerical string
		if (context == str or context == forceStr) { // 1
			return str;
		} else if (context == ch or context == forceCh) { // 2
			return ch;
		} else if (context == alpha) {
			return alpha;
		} else {
			return num;
		}
	} else {
		return white;
	}
}

/// @brief Postition indicator of token
struct CurrentTokenT {
	size_t begin;
	size_t count;
};

/// @brief Parses given type name into tokens
/// @param name - type name to parse
/// @param tokens - vector to save tokens to
void parseTokens(std::string_view& name, std::vector<std::string_view>& tokens) {
	TokenType context = white;
	CurrentTokenT currentToken{ 0, 0 };

	// saves current token to vector and resets currentToken
	auto saveToken = [&]() {
		tokens.emplace_back(name.substr(currentToken.begin, currentToken.count));
		currentToken.begin += currentToken.count;
		currentToken.count = 0;
		context = white;
	};

	for (size_t i = 0; i != name.length(); ++i) {
		TokenType type = getTokenType(name[i], context);
		switch (context) { // context -> symbol
			case white: // between tokens
				switch (type) { // new symbol
					case alpha:
					case punct:
					case str:
					case ch:
					case num:
						context = type;
						currentToken.begin = i;
						currentToken.count = 1;
						break;
				}
				break;
			case str:
				++currentToken.count;
				switch (type) {
					case strEnd:   saveToken(); break;
					case forceStr: context = forceStr; break;
				}
				break;
			case ch:
				++currentToken.count;
				switch (type) {
					case chEnd:	  saveToken(); break;
					case forceCh: context = forceCh; break;
				}
				break;
			case forceStr:
				++currentToken.count;
				context = str;
				break;
			case forceCh:
				++currentToken.count;
				context = ch;
				break;
			case num:
				switch (type) {
					case num: ++currentToken.count; break;
					case fltExp:
						++currentToken.count;
						context = fltExp;
						break;
					default:
						saveToken();
						--i;
						break;
				}
				break;
			case fltExp:
				switch (type) {
					case num:
						++currentToken.count;
						context = num;
						break;
				}
			case punct:
				switch (type) {
					case punct: ++currentToken.count; break;
					default:
						saveToken();
						--i;
						break;
				}
				break;
			case alpha:
				switch (type) {
					case alpha: ++currentToken.count; break;
					default:
						saveToken();
						--i;
						break;
				}
				break;
		}
	}
	if (currentToken.count != 0) {
		saveToken();
	}
}

/// @brief Returns true if token is one of the following:
/// @brief struct, class, enum, constexpr, constinit, consteval
/// @param token - token to check
bool specifierErasePred(const std::string_view& token) {
	return token == "struct" or token == "class" or token == "enum" or token == "constexpr" or token == "constinit" or
		token == "consteval";
}

#define _HANDLE_CV_IMPL(...)                                                                        \
	(multitoken.contains("const") ?                                                                 \
		 (multitoken.contains("volatile") ? "const volatile " __VA_ARGS__ : "const " __VA_ARGS__) : \
		 (multitoken.contains("volatile") ? "volatile " __VA_ARGS__ : "" __VA_ARGS__))

#define _HANDLE_CV(...) _HANDLE_CV_IMPL(__VA_ARGS__)

/// @brief Standardizes order of combination of tokens that form one type
/// @details C++ standard says that ex. unsigned long long == long unsigned long
/// @param multitoken - multiset of tokens to standardize
/// @return Tokens in standardized order
std::string_view multitokenToToken(std::multiset<std::string_view>& multitoken) {
	if (multitoken.contains("char")) {
		if (multitoken.contains("unsigned")) {
			return _HANDLE_CV("unsigned char");
		} else if (multitoken.contains("signed")) {
			return _HANDLE_CV("signed char");
		} else {
			return _HANDLE_CV("char");
		}
	} else if (multitoken.contains("short")) {
		if (multitoken.contains("unsigned")) {
			return _HANDLE_CV("unsigned short");
		} else {
			return _HANDLE_CV("short");
		}
	} else if (multitoken.contains("long")) {
		if (multitoken.count("long") == 1) {
			if (multitoken.contains("double")) {
				return _HANDLE_CV("long double");
			} else if (multitoken.contains("unsigned")) {
				return _HANDLE_CV("unsigned long");
			} else {
				return _HANDLE_CV("long");
			}
		} else { // count == 2
			if (multitoken.contains("unsigned")) {
				return _HANDLE_CV("unsigned long long");
			} else {
				return _HANDLE_CV("long long");
			}
		}
	} else if (multitoken.contains("int")) {
		if (multitoken.contains("unsigned")) {
			return _HANDLE_CV("unsigned int");
		} else {
			return _HANDLE_CV("int");
		}
	} else if (multitoken.contains("const")) {
		if (multitoken.contains("volatile")) {
			return "const volatile";
		} else {
			return "const";
		}
	} else if (multitoken.contains("volatile")) {
		return "volatile";
	} else {
		return "";
	}
}

/// @brief Formats numerical values
/// @param token - numerical value to format
/// @param toReturn - string to append formatted value to
void fmtNumeric(std::string_view& token, std::string& toReturn) {
	std::string formattedToken;
	if (token.find('.') != token.npos) { // floating point literal
		if (token.ends_with('f') or token.ends_with('F')) { // float
			formattedToken = token;
			formattedToken = std::to_string(std::stof(formattedToken));
		} else if (token.ends_with('l') or token.ends_with('L')) { // long double
			formattedToken = token;
			formattedToken = std::to_string(std::stold(formattedToken));
		} else { // double
			formattedToken = token;
			formattedToken = std::to_string(std::stod(formattedToken));
		}
	} else { // integer literal
		formattedToken = token;
		formattedToken = std::to_string(std::stoull(formattedToken));
	}

	toReturn += formattedToken;
}

/// @brief Formats string literal
/// @param tokens - vector of all tokens
/// @param i - index of current token
/// @param token - current token
/// @param toReturn - string to append formatted string to
void fmtString(std::vector<std::string_view>& tokens, size_t i, std::string_view& token, std::string& toReturn) {
	// processing literal type
	if (i != 0) {
		if (tokens[i - 1] == "L") {
			toReturn.pop_back();
			toReturn += "wchar_t{";
		} else if (tokens[i - 1] == "u8") {
			toReturn.pop_back();
			toReturn.pop_back();
			toReturn += "char8_t{";
		} else if (tokens[i - 1] == "u") {
			toReturn.pop_back();
			toReturn += "char16_t{";
		} else if (tokens[i - 1] == "U") {
			toReturn.pop_back();
			toReturn += "char32_t{";
		} else {
			goto __addChar;
		}
	} else {
__addChar:
		toReturn += "char{";
	}

	// appending chars
	for (size_t j = 1; j != token.length() - 1; ++j) {
		if (token[j] == '\\') { // escape sequence
			std::string temp;
			size_t processed;
			switch (token[j + 1]) {
				case '\'': toReturn += std::to_string(0x27) + ','; break;
				case '\"': toReturn += std::to_string(0x22) + ','; break;
				case '?':  toReturn += std::to_string(0x3f) + ','; break;
				case '\\': toReturn += std::to_string(0x5c) + ','; break;
				case 'a':  toReturn += std::to_string(0x07) + ','; break;
				case 'b':  toReturn += std::to_string(0x08) + ','; break;
				case 'f':  toReturn += std::to_string(0x0c) + ','; break;
				case 'n':  toReturn += std::to_string(0x0a) + ','; break;
				case 'r':  toReturn += std::to_string(0x0d) + ','; break;
				case 't':  toReturn += std::to_string(0x09) + ','; break;
				case 'v':  toReturn += std::to_string(0x0b) + ','; break;
				case 'e':  toReturn += std::to_string(0x1b) + ','; break;
				case 'x': // hex literal
					temp = token.substr(j + 2);
					toReturn += std::to_string(std::stoull(temp, &processed, 16)) + ',';
					j += processed + 1;
					break;
				case 'u': // unicode 4 digits
					temp = token.substr(j + 2, 4);
					toReturn += std::to_string(std::stoull(temp, nullptr, 16)) + ',';
					j += 4 + 1;
					break;
				case 'U': // unicode 8 digits
					temp = token.substr(j + 2, 8);
					toReturn += std::to_string(std::stoull(temp, nullptr, 16)) + ',';
					j += 8 + 1;
					break;
				default: // octal literal
					temp = token.substr(j + 1, 3);
					toReturn += std::to_string(std::stoull(temp, &processed, 8)) + ',';
					j += processed;
					break;
			}
			++j;
		} else {
			toReturn += std::to_string((int)token[j]) + ',';
		}
	}
	toReturn += "0}";
}

/// @brief Formats character literal
/// @param token - current token
/// @param toReturn - string to append formatted character to
void fmtChar(std::string_view& token, std::string& toReturn) {
	for (size_t j = 1; j < token.length() - 1; ++j) {
		if (token[j] == '\\') { // escape character
			std::string temp;
			size_t processed;
			switch (token[j + 1]) {
				case '\'': toReturn += 0x27; break;
				case '\"': toReturn += 0x22; break;
				case '?':  toReturn += 0x3f; break;
				case '\\': toReturn += 0x5c; break;
				case 'a':  toReturn += 0x07; break;
				case 'b':  toReturn += 0x08; break;
				case 'f':  toReturn += 0x0c; break;
				case 'n':  toReturn += 0x0a; break;
				case 'r':  toReturn += 0x0d; break;
				case 't':  toReturn += 0x09; break;
				case 'v':  toReturn += 0x0b; break;
				case 'e':  toReturn += 0x1b; break;
				case 'x': // hex literal
					temp = token.substr(j + 2);
					toReturn += std::to_string(std::stoull(temp, &processed, 16));
					j += processed + 1;
					break;
				case 'u': // unicode 4 digits
					temp = token.substr(j + 2, 4);
					toReturn += std::to_string(std::stoull(temp, nullptr, 16));
					j += 4 + 1;
					break;
				case 'U': // unicode 8 digits
					temp = token.substr(j + 2, 8);
					toReturn += std::to_string(std::stoull(temp, nullptr, 16));
					j += 8 + 1;
					break;
				default: // octal literal
					temp = token.substr(j + 1, 3);
					toReturn += std::to_string(std::stoull(temp, &processed, 8));
					j += processed;
					break;
			}
			++j;
		} else {
			toReturn += std::to_string((int)token[j]);
		}
	}
}

/// @brief Main formatting function
/// @param tokens - vector of tokens to format
/// @return Formatted type name
std::string finalFmt(std::vector<std::string_view>& tokens) {
	std::string toReturn;
	std::multiset<std::string_view> multitoken;
	for (size_t i = 0; i != tokens.size(); ++i) { // format literals
		auto&& token = tokens[i];
		if (isdigit(token[0])) { // format numerical literal
			fmtNumeric(token, toReturn);
		} else if (token[0] == '"') { // string literal
			fmtString(tokens, i, token, toReturn);
		} else if (token[0] == '\'') { // char literal
			fmtChar(token, toReturn);
		} else if (token == "true") {
			toReturn += '1';
		} else if (token == "false") {
			toReturn += '0';
		} else if (token == "signed" or token == "unsigned" or token == "char" or token == "short" or token == "int" or
				   token == "long" or token == "double" or token == "volatile" or token == "const") {
			multitoken.insert(token);
		}
#ifdef _MSC_VER // integer aliases used by MSVC
		else if (token == "__int8") {
			multitoken.insert("char");
		} else if (token == "__int16") {
			multitoken.insert("short");
		} else if (token == "__int32") {
			multitoken.insert("int");
		} else if (token == "__int64") {
			multitoken.insert("long");
			multitoken.insert("long");
		}
#endif
		else {
			if (not multitoken.empty()) {
				toReturn += multitokenToToken(multitoken);
				multitoken.clear();
			}
			toReturn += token;
		}
	}
	if (not multitoken.empty()) {
		toReturn += multitokenToToken(multitoken);
		multitoken.clear();
	}

	return toReturn;
}

std::string nameOfTypeFmt(std::string_view name) noexcept {
	std::vector<std::string_view> tokens;
	parseTokens(name, tokens);
	std::erase_if(tokens, specifierErasePred);
	return finalFmt(tokens);
}

} // namespace arch::meta::nameOfType
