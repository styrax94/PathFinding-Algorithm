/*
Student ID: w1581155
Saul Barrera


Algorithms: Theory Design and Implementation
*/
#pragma once
#include <iostream>
#include <Windows.h>
#include <Wincon.h>
#include <string>

using namespace std;

static class color
{
private:


#define std_con_out GetStdHandle(STD_OUTPUT_HANDLE)




public:


	static enum concol  // enumeration type
	{
		black = 0,
		dark_blue = 1,
		dark_green = 2,
		dark_aqua, dark_cyan = 3,
		dark_red = 4,
		dark_purple = 5, dark_pink = 5, dark_magenta = 5,
		dark_yellow = 6,
		dark_white = 7,
		gray = 8,
		blue = 9,
		green = 10,
		aqua = 11, cyan = 11,
		red = 12,
		purple = 13, pink = 13, magenta = 13,
		yellow = 14, white = 15
	};

	static void setColor(concol textcol, concol backcol)
	{
		// Combinethe two 4 -bit numbers into a single 8- bit number
		// If backcol is abcd in binary, and textcol is efgh, the result is abcdefgh

		unsigned short wAttributes = (backcol << 4) | textcol;

		// set the attributes for the console
		SetConsoleTextAttribute(std_con_out, wAttributes);
	}
};