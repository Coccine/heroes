/*
 * (C)occine, 2018
 * GNU GPL
 *
 * Console Game Center (CGC)
 *
 */


#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <locale.h>

typedef struct Enemy
{
	int xEnemy;
	int yEnemy;
	int getScore;
	int HP;

}Enemy;

// Размеры игрового поля
size_t width;
size_t height;
// Завершение и паузу игры
int gameOver;
char pauseGame;
// события передвижения игрока
typedef enum Direction { STOP, UP, DOWN, LEFT, RIGHT } Direction;
// события перемещения мячика
typedef enum DirectionBall {BallLEFT, BallRIGHT, BallULEFT, BallURIGHT, BallDLEFT, BallDRIGHT, BSTOP, BUP}DirectionBall;
// события движения мяча по значению платформы
typedef enum PlayerTouch {DEFAULT, FIRST1, FIRST2, FIRST3, SECOND1, SECOND2, SECOND3}PlayerTouch;

Direction dir; // Первый игрок в space war или в ping-pong
Direction dir2;// Второй игрок в пинг-понг
DirectionBall dboll;// Мячик
PlayerTouch pt;

Enemy enemy[10];

void setupPingPong(int* , int* , int* , int* , int* , int* , int* , int* , int* , int* , int* , int* , int* , int*);
void setupSpaceWars(int*, int*);

void drawPingPong(int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*);
void drawSpaceWars(int*,int*,int*,int*,int*);

void logicPingPong(int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*,int*);
void logicSpaceWars(int*, int*, int*,int*);

void inputPingPong(int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*);
void inputSpaceWars(int*,int*,int*,int*,int*);

void checkGoal(int*, int*, int*, int*);

void startMassage(int*);
void aboutInfo(int*);

void createEnemys();
void killEnemy(int*, int*);

int main(void)
{
	// Выбор действия
	int variant = 0;
	int exitApp = 0;

	// Позиция игрока space wars
	int px, py;

	// Позиция мячика ping-pong
	int xb, yb;
	// Позиция платформ ping-pong
	int first1X, first2X, first3X, first1Y, first2Y, first3Y;
	int second1X, second2X, second3X, second1Y, second2Y, second3Y;
	// Общий счет ping-pong
	int score_1 = 0;
	int score_2 = 0;
	int score = 0;
	// Вспомогательные переменные для
	// взаимодействия платформы и мячика ping-pong
	int hodX1 = 0, hodX2 = 0, hodX3 = 0;
	int hodY1 = 0, hodY2 = 0, hodY3 = 0;


	startMassage(&variant);
	srand((unsigned)time(NULL));
	while (!exitApp) {
		switch (variant)
		{
		case 1:
			system("color 3f");
			setupPingPong(&first1X, &first2X, &first3X, &first1Y, &first2Y, &first3Y, &second1X, &second2X, &second3X,
				&second1Y, &second2Y, &second3Y, &yb, &xb);
			while (!gameOver)
			{
				drawPingPong(&first1X, &first2X, &first3X, &first1Y, &first2Y, &first3Y, &second1X, &second2X, &second3X,
					&second1Y, &second2Y, &second3Y, &yb, &xb, &score_1, &score_2);
				
				inputPingPong(&exitApp, &first1X, &first2X, &first3X, &first1Y, &first2Y, &first3Y,
					&second1X, &second2X, &second3X, &second1Y, &second2Y, &second3Y, &xb, &yb);
				
				logicPingPong(&first1X, &first2X, &first3X, &first1Y, &first2Y, &first3Y, &second1X, &second2X, &second3X,
					&second1Y, &second2Y, &second3Y,&yb, &xb, &hodY1, &hodY2, &hodY3, &hodX1, &hodX2, &hodX3);
				checkGoal(&xb, &score_2, &yb, &score_1);
				Sleep(20);
			}
			break;
		case 2:
			system("color 1f");
			setupSpaceWars(&px,&py);
			while (!gameOver)
			{
				drawSpaceWars(&px,&py,&score,&yb,&xb);
				inputSpaceWars(&exitApp,&yb,&xb,&px,&py);
				logicSpaceWars(&px, &py, &yb,&xb);
				Sleep(20);
			}
			break;
		case 3:
			system("color 67");
			aboutInfo(&variant);
			break;
		case 4:
			exitApp = 1;
			break;
		}
	}

	system("pause");
	return 0;
}

