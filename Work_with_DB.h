#pragma once
#include "windows.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <set>
#include <conio.h>
#include <algorithm>
#include <stdio.h>
#include <io.h>
#include "Crypto.h"

using namespace std;

enum class Sex : char { man, woman, CombatHelicopter };	// А давай
enum class ExamType : char { exam, zach };

struct student {
	string group;
	string id;
	string surname;
	string name;
	string middle_name;
	string bday = "01.01.1999";
	string admyear = "1999"; // Год поступления
	string inst;
	string kaf;
	Sex sex = Sex::CombatHelicopter;
};
struct stud_score {
	int id;
	string stud_id;
	string subj = "";
	ExamType extype = ExamType::exam;
	short value = 0;
	int sem = 1;
};

class WwDB {
private:
	int stud_count = 0;
	int max_scid = 0;
	set<string> groups;

public:
	void __init__() {
		if (_access("_Score.bin", 0) != -1) 
			rename("_Score.bin", "Score.bin");
		if (_access("_Students.bin", 0) != -1) 
			rename("_Students.bin", "Students.bin");

		fstream students_file("Students.bin", fstream::out | fstream::app | fstream::binary);
		students_file.close();
		fstream stud_score_file("Score.bin", fstream::out | fstream::app | fstream::binary);
		stud_score_file.close();

		stud_count = students_count();


		students_file.open("Students.bin", fstream::in | fstream::binary);
		string tmp;
		while (!students_file.eof()) {
			getline(students_file, tmp);
			groups.insert(stud_parse(tmp, 2).group);
		}
		groups.erase(groups.begin());
		students_file.close();

		init_max_score_id();
	}

	vector<string> get_groups() {
		vector<string> v(groups.begin(), groups.end());
		return v;
	}

	void init_max_score_id() {
		fstream stsc_file("Score.bin", fstream::in | fstream::binary);
		string tmp;
		int _maxid = 0;
		while (!stsc_file.eof()) {
			getline(stsc_file, tmp);
			_maxid = stud_score_parse(tmp, 1).id;
			 if (_maxid > max_scid)
				 max_scid = _maxid;
		}
		stsc_file.close();
	}

	int get_max_score_id() {
		return max_scid;
	}

	int students_count() {
		fstream students_file("Students.bin", fstream::in | fstream::binary);
		string tmp;
		int n = 0;
		while (!students_file.eof()) {
			getline(students_file, tmp);
			n++;
		}
		students_file.close();
		return n-1;
	}

	student stud_parse(string st, int jsr = 0) {
		st = Crypto::Decrypt(st);
		string tmp;
		bool fl = false;
		student stout;
		for (int i = 0, j = 0; i < st.length(); ++i) {
			if (st[i] == '"') {
				j++;
				fl = j % 2;
				if (fl)
					tmp = "";
				else {
					switch (j)
					{
					case 2:
						stout.group = tmp;
						break;
					case 4:
						stout.id = tmp;
						break;
					case 6:
						stout.surname = tmp;
						break;
					case 8:
						stout.name = tmp;
						break;
					case 10:
						stout.middle_name = tmp;
						break;
					case 12:
						stout.bday = tmp;
						break;
					case 14:
						stout.admyear = tmp;
						break;
					case 16:
						stout.inst = tmp;
						break;
					case 18:
						stout.kaf = tmp;
						break;
					case 20:
						stout.sex = (tmp == "man") ? Sex::man : (tmp == "woman" ? Sex::woman : Sex::CombatHelicopter);
						break;
					default:
						break;
					}

					if (j > 0 && j == jsr)
						return stout;
				}
				continue;
			}
			if (fl) {
				tmp += st[i];
			}
		}

		return stout;
	}
	stud_score stud_score_parse(string st, int jsr = 0) {
		st = Crypto::Decrypt(st);
		string tmp;
		bool fl = false;
		stud_score stout;
		for (int i = 0, j = 0; i < st.length(); ++i) {
			if (st[i] == '"') {
				j++;
				fl = j % 2;
				//cout << 195 << " " << j << " ";
				if (fl) {
					if (tmp != "" && j == 1)
						stout.id = stoi(tmp);
					tmp = "";
				}
				else {
					switch (j)
					{
					case 2:
						stout.stud_id = tmp;
						break;
					case 4:
						stout.subj = tmp;
						break;
					case 6:
						stout.extype = (tmp == "exam") ? ExamType::exam : ExamType::zach;
						break;
					case 8:
						stout.value = stoi(tmp);
						break;
					case 10:
						stout.sem = stoi(tmp);
						break;
					default:
						break;
					}
				}
				continue;
			}
			if (j > 0 && j == jsr)
				return stout;

			if (fl) {
				tmp += st[i];
			}
			else {
				if (isdigit(st[i])) {
					tmp += st[i];
				}
			}
		}

		return stout;
	}

	bool check_student(student st) {
		fstream stfile("Students.bin", fstream::in | fstream::binary);

		string tmp;
		while (!stfile.eof()) {
			getline(stfile, tmp);
			if (stud_parse(tmp, 4).id == st.id) {
				WWC::ErrOut("Egor: Студент с таким шифром уже существует!");
				return false;
			}
		}

		stfile.close();
		return true;
	}

