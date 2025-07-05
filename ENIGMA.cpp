#include<iostream>
#include<conio.h>
#include<windows.h>

//接收键盘信号的宏
#define left 'K'
#define right 'M'
#define up 'H'
#define down 'P'

using namespace std;

const int rounds = 3; //一共设置几个轮子
string org, cry;
int rs[4], a, b, c; //rs是机器每个轮子的偏移值，取值范围是1-26（偏移多少位）
char letter, key; //one by one

void colorc(int x) { //1.红 2.绿 3.蓝
	if (x == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	if (x == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	if (x == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	if (x == 7)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void wheel() { //参数是看五个轮盘当中的哪一个
	rs[1]++;
	if (rs[1] >= 26) {
		rs[1] = 1;
		rs[2]++;
	}
	if (rs[2] >= 26) {
		rs[2] = 1;
		rs[3]++;
	}
}

int line[26];
int reflect(int drift, int thing) { //凯撒加密 ，制作字母表
	for (int k = 1; k <= 26; k++) {
		if (k + drift <= 26)line[k + drift] = k;
		else if (k + drift > 26)line[k + drift - 26] = k;
	}
	b = 64 + line[thing - 64];
	//cout <<"凯撒"<<drift<<"位后得："<<b<<"\n";
	return b;
}

int reflect2(int drift2, int thing2) { //凯撒加密 ，制作字母表
	for (int k = 1; k <= 26; k++) {
		if (k + drift2 <= 26)line[k] = k + drift2;
		else if (k + drift2 > 26)line[k] = k + drift2 - 26;
	}
	b = 64 + line[thing2 - 64];
	//cout <<"凯撒"<<0-drift2<<"位后得："<<b<<"\n";
	return b;
}

void cryp(int sty = 0) {
	for (int i = 0; i <= org.size() - 1; i++) {
		//cout <<"第"<<i<<"个字母";
		a = org[i];
		//cout <<org[i]<<"/"<<a<<"\n";
		for (int j = 1; j <= rounds; j++) {
			//cout <<"rs"<<j<<"是"<<rs[j]<<"\n";
			if (sty == 0)a = reflect(rs[j], a); //加密
			else  a = reflect2(rs[j], a);
		}
		wheel();
		letter = a;
		cry.append(1, letter);
		//cout <<"cry:"<<cry<<"\n";
	}
}

int judge;
void playc() {
	colorc(7);
	cry = "";
	cout << "Input the settings for every rotator";
	for (int i = 1; i <= rounds; i++) {
		while (1) {
			cout << "\nRotator #" << i << ":";
			cin >> rs[i];
			if (rs[i] >= 1 && rs[i] <= 26) break;
		}
	}
	while (1) {
		judge = 0;
		cout << "\n\nOriginal text/ciphertext (only in CAPITAL LETTERS):";
		cin >> org;
		for (int i = 0; i <= org.size() - 1; i++) {
			a = org[i];
			//cout <<org[i]<<"/"<<a<<"\n";
			if (a < 65 || a > 90)judge++;
		}
		//cout <<judge<<"\n";
		if (judge == 0)break;
	}
	cout << "Press [up button] to see the encrypted result, and press [down button] for the decoded result\n";
	while (1) {
		key = getch();
		if (key == up) {
			colorc(2);
			cryp(0);
			cout << "\n\n\nencrypted result:" << cry << "\n";
			break;
		}
		if (key == down) {
			colorc(3);
			cryp(1);
			cout << "\n\n\ndecoded result:" << cry << "\n";
			break;
		}
	}
}

int main() {
	playc();
	colorc(7);
	cout << "AGAIN? (press [up button] to restart, press [down button] to quit)\n";
	while (1) {
		key = getch();
		if (key == up) {
			playc(); main();
		}
		if (key == down) break;
	}
	return 0;
}
