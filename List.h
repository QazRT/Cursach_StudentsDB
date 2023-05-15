#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include "Work_with_DB.h"

using namespace std;

struct list {
    student data;
    struct list* next;
};

class ListClass {
private:
    list* myHead;
    int countItem = 0;

public:
    ListClass() {
        myHead;
        countItem = 0;
    }
    ~ListClass() {
        struct list* old = NULL;
        struct list* current = myHead;
        while (current != NULL) {
            old = current;
            current = current->next;
            delete old;
        }
    }

    void addItem(student _data)
    {
        struct list* newItem = new list();
        newItem->data = _data;
        if (countItem == 0)
            newItem->next = NULL;
        else
            newItem->next = myHead;
        myHead = newItem;
        countItem++;
    }
    void insertItem(int index, student _data) {
        if (not (index >= 0 and index <= countItem and countItem >= 0))
            return;
        if (index == 0)
            addItem(_data);
        else {
            struct list* current = myHead;
            for (int i = 0; i < index - 1; i++) {
                current = current->next;
            }
            struct list* newItem = new list();
            newItem->data = _data;
            newItem->next = current->next;
            current->next = newItem;
            countItem++;
        }
    }
    void push_back(student _data) {
        insertItem(countItem, _data);
    }

    int getCount() {
        return countItem;
    }

    student getItem(int index) {
        student _data = { "Error::list_error" };
        if (index >= 0 && index < countItem && countItem > 0) {
            list* current = myHead;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
            _data = current->data;
        }

        return _data;
    }
};