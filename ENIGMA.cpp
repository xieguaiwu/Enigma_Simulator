#include<iostream>
#include<conio.h>
#include<windows.h>

//���ռ����źŵĺ�
#define left 'K'
#define right 'M'
#define up 'H'
#define down 'P'

using namespace std;

const int rounds = 3; //һ�����ü�������
string org, cry;
int rs[4], a, b, c; //rs�ǻ���ÿ�����ӵ�ƫ��ֵ��ȡֵ��Χ��1-26��ƫ�ƶ���λ��
char letter, key; //one by one

void colorc(int x) { //1.�� 2.�� 3.��
	if (x == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	if (x == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	if (x == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	if (x == 7)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void wheel() { //�����ǿ�������̵��е���һ��
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
int reflect(int drift, int thing) { //�������� ��������ĸ��
	for (int k = 1; k <= 26; k++) {
		if (k + drift <= 26)line[k + drift] = k;
		else if (k + drift > 26)line[k + drift - 26] = k;
	}
	b = 64 + line[thing - 64];
	//cout <<"����"<<drift<<"λ��ã�"<<b<<"\n";
	return b;
}

int reflect2(int drift2, int thing2) { //�������� ��������ĸ��
	for (int k = 1; k <= 26; k++) {
		if (k + drift2 <= 26)line[k] = k + drift2;
		else if (k + drift2 > 26)line[k] = k + drift2 - 26;
	}
	b = 64 + line[thing2 - 64];
	//cout <<"����"<<0-drift2<<"λ��ã�"<<b<<"\n";
	return b;
}

void cryp(int sty = 0) {
	for (int i = 0; i <= org.size() - 1; i++) {
		//cout <<"��"<<i<<"����ĸ";
		a = org[i];
		//cout <<org[i]<<"/"<<a<<"\n";
		for (int j = 1; j <= rounds; j++) {
			//cout <<"rs"<<j<<"��"<<rs[j]<<"\n";
			if (sty == 0)a = reflect(rs[j], a); //����
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
