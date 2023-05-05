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

enum class Sex : char { man, woman, CombatHelicopter };	// А давай
enum class ExamType : char { exam, zach };

struct student {
	string group;
	string id;
	string surname;
	string name;
	string middle_name = "ND";
	string bday = "01.01.1999";
	string admyear = "1999"; // Год поступления
	string inst;
	string kaf;
	Sex sex = Sex::CombatHelicopter;
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
			//students_file >> tmp;
			////cout << tmp;
			//cache_write(tmp);
			//groups_set.insert(tmp);
			while (!students_file.eof()) {
				getline(students_file, tmp);
				groups_set.insert(stud_parse(tmp).group);

			}
			groups_set.erase(groups_set.begin());
			groups = groups_set;

			//for (int i = 0; i < groups_set.size(); ++i)
			//{
			//	cache_write(" " + *groups_set.begin());
			//	groups_set.erase(groups_set.begin());

			//}
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
			getline(students_file, tmp);
			n++;
		}
		students_file.close();
		return n-1;
	}

	student stud_parse(string st, int jsr = 0) {
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
					tmp = encryptDecrypt(tmp);
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
	stud_score stud_score_parse(string st) {
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
					tmp = encryptDecrypt(tmp);
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
						stout.value = stof(tmp);
						break;
					default:
						break;
					}
				}
				continue;
			}


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

	void add_student(student st) {
		fstream stfile("Students.bin", fstream::in | fstream::binary);
		
		string tmp;
		while (!stfile.eof()) {
			getline(stfile, tmp);
			if (stud_parse(tmp, 4).id == st.id) {
				WWC::ErrOut("Egor: Студент с таким шифром уже существует!");
				return;
			}
		}

		stfile.close();
		
		stfile.open("Students.bin", fstream::out | fstream::app | fstream::binary);
		string sex = (st.sex == Sex::man) ? "man" : (st.sex == Sex::woman ? "woman" : "CombatHelicopter");
		stfile << "{\"" << encryptDecrypt(st.group) << "\"\"" << encryptDecrypt(st.id) << "\"\"" << encryptDecrypt(st.surname) << "\"\"" << encryptDecrypt(st.name)
			<< "\"\"" << encryptDecrypt(st.middle_name) << "\"\"" << encryptDecrypt(st.bday) << "\"\"" << encryptDecrypt(st.admyear) << "\"\"" << encryptDecrypt(st.inst)
			<< "\"\"" << encryptDecrypt(st.kaf) << "\"\"" << encryptDecrypt(sex) << "\"}\n";
		stfile.close();

		groups.insert(st.group);

		stud_count += 1;
		cache_edit();
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

		stsc_file.close();
		return scores;
	}
	int add_student_score(stud_score stud_sc) {
		fstream stud_score_file("Score.bin", fstream::out | fstream::app | fstream::binary);

		string ex_type = (stud_sc.extype == ExamType::exam) ? "exam" : "zach";
		//stud_score_file << stud_sc.id << " " << stud_sc.stud_id << " " << stud_sc.subj << " " << ex_type << " " << stud_sc.value << " endl ";
		stud_score_file << "{" << stud_sc.id << "\"" << encryptDecrypt(stud_sc.stud_id) << "\"\"" << encryptDecrypt(stud_sc.subj) 
			<< "\"\"" << encryptDecrypt(ex_type) << "\"\"" << encryptDecrypt(to_string(stud_sc.value)) << "\"}\n";

		stud_score_file.close();


		return stud_sc.id;
	}
	void edit_student_score(stud_score stud_sc) {
		//vector<stud_score> scores;

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
				tmp_file << "{" << tmp_ssc.id << "\"" << encryptDecrypt(tmp_ssc.stud_id) << "\"\"" << encryptDecrypt(tmp_ssc.subj)
					<< "\"\"" << encryptDecrypt(ex_type) << "\"\"" << encryptDecrypt(to_string(tmp_ssc.value)) << "\"}\n";

				while (!stud_score_file.eof()) {
					getline(stud_score_file, tmp);
					tmp_file << tmp << "\n";
				}

				
				tmp_file.close();
				stud_score_file.close();

				remove("Score.bin");
				rename("tmp.bin", "Score.bin");

				return;
			}
			tmp_file << tmp << "\n";
		}
		tmp_file.close();
		stud_score_file.close();
		remove("tmp.bin");
		return;
	}
	
};