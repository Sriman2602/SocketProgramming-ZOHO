#include<iostream>
using namespace std;
int pos;
struct visited {
	int point,posx, posy, value;
}n[20];

int a[5][5]={0}, b[5][5] = {0},d[5][5]={0}, r, c, e, f, i, j, k, posx, posy,player;
int neig[7][7] = { 0 };
char arr[5][5],op[11][11]={ };
int point1, point2;

void isPoint(int r, int c) {
	for (i = 0; i < r; i++)
		for (j = 0; j < c; j++) {
			if ((i == 0 || i == r - 1) && ((j == 0) || (j == c - 1))) {
				arr[i][j] = 'c';
				b[i][j] = 2;
			}
			else if (i == 0 || i == r - 1 || j == 0 || j == c - 1){
				arr[i][j] = 'e';
				b[i][j] = 3;
			}
			else {
				arr[i][j] = 'm';
				b[i][j] = 4;
			}
	}
}
void fillstruct() {
	for (i = 0; i < r - 1; i++) {
		for (j = 0; j < c - 1; j++) {
			n[pos].point = pos;
			n[pos].posx = i;
			n[pos].posy = j;
			pos++;
		}
	}
}
//void printstruct() {
//	int len;
//	len = fillstruct();
//	cout << "POINT  POSX  POSY VISITED" << endl;
//	for (i = 0; i < len; i++) {
//		cout << n[i].point << "  " << n[i].posx << "  " << n[i].posy << "  " << n[i].value << endl;
//	}
//}

void fill1(int sou,int des) {
	for (i = 0; i < r; i++) {
		for (j = 0; j < c; j++) {
			if (sou == a[i][j]){
				d[i][j]++;
				if (des - sou == 1)
					op[2 * i][2 * j + 1] = '-';
				else if (des - sou == c)
					op[2 * i + 1][2 * j] = '|';
			}
			if (des == a[i][j])
				d[i][j]++;
			cout << d[i][j] << " ";
		}		
		cout << endl;
	}
	cout << endl;
	for (i = 0; i < 2*r-1; i++) {
		for (j = 0; j < 2 * c - 1; j++) {
			cout << op[i][j];
		}
		cout << endl;
	}
}

int swapplay(int player) {
	for (i = 0; i < r - 1; i++) {
		for (j = 0; j < c - 1; j++) {
			for (int l = 0; l < pos; l++) {
					if (n[l].posx == i && n[l].posy == j) {
						if(n[l].value==0){
							if (d[i][j] > 1 && d[i + 1][j] > 1 && d[i][j + 1] > 1 && d[i + 1][j + 1] > 1) {
								n[l].value = 1;
								cout << 2 * i + 1 << " " << 2 * j + 1 << endl;
								if (player == 1) {
									point1++;
									cout << "Player 1 Captured\n";
									return 1;
								}
								else {
									point2++;
									cout << "Player 2 Captured\n";
									return 2;
								}
							}
						}
						
					}
			}
		}
	}
	if (player == 1)
		return 2;
	return 1;
}

int exitcase() {
	for (i = 0; i < r; i++) {
		for (j = 0; j < c; j++) {
			if (d[i][j] != b[i][j]) {
				return 1;
			}
		}
	}
	return 0;
}

int main() {
	cin >> r >> c;
	memset(op, ' ', sizeof (op));
	for (i = 0; i < 2*r-1; i++) {
		for (j = 0; j < 2*c-1; j++)
			if (i % 2 == 0 && j % 2 == 0)
				op[i][j] = '.';
	}
	int play = 1;
	fillstruct();
	cout << "GAME INITIALIZED FOR" << endl;
	for (i = 0; i < r; i++) {
		for (j = 0; j < c; j++) {
			a[i][j] = k + 1;
			k++;
			cout << a[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	for (i = 0; i < 2*r-1; i++) {
		for (j = 0; j < 2*c-1; j++) {
			cout << op[i][j];
		}
		cout << endl;
	}
	cout << endl;
	isPoint(r, c);
	while (exitcase() == 1) {
		cout << "Enter input for Player "<<play <<":";
		cin >> e >> f;
		fill1(e,f);
		play = swapplay(play);
	}
	cout << "\n\n\t\t\t\tGAME OVER!!!!";
	cout << "\n\t P1: " << point1 << "\t P2: " << point2;
	if (point1 > point2) {
		cout << "\nPlayer 1 wins. ";
	}
	else if (point2 > point1) {
		cout << "\nPlayer 2 wins.";
	}
	else
		cout << "GAME TIED.";

}
