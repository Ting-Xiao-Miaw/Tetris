#include <iostream>
#include <curses.h>
#include <cmath>
#include <windows.h>
#include <string>
#include "wincursor.h"
#define pi acos(-1)

using namespace std;

unsigned short occupied[nBottomBorder][nRightBorder * 2] = { 0 };

struct Square
{
	int dx;
	int dy;
};

struct Tetromino
{
	Square s[4];
};

Tetromino t[7] = {
					0,0, 1,0, 2,0, 3,0 ,
					0,0, 0,1, 1,1, 2,1 ,
					0,1, 1,1, 2,1, 2,0 ,
					0,0, 1,0, 0,1, 1,1 ,
					0,1, 1,1, 1,0, 2,0 ,
					0,1, 1,1, 1,0, 2,1 ,
					0,0, 1,0, 1,1, 2,1
};

int max_y[7] = { 8, 6, 6, 4, 6, 6, 6 };

Square center[7] = { 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

class CTetromino
{
public:
	CTetromino(unsigned short id = 0, unsigned short x = 0, unsigned short y = 0)
	{
		type_id = id;
		pos_x = x;
		pos_y = y;
		rotation = 0;
		Draw();
		myScreen.redraw();
	}
	void Draw()
	{
		String sp(" ");
		double i = (rotation % 4) * pi / 2;
		for (int j = 0; j < 4; j++) {
			sp.show((t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) - (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y, ((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) + (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x, type_id + 1);
			sp.show((t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) - (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y, ((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) + (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x + 1, type_id + 1);
		}
	}
	void Erase()
	{
		String sp(" ");
		double i = (rotation % 4) * pi / 2;
		for (int j = 0; j < 4; j++) {
			sp.show((t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) - (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y, ((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) + (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x, 0);
			sp.show((t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) - (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y, ((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) + (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x + 1, 0);
		}
	}
	unsigned short Move(int type)
	{
		double i = (rotation % 4) * pi / 2;
		Border();
		Erase();
		myScreen.redraw();
		switch (type) {
		case 0:
			if (type_id != 3) {
				rotation++;
				if (check_rotate() == false) {
					rotation--;
				}
			}
			return 1;
			break;
		case 1:
				if (detect_confliction()) {
					Draw();
					update_occupy();
					myScreen.redraw();
					return 0;
				}
				else {
					pos_y++;
					Draw();
					myScreen.redraw();
					return 1;
				}
		case 2:
			for (int j = 0; j < 4; j++) {
				if (occupied[(t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) - (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y][((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) + (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x - 1] != 0) {
					Draw();
					myScreen.redraw();
					return 0;
				}
			}
			pos_x -= 2;
			Draw();
			myScreen.redraw();
			return 1;
			break;
		case 3:
			for (int j = 0; j < 4; j++) {
				if (occupied[(t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) - (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y][((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) + (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x + 2] != 0) {
					Draw();
					myScreen.redraw();
					return 0;
				}
			}
			pos_x += 2;
			Draw();
			myScreen.redraw();
			return 1;
			break;
		default:
			return 1;
			break;
		}
	}
	bool detect_confliction()
	{
		bool flag = false;
		double i = (rotation % 4) * pi / 2;
		for (int j = 0; j < 4; j++) {
			if (occupied[(t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) - (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y + 1][((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) + (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x] != 0 || occupied[(t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) - (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y + 1][((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) + (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x + 1] != 0) {
				flag = true;
				break;
			}
		}
		return flag;
	}
	void update_occupy() 
	{
		String sp(" ");
		double i = (rotation % 4) * pi / 2;
		int drop = 0;
		for (int j = 0; j < 4; j++) {
			occupied[(t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) - (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y][((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) + (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x] = type_id + 1;
			occupied[(t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) - (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y][((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) + (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x + 1] = type_id + 1;
		}
		for (int y = nTopBorder + 1; y < nBottomBorder; y++) {
			if (check_horizontal(y)) {
				for (int x = (nLeftBorder + 1) * 2; x < nRightBorder * 2; x++) {
					sp.show(y, x, 3);
				}	
				drop++;
			}
		}
		myScreen.redraw();
		Sleep(100);
		for (int y = nTopBorder + 1; y < nBottomBorder; y++) {
			if (check_horizontal(y)) {
				for (int x = (nLeftBorder + 1) * 2; x < nRightBorder * 2; x++) {
					sp.show(y, x, 0);
				}
				//new
				for (int j = y; j > nTopBorder; j--) {
					for (int x = (nLeftBorder + 1) * 2; x < nRightBorder * 2; x++) {
						occupied[j][x] = occupied[j - 1][x];
						sp.show(j, x, occupied[j][x]);
					}
				}
				//new
			}
		}
		Point(drop);
	}
	void Point(int drop)
	{
		String pt("Points:  ", nTopBorder + 1, nLeftBorder * 2 - 15);
		static int a = 0;
		a += drop * drop * 10;
		string str = to_string(a);
		String pt_s(str, nTopBorder + 1, nLeftBorder * 2 - 6);
		pt.show();
		pt_s.show();
	}
private:
	unsigned short type_id;
	unsigned short pos_x;
	unsigned short pos_y;
	unsigned short rotation;
	bool check_rotate()
	{
		double i = (rotation % 4) * pi / 2;
		for (int j = 0; j < 4; j++) {
			if (occupied[(t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) + (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y][((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) - (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x] != 0) {
				return false;
			}
			if (occupied[(t[type_id].s[j].dy - center[type_id].dy) * int(cos(i)) + (t[type_id].s[j].dx - center[type_id].dx) * int(sin(i)) + center[type_id].dy + pos_y][((t[type_id].s[j].dx - center[type_id].dx) * int(cos(i)) - (t[type_id].s[j].dy - center[type_id].dy) * int(sin(i)) + center[type_id].dx) * 2 + pos_x + 1] != 0) {
				return false;
			}
		}
		return true;
	}
	bool check_horizontal(int i) 
	{
		for (int j = (nLeftBorder + 1) * 2; j < nRightBorder * 2; j++) {
			if (occupied[i][j] == 0) {
				return false;
			}
		}
		return true;
	}
	void Border()
	{
		for (int y = 0; y < nBottomBorder; y++) {
			occupied[y][nLeftBorder * 2 + 1] = 1;
			occupied[y][nRightBorder * 2] = 1;
		}
		for (int x = nLeftBorder * 2; x < nRightBorder * 2 + 1; x++) {
			occupied[nBottomBorder][x] = 1;
		}
	}
};