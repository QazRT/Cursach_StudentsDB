#pragma once
#include <iostream>
#include <string.h>
#include "windows.h"
#include <conio.h>
#include <stdio.h>

using namespace std;

class WWC {
public:
	static void Cur2xy(int xpos, int ypos)
	{
		COORD scrn;
		HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
		scrn.X = xpos; scrn.Y = ypos;
		SetConsoleCursorPosition(hOuput, scrn);
	}
	static void ConsColor(int ForC = 0, int BackC = 0) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((BackC & 0x0F) << 4) + (ForC & 0x0F));
	}
	static void ShowConsoleCursor(bool showFlag)
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_CURSOR_INFO     cursorInfo;

		GetConsoleCursorInfo(out, &cursorInfo);
		cursorInfo.bVisible = showFlag; // set the cursor visibility
		SetConsoleCursorInfo(out, &cursorInfo);
	}
	static void ErrOut(string err) {
		WWC::ConsColor(FOREGROUND_RED);
		std::cout << std::endl << err << "\n";
		WWC::ConsColor(15);
	}
};