///////////// Инициализация значений и подготовки игр  ////////////
void setupPingPong(int *first1X, int *first2X, int *first3X, int *first1Y, int *first2Y, int *first3Y,
	int *second1X, int *second2X, int *second3X, int *second1Y, int *second2Y, int *second3Y,
	int *yb, int *xb)
{
	gameOver = 0;
	dir = STOP;
	width = 19;
	height = 80;
	dboll = 0;
	pt = DEFAULT;
	*first1X = 1; *first2X = 1; *first3X = 1;
	*first1Y = width / 2 - 1; *first2Y = width / 2; *first3Y = width / 2 + 1;

	*second1X = height - 2; *second2X = height - 2; *second3X = height - 2;
	*second1Y = width / 2 - 1; *second2Y = width / 2; *second3Y = width / 2 + 1;
	*yb = width / 2;
	*xb = height / 2;
}
void setupSpaceWars(int *px, int *py)
{
	gameOver = 0;
	dir = STOP;
	width = 30;
	height = 20;
	*px = height / 2;
	*py = width - 2;
	createEnemys();
}


//////////// Прорисовка игрового поля //////////////
void drawPingPong(int *first1X, int *first2X, int *first3X, int *first1Y, int *first2Y, int *first3Y,
	int *second1X, int *second2X, int *second3X, int *second1Y, int *second2Y, int *second3Y,
	int *yb, int *xb, int *score_1, int *score_2)
{
	system("cls");
	char **area;
	area = (char**)calloc(sizeof(char*), width);
	for (size_t i = 0; i < width; i++)
	{
		area[i] = (char*)calloc(sizeof(char), height);
	}

	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			if (j == 0 || j == height - 1)
				area[i][j] = 179;
			if (i == 0 || i == width - 1)
				area[i][j] = 196;
			if (j == *first1X && (i == *first1Y || i == *first2Y || i == *first3Y))
				area[i][j] = 178;
			if (j == *second1X && (i == *second1Y || i == *second2Y || i == *second3Y))
				area[i][j] = 178;
			if (j == *xb && i == *yb)
				area[i][j] = 'O';
		}
	}
	// Прорисовка углов карты
	area[0][0] = 218;
	area[0][height - 1] = 191;
	area[width - 1][0] = 192;
	area[width - 1][height - 1] = 217;

	for (size_t i = 0; i < width; i++) {
		for (size_t j = 0; j < height; j++)
		{
			printf("%c", area[i][j]);
		}
		puts("");
	}
	printf("score_1: %d\t\t\t\tscore_2: %d", *score_1, *score_2);
	// Освобождение паямти
	for (size_t i = 0; i < width; i++)
		free(area[i]);
	free(area);
}
void drawSpaceWars(int *px, int *py, int *score,int *yb, int *xb)
{
	system("cls");

	char **area;
	area = (char**)calloc(sizeof(char*), width);
	for (size_t i = 0; i < width; i++)
	{
		area[i] = (char*)calloc(sizeof(char), height);
	}

	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			if (j == 0 || j == height - 1)
				area[i][j] = 179;
			if (i == 0 || i == width - 1)
				area[i][j] = 196;
			if (j == *px && i == *py)
				area[i][j] = 202;
			for (size_t k = 0; k < height - 2; k++) {
				if (enemy[k].xEnemy == j && enemy[k].yEnemy == i)
				{
					if (enemy[k].HP == 10)
						area[i][j] = 203;
					else if (enemy[k].HP == 6)
						area[i][j] = 209;
					else if (enemy[k].HP == 4)
						area[i][j] = 194;
				}
			}
			if (*yb == i && *xb == j)
				area[i][j] = 253;
		}

	}
	// Прорисовка углов карты
	area[0][0] = 218;
	area[0][height - 1] = 191;
	area[width - 1][0] = 192;
	area[width - 1][height - 1] = 217;

	for (size_t i = 0; i < width; i++) {
		printf("\t\t\t\t");
		for (size_t j = 0; j < height; j++)
		{
			printf("%c", area[i][j]);
		}
		puts("");
	}
	printf("\t\t\tscore: %d", *score);
	// Освобождение памяти
	for (size_t i = 0; i < width; i++)
		free(area[i]);
	free(area);
}


