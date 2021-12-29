#pragma once
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

enum class TYPE
{
	Id,
	IntNum,
	Int,
	ArrayInt,
	If,
	Else,
	While,
	Read,
	Write,
	LeftBrace,
	RightBrace,
	LeftSquareBracket,
	RightSquareBracket,
	LeftRoundBracket,
	RightRoundBracket,
	Plus,
	Minus,
	Multiply,
	Divide,
	Semicolon,
	Comma,
	Less,
	Assign,
	More,
	Equal,
	LessOrEqual,
	MoreOrEqual,
	NotEqual,
	EoF,
	Error
};

struct Lexeme
{
	int line, pos;
	TYPE _type;
	string value;
};

class LexicalAnalyzer {
public:
	void step1() 
	{
		Lexeme _lexeme;
		int _line = 1;
		int _pos = 1;
		while (_index < input_text.size()) {
			while (isspace(input_text[_index])) {
				switch (input_text[_index++]) {
				case ' ':
					++_pos;
					break;
				case '\n':
					++_line; _pos = 1;
					break;
				case '\t':
					_pos += 4;
					break;
				}
			}
			_lexeme = NextLex();
			_lexeme.line = _line;
			_lexeme.pos = _pos;

			if (_lexeme._type == TYPE::Error) {
				string msg = "Lexer: Значение = '" + _lexeme.value + "', стр = " + to_string(_line) + ", поз = " + to_string(_pos);
				throw runtime_error(msg);
			}

			_pos += _lexeme.value.size();

			data.push_back(_lexeme);
		}

		_lexeme._type = TYPE::EoF;
		_lexeme.line = _line;
		_lexeme.pos = _pos;
		data.push_back(_lexeme);
	}
	vector<Lexeme> GetLexData() 
	{
		return data;
	}
	LexicalAnalyzer(const string& pr_text) 
	{
		input_text = pr_text;
		_index = 0;
	}

private:
	Lexeme NextLex() 
	{
		char cur_ch = input_text[_index];
		_index++;

		Lexeme result;
		result._type = TYPE::Error;
		result.value = cur_ch;

		if (ischar(cur_ch)) {
			result._type = TYPE::Id;
			cur_ch = input_text[_index];
			while (_index < input_text.size() && ischar(cur_ch) || isdigit(cur_ch)) {
				result.value += cur_ch;
				_index++;
				cur_ch = input_text[_index];
			}
			if (result.value == "integer") {
				result._type = TYPE::Int;
			}
			else if (result.value == "array") {
				result._type = TYPE::ArrayInt;
			}
			else if (result.value == "if") {
				result._type = TYPE::If;
			}
			else if (result.value == "else") {
				result._type = TYPE::Else;
			}
			else if (result.value == "while") {
				result._type = TYPE::While;
			}
			else if (result.value == "read") {
				result._type = TYPE::Read;
			}
			else if (result.value == "write") {
				result._type = TYPE::Write;
			}
		}
		else if (isdigit(cur_ch)) {
			result._type = TYPE::IntNum;
			cur_ch = input_text[_index];

			while (_index < input_text.size() && isdigit(cur_ch)) {
				result.value += cur_ch;
				_index++;
				cur_ch = input_text[_index];
			}

			if (_index < input_text.size() && ischar(cur_ch)) {
				while (_index < input_text.size() && ischar(cur_ch)) {
					result.value += cur_ch;
					++_index;
					cur_ch = input_text[_index];
				}
				result._type = TYPE::Error;
			}
			if (_index < input_text.size() && ischar(cur_ch)) {
				while (_index < input_text.size() && ischar(cur_ch)) {
					result.value += cur_ch;
					++_index;
					cur_ch = input_text[_index];
				}
				result._type = TYPE::Error;
			}
		}
		else if (cur_ch == '{') {
			result._type = TYPE::LeftBrace;
		}
		else if (cur_ch == '}') {
			result._type = TYPE::RightBrace;
		}
		else if (cur_ch == '[') {
			result._type = TYPE::LeftSquareBracket;
		}
		else if (cur_ch == ']') {
			result._type = TYPE::RightSquareBracket;
		}
		else if (cur_ch == '(') {
			result._type = TYPE::LeftRoundBracket;
		}
		else if (cur_ch == ')') {
			result._type = TYPE::RightRoundBracket;
		}
		else if (cur_ch == '+') {
			result._type = TYPE::Plus;
		}
		else if (cur_ch == '-') {
			result._type = TYPE::Minus;
		}
		else if (cur_ch == '*') {
			result._type = TYPE::Multiply;
		}
		else if (cur_ch == '/') {
			result._type = TYPE::Divide;
		}
		else if (cur_ch == ';') {
			result._type = TYPE::Semicolon;
		}
		else if (cur_ch == ',') {
			result._type = TYPE::Comma;
		}
		else if (cur_ch == '<') {
			result._type = TYPE::Less;
			if (_index < input_text.size() && input_text[_index] == '=') {
				_index++;
				result._type = TYPE::LessOrEqual;
				result.value = "<=";
			}
		}
		else if (cur_ch == '=') {
			result._type = TYPE::Assign;
			if (_index < input_text.size() && input_text[_index] == '=') {
				_index++;
				result._type = TYPE::Equal;
				result.value = "==";
			}
		}
		else if (cur_ch == '>') {
			result._type = TYPE::More;
			if (_index < input_text.size() && input_text[_index] == '=') {
				_index++;
				result._type = TYPE::MoreOrEqual;
				result.value = ">=";
			}
		}
		else if (cur_ch == '!' && _index < input_text.size() && input_text[_index] == '=') {
			_index++;
			result._type = TYPE::NotEqual;
			result.value = "!=";
		}

		return result;
	}
	bool ischar(char ch) 
	{
		return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
	}

	string input_text;
	int _index;
	vector<Lexeme> data;
};