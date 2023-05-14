#include <iostream>
#include <string>
#include <conio.h>
#include "EditDataClass.h"
#include "Menu.h"
#include "WorkWithConsole.h"
#include "Work_with_DB.h"
#include "List.h"
#include <thread>
#include <chrono>

using namespace std;


string selectedItem;

void GroupMenu(vector<string> groups) {
   MenuClass* mainMenu = new MenuClass("Главное меню");
    for (int i = 0; i < groups.size(); ++i)
        mainMenu->addMenuItem(groups[i]);

    selectedItem = mainMenu->group_select();
    delete mainMenu;
}

student StudentsListMenu(vector<student> studs) {
    MenuClass* stud_list = new MenuClass("Список студентов группы "+selectedItem);

    //cout << studs[0].id;
    for (int i = 0; i < studs.size(); ++i)
        stud_list->addMenuItem(studs[i]);

    student stud_selectedItem = stud_list->stud_select();
    delete stud_list;

    return stud_selectedItem;
}

void AddStudent(student& stud) {
    system("cls");
    MenuClass* stud_add_menu = new MenuClass("Изменение студента с шифром: " + stud.id);
    stud = stud_add_menu->EditStudInfoMenu(stud);

    delete stud_add_menu;
    return;
}

int StudentEdit(student stud) {
    MenuClass* stud_edit_menu = new MenuClass("Студент: " + stud.id + " " + stud.surname + " " + stud.name + " " + stud.middle_name);
    int code = stud_edit_menu->stud_edit(stud.id);

    delete stud_edit_menu;
    return code;
}
student EditStudInfo(student stud) {
    MenuClass* stud_edit_menu = new MenuClass("Изменение студента с шифром: " + stud.id);
    stud = stud_edit_menu->EditStudInfoMenu(stud);

    delete stud_edit_menu;
    return stud;
}

bool ExitMenu() {
    MenuClass* ExMenu = new MenuClass("");
    ExMenu->addMenuItem("Да");
    ExMenu->addMenuItem("Нет");
    cout << "Вы точно хотите выйти?";
    if (!ExMenu->ItemSelect({ {0, {FOREGROUND_INTENSITY, 0, FOREGROUND_GREEN}}, {1, {FOREGROUND_INTENSITY, 0, FOREGROUND_RED}} })) {
        system("cls");
        exit(1);
    }
    system("cls");
    WWC::ConsColor(15);
    return false;
}

stud_score EditStudScore(string stud_id) {
    MenuClass* EDSc = new MenuClass("Изменение данных зачетки");
    stud_score stsc = EDSc->EditStudScoreMenu(stud_id);
    delete EDSc;
    if (stsc.stud_id == "-1")
        return stsc;
    
    EDSc = new MenuClass("Изменение данных оценки");
    stsc = EDSc->EditScoreInfoMenu(stsc);
    delete EDSc;

    return stsc;
}

void preparing() {
    cout << "Preparing |";
    while (true) {
        cout << "\b/";
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "\b--";
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "\b \b\b\\";
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "\b|";
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void variant(WwDB* wwdb) {
    thread t(preparing);
    t.detach();

    vector<student> students = wwdb->get_students_by_group(selectedItem);
    ListClass* otl_stud = new ListClass();
    ListClass* bad_stud = new ListClass();
    bool fl = true;

    for (int i = 0; i < students.size(); ++i) {
        fl = true;
        vector<stud_score> stsc = wwdb->get_student_score(students[i].id);

        for (int i = 0; i < stsc.size(); ++i) {
            if (stsc[i].value == '-' || stsc[i].value < 4) {
                bad_stud->push_back(students[i]);
                fl = false;
                break;
            }
        }

        if (fl)
            otl_stud->push_back(students[i]);
    }

    t.~thread();
    system("cls");

    student tmp;
    cout << "Отличники/хорошисты:\n";
    for (int i = 0; i < otl_stud->getCount(); ++i) {
        tmp = otl_stud->getItem(i);
        cout << "   " << tmp.id << " " << tmp.surname << " " << tmp.name << " " << tmp.middle_name << " " << tmp.bday << endl;
    }

    cout << "\nТроечники:\n";
    for (int i = 0; i < bad_stud->getCount(); ++i) {
        tmp = bad_stud->getItem(i);
        cout << "   " << tmp.id << " " << tmp.surname << " " << tmp.name << " " << tmp.middle_name << " " << tmp.bday << endl;
    }

    delete otl_stud;
    delete bad_stud;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    WwDB* wwdb = new WwDB();
    wwdb->__init__();

    
    Groupsm:
    GroupMenu(wwdb->get_groups());
    if (selectedItem == "n") {
        system("cls");
        student stud;
        EditDataClass* edc = new EditDataClass();
        while (true) {
            cout << "Введите шифр студента: ";
            WWC::ShowConsoleCursor(true);
            stud.id = edc->getData(editType::stud_id);

            if (wwdb->check_student(stud))
                break;
        }

        AddStudent(stud);
        
        if (stud.id == "-1")
            goto Groupsm;

        wwdb->add_student(stud);
        delete edc;
        system("cls");
        goto Groupsm;
    }
    else if (selectedItem == "-1") {
        system("cls");
        ExitMenu();
        goto Groupsm;
    }


    Studlistm:
    student stud_selectedItem = StudentsListMenu(wwdb->get_students_by_group(selectedItem));

    if (stud_selectedItem.id == "-1")
        goto Groupsm;

    else if (stud_selectedItem.id == "var") {
        system("cls");
        variant(wwdb);
        exit(0);
    }

    StudEditm:
    int excode = StudentEdit(stud_selectedItem);
    switch (excode) {
    case -1:
        goto Studlistm;
        break;
    case 0: {
        student tmp = EditStudInfo(stud_selectedItem);
        if (tmp.id != "-1") {
            wwdb->edit_student(tmp);
            stud_selectedItem = tmp;
        }

        system("cls");
        goto StudEditm;
    }
    break;
    case 1: {
        stud_score stsc = EditStudScore(stud_selectedItem.id);

        if ( stsc.id == -2 || stsc.stud_id == "-1")
            goto StudEditm;
        else if (stsc.stud_id == "ddd") {
            wwdb->delete_score(stsc.id);
        }
        else if (stsc.id == -1) {
            stsc.id = wwdb->get_max_score_id() + 1;
            wwdb->add_student_score(stsc);
        }
        else
            wwdb->edit_student_score(stsc);
        system("cls");
        goto StudEditm;
        }
    case 2:
        cout << stud_selectedItem.id;
        wwdb->delete_student(stud_selectedItem.id);
        break;
    }

    delete wwdb;  
}
