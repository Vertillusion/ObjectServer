#include "Object.h"
#include <functional>
#include <random>
#include <stdexcept>

std::string _random() {
	std::random_device randomDevice;
	std::mt19937 gen(randomDevice());
	std::uniform_int_distribution<> dis(0, 999999999);
	return std::to_string(dis(gen));
}

/// <summary>
/// 获取类型
/// </summary>
std::string ObjectMember::getType() const {
	if (value[0] >= '0' && value[0] <= '9') return "int";
	else return "string";
}

ObjectMember::ObjectMember() { }
ObjectMember::ObjectMember(std::string value)
	: value(value) { }

enum TokenType {
	Identifier = 1,
	LeftCurlyBracket,
	RightCurlyBracket,
	Colon,
	Comma,
	EndOfFile,
	// Int 和 String 都属于 Value.
	Int = 0x1000,
	String = 0x0100,
	Value = 0x1100,
};

struct Token {
	int type;
	int pos;
	int end;
	std::string text;
};

class ObjectParser {
public:
	ObjectParser(std::string source)
		: _source(source), _end(source.length()) {
	}

	/// <summary>
	/// Syntax:
	/// 
	/// object = identifier '{' members '}'
	/// members = member ',' members | member ',' | member
	/// member = identifier ':' value
	/// value = number | string
	/// </summary>
	Object parse() {
		nextToken();
		assert(TokenType::Identifier);
		_object.objectName = _token.text;

		nextToken();
		assert(TokenType::LeftCurlyBracket);

		parseMembers();

		return _object;
	}

private:
	std::string _source;
	int _pos = 0;
	int _end;
	Token _token;
	Object _object;

	/// <summary>
	/// members = member ',' members | member ',' | member
	/// member = identifier ':' value
	/// </summary>
	void parseMembers() {
		bool hasId = false;
		while (true) {
			nextToken();
			if (_token.type == TokenType::RightCurlyBracket) break;

			assert(TokenType::Identifier);
			std::string memberName = _token.text;
			if (memberName == "id") hasId = true;

			nextToken();
			assert(TokenType::Colon);

			nextToken();
			assert(TokenType::Value);

			_object.members[memberName] = _token.text;

			nextToken();
			if (_token.type == TokenType::RightCurlyBracket) break;

			assert(TokenType::Comma);
		}
		// Generate ID
		if (!hasId) {
			_object.members["id"] = _random();
		}
	}

	/// <summary>
	/// Read next token
	/// </summary>
	void nextToken() {
		while (true) {
			if (_pos >= _end) {
				_token = Token(TokenType::EndOfFile, _pos, _pos, "");
				return;
			}

			char const ch = _source[_pos];
			switch (ch) {
			case '{':
				_token = Token(TokenType::LeftCurlyBracket, _pos, ++_pos, std::string(1, ch));
				return;
			case '}':
				_token = Token(TokenType::RightCurlyBracket, _pos, ++_pos, std::string(1, ch));
				return;
			case ':':
				_token = Token(TokenType::Colon, _pos, ++_pos, std::string(1, ch));
				return;
			case ',':
				_token = Token(TokenType::Comma, _pos, ++_pos, std::string(1, ch));
				return;
			case '"':
				_token = scanString();
				return;


			case ' ':
			case '\n':
			case '\r':
				++_pos;
				continue;

			default:
				if (std::isalpha(ch)) {
					_token = scanIdentifier();
				} else if (std::isdigit(ch)) {
					_token = scanNumber();
				}
				return;
			}
		}
	}

	Token scanIdentifier() {
		int const identifierStart = _pos;
		std::string identifier(1, _source[_pos++]);

		while (true) {
			if (_pos >= _end) break;
			char const ch = _source[_pos];
			if (ch == ' ' || ch == ':' || ch == '{' || ch == '"') break;
			identifier += ch;
			++_pos;
		}

		return Token(TokenType::Identifier, identifierStart, _pos, identifier);
	}

	Token scanNumber() {
		int const numberStart = _pos;
		std::string number(1, _source[_pos++]);

		while (true) {
			if (_pos >= _end) break;
			char const ch = _source[_pos];
			if (!(ch >= '0' && ch <= '9')) break;
			number += ch;
			++_pos;
		}

		return Token(TokenType::Int, numberStart, _pos, number);
	}

	Token scanString() {
		int const stringStart = _pos;
		std::string str(1, _source[_pos++]);

		while (true) {
			if (_pos >= _end) break;
			char const ch = _source[_pos];
			if (ch == '"') break;
			str += ch;
			++_pos;
		}

		str += '"';
		if (_source[_pos++] != '"') throw std::runtime_error{ "Unexpected token." };

		return Token(TokenType::String, stringStart, _pos, str);
	}

	void assert(int expectedToken) {
		if ((expectedToken & _token.type) == 0) {
			throw std::runtime_error{ "Unexpected token." };
		}
	}
};

// https://github.com/thzt/web.frontend.component/blob/master/library/list-parser/parser.js
Object Object::parse(std::string source) {
	ObjectParser parser(source);
	return parser.parse();
}

std::string Object::serialize() {
	std::string result = objectName + "{";
	for (auto const& member : members) {
		result += member.first + ":" + member.second.value + ",";
	}
	result += "}";

	return result;
}

std::string Object::description() const {
	std::string desc = objectName + "{";
	for (auto const& member : members) {
		desc += member.first + " " + member.second.getType() + ",";
	}
	desc += "}";
	return desc;
}