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

int StudentEdit(student stud) {
    MenuClass* stud_edit_menu = new MenuClass("Студент: " + stud.id + " " + stud.surname + " " + stud.name + " " + stud.middle_name);
    int code = stud_edit_menu->stud_edit();

    delete stud_edit_menu;
    return code;
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

    Studlistm:
    student stud_selectedItem = StudentsListMenu(wwdb->get_students_by_group(selectedItem));

    if (stud_selectedItem.id == "-1")
        goto Groupsm;

    int excode = StudentEdit(stud_selectedItem);
    switch (excode) {
    case -1:
        goto Studlistm;
        break;
    case 0:
        //wwdb->add_student_score({0, "22Б0869", "Hui", ExamType::exam, 4.5});
        wwdb->add_student("БИСО-02-22", "22Б0831", "asd", "asd", "asd");
        //cout << wwdb->encryptDecrypt("asd");
        break;
    case 1:
        wwdb->edit_student_score({ 0, "22Б0869", "Hui", ExamType::exam, 3.0});
        break;
    }

    delete wwdb;  
}
