#include<iostream>
#include<conio.h>
#include<ios>
#include<limits>
#include<windows.h>
#include<winuser.h>
#include<winbase.h>

//接收键盘信号的宏
#define left 'K'
#define right 'M'
#define up 'H'
#define down 'P'

using namespace std;

const unsigned char rounds = 3; //一共设置几个轮子
string org, cry, tofind;
unsigned int rs[4], a, b, c; //rs是机器每个轮子的偏移值，取值范围是1-26（偏移多少位）
char letter, key; //one by one
bool isbadint;

void colorc(int x) { //1.红 2.绿 3.蓝
	if (x == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	if (x == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	if (x == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	if (x == 7)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void badint() {
	if (!cin.fail()) {
		if (cin.peek() != '\n') {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			isbadint = true;
		} else {
			isbadint = false;
		}
	}
	else {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		isbadint = true;
	}
}

void note_clipp() {
	cout <<"(The text has been added to your clippboard)\n";
}

void setcopy(char* tocopy) {
	HWND hWnd = NULL;
	OpenClipboard(hWnd);
	EmptyClipboard();
	HANDLE hHandle = GlobalAlloc(GMEM_FIXED, 32);
	char* pData = (char*) GlobalLock(hHandle);
	strcpy(pData, tocopy);
	SetClipboardData(CF_TEXT, hHandle);
	GlobalUnlock(hHandle);
	CloseClipboard();
}

void wheel() {
	rs[1]++;
	if (rs[1] >= 26) {
		rs[1] = 1;
		rs[2]++;
	}
	if (rs[2] >= 26) {
		rs[2] = 1;
		rs[3]++;
	}
	if (rs[3] >= 26)rs[rounds] = 26;
}

int line[27];
int reflect(int drift, int thing) { //凯撒加密 ，制作字母表
	for (int k = 1; k <= 26; k++) {
		if (k + drift <= 26)line[k + drift] = k;
		else if (k + drift > 26)line[k + drift - 26] = k;
	}
	b = 64 + line[thing - 64];
	return b;
}

int reflect2(int drift2, int thing2) { //凯撒加密 ，制作字母表
	for (int k = 1; k <= 26; k++) {
		if (k + drift2 <= 26)line[k] = k + drift2;
		else if (k + drift2 > 26)line[k] = k + drift2 - 26;
	}
	b = 64 + line[thing2 - 64];
	return b;
}

void cryp(int sty = 0) {
	for (int i = 0; i <= org.size() - 1; i++) {
		a = org[i];
		//cout <<org[i]<<"/"<<a<<"\n";
		for (int j = 1; j <= rounds; j++) {
			if (sty == 0)a = reflect(rs[j], a); //加密
			else  a = reflect2(rs[j], a);
		}
		wheel();
		letter = a;
		cry.append(1, letter);
	}
}

void orgize() {
	colorc(7);
	cry = "";
	tofind = "";
}

int judge;
void input_orgize(bool regular = true) {
	while (1) {
		judge = 0;
		cout << "(only in CAPITAL LETTERS WITHOUT SPACE):";
		if (regular)cin >> org;
		else cin >> tofind;
		if (regular) {
			for (int i = 0; i <= org.size() - 1; i++) {
				a = org[i];
				if (a < 65 || a > 90)judge++;
			}
		}
		else {
			for (int i = 0; i <= tofind.size() - 1; i++) {
				a = tofind[i];
				if (a < 65 || a > 90)judge++;
			}
		}
		if (judge == 0)break;
	}
}

void playc() {
	cout << "Input the settings for every rotator";
	for (int i = 1; i <= rounds; i++) {
		while (1) {
			cout << "\nRotator #" << i << ":";
			cin >> rs[i];
			badint();
			if (!isbadint && rs[i] >= 1 && rs[i] <= 26) break;
		}
	}
	cout << "\n\nCiphertext/Original text ";
	input_orgize();
	cout << "Press [up button] to see the encrypted result, and press [down button] for the decoded result\n";
	while (1) {
		key = _getch();
		if (key == up) {
			colorc(2);
			cryp(0);
			cout << "\n\n\nencrypted result:" << cry << "\n";
			setcopy(cry.data());
			note_clipp();
			break;
		}
		if (key == down) {
			colorc(3);
			cryp(1);
			cout << "\n\n\ndecoded result:" << cry << "\n";
			setcopy(cry.data());
			note_clipp();
			break;
		}
	}
}

void brute_force() {
	cout << "\n\nCiphertext ";
	input_orgize();
	cout << "All the possible decoded results are shown:\n";
	for (int h = 1; h <= 26; h++) {
		for (int i = 1; i <= 26; i++) {
			for (int l = 1; l <= 26; l++) {
				cry = "";
				rs[1] = h;
				rs[2] = i;
				rs[3] = l;
				cryp(1);
				colorc(3);
				if (tofind == "" || cry.find(tofind) != string::npos)cout << h << " " << i << " " << l;
				colorc(7);
				if (tofind == "" || cry.find(tofind) != string::npos)cout << ":" << cry << "\n\n";
			}
		}
	}
}

void mode_choose() {
	orgize();
	cout << "------------------------------\n";
	cout << "The settings of rotators are:\nA) Known\nB) Unknown\n(Press a key to continue)\n";
	while (1) {
		key = _getch();
		if (key == 'A' || key == 'a') {
			playc();
			break;
		}
		if (key == 'B' || key == 'b') {
			cout << "Is there any text to search within the decoded result? (Press [up button] to confirm, press [down button] to refuse)\n";
			while (1) {
				key = _getch();
				if (key == up) {
					while (1) {
						cout << "Input the text to be searched:";
						input_orgize(false);
						break;
					}
					break;
				}
				if (key == down) break;
			}
			brute_force();
			break;
		}
	}
}

int main() {
	bool restart = true;
	while (restart) {
		mode_choose();
		colorc(7);
		cout << "AGAIN? (Press [up button] to restart, press [down button] to quit)\n";
		while (1) {
			key = _getch();
			if (key == up) {
				break;
			}
			if (key == down) break;
		}
	}
	return 0;
}