//////////// Игровая логика  ////////////
void logicPingPong(int *first1X, int *first2X, int *first3X, int *first1Y, int *first2Y, int *first3Y,
	int *second1X, int *second2X, int *second3X, int *second1Y, int *second2Y, int *second3Y,
	int *yb, int *xb, int *hodY1, int *hodY2, int *hodY3, int *hodX1, int *hodX2, int *hodX3)
{
	// Движение 1-го игрока
	switch (dir)
	{
	case UP:
		for (int i = 0; i < 3; i++) {
			if (*first1Y != 1) {
				*first1Y -= 1;
				*first2Y -= 1;
				*first3Y -= 1;;
				dir = STOP;
			}
		}
		break;
	case DOWN:
		for (int i = 0; i < 3; i++)
		{
			if (*first3Y != width - 2) {
				*first1Y += 1;
				*first2Y += 1;
				*first3Y += 1;
				dir = STOP;
			}
		}
		break;
	}
	// Движение 2-го игрока
	switch (dir2)
	{
	case UP:
		for (int i = 0; i < 3; i++)
		{
			if (*second1Y != 1)
			{
				*second1Y -= 1;
				*second2Y -= 1;
				*second3Y -= 1;
				dir2 = STOP;
			}
		}
		break;
	case DOWN:
		for (int i = 0; i < 3; i++)
		{
			if (*second3Y != width - 2)
			{
				*second1Y += 1;
				*second2Y += 1;
				*second3Y += 1;
				dir2 = STOP;
			}
		}
		break;
	}
	// Обработка отбивания
	switch (pt)
	{
	case FIRST1:
		*hodY1 = 0;
		*hodY2 = 0;
		*hodY3 = 0;
		if (*yb == 1 || *yb == width - 2)
			*hodX1 += 1;
		if (*hodX1 % 2 == 0)
			dboll = BallURIGHT;
		else if (*hodX1 % 2 != 0)
			dboll = BallDRIGHT;
		else
			dboll = BallURIGHT;
		break;
	case FIRST2:
		dboll = BallRIGHT;
		break;
	case FIRST3:
		if (*yb == 1 || *yb == width - 2)
			*hodX2 += 1;
		if (*hodX2 % 2 == 0)
			dboll = BallDRIGHT;
		else if (*hodX2 % 2 != 0)
			dboll = BallURIGHT;
		else
			dboll = BallDRIGHT;
		break;
	case SECOND1:
		*hodX1 = 0;
		*hodX2 = 0;
		if (*yb == 1 || *yb == width - 2)
			*hodY1 += 1;
		if (*hodY1 % 2 == 0)
			dboll = BallULEFT;
		else if (*hodY1 % 2 != 0)
			dboll = BallDLEFT;
		else
			dboll = BallULEFT;
		break;
	case SECOND2:
		dboll = BallLEFT;
		break;
	case SECOND3:
		if (*yb == 1 || *yb == width - 2)
			*hodY2 += 1;
		if (*hodY2 % 2 == 0)
			dboll = BallDLEFT;
		else if (*hodY2 % 2 != 0)
			dboll = BallULEFT;
		else
			dboll = BallDLEFT;
		break;
	}
	// Движение мяча
	//dboll = rand() % 9;
	switch (dboll)
	{
	case BallLEFT:
		if (*xb != 1) {
			*xb -= 1;
		}
		break;
	case BallRIGHT:
		if (*xb != height - 2) {
			*xb += 1;
		}
		break;
	case BallDLEFT:
		if (*yb != width - 2 && *xb != 1) {
			*yb += 1;
			*xb -= 1;
		}
		break;
	case BallDRIGHT:
		if (*yb != width - 2 && *xb != height - 2) {
			*yb += 1;
			*xb += 1;
		}
		break;
	case BallULEFT:
		if (*yb != 1 && *xb != 1) {
			*yb -= 1;
			*xb -= 1;
		}
		break;
	case BallURIGHT:
		if (*yb != 1 && *xb != height - 2) {
			*yb -= 1;
			*xb += 1;
		}
		break;
	}
	dboll = DEFAULT;
}
void logicSpaceWars(int *px, int *py, int *yb,int *xb)
{
	switch (dir)
	{
	case LEFT:
		if (*px != 1)
		{
			*px -= 1;
			dir = STOP;
		}
		break;
	case RIGHT:
		if (*px != height - 2)
		{
			*px += 1;
			dir = STOP;
		}
		break;
	case UP:
		if (*py != width - 5)
		{
			*py -= 1;
			dir = STOP;
		}
		break;
	case DOWN:
		if (*py != width - 2)
		{
			*py += 1;
			dir = STOP;
		}
		break;
	}
	switch (dboll)
	{
	case UP:
		if (*yb != 2) {
			*yb -= 1;
		}
		else {
			*yb = 100;
			dboll = BSTOP;
		}
			break;
	}
	killEnemy(yb,xb);
}


