#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>


const int width = 50;
const int height = 10;
int gameOver;
int x1, y1, x2, y2, xb, yb;
int score_1 = 0;
int score_2 = 0;


enum Direction { STOP, UP, DOWN };
enum Direction dir;

void setupGame()
{
	gameOver = 0;
	dir = STOP;
	x1 = 0; y1 = height / 2;
	x2 = 50 - 3; y2 = height / 2;
	xb = 50 / 2 - 1;
	yb = 10 / 2 - 1;
}

void drawArea()
{
	system("cls");
	for (int i = 0; i < width +1; i++) {
		if (i == 0) {
			printf("%c", 218);
		}
		else if (i == width) {
			printf("%c", 191);
		}
		else {
			printf("%c", 196);
		}
	}
	puts("");

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0) {
				printf("%c", 179);
			}
			if (j == width -1) {
				printf("%c", 179);
			}
			if (i == y1 && j == x1) {
				printf("%c", 178);
			}
			if (i == y2 && j == x2) {
				printf("%c", 178);
			}
			if (i == yb && j == xb) {
				printf("%c", 253);
			}
			else printf(" ");
		}
		printf("\n");
	}
	for (int i = 0; i < width + 1; i++) {
		if (i == 0) {
			printf("%c", 192);
		}
		else if (i == width) {
			printf("%c", 217);
		}
		else {
			printf("%c", 196);
		}
	}
	puts("");
	printf("score_1: %d\t\t\t\tscore_2: %d", score_1, score_2);
}



void gameLogic()
{
	switch (dir)
	{
	case UP:
		if (y1 > 0) {
			y1--;
			dir = STOP;
		}
		break;
	case DOWN:
		if (y1 < 11) {
			y1++;
			dir = STOP;
		}
		break;
	}
}

void input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'x':
			gameOver = 1;
			break;
		}
	}
}

int main(void)
{
	setupGame();
	while (!gameOver)
	{
		drawArea();
		input();
		gameLogic();
		Sleep(20);
	}

	getchar();
	return 0;
}
