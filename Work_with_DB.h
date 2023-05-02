#pragma once
#include "windows.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <set>
#include <conio.h>
#include <algorithm>
#include <stdio.h>

using namespace std;

enum class ExamType : char { exam, zach };

struct student {
	string id;
	string surname;
	string name;
	string middle_name = "";
	string group;
};
struct stud_score {
	int id;
	string stud_id;
	string subj;
	ExamType extype;
	float value;
};

class WwDB {
private:
	int stud_count = 0;
	set<string> groups;

public:
	void __init__() {
		fstream students_file("Students.bin", fstream::out | fstream::app | fstream::binary);
		students_file.close();
		fstream tmpf("tmp", fstream::out | fstream::app | fstream::binary);
		tmpf.close();
		fstream stud_score_file("Score.bin", fstream::out | fstream::app | fstream::binary);
		stud_score_file.close();
		fstream cache("cache", fstream::out | fstream::app | fstream::binary);
		cache.close();

		stud_count = students_count();

		cache_edit();
	}

	void cache_edit() {
		fstream cache;
		cache.open("cache", fstream::in | fstream::binary);
		
		if (cache.peek() == EOF || !cache.is_open()) {
			cache.close();

			fstream students_file("Students.bin", fstream::in | fstream::binary);
			string tmp;
			set<string> groups_set;
			students_file >> tmp;
			cout << tmp;
			cache_write(tmp);
			groups_set.insert(tmp);
			while (!students_file.eof()) {
				students_file >> tmp;
				if (tmp == "endl")
				{
					students_file >> tmp;
					if (tmp != "endl")
						groups_set.insert(tmp);
				}
			}
			groups = groups_set;

			for (int i = 0; i < groups_set.size(); ++i)
			{
				cache_write(" " + *groups_set.begin());
				groups_set.erase(groups_set.begin());

			}
			students_file.close();
		}
		else {
			string tmp;
			while (!cache.eof()) {
				cache >> tmp;
				groups.insert(tmp);
			}

			cache.close();
		}
	}

	vector<string> get_groups() {
		vector<string> v(groups.begin(), groups.end());
		return v;
	}

	char* cache_read(int buff_size, int start_location=0) {
		char out[100];
		fstream cache("cache", fstream::in | fstream::binary);
		cache.seekg(start_location);
		cache.read((char*)&out, buff_size);
		cache.close();
		return out;
	}

	void cache_write(string _cache, fstream::openmode openmode = (fstream::app)) {
		fstream cache("cache", openmode | fstream::out | fstream::binary);
		cache << _cache;
		cache.close();
	}


	string encryptDecrypt(string toEncrypt) {
		char key[3] = { 'Z', 'A', 'D' };
		string output = toEncrypt;

		for (int i = 0; i < toEncrypt.size(); i++)
			output[i] = toEncrypt[i] ^ key[i % (sizeof(key) / sizeof(char))];

		return output;
	}


	int students_count() {
		fstream students_file("Students.bin", fstream::in | fstream::binary);
		string tmp;
		int n = 0;
		while (!students_file.eof()) {
			students_file >> tmp;
			if (tmp == "endl")
			{
				n++;
			}
		}
		students_file.close();
		return n-1;
	}

	void add_student(string group, string id, string surname, string name, string middle_name = "ND") {
		fstream students_file("Students.bin", fstream::in | fstream::binary);
		int n = 0;
		char ch[11];
		string tmp_n;
		for (int i = 0; i < stud_count; ++i) {
			n += 10;
			students_file.seekg(n);
			students_file >> ch;
			if (id == ch) {
				WWC::ErrOut("Egor: ������� � ����� ������ ��� ����������!");
				return;
			}
			n += 9;
			tmp_n = "";
			while (tmp_n != "endl") {
				students_file >> tmp_n;
				n += tmp_n.length()+1;
			}
		}
		students_file.close();
		students_file.open("Students.bin", fstream::out | fstream::app | fstream::binary);
		//students_file << group << " " << id << " " << surname << " " << name << " " << middle_name << " endl ";
		students_file << "{\"" << group << "\"; " << id << " ; \"" << encryptDecrypt(surname) << "\"; \"" << encryptDecrypt(name) << "\"; \"" << encryptDecrypt(middle_name) << "\"}\n";
		students_file.close();

		groups.insert(group);

		stud_count += 1;
		cache_edit();
	}

	vector<student> get_students_by_group(string _group) {
		vector<student> students;

		fstream students_file("Students.bin", fstream::in | fstream::binary);
		char ch[11];
		while (!students_file.eof()) {
			students_file >> ch;
			//cout << "ch = " << ch << endl;
			if (ch == _group) {
				student tmp_stud;
				tmp_stud.group = _group;
				students_file >> tmp_stud.id;
				students_file >> tmp_stud.surname;
				students_file >> tmp_stud.name;
				students_file >> tmp_stud.middle_name;

				students.push_back(tmp_stud);
			}

		}
		students_file.close();


		return students;
	}


	void get_student_score(string stud_id) {

	}
	int add_student_score(stud_score stud_sc) {
		fstream stud_score_file("Score.bin", fstream::out | fstream::app | fstream::binary);

		string ex_type = (stud_sc.extype == ExamType::exam) ? "exam" : "zach";
		stud_score_file << stud_sc.id << " " << stud_sc.stud_id << " " << stud_sc.subj << " " << ex_type << " " << stud_sc.value << " endl ";

		stud_score_file.close();


		return stud_sc.id;
	}
	void edit_student_score(stud_score stud_sc) {
		//vector<stud_score> scores;

		fstream stud_score_file("Score.bin", fstream::in | fstream::out | fstream::binary);
		int n = 0;
		string ch;
		//int id;
		while (!stud_score_file.eof()) {
			//stud_score_file >> id;
			//cout << "ch = " << ch << endl;
			//n += 10;
			//stud_score_file.seekg(n);
			stud_score_file >> ch;
			n += ch.size()+1;

			if (atoi((char*)&ch) == stud_sc.id) {
				// ������ ������
				stud_score_file.close();
				fstream stud_score_file("Score.bin", fstream::out | fstream::binary);

				cout << n;
				
				string ex_type = (stud_sc.extype == ExamType::exam) ? "exam" : "zach";
				
				stud_score_file.seekp(2);
				stud_score_file.put('a');
				//tmpf << stud_sc.stud_id << " " << stud_sc.subj << " " << ex_type << " " << stud_sc.value;

				stud_score_file.close();

				return;
			}

			//if (stud_sc.stud_id == ch) {	// ������, ��������� ������ ��������
			//	tmpscore.id = id;
			//	tmpscore.stud_id = stud_sc.stud_id;
			//	
			//	stud_score_file >> tmp_n;
			//	tmpscore.extype = tmp_n == "exam" ? ExamType::exam : ExamType::zach;

			//	stud_score_file >> tmpscore.value;

			//	scores.push_back(tmpscore);
			//}

			//n += 9;
			//tmp_n = "";
			//while (tmp_n != "endl") {
			//	stud_score_file >> tmp_n;
			//	n += tmp_n.length() + 1;
			//}

		}
		stud_score_file.close();
		return;
	}
	
};