﻿#pragma once
#include <iostream>
#include <string.h>
#include "windows.h"
#include <conio.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <iomanip>
#include "EditDataClass.h"
#include "WorkWithConsole.h"
#include "Work_with_DB.h"

using namespace std;

enum class SelectType : char { string, student, EditField };
struct text_color {
	int forc;
	int bgc;
	int hoverc;
};

class MenuClass {
private:
	int count_items = 0;
	vector<string> string_items;
	vector<pair<string, string>> string2_items;
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
	void addMenuItem(pair<string, string> _item) {
		string2_items.push_back(make_pair(_item.first, _item.second));
		count_items++;
	}


	string get_stud_string(int i) {
		return stud_items[i].id + " " + stud_items[i].surname + " " + stud_items[i].name + " " + stud_items[i].middle_name;
	}

	void item_print(int i, bool fl = false) {
		switch (st) {
		case SelectType::string:
			cout << "•" << string_items[i] << endl;
			break;
		case SelectType::student:
			cout << "•" << get_stud_string(i) << endl;
			break;
		case SelectType::EditField:
			WWC::ConsColor(fl ? FOREGROUND_GREEN : 15); cout << string2_items[i].first;
			WWC::ConsColor(FOREGROUND_INTENSITY); cout << string2_items[i].second << endl;
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
				item_print(i, true);
				WWC::ConsColor(FOREGROUND_INTENSITY);
			}
			else
				item_print(i);
		}
	}

	int ItemSelect(map<int, text_color> colors = {}, short fl = 0) {
		char ch = 0;
		draw(colors);
		WWC::ShowConsoleCursor(false);
		while (ch != 13) {
			ch = _getch();

			if (fl == 1 && (ch == 'n' || ch == 'N' || ch == 'т' || ch == 'Т')) {
				selectedItem = 'n';
				break;
			}
			else if (fl == 2 && ch == 83) {
				selectedItem = 'd';
				break;
			}

			else if (ch == 75 || ch == 97 || ch == 8) // Left
			{
				selectedItem = -1;
				break;
			}

			if (count_items == 0)
				continue;

			if (ch == 72 || ch == 119) // Up
			{
				WWC::Cur2xy(0, selectedItem + 1);
				if ((colors[selectedItem].forc | colors[selectedItem].bgc) == 0)
					WWC::ConsColor(FOREGROUND_INTENSITY);
				else
					WWC::ConsColor(colors[selectedItem].forc, colors[selectedItem].bgc);
				item_print(selectedItem);
				selectedItem -= selectedItem > 0 ? 1 : 0;
				WWC::Cur2xy(0, selectedItem + 1);

				if (colors[selectedItem].hoverc == 0)
					WWC::ConsColor(FOREGROUND_GREEN);
				else
					WWC::ConsColor(colors[selectedItem].hoverc);

				item_print(selectedItem, true);
			}
			else if (ch == 80 || ch == 115) // Down
			{
				WWC::Cur2xy(0, selectedItem + 1);
				if ((colors[selectedItem].forc | colors[selectedItem].bgc) == 0)
					WWC::ConsColor(FOREGROUND_INTENSITY);
				else
					WWC::ConsColor(colors[selectedItem].forc, colors[selectedItem].bgc);
				item_print(selectedItem);
				selectedItem += selectedItem < count_items - 1 ? 1 : 0;
				WWC::Cur2xy(0, selectedItem + 1);

				if (colors[selectedItem].hoverc == 0)
					WWC::ConsColor(FOREGROUND_GREEN);
				else
					WWC::ConsColor(colors[selectedItem].hoverc);

				item_print(selectedItem, true);
			}
			else if (ch == 77 || ch == 100) // Right
				break;

			Sleep(10);
		}
		//WWC::ShowConsoleCursor(true); ZAD
		WWC::Cur2xy(0, 0);
		return selectedItem;
	}


	string group_select() {
		st = SelectType::string;
		ItemSelect({}, true);
		system("cls");
		WWC::ConsColor(15);

		if (selectedItem == 'n')
			return "n";
		if (selectedItem == -1)
			return "-1";

		return string_items[selectedItem];
	}

	student stud_select() {
		st = SelectType::student;
		ItemSelect();
		system("cls");
		WWC::ConsColor(15);

		if (selectedItem == -1)
		{
			student stud = { "", "-1" };
			return stud;
		}

		return stud_items[selectedItem];
	}

	int stud_edit(string stud_id) {
		st = SelectType::string;

		addMenuItem("Изменить данные о студенте");
		addMenuItem("Изменить данные зачетной книжки студента");
		addMenuItem("Удалить студента");

		WWC::Cur2xy(0, 7);
		WWC::ConsColor(15);

		WwDB* wwdb = new WwDB();
		vector<stud_score> scr = wwdb->get_student_score(stud_id);

		cout.width(85);  cout.fill('_'); cout << "_" << endl;
		cout.width(4); cout << left << "id";
		cout.width(10); cout << left << "|_Семестр";
		cout.width(52); cout << left << "|_Предмет";
		cout.width(9); cout << left << "|_Вид";
		cout.width(10); cout << left << "|_Оценка"; cout << "|" << endl;
		cout.fill(' ');

		for (int i = 0; i < scr.size(); ++i) {
			if (i >= 1 && scr[i].sem != scr[i - 1].sem)
			{
				cout.fill('-');
				cout.width(4); cout << "-";
				cout.width(10); cout << "|-";
				cout.width(52); cout << "|-";
				cout.width(9); cout << "|-";
				cout.width(10); cout << "|-"; cout << "|" << endl;
				cout.fill(' ');
			}

			WWC::ConsColor(FOREGROUND_INTENSITY);
			cout.width(3);  cout << left << scr[i].id;
			WWC::ConsColor(15);
			cout << " | ";

			cout.width(8);
			cout << left << scr[i].sem << "| ";

			cout.width(50);
			cout << left << scr[i].subj << "| ";

			cout.width(7);
			cout << ((scr[i].extype == ExamType::exam) ? "exam" : "zach") << "| ";

			cout.width(8);
			if (scr[i].value == '+') {
				WWC::ConsColor(FOREGROUND_GREEN);
				cout << "Зачёт";
				WWC::ConsColor(15);
				cout << "|" << endl;
				continue;
			}
			else if (scr[i].value == '-') {
				WWC::ConsColor(12);
				cout << "Незачёт";
				WWC::ConsColor(15);
				cout << "|" << endl;
				continue;
			}
			else if (scr[i].value < 3)
				WWC::ConsColor(12);
			else if (scr[i].value >= 3 && scr[i].value < 4)
				WWC::ConsColor(14);
			else if (scr[i].value >= 4)
				WWC::ConsColor(FOREGROUND_GREEN);
			cout << scr[i].value;

			WWC::ConsColor(15);
			cout << "|" << endl;
		}
		WWC::ConsColor(15);

		WWC::Cur2xy(0, 0);
		ItemSelect({ {2, {12, 0, FOREGROUND_RED}} });
		system("cls");

		WWC::ConsColor(15);
		if (selectedItem == -1)
			return -1;

		delete wwdb;
		return selectedItem;
	}

	student EditStudInfoMenu(student stud) {
		st = SelectType::EditField;


		addMenuItem(make_pair("Группа: ", stud.group));
		addMenuItem(make_pair("Фамилия: ", stud.surname));
		addMenuItem(make_pair("Имя: ", stud.name));
		addMenuItem(make_pair("Отчество: ", stud.middle_name));
		addMenuItem(make_pair("Дата рождения: ", stud.bday));
		addMenuItem(make_pair("Год поступления: ", stud.admyear));
		addMenuItem(make_pair("Институт: ", stud.inst));
		addMenuItem(make_pair("Кафедра: ", stud.kaf));
		string sex = ((stud.sex == Sex::man) ? "Мужской" : (stud.sex == Sex::woman ? "Женский" : "Боевой вертолет КА-52"));
		addMenuItem(make_pair("Пол: ", sex));
		addMenuItem(make_pair("\nСохранить", ""));

		EditDataClass* edc = new EditDataClass();
		while (true) {
			ItemSelect();
			if (selectedItem == 9)
				break;
			if (selectedItem == -1) {
				system("cls");
				return {"", "-1"};
			}

			WWC::Cur2xy(0, selectedItem + 1);
			cout << string2_items[selectedItem].first; WWC::ConsColor(15); // cout << string2_items[selectedItem].second;

			WWC::ShowConsoleCursor(true);

			editType et;
			switch (selectedItem)
			{
			case 0:
				et = editType::group;
				break;
			case 4:
				et = editType::date;
				break;
			case 5:
				et = editType::onlyDigit;
				break;
			case 8:
			{
				MenuClass* sex_sel = new MenuClass("");
				string sex = sex_sel->sexselect(5, selectedItem + 1);
				delete sex_sel;
				string2_items[selectedItem].second = sex;
				system("cls");
				continue;
			}
			case 7:
				et = editType::all;
				break;
			default:
				et = editType::onlyAlpha;
				break;
			}

			edc->setData(string2_items[selectedItem].second);
			string data = edc->getData(et);
			string2_items[selectedItem].second = data;
			system("cls");
		}
		delete edc;

		stud.group = string2_items[0].second;
		stud.surname = string2_items[1].second;
		stud.name = string2_items[2].second;
		stud.middle_name = string2_items[3].second;
		stud.bday = string2_items[4].second;
		stud.admyear = string2_items[5].second;
		stud.inst = string2_items[6].second;
		stud.kaf = string2_items[7].second;
		stud.sex = (string2_items[8].second == "Мужской") ? Sex::man : (string2_items[8].second == "Женский" ? Sex::woman : Sex::CombatHelicopter);

		return stud;
	}

	stud_score EditStudScoreMenu(string stud_id) {
		st = SelectType::string;

		WWC::ConsColor(15);

		WwDB* wwdb = new WwDB();
		vector<stud_score> scr = wwdb->get_student_score(stud_id);

		for (int i = 0; i < scr.size(); ++i)
			addMenuItem("Семестр " + to_string(scr[i].sem) + ": " + scr[i].subj);
		addMenuItem("\b \b\nДобавить");
		ItemSelect();

		system("cls");
		WWC::ConsColor(15);

		if (selectedItem == -1)
			return { -2, "-1" };

		if (selectedItem == scr.size()) {
			return { -1, stud_id };
		}


		return scr[selectedItem];
	}

	stud_score EditScoreInfoMenu(stud_score stsc) {
		st = SelectType::EditField;

		addMenuItem(make_pair("Семестр: ", to_string(stsc.sem)));
		addMenuItem(make_pair("Предмет: ", stsc.subj));
		addMenuItem(make_pair("Тип оценивания: ", ((stsc.extype == ExamType::exam) ? "Экзамен" : "Зачёт")));
		string oc;
		if (stsc.extype == ExamType::exam)
			oc = to_string(stsc.value);
		else if (stsc.value == '+')
			oc = "Зачёт";
		else
			oc = "Незачёт";
		addMenuItem(make_pair("Оценка: ", oc));
		addMenuItem(make_pair("\nСохранить", ""));

		EditDataClass* edc = new EditDataClass();
		while (true) {
			ItemSelect({}, 2);
			if (selectedItem == 'd') {
				system("cls");
				return { stsc.id, "ddd"};
			}
			if (selectedItem == 4)
				break;
			if (selectedItem == -1) {
				system("cls");
				return { -2 };
			}

			WWC::Cur2xy(0, selectedItem + 1);
			cout << string2_items[selectedItem].first; WWC::ConsColor(15);

			WWC::ShowConsoleCursor(true);

			editType et;
			switch (selectedItem) 
			{
			case 0:
				et = editType::onlyDigit;
				break;
			case 2:
			{
				MenuClass* ExTSel = new MenuClass("");
				ExTSel->addMenuItem("Экзамен");
				ExTSel->addMenuItem("Зачёт");
				string2_items[selectedItem].second = ExTSel->string_items[ExTSel->GorItemSelect(16, selectedItem + 1)];
				delete ExTSel;

				if (string2_items[2].second == "Зачёт")
					string2_items[3].second = "Незачёт";
				else 
					string2_items[3].second = "0";

				system("cls");
				continue;
			}
			case 3:
				if (string2_items[2].second == "Зачёт") {
					MenuClass* ZNZSel = new MenuClass("");
					ZNZSel->addMenuItem("Зачёт");
					ZNZSel->addMenuItem("Незачёт");
					string2_items[selectedItem].second = ZNZSel->string_items[ZNZSel->GorItemSelect(8, selectedItem + 1)];
					delete ZNZSel;
					system("cls");
					continue;
				}
				et = editType::onlyDigit;
				break;
			default:
				et = editType::onlyAlpha;
				break;
			}

			edc->setData(string2_items[selectedItem].second);
			string data = edc->getData(et);
			string2_items[selectedItem].second = data;
			system("cls");
		}
		delete edc;

		stsc.sem = stoi(string2_items[0].second);
		stsc.subj = string2_items[1].second;
		stsc.extype = (string2_items[2].second == "Экзамен" ? ExamType::exam : ExamType::zach);
		if (stsc.extype == ExamType::exam)
			stsc.value = stoi(string2_items[3].second);
		else
			stsc.value = string2_items[3].second == "Зачёт" ? '+' : '-';

		return stsc;
	}

	void GorDraw() {
		for (int i = 0; i < count_items; ++i) {
			if (i == selectedItem) {
				WWC::ConsColor(FOREGROUND_GREEN);
				cout << string_items[i] << " ";
				WWC::ConsColor(FOREGROUND_INTENSITY);
			}
			else
				cout << string_items[i] << " ";
		}
	}
	int GorItemSelect(int x = 0, int y = 0) {
		WWC::Cur2xy(x, y);
		GorDraw();


		char ch = 0;
		WWC::ShowConsoleCursor(false);
		while (ch != 13) {
			ch = _getch();

			if (ch == 8) {
				selectedItem = -1;
				break;
			}
			if (ch == 75 || ch == 97) // Left
			{
				selectedItem -= selectedItem > 0 ? 1 : 0;
				WWC::Cur2xy(x, y);
				GorDraw();
			}
			if (ch == 77 || ch == 100) { // Right
				selectedItem += selectedItem < count_items - 1 ? 1 : 0;
				WWC::Cur2xy(x, y);
				GorDraw();
			}

			Sleep(10);
		}

		return selectedItem;
	}

	string sexselect(int x = 0, int y = 0) {
		addMenuItem("Мужской");
		addMenuItem("Женский");
		addMenuItem("Боевой вертолет КА-52");

		GorItemSelect(x, y);
		return string_items[selectedItem];
	}
};