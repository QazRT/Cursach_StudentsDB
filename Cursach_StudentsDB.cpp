#include <iostream>
#include <string>
#include <conio.h>
#include "EditDataClass.h"
#include "Menu.h"
#include "WorkWithConsole.h"
#include "Work_with_DB.h"

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

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    WwDB* wwdb = new WwDB();
    wwdb->__init__();
    //wwdb->add_student("БИСО-03-22", "22Б0863", "asd", "asd", "asd");
    //wwdb->add_student("БИСО-02-22", "22Б0864", "dsa", "dda");


    //wwdb->get_students_by_group("БИСО-02-22");

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

    StudEditm:
    int excode = StudentEdit(stud_selectedItem);
    switch (excode) {
    case -1:
        goto Studlistm;
        break;
    case 0: {
        //wwdb->add_student_score({4, "22Б0864", "Hu", ExamType::exam, 3});
        //cout << wwdb->encryptDecrypt("asd");
        //wwdb->add_student({ "БИСО-02-22", "22Б0864", "asd", "asd", "asd", "20.03.2003", "2021", "ИКБ", "КБ-2"});
        student tmp = EditStudInfo(stud_selectedItem);
        if (tmp.id != "-1") {
            wwdb->edit_student(tmp);
            stud_selectedItem = tmp;
        }

        system("cls");
        goto StudEditm;
    }
    break;
    case 1:
        wwdb->add_student_score({6, "22Б0864", "KYUV", ExamType::zach, '-'});
        break;
    case 2:
        break;
    }

    delete wwdb;  
}
