#pragma once
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <stdexcept>
#include "LexicalAnalyzer.cpp"

using namespace std;

enum class OpsElemType 
{
	VarName,
	IntNum,
	Operation,
	Error
};

enum class OpsElemOper 
{
	Read,
	Write,
	Plus,
	Minus,
	Milt,
	Div,
	Less,
	Assign,
	More,
	Equal,
	LessOrEqual,
	MoreOrEqual,
	NotEqual,
	Jump,
	JumpIfFalse,
	Index,
	Error
};

struct OpsElem {
	OpsElemType type = OpsElemType::Error;
	OpsElemOper operation = OpsElemOper::Error;
	string _name;
	int _num = 0;
	int line;
	int pos;

	OpsElem(string name, const Lexeme& l)
	{
		type = OpsElemType::VarName;
		_name = name;
		line = l.line;
		pos = l.pos;
	}

	OpsElem(OpsElemOper op, const Lexeme& l)
	{
		type = OpsElemType::Operation;
		operation = op;
		line = l.line;
		pos = l.pos;
	}

	OpsElem(int number, const Lexeme& l)
	{
		type = OpsElemType::IntNum;
		_num = number;
		line = l.line;
		pos = l.pos;
	}


	OpsElem(int number, int l, int p)
	{
		type = OpsElemType::IntNum;
		_num = number;
		line = l;
		pos = p;
	}

};

struct SyntaxData
{
	vector<OpsElem> ops;
	map<string, int> int_table;
	map<string, vector<int>> arrayInt_table;
};

