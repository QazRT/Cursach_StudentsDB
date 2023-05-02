#pragma once
#include <iostream>
#include <string.h>
#include "windows.h"
#include <conio.h>
#include <stdio.h>
#include <vector>
#include <map>
#include "EditDataClass.h"
#include "WorkWithConsole.h"
#include "Work_with_DB.h"

using namespace std;

enum class SelectType : char { string, student };
struct text_color {
	int forc;
	int bgc;
	int hoverc;
};

class MenuClass {
private:
	int count_items = 0;
	vector<string> string_items;
	vector<student> stud_items;
	string upTitle; //Заголовок меню (сверху)
	string downTitle; //Заголовок  меню (снизу)
	int selectedItem = 0;
	SelectType st;
public:
	MenuClass(const string _upTitle) {
		upTitle = move(_upTitle);
	}


	void addMenuItem(string _item) {
		string_items.push_back(_item);
		count_items++;
	}
	void addMenuItem(student _item) {
		stud_items.push_back(_item);
		count_items++;
	}


	string get_stud_string(int i) {
		return stud_items[i].id + " " + stud_items[i].surname + " " + stud_items[i].name + " " + stud_items[i].middle_name;
	}

	void item_print(int i) {
		switch(st) {
		case SelectType::string:
			cout << "•" << string_items[i] << endl;
			break;
		case SelectType::student:
			cout << "•" << get_stud_string(i) << endl;
			break;
		}
	}
	void draw(map<int, text_color> colors) {
		cout << upTitle << endl;
		for (int i = 0; i < count_items; ++i) {
			if ((colors[i].forc | colors[i].bgc | colors[i].hoverc) != 0)
				WWC::ConsColor(colors[i].forc, colors[i].bgc);
			if (i == selectedItem) {
				WWC::ConsColor(FOREGROUND_GREEN);
				item_print(i);
				WWC::ConsColor(FOREGROUND_INTENSITY);
			}
			else
				item_print(i);
		}
	}

	int ItemSelect(bool is_1_menu = false, map<int, text_color> colors = {}) {
		char ch = 0;
		draw(colors);
		WWC::ShowConsoleCursor(false);
		while (ch != 13) {
			ch = _getch();
			if (ch == 72 || ch == 119) // Up
			{
				WWC::Cur2xy(0, selectedItem+1);
				if ((colors[selectedItem].forc | colors[selectedItem].bgc) == 0)
					WWC::ConsColor(FOREGROUND_INTENSITY);
				else
					WWC::ConsColor(colors[selectedItem].forc, colors[selectedItem].bgc);
				item_print(selectedItem);
				selectedItem -= selectedItem > 0 ? 1 : 0;
				WWC::Cur2xy(0, selectedItem+1);

				if (colors[selectedItem].hoverc == 0)
					WWC::ConsColor(FOREGROUND_GREEN);
				else
					WWC::ConsColor(colors[selectedItem].hoverc);

				item_print(selectedItem);
			}
			if (ch == 80 || ch == 115) // Down
			{
				WWC::Cur2xy(0, selectedItem + 1);
				if ((colors[selectedItem].forc | colors[selectedItem].bgc) == 0)
					WWC::ConsColor(FOREGROUND_INTENSITY);
				else
					WWC::ConsColor(colors[selectedItem].forc, colors[selectedItem].bgc);
				item_print(selectedItem);
				selectedItem += selectedItem < count_items-1 ? 1 : 0;
				WWC::Cur2xy(0, selectedItem + 1);

				if (colors[selectedItem].hoverc == 0)
					WWC::ConsColor(FOREGROUND_GREEN);
				else
					WWC::ConsColor(colors[selectedItem].hoverc);

				item_print(selectedItem);
			}
			if (ch == 75 || ch == 97 || ch == 8) // Left
			{
				if (!is_1_menu) {
					selectedItem = -1;
					break;
				}
			}
			if (ch == 77 || ch == 100) // Right
				break;
			
			Sleep(10);
		}
		//WWC::ShowConsoleCursor(true); ZAD
		WWC::Cur2xy(0, 0);
		return ch;
	}


	string group_select() {
		st = SelectType::string;
		ItemSelect(true);
		system("cls");
		WWC::ConsColor(15);
		/*WWC::ConsColor(15);
		EditDataClass* cl = new EditDataClass();
		cl->setLabel("Введите число");
		int selectItem = cl->getData(editType::onlyDigit, 0, count_items);
		delete cl;*/
		return string_items[selectedItem];
	}

	student stud_select() {
		st = SelectType::student;
		ItemSelect();
		system("cls");
		WWC::ConsColor(15);

		if (selectedItem == -1)
		{
			student stud = {"-1"};
			return stud;
		}

		return stud_items[selectedItem];
	}

	int stud_edit() {
		st = SelectType::string;

		addMenuItem("Изменить данные о студенте");
		addMenuItem("Изменить данные зачетной книжки студента");
		addMenuItem("Удалить студента");

		WWC::Cur2xy(0, 7);
		cout.width(70);  cout.fill('_'); cout << "_" << endl;

		WWC::Cur2xy(0, 0);
		ItemSelect(false, { {2, {12, 0, FOREGROUND_RED}} });
		system("cls");

		WWC::ConsColor(15);
		if (selectedItem == -1)
			return -1;

		return selectedItem;
	}
};