	void add_student(student st) {
		fstream stfile("Students.bin", fstream::out | fstream::app | fstream::binary);
		string sex = (st.sex == Sex::man) ? "man" : (st.sex == Sex::woman ? "woman" : "CombatHelicopter");
		//stfile << "{\"" << encryptDecrypt(st.group) << "\"\"" << encryptDecrypt(st.id) << "\"\"" << encryptDecrypt(st.surname) << "\"\"" << encryptDecrypt(st.name)
		//	<< "\"\"" << encryptDecrypt(st.middle_name) << "\"\"" << encryptDecrypt(st.bday) << "\"\"" << encryptDecrypt(st.admyear) << "\"\"" << encryptDecrypt(st.inst)
		//	<< "\"\"" << encryptDecrypt(st.kaf) << "\"\"" << encryptDecrypt(sex) << "\"}\n";
		stfile << Crypto::Encrypt("{\"" + st.group + "\"\"" + st.id + "\"\"" + st.surname + "\"\"" + st.name
			+ "\"\"" + st.middle_name + "\"\"" + st.bday + "\"\"" + st.admyear + "\"\"" + st.inst
			+ "\"\"" + st.kaf + "\"\"" + sex + "\"}") << "\n";
		
		stfile.close();

		groups.insert(st.group);

		stud_count += 1;
	}

	void edit_student(student st) {
		fstream stfile("Students.bin", fstream::in | fstream::binary);
		fstream tmp_file("tmp.bin", fstream::out | fstream::binary);
		int n = 0;
		string tmp;

		while (!stfile.eof()) {
			getline(stfile, tmp);
			student tmp_st = stud_parse(tmp);

			if (tmp_st.id == st.id) {
				// замена данных
				tmp_st = st;

				string sex = (st.sex == Sex::man) ? "man" : (st.sex == Sex::woman ? "woman" : "CombatHelicopter");
				//tmp_file << "{\"" << encryptDecrypt(st.group) << "\"\"" << encryptDecrypt(st.id) << "\"\"" << encryptDecrypt(st.surname) << "\"\"" << encryptDecrypt(st.name)
				//	<< "\"\"" << encryptDecrypt(st.middle_name) << "\"\"" << encryptDecrypt(st.bday) << "\"\"" << encryptDecrypt(st.admyear) << "\"\"" << encryptDecrypt(st.inst)
				//	<< "\"\"" << encryptDecrypt(st.kaf) << "\"\"" << encryptDecrypt(sex) << "\"}\n";
				tmp_file << Crypto::Encrypt("{\"" + st.group + "\"\"" + st.id + "\"\"" + st.surname + "\"\"" + st.name
					+ "\"\"" + st.middle_name + "\"\"" + st.bday + "\"\"" + st.admyear + "\"\"" + st.inst
					+ "\"\"" + st.kaf + "\"\"" + sex + "\"}") << "\n";

				groups.insert(st.group);

				while (!stfile.eof()) {
					getline(stfile, tmp);
					tmp_file << tmp << "\n";
				}


				tmp_file.close();
				stfile.close();

				try {
					rename("Students.bin", "_Students.bin");
					rename("tmp.bin", "Students.bin");
					remove("_Students.bin");
				}
				catch (exception e) {
					WWC::ErrOut("Egor: Ошибка записи базы!");
				}

				return;
			}
			tmp_file << tmp << "\n";
		}
		tmp_file.close();
		stfile.close();
		remove("tmp.bin");
		return;
	}


	vector<student> get_students_by_group(string _group) {
		vector<student> students;

		fstream students_file("Students.bin", fstream::in | fstream::binary);
		string tmp;
		while (!students_file.eof()) {
			getline(students_file, tmp);
			//cout << "ch = " << ch << endl;
			student st = stud_parse(tmp);
			if (st.group == _group) 
				students.push_back(st);
		}
		students_file.close();

		sort(students.begin(), students.end(), [](student& left, student right) {
			return left.id < right.id;
			});

		return students;
	}