class SyntaxisAnalyzer {
public:
	void step2() 
    {
        Magazine.emplace(State::S);
        Generator.push(Tasks::Empty);

        int current_lexeme_idx = 0;
        _lexeme = input_data[current_lexeme_idx];

        while (!Generator.empty() && !Magazine.empty())
        {
            MgznElem mgzn_elem = Magazine.top(); Magazine.pop();
            _state = mgzn_elem.state;
            _task = Generator.top(); Generator.pop();

            run_task();

            if (mgzn_elem._terminal) {

                if (_lexeme._type == TYPE::EoF) {
                    string msg = "Synt: магазин не пустой;\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                    throw runtime_error(msg);
                }

                if (mgzn_elem.lexeme == _lexeme._type) {
                    current_lexeme_idx++;
                    _lexeme = input_data[current_lexeme_idx];
                }
                else {
                    string msg = "Synt: неожиданная лексема;\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                    throw runtime_error(msg);
                }
            }
            else {
                run_rule();
            }
        }

        if (_lexeme._type != TYPE::EoF) {
            string msg = "неизвестные лексемы;\n  стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
            throw runtime_error(msg);
        }
    }
	SyntaxData GetSyntData() 
    {
        return data;
    }
    SyntaxisAnalyzer(vector<Lexeme> lexemes)
    {
        input_data = lexemes;
    }

private:
	void run_rule() 
    {
        switch (_state)
        {
        case State::S:
        {
            switch (_lexeme._type)
            {
            case TYPE::Int:
            {
                Magazine.emplace(State::S);
                Magazine.emplace(State::J);
                Magazine.emplace(TYPE::Int);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T6);
                break;
            }
            case TYPE::ArrayInt:
            {
                Magazine.emplace(State::S);
                Magazine.emplace(State::P);
                Magazine.emplace(TYPE::ArrayInt);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T7);
                break;
            }
            case TYPE::Id:
            {
                Magazine.emplace(State::Q);
                Magazine.emplace(TYPE::Semicolon);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::Assign);
                Magazine.emplace(State::H);
                Magazine.emplace(TYPE::Id);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Assign);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::VarId);
                break;
            }
            case TYPE::Read:
            {
                Magazine.emplace(State::Q);
                Magazine.emplace(TYPE::Semicolon);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::H);
                Magazine.emplace(TYPE::Id);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::Read);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Read);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::VarId);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Write:
            {
                Magazine.emplace(State::Q);
                Magazine.emplace(TYPE::Semicolon);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::Write);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Write);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::If:
            {
                Magazine.emplace(State::Q);
                Magazine.emplace(State::Z);
                Magazine.emplace(State::K);
                Magazine.emplace(TYPE::RightBrace);
                Magazine.emplace(State::Q);
                Magazine.emplace(State::A);
                Magazine.emplace(TYPE::LeftBrace);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::C);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::If);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T3);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T1);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::While:
            {
                Magazine.emplace(State::Q);
                Magazine.emplace(TYPE::RightBrace);
                Magazine.emplace(State::Q);
                Magazine.emplace(State::A);
                Magazine.emplace(TYPE::LeftBrace);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::C);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::While);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T5);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T1);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T4);
                break;
            }
            default:
            {
                string msg = "Synt:\n  стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::J:
        {
            switch (_lexeme._type)
            {
            case TYPE::Id:
            {
                Magazine.emplace(State::M);
                Magazine.emplace(TYPE::Id);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T8);
                break;
            }
            default:
            {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::M:
        {
            switch (_lexeme._type)
            {
            case TYPE::Comma:
            {
                Magazine.emplace(State::M);
                Magazine.emplace(TYPE::Id);
                Magazine.emplace(TYPE::Comma);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T8);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Semicolon:
            {
                Magazine.emplace(TYPE::Semicolon);

                Generator.emplace(Tasks::Empty);
                break;
            }
            default:
            {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::P:
        {
            switch (_lexeme._type)
            {
            case TYPE::Id:
            {
                Magazine.emplace(State::N);
                Magazine.emplace(TYPE::RightSquareBracket);
                Magazine.emplace(TYPE::IntNum);
                Magazine.emplace(TYPE::LeftSquareBracket);
                Magazine.emplace(TYPE::Id);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T9);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T8);
                break;
            }
            default:
            {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::N:
        {
            switch (_lexeme._type)
            {
            case TYPE::Comma:
            {
                Magazine.emplace(State::N);
                Magazine.emplace(TYPE::RightSquareBracket);
                Magazine.emplace(TYPE::IntNum);
                Magazine.emplace(TYPE::LeftSquareBracket);
                Magazine.emplace(TYPE::Id);
                Magazine.emplace(TYPE::Comma);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T9);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T8);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Semicolon:
            {
                Magazine.emplace(TYPE::Semicolon);

                Generator.emplace(Tasks::Empty);
                break;
            }
            default:
            {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::Q:
        {
            switch (_lexeme._type)
            {
            case TYPE::Id:
            {
                Magazine.emplace(State::Q);
                Magazine.emplace(TYPE::Semicolon);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::Assign);
                Magazine.emplace(State::H);
                Magazine.emplace(TYPE::Id);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Assign);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::VarId);
                break;
            }
            case TYPE::Read:
            {
                Magazine.emplace(State::Q);
                Magazine.emplace(TYPE::Semicolon);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::H);
                Magazine.emplace(TYPE::Id);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::Read);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Read);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::VarId);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Write:
            {
                Magazine.emplace(State::Q);
                Magazine.emplace(TYPE::Semicolon);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::Write);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Write);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::If:
            {
                Magazine.emplace(State::Q);
                Magazine.emplace(State::Z);
                Magazine.emplace(State::K);
                Magazine.emplace(TYPE::RightBrace);
                Magazine.emplace(State::Q);
                Magazine.emplace(State::A);
                Magazine.emplace(TYPE::LeftBrace);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::C);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::If);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T3);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T1);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::While:
            {
                Magazine.emplace(State::Q);
                Magazine.emplace(TYPE::RightBrace);
                Magazine.emplace(State::Q);
                Magazine.emplace(State::A);
                Magazine.emplace(TYPE::LeftBrace);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::C);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::While);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T5);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T1);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T4);
                break;
            }
            default:
            {
                break;
            }
            }
            break;
        }
        case State::A:
        {
            switch (_lexeme._type)
            {
            case TYPE::Id:
            {
                Magazine.emplace(TYPE::Semicolon);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::Assign);
                Magazine.emplace(State::H);
                Magazine.emplace(TYPE::Id);

                Generator.emplace(Tasks::Assign);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::VarId);
                break;
            }
            case TYPE::Read:
            {
                Magazine.emplace(TYPE::Semicolon);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::H);
                Magazine.emplace(TYPE::Id);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::Read);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Read);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::VarId);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Write:
            {
                Magazine.emplace(TYPE::Semicolon);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::Write);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Write);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::If:
            {
                Magazine.emplace(State::Z);
                Magazine.emplace(State::K);
                Magazine.emplace(TYPE::RightBrace);
                Magazine.emplace(State::Q);
                Magazine.emplace(State::A);
                Magazine.emplace(TYPE::LeftBrace);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::C);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::If);

                Generator.emplace(Tasks::T3);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T1);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::While:
            {
                Magazine.emplace(TYPE::RightBrace);
                Magazine.emplace(State::Q);
                Magazine.emplace(State::A);
                Magazine.emplace(TYPE::LeftBrace);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::C);
                Magazine.emplace(TYPE::LeftRoundBracket);
                Magazine.emplace(TYPE::While);

                Generator.emplace(Tasks::T5);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T1);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T4);
                break;
            }
            default:
            {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::H:
        {
            switch (_lexeme._type)
            {
            case TYPE::LeftSquareBracket:
            {
                Magazine.emplace(TYPE::RightSquareBracket);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::LeftSquareBracket);

                Generator.emplace(Tasks::Index);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            default:
            {
                break;
            }
            }
            break;
        }
        case State::C:
        {
            switch (_lexeme._type)
            {
            case TYPE::LeftRoundBracket:
            {
                Magazine.emplace(State::L);
                Magazine.emplace(State::U);
                Magazine.emplace(State::V);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::LeftRoundBracket);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Id:
            {
                Magazine.emplace(State::L);
                Magazine.emplace(State::U);
                Magazine.emplace(State::V);
                Magazine.emplace(State::H);
                Magazine.emplace(TYPE::Id);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::VarId);
                break;
            }
            case TYPE::IntNum:
            {
                Magazine.emplace(State::L);
                Magazine.emplace(State::U);
                Magazine.emplace(State::V);
                Magazine.emplace(TYPE::IntNum);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::IntNumber);
                break;
            }
            default:
            {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::L:
        {
            switch (_lexeme._type)
            {
            case TYPE::Less:
            {
                Magazine.emplace(State::Z);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::Less);

                Generator.emplace(Tasks::Less);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::More:
            {
                Magazine.emplace(State::Z);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::More);

                Generator.emplace(Tasks::More);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Equal:
            {
                Magazine.emplace(State::Z);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::Equal);

                Generator.emplace(Tasks::Equal);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::LessOrEqual:
            {
                Magazine.emplace(State::Z);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::LessOrEqual);

                Generator.emplace(Tasks::LessOrEqual);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::MoreOrEqual:
            {
                Magazine.emplace(State::Z);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::MoreOrEqual);

                Generator.emplace(Tasks::MoreOrEqual);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::NotEqual:
            {
                Magazine.emplace(State::Z);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::NotEqual);

                Generator.emplace(Tasks::NotEqual);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            default:
            {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::K:
        {
            switch (_lexeme._type)
            {
            case TYPE::Else:
            {
                Magazine.emplace(TYPE::RightBrace);
                Magazine.emplace(State::Q);
                Magazine.emplace(State::A);
                Magazine.emplace(TYPE::LeftBrace);
                Magazine.emplace(TYPE::Else);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::T2);
                break;
            }
            default:
            {
                break;
            }
            }
            break;
        }
        case State::G:
        {
            switch (_lexeme._type)
            {
            case TYPE::LeftRoundBracket:
            {
                Magazine.emplace(State::U);
                Magazine.emplace(State::V);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::LeftRoundBracket);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Id:
            {
                Magazine.emplace(State::U);
                Magazine.emplace(State::V);
                Magazine.emplace(State::H);
                Magazine.emplace(TYPE::Id);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::VarId);
                break;
            }
            case TYPE::IntNum:
            {
                Magazine.emplace(State::U);
                Magazine.emplace(State::V);
                Magazine.emplace(TYPE::IntNum);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::IntNumber);
                break;
            }
            default:
            {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::U:
        {
            switch (_lexeme._type)
            {
            case TYPE::Plus:
            {
                Magazine.emplace(State::U);
                Magazine.emplace(State::T);
                Magazine.emplace(TYPE::Plus);

                Generator.emplace(Tasks::Plus);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Minus:
            {
                Magazine.emplace(State::U);
                Magazine.emplace(State::T);
                Magazine.emplace(TYPE::Minus);

                Generator.emplace(Tasks::Minus);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            default:
            {
                break;
            }
            }
            break;
        }
        case State::T:
        {
            switch (_lexeme._type)
            {
            case TYPE::LeftRoundBracket:
            {
                Magazine.emplace(State::V);
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::LeftRoundBracket);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Id:
            {
                Magazine.emplace(State::V);
                Magazine.emplace(State::H);
                Magazine.emplace(TYPE::Id);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::VarId);
                break;
            }
            case TYPE::IntNum:
            {
                Magazine.emplace(State::V);
                Magazine.emplace(TYPE::IntNum);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::IntNumber);
                break;
            }
            default:
            {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::V:
        {
            switch (_lexeme._type)
            {
            case TYPE::Multiply:
            {
                Magazine.emplace(State::V);
                Magazine.emplace(State::W);
                Magazine.emplace(TYPE::Multiply);

                Generator.emplace(Tasks::Mult);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Divide:
            {
                Magazine.emplace(State::V);
                Magazine.emplace(State::W);
                Magazine.emplace(TYPE::Divide);

                Generator.emplace(Tasks::Div);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            default:
            {
                break;
            }
            }
            break;
        }
        case State::W:
        {
            switch (_lexeme._type)
            {
            case TYPE::LeftRoundBracket:
            {
                Magazine.emplace(TYPE::RightRoundBracket);
                Magazine.emplace(State::G);
                Magazine.emplace(TYPE::LeftRoundBracket);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::Empty);
                break;
            }
            case TYPE::Id:
            {
                Magazine.emplace(State::H);
                Magazine.emplace(TYPE::Id);

                Generator.emplace(Tasks::Empty);
                Generator.emplace(Tasks::VarId);
                break;
            }
            case TYPE::IntNum:
            {
                Magazine.emplace(TYPE::IntNum);

                Generator.emplace(Tasks::IntNumber);
                break;
            }
            default:
            {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }
            }
            break;
        }
        case State::Z:
        {
            break;
        }
        case State::Error:
        default:
            string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
            throw runtime_error(msg);
        }
    }
	void run_task() 
    {
        switch (_task)
        {
        case Tasks::Empty:
            break;
        case Tasks::VarId:
            data.ops.emplace_back(_lexeme.value, _lexeme);
            break;
        case Tasks::IntNumber:
        {
            int num = stoi(_lexeme.value);
            data.ops.emplace_back(num, _lexeme);
            break;
        }
        case Tasks::Read:
            data.ops.emplace_back(OpsElemOper::Read, _lexeme);
            break;
        case Tasks::Write:
            data.ops.emplace_back(OpsElemOper::Write, _lexeme);
            break;
        case Tasks::Plus:
            data.ops.emplace_back(OpsElemOper::Plus, _lexeme);
            break;
        case Tasks::Minus:
            data.ops.emplace_back(OpsElemOper::Minus, _lexeme);
            break;
        case Tasks::Mult:
            data.ops.emplace_back(OpsElemOper::Milt, _lexeme);
            break;
        case Tasks::Div:
            data.ops.emplace_back(OpsElemOper::Div, _lexeme);
            break;
        case Tasks::Less:
            data.ops.emplace_back(OpsElemOper::Less, _lexeme);
            break;
        case Tasks::Assign:
            data.ops.emplace_back(OpsElemOper::Assign, _lexeme);
            break;
        case Tasks::More:
            data.ops.emplace_back(OpsElemOper::More, _lexeme);
            break;
        case Tasks::Equal:
            data.ops.emplace_back(OpsElemOper::Equal, _lexeme);
            break;
        case Tasks::LessOrEqual:
            data.ops.emplace_back(OpsElemOper::LessOrEqual, _lexeme);
            break;
        case Tasks::MoreOrEqual:
            data.ops.emplace_back(OpsElemOper::MoreOrEqual, _lexeme);
            break;
        case Tasks::NotEqual:
            data.ops.emplace_back(OpsElemOper::NotEqual, _lexeme);
            break;
        case Tasks::Index:
            data.ops.emplace_back(OpsElemOper::Index, _lexeme);
            break;
        case Tasks::T1:
        {
            Marks.push(data.ops.size());
            data.ops.emplace_back(0, _lexeme);
            data.ops.emplace_back(OpsElemOper::JumpIfFalse, _lexeme);
            break;
        }
        case Tasks::T2:
        {
            int place = Marks.top(); Marks.pop();
            Marks.push(data.ops.size());
            data.ops.emplace_back(0, _lexeme);
            data.ops.emplace_back(OpsElemOper::Jump, _lexeme);
            data.ops[place]._num = data.ops.size();
            break;
        }
        case Tasks::T3:
        {
            int place = Marks.top(); Marks.pop();
            data.ops[place]._num = data.ops.size();
            break;
        }
        case Tasks::T4:
        {
            Marks.push(data.ops.size());
            break;
        }
        case Tasks::T5:
        {
            int place = Marks.top(); Marks.pop();
            data.ops.emplace_back(Marks.top(), _lexeme); Marks.pop();
            data.ops.emplace_back(OpsElemOper::Jump, _lexeme);
            data.ops[place]._num = data.ops.size();
            break;
        }
        case Tasks::T6:
        {
            _table = table::Int;
            break;
        }
        case Tasks::T7:
        {
            _table = table::ArrayInt;
            break;
        }
        case Tasks::T8:
        {
            string name = _lexeme.value;

            if (data.int_table.count(name) || data.arrayInt_table.count(name)) {
                string msg = "переопределение переменной:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }

            if (_table == table::Int) {
                data.int_table.insert({ name, 0 });
            }
            else {
                last_arr = name;
            }

            break;
        }
        case Tasks::T9:
        {
            int size = stoi(_lexeme.value);

            if (_table == table::ArrayInt) {
                data.arrayInt_table.insert({ last_arr, vector<int>(size) });
            }
            else {
                string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
                throw runtime_error(msg);
            }

            break;
        }
        default: {
            string msg = "Synt:\n стр = " + to_string(_lexeme.line) + ", поз = " + to_string(_lexeme.pos);
            throw runtime_error(msg);
        }
        }
    }

	enum class table {
		Int, ArrayInt,
	};

	enum class State {
		S, J, M, A, Q, P, N, H, C, L, K, G, U, T, V, W, Z, Error
	};

	enum class Tasks
	{
		Empty,
		VarId,
		IntNumber,
		Read,
		Write,
		Plus,
		Minus,
		Mult,
		Div,
		Less,
		Assign,
		More,
		Equal,
		LessOrEqual,
		MoreOrEqual,
		NotEqual,
		Index,
		T1,
		T2,
		T3,
		T4,
		T5,
		T6,
		T7,
		T8,
		T9,
	};

	struct MgznElem
	{
		bool _terminal;
		TYPE lexeme;
		State state;

		MgznElem(TYPE l) {
			_terminal = true;
			lexeme = l;
			state = State::Error;
		}

		MgznElem(State s) {
			_terminal = false;
			lexeme = TYPE::Error;
			state = s;
		}
	};

	Tasks _task;
	Lexeme _lexeme;
	State _state;
	table _table;
	string last_arr;
	stack<MgznElem> Magazine;
	stack<Tasks> Generator;
	stack<int> Marks;
	SyntaxData data;
	vector<Lexeme> input_data;
};