//////////// Проверка вводимых данных ////////////
void inputPingPong(int *exitApp, int *first1X, int *first2X, int *first3X, int *first1Y, int *first2Y, int *first3Y,
	int *second1X, int *second2X, int *second3X, int *second1Y, int *second2Y, int *second3Y, int *xb, int *yb)
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
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case '8':
			dir2 = UP;
			break;
		case '2':
			dir2 = DOWN;
			break;
		case 'x':
			printf("\n\nClose game.\n");
			gameOver = 1;
			*exitApp = 1;
			break;
		case 'p':
			gameOver = 1;
			printf("\nPause game\n\n");
			system("pause");
		}
	}
	if (*first1X + 1 == *xb && *first1Y == *yb) {
		pt = FIRST1;
	}
	else if (*first2X + 1 == *xb && *first2Y == *yb) {
		pt = FIRST2;
	}
	else if (*first3X + 1 == *xb && *first3Y == *yb) {
		pt = FIRST3;
	}
	if (*second1X - 1 == *xb && *second1Y == *yb) {
		pt = SECOND1;
	}
	else if (*second2X - 1 == *xb && *second2Y == *yb) {
		pt = SECOND2;
	}
	else if (*second3X - 1 == *xb && *second3Y == *yb) {
		pt = SECOND3;
	}
}
void inputSpaceWars(int *exitApp, int* yb, int* xb, int *px, int *py)
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
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'e':
			dboll = UP;
			*yb = *py - 1;
			*xb = *px;
			break;
		case 'x':
			printf("\n\nClose game.\n");
			gameOver = 1;
			*exitApp = 1;
			break;
		case 'p':
			gameOver = 1;
			printf("\nPause game\n\n");
			system("pause");
		}
	}
}


//////////// Пороверка некотрых данных игр ////////////
void checkGoal(int *xb, int *score_2, int *yb, int *score_1)
{
	if (*xb == 1)
	{
		*score_2 += 1;
		*xb = height / 2;
		*yb = width / 2;
	}
	if (*xb == height - 2) {
		*score_1 += 1;
		*xb = height / 2;
		*yb = width / 2;
	}
}