	vector<stud_score> get_student_score(string _stud_id) {
		fstream stsc_file("Score.bin", fstream::in | fstream::binary);
		string tmp;
		vector<stud_score> scores;

		while (!stsc_file.eof()) {
			getline(stsc_file, tmp);
			stud_score tmp_ssc = stud_score_parse(tmp);

			//cout << tmp_ssc.stud_id << " " << _stud_id << "\n";
			if (tmp_ssc.stud_id == _stud_id) {
				scores.push_back(tmp_ssc);
			}
		}

		sort(scores.begin(), scores.end(), [](stud_score& left, stud_score& right) {
			return left.sem < right.sem;
			});	// Сортировка по семестру

		stsc_file.close();
		return scores;
	}
	int add_student_score(stud_score stud_sc) {
		fstream stud_score_file("Score.bin", fstream::out | fstream::app | fstream::binary);

		string ex_type = (stud_sc.extype == ExamType::exam) ? "exam" : "zach";
		//stud_score_file << stud_sc.id << " " << stud_sc.stud_id << " " << stud_sc.subj << " " << ex_type << " " << stud_sc.value << " endl ";
		stud_score_file << Crypto::Encrypt("{" + to_string(stud_sc.id) + "\"" + stud_sc.stud_id + "\"\"" + stud_sc.subj
			+ "\"\"" + ex_type + "\"\"" + to_string(stud_sc.value) + "\"\"" + to_string(stud_sc.sem) + "\"}") << "\n";

		stud_score_file.close();

		max_scid++;

		return stud_sc.id;
	}
	void edit_student_score(stud_score stud_sc) {
		fstream stud_score_file("Score.bin", fstream::in | fstream::binary);
		fstream tmp_file("tmp.bin", fstream::out | fstream::binary);
		int n = 0;
		string tmp;
		//int id;
		while (!stud_score_file.eof()) {
			getline(stud_score_file, tmp);
			stud_score tmp_ssc = stud_score_parse(tmp);

			if (tmp_ssc.id == stud_sc.id) {
				// замена данных
				tmp_ssc.extype = stud_sc.extype;
				tmp_ssc.subj = stud_sc.subj;
				tmp_ssc.value = stud_sc.value;
				
				string ex_type = (tmp_ssc.extype == ExamType::exam) ? "exam" : "zach";
				cout << ex_type;
				tmp_file << Crypto::Encrypt("{" + to_string(stud_sc.id) + "\"" + stud_sc.stud_id + "\"\"" + stud_sc.subj
					+ "\"\"" + ex_type + "\"\"" + to_string(stud_sc.value) + "\"\"" + to_string(stud_sc.sem) + "\"}") << "\n";

				while (!stud_score_file.eof()) {
					getline(stud_score_file, tmp);
					tmp_file << tmp << "\n";
				}

				
				tmp_file.close();
				stud_score_file.close();

				try {
					rename("Score.bin", "_Score.bin");
					rename("tmp.bin", "Score.bin");
					remove("_Score.bin");
				}
				catch (exception e) {
					WWC::ErrOut("Egor: Ошибка записи базы!");
				}

				return;
			}
			tmp_file << tmp << "\n";
		}
		tmp_file.close();
		stud_score_file.close();
		remove("tmp.bin");
		return;
	}	


	void delete_score(int _id) {
		fstream stud_score_file("Score.bin", fstream::in | fstream::binary);
		fstream tmp_file("tmp.bin", fstream::out | fstream::binary);
		int n = 0;
		string tmp;

		while (!stud_score_file.eof()) {
			getline(stud_score_file, tmp);
			stud_score tmp_ssc = stud_score_parse(tmp);

			if (tmp_ssc.id == _id) {
				while (!stud_score_file.eof()) {
					getline(stud_score_file, tmp);
					tmp_file << tmp << "\n";
				}


				tmp_file.close();
				stud_score_file.close();

				try {
					rename("Score.bin", "_Score.bin");
					rename("tmp.bin", "Score.bin");
					remove("_Score.bin");
				}
				catch (exception e) {
					WWC::ErrOut("Egor: Ошибка записи базы!");
				}

				return;
			}
			tmp_file << tmp << "\n";
		}
		tmp_file.close();
		stud_score_file.close();
		remove("tmp.bin");
		return;
	}
	
	void delete_students_score(string stud_id) {
		fstream stud_score_file("Score.bin", fstream::in | fstream::binary);
		fstream tmp_file("tmp.bin", fstream::out | fstream::binary);
		int n = 0;
		string tmp;

		while (!stud_score_file.eof()) {
			getline(stud_score_file, tmp);
			stud_score tmp_ssc = stud_score_parse(tmp, 2);

			if (tmp_ssc.stud_id == stud_id)
				continue;
			tmp_file << tmp << "\n";
		}


		tmp_file.close();
		stud_score_file.close();

		try {
			rename("Score.bin", "_Score.bin");
			rename("tmp.bin", "Score.bin");
			remove("_Score.bin");
		}
		catch (exception e) {
			WWC::ErrOut("Egor: Ошибка записи базы!");
		}

		remove("tmp.bin");
		return;
	}

	void delete_student(string stud_id) {
		delete_students_score(stud_id);

		fstream stud_file("Students.bin", fstream::in | fstream::binary);
		fstream tmp_file("tmp.bin", fstream::out | fstream::binary);
		int n = 0;
		string tmp;

		while (!stud_file.eof()) {
			getline(stud_file, tmp);
			student tmp_st = stud_parse(tmp, 4);

			if (tmp_st.id == stud_id)
				continue;
			tmp_file << tmp << "\n";
		}


		tmp_file.close();
		stud_file.close();

		try {
			rename("Students.bin", "_Students.bin");
			rename("tmp.bin", "Students.bin");
			remove("_Students.bin");
		}
		catch (exception e) {
			WWC::ErrOut("Egor: Ошибка записи базы!");
		}

		remove("tmp.bin");
		return;
	}
};