#include <iostream>
#include <fstream>
#include <string>
#include "LexicalAnalyzer.cpp"
#include "SyntaxisAnalyzer.cpp"
#include "OPSinterpreter.cpp"
#include <Windows.h>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string label = "test";
    string fName;
	for (int i = 1; i <= 4; i++)
	{
		fName = label + to_string(i) + ".txt";
		ifstream input = ifstream(fName);
		string data;
		getline(input, data, '\0');
		cout << "Запуск теста №" << i << endl;
		try
		{
			LexicalAnalyzer LexAnalyzer(data); LexAnalyzer.step1();

			SyntaxisAnalyzer SyntAnalyzer(LexAnalyzer.GetLexData()); SyntAnalyzer.step2();

			OPSinterpreter OPSInterp(SyntAnalyzer.GetSyntData()); OPSInterp.step3();
		}

		catch (const exception& exception)
		{
			cerr << exception.what() << endl;
		}
		cout << endl << endl;
	}
}