/////////// Стартовое окно приложения ///////
void startMassage(int *variant)
{
	system("color e1");
	printf("\t\tWelcome to console game center\n\n");
	puts("____________________________¶¶____¶¶");
	puts("_______________________¶¶____¶¶____¶¶");
	puts("________________________¶¶____¶¶____¶¶");
	puts("_________________¶¶______¶¶____¶¶____¶¶");
	puts("	__________________¶¶¶¶_____¶¶____¶¶____¶¶");
	puts("_____________¶¶______¶¶_________________¶¶");
	puts("______________¶¶¶¶________________________¶¶");
	puts("________________¶¶¶___________¶¶_________¶¶");
	puts("_________________¶¶_____¶¶_____¶¶______¶¶");
	puts("__________________¶¶_____¶¶____________¶¶");
	puts("__________________¶¶___________________¶¶");
	puts("_____¶¶___¶¶______¶¶____________¶¶_____¶¶");
	puts("______¶¶__¶¶______¶¶____¶¶_____¶¶_____¶¶");
	puts("_______¶¶_¶¶_______¶¶______¶¶¶¶¶_____¶¶");
	puts("______¶¶¶¶¶¶¶_______¶¶______________¶¶");
	puts("_____¶¶¶¶¶¶¶¶¶¶_______¶¶¶¶¶¶¶¶¶¶¶¶¶¶__¶¶");
	puts("_____¶¶¶¶¶¶¶¶¶¶___________¶¶___________¶¶");
	puts("_______¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶¶____________¶¶");
	puts("__________________________¶¶_____________¶¶");
	printf("Press ENTER for continue...");
	getchar();
	system("cls");
	puts("Menu");
	puts("1. Ping-pong");
	puts("2. Space wars DEBUG-MODE.");
	puts("3. About games and Settings");
	puts("4. Exit");
	scanf_s("%d", variant);
}


////////// Информация     ///////////
void aboutInfo(int *variant)
{
	system("cls");
	puts("*(C)occine, 2018");
	puts("*GNU GPL");
	puts("*");
	puts("*Console Game Center (CGC)\n\n");
	puts("______________________________");
	setlocale(LC_ALL, "ru");
	puts("PING_PONG_GAME: ");
	puts("1.Об игре: ");
	puts("Настольный теннис (иногда в разговорной речи ошибочно употребляется название пинг-понг) - ");
	puts("— олимпийский вид спорта, спортивная игра с мячом, в которой используют ракетки и игровой стол.");
	puts("Задачей игроков является при помощи ракеток отправить мяч на сторону соперника так, ");
	puts("чтобы тот не смог его возвратить назад в соответствии с правилами.\n\n");
	puts("2. Управление: ");
	puts("Первый игрок управляет платформой слева клавишами 'w' -> вверх 's' -> вниз.");
	puts("Второй игрок управляет платформой справа клавишами '8' -> вверх '2' -> вниз.");
	puts("Дополнительные клавиши: 'x' -> выход 'p' -> пауза.");
	puts("Если мячь был не отбит частью плаформы, то противнику засчитывается 1 очко.");
	puts("РЕКАМЕНДАЦИЯ: ожидать отбивание мяча противником и после двигать свою платформу");
	puts("_______________________________");
	puts("SPACE_WARS_GAME");
	puts("");
	puts("");
	puts("");
	puts("");
	puts("");
	setlocale(LC_ALL, "C");
	system("pause");
	system("cls");
	startMassage(variant);
}


///////// Создание противников /////////
void createEnemys()
{
	for (size_t i = 0; i < height-2; i++)
	{
		enemy[i].xEnemy = i + 1;
		enemy[i].yEnemy = 2;
		if (i == 1 || i == height - 3)
		{
			enemy[i].getScore = 20;
			enemy[i].HP = 10;
		}
		else if (i == 3 || i == height - 5 ){
			enemy[i].getScore = 15;
			enemy[i].HP = 6;
		}
		else if (i % 2 != 0){
			enemy[i].getScore = 10;
			enemy[i].HP = 4;
		}
	}
}

///// Уничтожение противников ////////
void killEnemy(int *yb, int *xb)
{
	for (size_t i = 0; i < width / 2; i++)
	{
		if (*yb + 1 == enemy[i].yEnemy && *xb == enemy[i].xEnemy) {
			enemy[i].HP -= 1;
		}
		if (enemy[i].HP <= 0) {
			enemy[i].xEnemy = 5;
			enemy[i].yEnemy = 10;
		}
	}
}
