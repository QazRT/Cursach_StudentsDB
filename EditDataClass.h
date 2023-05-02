#pragma once
#include "windows.h"
#include <iostream>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include "WorkWithConsole.h"

using namespace std;

enum class editType : char { group, stud_id, onlyDigit, onlyAlpha, all };

class EditDataClass {
private:
	string label;
	string data;
public:
	EditDataClass() {
		label = "";
		data = "";
	}
	void setLabel(string _label = "Введите значение") {
		if (_label.length() > 1)
			this->label = _label;
		else
			label = "";
	}
	bool isDigit(char ch) {
		if (ch >= 48 and ch <= 57)
			return true;
		else
			return false;
	}
	bool isStringDigit(string _str) {
		for (int i = 0; i < _str.length(); i++)
			if (not isDigit(_str[i]))
				return false;
		if (_str.length() == 0)
			return false;
		return true;
	}
	bool isAlpha(int ch) {
		if (ch >= 65 and ch <= 90)
			return true;
		if (ch >= 97 and ch <= 122)
			return true;
		if (ch >= -200 and ch <= -1)
			return true;
		return false;
	}
	bool isSpace(char ch) {
		if (ch == 32)
			return true;
		else
			return false;
	}
	bool isSpec(char ch) {
		if (ch >= 33 and ch <= 47)
			return true;
		else
			return false;
	}
	void clear(string _data = "") {
		system("cls");
		data = _data;
	}

	string getData(enum class editType et) {
		char ch = 0;
		cout << label << endl << data;
		while (ch != 13) {
			ch = _getch();
			if (ch == 8) {
				if (data.length() > 0) {
					data.pop_back();
					//system("cls");
					cout << "\b \b";
				}
				continue;
			}
			if (et == editType::stud_id) {
				if (data.size() < 2)
					if (isDigit(ch)) {
						cout << ch;
						data.push_back(ch);
					}
				if (data.size() == 2)
					if (isAlpha(ch)) {
						cout << ch;
						data.push_back(ch);
					}
				if (data.size() > 2 && data.size() < 7)
					if (isDigit(ch)) {
						cout << ch;
						data.push_back(ch);
					}
			}
			if (et == editType::group) {
				if (0 <= data.size() && data.size() <= 4)
					if (isAlpha(ch)) {
						cout << ch;
						data.push_back(ch);
					}
				if (data.size() == 4 || data.size() == 7) {
					cout << "-";
					data.push_back('-');
				}
				if (data.size() == 5 || data.size() == 6 || data.size() == 8 || data.size() == 9)
					if (isDigit(ch)) {
						cout << ch;
						data.push_back(ch);
					}
			}
			if (et == editType::onlyDigit)
				if (isDigit(ch)) {
					cout << ch;
					data.push_back(ch);
				}
			if (et == editType::onlyAlpha)
				if (isAlpha(ch)) {
					cout << ch;
					data.push_back(ch);
				}
			if (et == editType::all) {
				cout << ch;
				data.push_back(ch);
			}
		}
		return data;
	}
	int getData(enum class editType et, int min, int max) {
	DiapError:
		if (et == editType::onlyDigit) {
			getData(et);
			int num = max + 1;
			if (isStringDigit(data))
				num = atoi(data.c_str());

			if (data.length() < 1) {
				WWC::ConsColor(FOREGROUND_RED);
				cout << endl << "Egor: Введите число\n";
				WWC::ConsColor(15);
				goto DiapError;
			}			
			if (!(num >= min and num <= max)) {
				WWC::ConsColor(FOREGROUND_RED);
				cout << endl << "Egor: Введенное число (" << num << ") Не входит в отрезок [" << min << "; " << max << "]\n";
				WWC::ConsColor(15);
				goto DiapError;
			}
			return num;
		}
	}
	string getData(enum class editType et, int len) {
	LenError:
		if (et == editType::onlyAlpha) {
			getData(et);
			if (data.length() < 1) {
				WWC::ConsColor(FOREGROUND_RED);
				cout << endl << "Egor: Введите строку\n";
				WWC::ConsColor(15);
				goto LenError;
			}
			if (data.length() > len) {
				WWC::ConsColor(FOREGROUND_RED);
				cout << endl << "Egor: Длина строки больше допустимой (" << data.length() << ") Разрешено: " << len << endl;
				WWC::ConsColor(15);
				goto LenError;
			}
			return data;
		}
	}
};
