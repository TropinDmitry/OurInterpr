#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "SyntaxisAnalyzer.cpp"
#include <iostream>

using namespace std;

class OPSinterpreter
{
public:
	void step3()
	{
		stack<OpsElem> magazine;
		auto& ops = input_data.ops;
		auto& int_table = input_data.int_table;
		auto& arrayInt_table = input_data.arrayInt_table;

		for (size_t i = 0; i < ops.size(); ++i)
		{
			switch (ops[i].type)
			{
			case OpsElemType::VarName:
			{
				if (!int_table.count(ops[i]._name) && !arrayInt_table.count(ops[i]._name))
				{
					string msg = "OPS: Неизвестная переменная;\n стр = " + to_string(ops[i].line) + ", поз = " + to_string(ops[i].pos);
					throw runtime_error(msg);
				}
				magazine.push(ops[i]);
				break;
			}
			case OpsElemType::IntNum:
			{
				magazine.push(ops[i]);
				break;
			}
			case OpsElemType::Operation:
			{
				switch (ops[i].operation)
				{
				case OpsElemOper::Read:
				{
					auto a = magazine.top(); magazine.pop();
					int number;
					cin >> number;
					SetInt(a, number);

					break;
				}
				case OpsElemOper::Write:
				{
					auto a = magazine.top(); magazine.pop();

					cout << GetInt(a) << endl;

					break;
				}
				case OpsElemOper::Plus:
				{
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					int result;
					result = GetInt(a) + GetInt(b);
					magazine.emplace(result, a.line, a.pos);

					break;
				}
				case OpsElemOper::Minus:
				{
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					int result;
					result = GetInt(a) - GetInt(b);
					magazine.emplace(result, a.line, a.pos);

					break;
				}
				case OpsElemOper::Milt:
				{
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					int result;
					result = GetInt(a) * GetInt(b);
					magazine.emplace(result, a.line, a.pos);

					break;
				}
				case OpsElemOper::Div:
				{
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					int result;
					result = GetInt(a) / GetInt(b);
					magazine.emplace(result, a.line, a.pos);

					break;
				}
				case OpsElemOper::Less:
				{
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					bool result;
					result = GetInt(a) < GetInt(b);
					magazine.emplace(result, a.line, a.pos);

					break;
				}
				case OpsElemOper::Assign:
				{
					// a = b;
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					SetInt(a, GetInt(b));

					break;
				}
				case OpsElemOper::More:
				{
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					bool result;
					result = GetInt(a) > GetInt(b);
					magazine.emplace(result, a.line, a.pos);

					break;
				}
				case OpsElemOper::Equal:
				{
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					bool result;
					result = GetInt(a) == GetInt(b);
					magazine.emplace(result, a.line, a.pos);

					break;
				}
				case OpsElemOper::LessOrEqual:
				{
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					bool result;
					result = GetInt(a) <= GetInt(b);
					magazine.emplace(result, a.line, a.pos);

					break;
				}
				case OpsElemOper::MoreOrEqual:
				{
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					bool result;
					result = GetInt(a) >= GetInt(b);
					magazine.emplace(result, a.line, a.pos);

					break;
				}
				case OpsElemOper::NotEqual:
				{
					auto b = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					bool result;
					result = GetInt(a) != GetInt(b);
					magazine.emplace(result, a.line, a.pos);

					break;
				}
				case OpsElemOper::Jump:
				{
					auto _mark = magazine.top(); magazine.pop();

					i = GetInt(_mark) - 1;
					break;
				}
				case OpsElemOper::JumpIfFalse:
				{
					auto _mark = magazine.top(); magazine.pop();
					auto a = magazine.top(); magazine.pop();

					if (GetInt(a) == 0) {
						i = GetInt(_mark) - 1;
					}
					break;
				}
				case OpsElemOper::Index:
				{
					auto idx = magazine.top(); magazine.pop();
					auto arr = magazine.top(); magazine.pop();

					arr._num = GetInt(idx);

					magazine.push(arr);
					break;
				}
				default:
				{
					string msg = "OPS: неизвестный оператор;\n стр = " + to_string(ops[i].line) + ", поз = " + to_string(ops[i].pos);
					throw runtime_error(msg);
				}
				}
				break;
			}
			default:
			{
				string msg = "OPS: неизвестный ОПС элемент;\n стр = " + to_string(ops[i].line) + ", поз = " + to_string(ops[i].pos);
				throw runtime_error(msg);
			}
			}
		}
	}

	OPSinterpreter(SyntaxData data)
	{
		input_data = data;
	}
private:
	int GetInt(OpsElem& item)
	{
		if (item.type == OpsElemType::IntNum)
		{
			return item._num;
		}
		else if (input_data.int_table.count(item._name))
		{
			return input_data.int_table[item._name];
		}
		else if (input_data.arrayInt_table.count(item._name))
		{
			return input_data.arrayInt_table[item._name][item._num];
		}
		else
		{
			string msg = "OPS: ожидалась переменная или число;\n стр = " + to_string(item.line) + ", поз = " + to_string(item.pos);
			throw runtime_error(msg);
		}
	}
	void SetInt(OpsElem& item, int num)
	{
		if (input_data.int_table.count(item._name))
		{
			input_data.int_table[item._name] = num;
		}
		else if (input_data.arrayInt_table.count(item._name))
		{
			input_data.arrayInt_table[item._name][item._num] = num;
		}
		else
		{
			string msg = "OPS: ожидалась переменная; стр = " + to_string(item.line) + ", поз = " + to_string(item.pos);
			throw runtime_error(msg);
		}
	}

	SyntaxData input_data;
};