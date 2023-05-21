#pragma once
#include <iostream>
#include <string.h>
#include "windows.h"
#include <conio.h>
#include <stdio.h>

using namespace std;

class WWC {
public:
	static void ConsColor(int ForC = 0, int BackC = 0) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((BackC & 0x0F) << 4) + (ForC & 0x0F));
	}

	static void ErrOut(string err) {
		WWC::ConsColor(FOREGROUND_RED);
		std::cout << std::endl << err << "\n";
		WWC::ConsColor(15);
	}
};