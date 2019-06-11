#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>
using namespace std;
void gotoxy(int xpos, int ypos); 

const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;
const int KEY_SPACE = 32;
const int KEY_ESC = 27;
const int KEY_ENTER = 13;

const int SIZEX = 16;
const int SIZEY = 20;
const int FMAP_COUNTS = 7;

#define TRUE 1 
#define FALSE 0   

#define SCR_SP  '\42'  
#define SCR_OB  '\43'  
char main_color[] = "color ##";
int screen[SIZEX][SIZEY] = { 0 }; 
int map[4][4]; 
int px, py, score, nextmap;
int f = 4;
const char* GAME_TITLE ="**********  *********  **********  ********  **     ***   *********  \n"
						"**  **  **  **         **  **  **  **    **  **    ****   **     **  \n"
						"**  **  **  **         **  **  **  **    **  **   ** **   **         \n"
						"    **      *********      **      ********  **  **  **   **         \n"
						"    **      **             **      **        ** **   **   **         \n"
						"    **      **             **      **        ****    **   **     **  \n"
						"  ******    *********    ******   ****       ****    **   *********  \n\n";
int fmap[FMAP_COUNTS][4][4] 
{
	{
		{1, 1, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0},
		{1, 0, 0, 0}
	},
	{
		{0, 0, 1, 0},
		{1, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{1, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{0, 1, 1, 0},
		{1, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{1, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{1, 1, 1, 0},
		{0, 1, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	}
};

void print(void) 
{
	int i, j;
	int buff[SIZEX][SIZEY];

	for (i = 0; i < SIZEY; i++) 
		for (j = 0; j < SIZEX; j++) 
			buff[j][i] = screen[j][i]; 
	for (i = 0; i < f; i++) 
		for (j = 0; j < f; j++) 
			if (map[j][i]) buff[j + px][i + py] = 1; 

	gotoxy(0, 0); 
	for (i = 0; i < SIZEY; i++)
	{
		for (j = 0; j < SIZEX; j++)
		{
			putchar(buff[j][i] == 0 ? SCR_SP : SCR_OB); 
		}
		putchar('\n');
	}

	gotoxy(SIZEX + 1, 0);
	cout << "Score:" << score ;
}

void printnextmap(void)
{
	int i, j;

	gotoxy(SIZEX + 1, 2);
	cout << "Next: ";

	for (i = 0; i < f; i++)
	{
		gotoxy(SIZEX + 2, i + 3);
		for (j = 0; j < f; j++)
		{
			putchar(fmap[nextmap][j][i] == 0 ? ' ' : SCR_OB);
		}
	}
}
 
int getkey(void) 
{
	int c;
	if (_kbhit())  
	{
		if ((c = _getch()) == 224)
			c = _getch();
		 
		return c;
	}
	return FALSE;
}
  
void gotoxy(int xpos, int ypos) 
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE); 
	scrn.X = xpos; scrn.Y = ypos; 
	SetConsoleCursorPosition(hOuput, scrn); 
}
 
int valnewpos(int x, int y) 
{
	int i, j;
	if (x < 0) return FALSE;
	for (i = 0; i < f; i++)
	{
		for (j = 0; j < f; j++)
		{
			if (map[j][i])
			{
				if ((j + x >= SIZEX) || (i + y >= SIZEY)) return FALSE; 
				if (screen[j + x][i + y])
				{
					return FALSE; 
				}
			}
		}
	}
	return TRUE; 
}

#define inv(x) ((x * (-1)) + 3)
 
void rotatemap(void) 
{
 
	int _map[4][4];
	int i, j, sx = 4, sy = 4;

	for (i = 0; i < f; i++)
		for (j = 0; j < f; j++)
		{
			_map[j][i] = map[j][i]; 
			if (map[j][i]) 
			{
				if (i < sx) sx = i; 
				if (inv(j) < sy) sy = inv(j); 
			}
			map[j][i] = 0; 
		}

	for (i = 0; i < f; i++)
		for (j = 0; j < f; j++)
			if (_map[inv(i)][j]) map[j - sx][i - sy] = 1; 
	if (!valnewpos(px, py)) 
		for (i = 0; i < f; i++)
		for (j = 0; j < f; j++)
			map[j][i] = _map[j][i]; 
}
 
int rnd(int max)
{
	max++;
	return (int)(rand() * max / RAND_MAX);
}
 
void sleep(int milsec)
{
	clock_t t = clock();
	while (clock() - t < milsec);
}

void deleteline(void)
{
	int i, j, k, cl;

	for (i = SIZEY - 1; i >= 0; i--)
	{
		cl = 1;
		for (j = 0, cl = 1; j < SIZEX; j++)
		{
			if (!screen[j][i]) cl = 0;
		}
		if (cl)
		{
			gotoxy(0, i);
			for (k = 0; k < SIZEX; k++) putchar('_'), sleep(20);

			score += (((i * (-1)) + SIZEY) * 10);

			for (k = i; k > 0; k--)
			{
				for (j = 0; j < SIZEX; j++)
				{
					screen[j][k] = screen[j][k - 1];
				}
			}
			i++;
			print();
		}
	}
}
 
void createmap(void)
{
	int i, j;

	for (i = 0; i < f; i++)
		for (j = 0; j < f; j++)
			map[j][i] = fmap[nextmap][j][i];
	py = 0;
	px = SIZEX / 2;

	nextmap = rnd(FMAP_COUNTS - 1);
	printnextmap();
}
 
void clearscreen(void)
{
	int i, j;
	for (i = 0; i < SIZEY; i++)
		for (j = 0; j < SIZEX; j++)
			screen[j][i] = 0;
}

void createrndscreen(void)
{
	int i, j, rn;
	clearscreen();
	rn = rnd(10);
	for (i = SIZEY - 1; i >= (SIZEY - 1) - rn; i--)
		for (j = 0; j < SIZEX; j++)
		{
			screen[j][i] = rnd(1);
		}
} 

void startgame(void) 
{
	int i, j, c;
	time_t tm;

	system("cls");
	px = SIZEX / 2;
	py = 0;
	score = 0;

	tm = clock();

	nextmap = rnd(FMAP_COUNTS - 1);
	createmap();

	while (TRUE)
	{
		c = getkey();

		switch (c)
		{
		case KEY_UP:
			rotatemap();
			break;
		case KEY_SPACE:
			rotatemap();
			break;
		case KEY_DOWN:
			for (; valnewpos(px, py + 1); py++);
			for (i = 0; i < f; i++)
				for (j = 0; j < f; j++)
					if (map[j][i]) screen[px + j][py + i] = 1;

			print();
			deleteline();
			createmap();
			break;
		case KEY_LEFT:
			if (valnewpos(px - 1, py)) px--;
			break;
		case KEY_RIGHT:
			if (valnewpos(px + 1, py)) px++;
			break;
		case 'p':
			_getch();
			break;
		case KEY_ESC:
			return;
		}

		if ((clock() - tm) > 720)
		{
			tm = clock();

			if (!(valnewpos(px, py + 1)))
			{
				for (i = 0; i < f; i++)
					for (j = 0; j < f; j++)
						if (map[j][i]) screen[px + j][py + i] = 1;

				createmap();
				deleteline();
			}
			else py++;
		}

		print();

		for (i = 0; i < SIZEX; i++)
		{
			if (screen[i][0])
			{
				system("cls");
				gotoxy(2, 8);
				cout << "GAME OVER!!1!" ;
				return;
			}
		}
	}
}

void gamemenu(void)
{
	int p = 1, c = 0;
	const char* GAME_MENU = "                     +===============================+\n"
		"                     |  1. Start                     |\n"
		"                     +===============================+\n"
		"                     |  2. Start(difficult)          |\n"
		"                     +===============================+\n"
		"                     |  3. Control                   |\n"
		"                     +===============================+\n"
		"                     |  4. Exit the game             |\n "
		"                    +===============================+";
	system("cls"); 
	cout << GAME_TITLE << GAME_MENU;
	while (TRUE)
	{
		c = _getch();
		switch (c)
		{
		case '1':
		case '2':
		case '3':
		case '4':
			p = c - '0';
		case KEY_ENTER:
			switch (p)
			{
			case 1:
				clearscreen();
				startgame();
				gotoxy(0, SIZEY); 
				cout << "Press ESC to exit to the main menu...";
				while (_getch() != KEY_ESC);
				break;
			case 2:
				clearscreen();
				createrndscreen();
				startgame();
				gotoxy(0, SIZEY); 
				cout <<"Press ESC to exit to the main menu..."; 
				while (_getch() != KEY_ESC);
				break;
			case 3:
				system("cls");
				cout << GAME_TITLE <<
					"+=======+=======+\n"
					"|<      |left   |\n"
					"|>      |right  |\n"
					"|       |down   |\n"
					"|Space  |Turn   |\n"
					"|P      |Pause  |\n"
					"|ESC    |exit   |\n"
					"+=======+=======+\n\n"
					"Press the key...\n";
				_getch();
				if (_kbhit())
					_getch();
				break;
			case 4: return;
			}
			system("cls"); 
			cout <<  GAME_TITLE << GAME_MENU;
			p = 1;
			break;
		case KEY_UP:
			if (p > 1) p--;
			break;
		case KEY_DOWN:
			if (p < 4) p++;
			break;
		case KEY_ESC:
			return;
		}

	}
}

int main()
{
	system("color 47");
	cout << GAME_TITLE ;
	cout<<"              Press any key to continue.";

	_getch();
	if (_kbhit()) _getch();
	gamemenu();
}
