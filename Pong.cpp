#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#define VERTICAL_LINE 186
#define ORIZONTAL_LINE 205
#define SQUARE 219
#define N_MAX 19
#define M_MAX 71 //setarea dimensiunilor hartii este facuta pentru a suporta 5 viteze
char map[N_MAX][M_MAX];
int i, j, viteza_x, viteza_y;
int modJoc, nivelVsPC;
int x_ball_old, y_ball_old;
int x_ball_new, y_ball_new;   //coordonatele mingii,cele vechi fiind utilizate pentru a reusi o implementare fara "fluctuatii" a jocului
int puncte1, puncte2;
int lungimepaleta = 5;
int lungimepaleta1 = 5;
int dificultate;
int paleta1_y[5], paleta2_y[5];  //vectorii de constructie a paletelor
int crestere_viteza1, crestere_viteza2;  //variabile initializate cu valori din {-1,0,1} pentru a ajuta la cresterea vitezei
int oldpaleta2_y[5];  //vector utilizat pentru a retine vechile coordonate ale paletei 2 cu scopul afisarii paletei in modul Player Vs Computer
int nivelvsPC;
void hidecursor();
void gotoXY(int x, int y);
void setup();
void showMap();
void restartMap();
void ballMove(int viteza_x, int viteza_y);
void ballHitsMap();
int isPoint();
void kbTouch(); //asimilare a tastelor apasate
void ballHitsPalette();
bool gameover();
void asimilarePuncte(int point);
void Serve();
void menu();
void pVp();
void playervsPC();
int randomWay();
void paletteMovePC();
void kbTouchvsPC();
int randomForPalette(); //functie folosita in generarea unor numere necesare pentru a se lua o decizie referitoare la miscarea paletei in modul Player vs Computer
void moveAfterBall(int k);
void initialMenu();
void menu2();




int main()
{
	initialMenu();
	hidecursor();
	char c;
	c = _getch();

	switch (c)  //alegerea modului de joc(Player vs Player sau Player vs Computer)
	{
	case '1':
	{   modJoc = 1;
	break;
	}
	case '2':
	{
		modJoc = 2;
		break;
	}
	default:
	{   system("cls");
	printf("Tasta introdusa nu este una permisa.Va rugam apasati orice alta tasta pentru a incheia programul si reveniti!");
	_getch();
	exit(1);
	break;
	}
	}
	if (modJoc == 1) pVp();
	else playervsPC();
	return 0;
}
void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
void gotoXY(int x, int y)
{
	COORD coord = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void setup()
{
	for (i = 0; i < N_MAX; i++)
		for (j = 0; j <= 3; j++)
			map[i][j] = ' ';
	Sleep(100);
	for (i = 1; i < N_MAX - 1; i++)
		map[i][4] = (char)SQUARE;
	for (i = 1; i < N_MAX - 1; i++)
		map[i][M_MAX - 1] = (char)SQUARE;
	for (j = 4; j < M_MAX - 1; j++)
		map[0][j] = (char)SQUARE;
	for (j = 4; j < M_MAX - 1; j++)
		map[N_MAX - 1][j] = (char)SQUARE;//initializarea matricei



	map[0][4] = (char)SQUARE;
	map[0][M_MAX - 1] = (char)SQUARE;
	map[N_MAX - 1][4] = (char)SQUARE;
	map[N_MAX - 1][M_MAX - 1] = (char)SQUARE;
	int init = 7;
	int init1 = 7;
	for (i = 0; i < lungimepaleta; i++)
		oldpaleta2_y[i] = paleta2_y[i] = init++;
	for (i = 0; i < lungimepaleta1; i++)
		paleta1_y[i] = init1++;
	if ((puncte1 + puncte2) % 4 < 2)
	{
		x_ball_old = 7;
		y_ball_old = 9;   //plasarea mingii inainte de serva                           
		if (dificultate == 3)
			y_ball_old = 8;


	}
	else
	{
		x_ball_old = M_MAX - 4;
		y_ball_old = 9;
		if (dificultate == 3)
			y_ball_old = 8;
	}
	gotoXY(x_ball_old, y_ball_old);
	printf("o");       //afisarea mingii pe pozitia de serva


}
void showMap()
{

	for (i = 0; i < N_MAX; i++)
	{
		for (j = 0; j < M_MAX; j++)
			printf("%c", map[i][j]);
		printf("\n");
	}
	printf("\n\n");
	printf("\n\n\n");
	printf("     Jucatorul 1\tJucatorul 2 ");
	printf("\n");
	printf("     %d\t\t\t%d", puncte1, puncte2);
	printf("\n\n");

}
void restartMap()
{
	system("cls");
	for (i = 1; i < N_MAX - 1; i++)
		for (j = 1; j < M_MAX - 1; j++)
			map[i][j] = ' ';
	for (i = 1; i < N_MAX - 1; i++)
		map[i][4] = (char)SQUARE;
	j = M_MAX / 2 + 3;
	for (i = 1; i < N_MAX - 1; i = i + 2)
		map[i][j] = (char)SQUARE;
	lungimepaleta = 3;
	lungimepaleta1 = 3;
	map[paleta1_y[0]][6] = (char)SQUARE;
	map[paleta1_y[1]][6] = (char)SQUARE;
	map[paleta1_y[2]][6] = (char)SQUARE;
	map[paleta2_y[0]][M_MAX - 3] = (char)SQUARE;
	map[paleta2_y[1]][M_MAX - 3] = (char)SQUARE;
	map[paleta2_y[2]][M_MAX - 3] = (char)SQUARE;
	//stabilirea dimensiunilor paletelor in functie de modul de joc.Daca modul de joc este Player VS Computer, se va modifica doar dimensiunea paletei 1(cea care este actionata manual)
	if (modJoc == 1)

	{
		if (dificultate == 2)
		{
			if (modJoc == 1)
			{
				lungimepaleta1 = lungimepaleta = 4;
				map[paleta1_y[3]][6] = (char)SQUARE;
				map[paleta2_y[3]][M_MAX - 3] = (char)SQUARE;
			}
		}
	}
	else {
		if (nivelVsPC == 2)
		{
			lungimepaleta1 = 4;
			lungimepaleta = 3;
			map[paleta1_y[3]][6] = (char)SQUARE;
		}
	}
	if (modJoc == 1)
	{
		if (dificultate == 1)
		{

			lungimepaleta1 = lungimepaleta = 5;
			map[paleta1_y[3]][6] = (char)SQUARE;
			map[paleta2_y[3]][M_MAX - 3] = (char)SQUARE;
			map[paleta1_y[4]][6] = (char)SQUARE;
			map[paleta1_y[4]][M_MAX - 3] = (char)SQUARE;


		}
	}
	else {
		if (nivelVsPC == 1)
		{
			lungimepaleta1 = 5;
			lungimepaleta = 3;
			map[paleta1_y[4]][6] = (char)SQUARE;
			map[paleta1_y[3]][6] = (char)SQUARE;
		}
	}
}
void ballMove(int viteza_x, int viteza_y)
{
	x_ball_new = x_ball_old + viteza_x;
	y_ball_new = y_ball_old + viteza_y;

	gotoXY(x_ball_old, y_ball_old);
	printf(" ");
	gotoXY(x_ball_new, y_ball_new);
	printf("o");
	Sleep(100);
	if (x_ball_old == M_MAX / 2)
	{
		for (i = 1; i <= N_MAX - 1; i = i + 2)
			if (i == y_ball_old)
			{
				gotoXY(M_MAX / 2 + 3, y_ball_old);
				printf("%c", SQUARE);
			}
	}
	x_ball_old = x_ball_new;
	y_ball_old = y_ball_new;


}
void ballHitsMap()
{

	if (x_ball_new <= 5)
	{
		for (i = 0; i < N_MAX; i++)
			map[i][5] = (char)SQUARE;
	}
	if (y_ball_new <= 1 || y_ball_new >= N_MAX - 2)
		viteza_y = -viteza_y;
}
int isPoint()
{
	if (x_ball_new + viteza_x <= 5)
	{

		gotoXY(x_ball_new, y_ball_new);
		printf(" ");
		Sleep(100);//Sleep() este folosit cu scopul de  a da un scurt delay inainte de replasarea mingii pe pozitia de serva
		return 1;
	}
	if (x_ball_new + viteza_x >= M_MAX - 2)
	{
		gotoXY(x_ball_new, y_ball_new);
		printf(" ");
		Sleep(100);
		return 2;
	}
	return 0;
}
void kbTouch()
{
	char c;
	if (_kbhit())
	{
		c = _getch();
		switch (c)
		{
		case 'i':

		{  if (paleta2_y[0] >= 2)
			for (i = 0; i < lungimepaleta; i++)
			{
				gotoXY(M_MAX - 3, paleta2_y[i]);
				printf(" ");
				paleta2_y[i]--;
				gotoXY(M_MAX - 3, paleta2_y[i]);
				printf("%c", (char)SQUARE);
			}  //miscarea in sus a paletei 2 in modul PVP
		break;
		}
		case 'k':
		{   	if (paleta2_y[lungimepaleta - 1] <= N_MAX - 3)
			for (i = lungimepaleta - 1; i >= 0; i--)
			{
				gotoXY(M_MAX - 3, paleta2_y[i]);
				printf(" ");
				paleta2_y[i]++;
				gotoXY(M_MAX - 3, paleta2_y[i]);
				printf("%c", (char)SQUARE);
			}//miscarea in jos a paletei 2 in modul PVP

		break;
		}
		case 'w':
		{  if (paleta1_y[0] >= 2)
			for (i = 0; i < lungimepaleta1; i++)
			{
				gotoXY(6, paleta1_y[i]);
				printf(" ");
				paleta1_y[i]--;
				gotoXY(6, paleta1_y[i]);
				printf("%c", (char)SQUARE);
			} //miscarea in sus a paletei 1 in modul PVP
		break;
		}
		case 's':
		{   if (paleta1_y[lungimepaleta1 - 1] <= N_MAX - 3)
			for (i = lungimepaleta1 - 1; i >= 0; i--)
			{
				gotoXY(6, paleta1_y[i]);
				printf(" ");
				paleta1_y[i]++;
				gotoXY(6, paleta1_y[i]);
				printf("%c", (char)SQUARE);
			} //miscarea in jos a paletei 1 in modul PVP
		break;
		}
		case ' ':
		{
			char s;
			while (1)
			{
				s = _getch();
				if (s == ' ')
					break;
			}//Modul prin care este pusa pauza jocului prin tasta Space
			break;
		}
		case 'o':
		{
			crestere_viteza2 = -1;
			break; //cresterea vitezei atunci cand mingea ricoseaza din paleta 2
		}

		case 'q':
		{
			crestere_viteza1 = 1;
			break; //cresterea vitezei atunci cand mingea ricoseaza din paleta 1
		}
		case 'a':
		{
			crestere_viteza1 = -1;
			break; //scaderea vitezei atunci cand mingea ricoseaza din paleta 1
		}


		case 'l':
		{
			crestere_viteza2 = 1;
			break; //scaderea vitezei atunci cand mingea ricoseaza din paleta 2
		}

		}

	}

}
int randomWay()  //functie care contribuie la generarea unei directii random pentru viteza pe axa Oy
{
	srand(time(NULL));
	int c;
	c = rand() % 3;
	if (c == 0) return -1;
	if (c == 1) return 0;
	if (c == 2) return 1;
}
void ballHitsPalette()
{
	if (x_ball_new == 7)
	{
		for (i = 1; i < lungimepaleta1 - 1; i++)
			if (y_ball_new == paleta1_y[i])
			{
				viteza_x = -viteza_x + crestere_viteza1;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza1;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++; //randurile 355-361 sunt utilizate in mod repetat in aceasta functie cu scopul de a seta viteza minima 1 si viteza maxima 5
				viteza_y = randomWay();
			}
		if (y_ball_new == paleta1_y[0])
		{
			if (paleta1_y[0] == 1)//atunci cand partea de sus a paletei este "lipita" de marginea superioara a chenarului, pentru rezolvarea unui bug intalnit in mod repetat,mingea va trebui sa coboare neaparat
			{
				viteza_x = -viteza_x + crestere_viteza1;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza1;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = 1;
			}
			else
			{
				viteza_x = -viteza_x + crestere_viteza1;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza1;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = randomWay();//in caz contrar, directia este random
			}
		}
		if (y_ball_new == paleta1_y[lungimepaleta1 - 1])
		{
			if (paleta1_y[lungimepaleta1 - 1] == N_MAX - 2)//atunci cand partea de jos a paletei este "lipita" de marginea inferioara a chenarului, pentru rezolvarea unui bug intalnit in mod repetat,mingea va trebui sa urce neaparat
			{
				viteza_x = -viteza_x + crestere_viteza1;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza1;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = -1;
			}
			else
			{
				viteza_x = -viteza_x + crestere_viteza1;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza1;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = randomWay();//in caz contrar, directia este random
			}
		}
		if (y_ball_new == paleta1_y[0] - 1)//mingea atinge coltul de sus al paletei 1 si este trimisa spre o directie random.
		{
			if (viteza_y == 1)//Conditia if de pe linia 410 permite aceasta trimitere a mingii doar in momentul in care mingea coboara pentru a oferi un efect vizual apropiat de realitate
			{
				viteza_x = -viteza_x + crestere_viteza1;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza1;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = randomWay();
			}
		}
		if (y_ball_new == paleta1_y[lungimepaleta1 - 1] + 1)//mingea atinge coltul de jos al paletei 1 si este trimisa spre o directie random
		{
			if (viteza_y == -1) //Conditia if de pe linia 424 permite aceasta trimitere a mingii doar in momentul in care mingea urca pentru a oferi un efect vizual apropiat de realitate
			{
				viteza_x = -viteza_x + crestere_viteza1;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza1;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = randomWay();
			}
		}
		crestere_viteza1 = 0; //variabila primeste valoarea nula pentru a nu creste viteza la fiecare contact dintre minge si paleta
	}
	if (x_ball_new == M_MAX - 4)
	{
		for (i = 1; i < lungimepaleta - 1; i++)
			if (y_ball_new == paleta2_y[i])
			{
				viteza_x = -viteza_x + crestere_viteza2;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza2;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = randomWay();//miscare random la atingerea mingii cu paleta 2
			}
		if (y_ball_new == paleta2_y[0])
		{
			if (paleta2_y[0] == 1)//atunci cand partea de sus a paletei este "lipita" de marginea superioara a chenarului, pentru rezolvarea unui bug intalnit in mod repetat,mingea va trebui sa coboare neaparat
			{
				viteza_x = -viteza_x + crestere_viteza2;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza2;
				viteza_y = 1;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = 1;
			}
			else
			{
				viteza_x = -viteza_x + crestere_viteza2;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza2;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = randomWay();//in caz contrar, directia este random
			}
		}
		if (y_ball_new == paleta2_y[lungimepaleta - 1])
		{
			if (paleta2_y[lungimepaleta - 1] == N_MAX - 2)//atunci cand partea de jos a paletei este "lipita" de marginea inferioara a chenarului, pentru rezolvarea unui bug intalnit in mod repetat,mingea va trebui sa urce neaparat
			{
				viteza_x = -viteza_x + crestere_viteza2;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza2;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = -1;
			}
			else
			{
				viteza_x = -viteza_x + crestere_viteza2;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza2;
				if (viteza_x == 6)
					viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = randomWay();//in caz contrar, directia este random
			}
		}
		if (y_ball_new == paleta2_y[0] - 1)//mingea atinge coltul de sus al paletei 2 si este trimisa spre o directie random.
		{
			if (viteza_y == 1) //Conditia if de pe linia 504 permite aceasta trimitere a mingii doar in momentul in care mingea coboara pentru a oferi un efect vizual apropiat de realitate
			{
				viteza_x = -viteza_x + crestere_viteza2;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza2;
				if (viteza_x == 6)
					if (viteza_x == 6)
						viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
				viteza_y = randomWay();
			}
		}
		if (y_ball_new == paleta2_y[lungimepaleta - 1] + 1)//mingea atinge coltul de jos al paletei 2 si este trimisa spre o directie random.
		{
			if (viteza_y == -1)//Conditia if de pe linia 527 permite aceasta trimitere a mingii doar in momentul in care mingea urca pentru a oferi un efect vizual apropiat de realitate
			{
				viteza_x = -viteza_x + crestere_viteza2;
				if (viteza_x == 0)
					viteza_x = -crestere_viteza2;
				if (viteza_x == 6)
					if (viteza_x == 6)
						viteza_x--;
				if (viteza_x == -6)
					viteza_x++;
			}

			viteza_y = randomWay();
		}
		crestere_viteza2 = 0;

	}



}
bool gameover()
{
	if (puncte1 == 11 || puncte2 == 11)
		return true;
	return false;
}
void asimilarePuncte(int point)//functie folosita in concordanta cu isPoint() cu scopul de a retine punctajele celor doi jucatori in vederea afisarii
{
	if (point == 1) puncte2++;
	if (point == 2) puncte1++;

}

void Serve()
{
	int sumapunctaje;
	bool serveste1 = false, serveste2 = false;
	sumapunctaje = puncte1 + puncte2;
	if (sumapunctaje % 4 == 0 || sumapunctaje % 4 == 1) //conditii necesare pentru a schimba paleta de serva din 2 in 2 puncte
	{
		serveste1 = true;
		x_ball_new = 7;
		y_ball_new = 9;
		if (dificultate == 3)
			y_ball_new = 8;//reinitializare minge pentru serva                                     
	}
	else
	{
		serveste2 = true;
		x_ball_new = M_MAX - 4;
		y_ball_new = 9;
		if (dificultate == 3)
			y_ball_new = 8;//reinitializare minge pentru serva
	}
	char dirServa;
	gotoXY(x_ball_new, y_ball_new);
	printf("o"); //afisare minge dupa reinitializare

	while (serveste1 || modJoc == 1)//conditie necesara pentru modul Player vs Computer care conduce la serva automata a jucatorului controlat de Computer
		if (_kbhit())
		{
			dirServa = _getch();
			switch (dirServa)
			{
			case 'd':
			{   if (serveste1)
			{
				viteza_x = 2;
				viteza_y = randomWay(); //serva are o directie random pe Oy si poate fi realizata doar prin tasta d, depinzand in mod expres de jucatorul care serveste,prin variabila booleana serveste1
			}
			else continue;
			break;
			}
			case 'j':
			{   if (serveste2)
			{
				if (modJoc == 1)
				{
					viteza_x = -2;  //serva are o directie random pe Oy si poate fi realizata doar prin tasta d, depinzand in mod expres de jucatorul care serveste,prin variabila booleana serveste2
					viteza_y = randomWay();
				}
			}

			else continue;    //functia continue este utilizata pentru a reintra in ciclul repetitiv din functia finala( pVp sau playerVsComputer) si a nu permite serva prin apasarea oricei taste                     
			break;
			}
			default:
				continue;

			}
			break;
		}
	if (modJoc == 2 && serveste2)
	{
		Sleep(100);
		viteza_x = -2;
		viteza_y = randomWay();
	} //serva pentru modul Player Vs Computer stabilita random, dupa un delay,pentru a permite jucatorului 1 sa reactioneze

}
void menu()
{
	system("cls");
	char c;
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10); //atribuirea unei culori consolei
	printf("\Pentru alegerea dificultatii jocului apasati tastele corespunzatoare\n1-Usor\n2-Mediu\n3-Greu");
	hidecursor();
	c = _getch();
	switch (c)
	{

	case '1':
	{
		dificultate = 1;
		printf("\nDificultatea jocului va fi usoara.");
		break;
	}
	case '2':
	{
		dificultate = 2;
		printf("\nDificultatea jocului va fi medie.");
		break;
	}
	case '3':
	{
		dificultate = 3;
		printf("\nDificultatea jocului va fi grea.");
		break;
	}
	default:
	{   system("cls");
	printf("NICIUNUL DINTRE NIVELE NU A FOST ALES.VA RUGAM REVENITI!");
	_getch();
	exit(1);
	break;
	}
	}

	printf("\nPentru a incepe jocul apasati tasta ENTER : ");
	SetConsoleTextAttribute(hConsole, 9); //setarea culorii pentru joc

}
void menu2()//Alegerea dificultatii pentru modul Player vs Computer
{
	system("cls");
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	printf("Ati ales modul Player vs Computer");
	printf("\nSelectati nivelul apasand tasta corespunzatoare");
	printf("\nTasta 1 ->Usor");
	printf("\nTasta 2 ->Mediu");
	printf("\nTasta 3 ->Greu");
	printf("\nTasta 4 ->Imposibil");
	char c;
	hidecursor();
	c = _getch();
	switch (c)
	{
	case '1':
	{
		nivelVsPC = 1;
		break;
	}
	case '2':
	{
		nivelVsPC = 2;
		break;
	}
	case '3':
	{
		nivelVsPC = 3;
		break;
	}
	case '4':
	{
		nivelVsPC = 4;
		break;
	}
	default:
	{
		printf("Niciunul dintre nivele nu a fost selectat. Va rugam parasiti consola prin apasarea oricarei taste si reveniti!");
		_getch();
		exit(1);
		break;
	}
	}
	SetConsoleTextAttribute(hConsole, 9);

}
void pVp() //functia finala pentru desfasurarea jocului pe modul Player Vs Player
{
	int point;
	menu();
	char c;

	c = _getch();
	if (c == 13)
	{
		hidecursor();
		setup();
		restartMap();
		showMap();
		while (!gameover())
		{
			Serve();
			restartMap();
			showMap();
			point = isPoint();
			while (!point)
			{
				gotoXY(5, N_MAX + 3);
				printf("VITEZA:%d", abs(viteza_x)); //afisarea vitezei sub mapa de joc
				ballMove(viteza_x, viteza_y);
				ballHitsMap();
				kbTouch();
				ballHitsPalette();
				point = isPoint();
				asimilarePuncte(point);
				
			}
			restartMap();
			setup();
			restartMap();
			showMap();


		}


	}
	else
	{
		system("cls");
		printf("Tasta apasata nu a fost ENTER,va rugam reveniti!\nPentru a continua apasati orice alta tasta!");
		_getch();
		exit(1);
	}
	if (gameover())
	{
		system("cls");
		printf("Jocul s-a incheiat!\n");
		if (puncte1 == 11) printf("Jucatorul 1 a castigat");
		else printf("Jucatorul 2 a castigat");
	}
}
void kbtouchvsPC()  //functie similara celei kbtouch(), insa restrictionata la paleta 1 din cauza modului de joc Player vs Computer
{
	char c;
	if (_kbhit())
	{
		c = _getch();
		switch (c)
		{
		case 'w':
		{  if (paleta1_y[0] >= 2)
			for (i = 0; i < lungimepaleta1; i++)
			{
				gotoXY(6, paleta1_y[i]);
				printf(" ");
				paleta1_y[i]--;
				gotoXY(6, paleta1_y[i]);
				printf("%c", (char)SQUARE);
			}
		break;
		}
		case 's':
		{   if (paleta1_y[lungimepaleta1 - 1] <= N_MAX - 3)
			for (i = lungimepaleta1 - 1; i >= 0; i--)
			{
				gotoXY(6, paleta1_y[i]);
				printf(" ");
				paleta1_y[i]++;
				gotoXY(6, paleta1_y[i]);
				printf("%c", (char)SQUARE);
			}
		break;
		}
		case ' ':
		{
			char s;
			while (1)
			{
				s = _getch();
				if (s == ' ')
					break;
			}
			break;
		}
		case 'q':
		{
			crestere_viteza1 = 1;
			break;
		}
		case 'a':
		{
			crestere_viteza1 = -1;
			break;
		}
		}
	}
}
void playervsPC() //functia finala pentru rularea jocului in modul Player Vs Computer
{
	lungimepaleta = 3;
	menu2();
	system("cls");
	printf("Pentru a incepe, apasati tasta ENTER:");
	char c;
	int point;
	c = _getch();
	if (c == 13)
	{
		hidecursor();
		setup();
		restartMap();
		showMap();
		while (!gameover())
		{
			Serve();
			restartMap();
			showMap();
			point = isPoint();
			while (!point)
			{
				gotoXY(5, N_MAX + 3);
				printf("VITEZA:%d", abs(viteza_x));
				point = isPoint();
				asimilarePuncte(point);
				ballMove(viteza_x, viteza_y);
				paletteMovePC();
				ballHitsMap();
				kbtouchvsPC();
				ballHitsPalette();
				//afisarea vitezei dedesubtul chenarului
			}
			setup();
			restartMap();
			showMap();

		}


	}
	else
	{
		system("cls");
		printf("Tasta apasata nu a fost ENTER,va rugam reveniti!\nPentru a continua apasati orice alta tasta!");
		_getch();
		exit(1);
	}
	if (gameover())
	{
		system("cls");
		printf("Jocul s-a incheiat!\n");
		if (puncte1 == 11) printf("Jucatorul 1 a castigat");
		else printf("Jucatorul 2 a castigat");
	}
}
int randomForPalette()
{
	srand(time(NULL));
	int aux;
	aux = rand() % 10;
	return aux;
}
void moveAfterBall(int k)  //Variabila k reprezinta o eroare de k unitati in amplasarea centrului paletei fata de pozitia bilei
{
	if (y_ball_new + k >= N_MAX - 2 || y_ball_new - k <= 1)//restrictionare a paletei in interiorul chenarului
	{

		if (y_ball_new + k >= N_MAX - 2)
		{
			paleta2_y[0] = N_MAX - 4;
			paleta2_y[1] = N_MAX - 3;
			paleta2_y[2] = N_MAX - 2;
		}
		if (y_ball_new - k == 1)
		{
			paleta2_y[0] = 1;
			paleta2_y[1] = 2;
			paleta2_y[2] = 3;
		}//cele doua if uri sunt utilizate pentru a depista in ce caz paleta iese din chenar si o reinitializeaza in capatul acestuia

	}
	else
	{
		paleta2_y[0] = y_ball_new - 1 + k;
		paleta2_y[1] = y_ball_new + k;    //in caz contrar, paleta se misca dupa coordonata pe Oy a bilei, tinand cont de eroarea k
		paleta2_y[2] = y_ball_new + 1 + k;
	}
	for (i = 0; i < 3; i++)
	{
		gotoXY(M_MAX - 3, oldpaleta2_y[i]);
		printf(" ");
	}
	for (i = 0; i < 3; i++)
	{
		gotoXY(M_MAX - 3, paleta2_y[i]);
		printf("%c", SQUARE);
	}
	for (i = 0; i < 3; i++)
		oldpaleta2_y[i] = paleta2_y[i]; //miscarea paletei se realizeaza prin retinerea coordonatelor vechi si afisarea prin intermediul functiei gotoXY() la pozitiile aferente
}
void paletteMovePC()
{
	int chancenumber;
	if (nivelVsPC == 4)
		moveAfterBall(0);  //miscarea se va produce exact dupa y-ul bilei
	if (nivelVsPC == 3)
	{
		int chancenumber;
		if (x_ball_new <= M_MAX / 2 + 1)
			moveAfterBall(0); //paleta controlata de computer se misca dupa y-ul bilei pana la trecerea acesteia de fileu
		else
		{
			chancenumber = randomForPalette();
			if (chancenumber <= 8) //sanse de 90% de lovire a mingii
				moveAfterBall(0);
			else
			{
				if (viteza_x > 0)
					moveAfterBall(2);  //miscarea se va produce cu o eroare de 2 pozitii si va conduce la pierderea punctului
				else
					moveAfterBall(0);


			}

		}
	}
	if (nivelVsPC == 2)
	{
		if (x_ball_new <= M_MAX / 2 + 1)
			moveAfterBall(0);
		else
		{
			chancenumber = randomForPalette();
			if (chancenumber <= 6) //sanse de 70% de lovire a mingii
				moveAfterBall(0);
			else
			{
				if (viteza_x > 0)
					moveAfterBall(2); //miscarea se va produce cu o eroare de 2 pozitii si va conduce la pierderea punctului
				else
					moveAfterBall(0);

			}

		}
	}

	if (nivelVsPC == 1)
	{
		if (x_ball_new <= M_MAX / 2 + 1)
			moveAfterBall(0);
		else
		{
			chancenumber = randomForPalette();
			if (chancenumber <= 4) //sanse de 50% de lovire a mingii
				moveAfterBall(0);
			else
			{
				if (viteza_x > 0)
					moveAfterBall(2); //miscarea se va produce cu o eroare de 2 pozitii si va conduce la pierderea punctului
				else
					moveAfterBall(0);
			}

		}
	}
}
void initialMenu()
{
	printf("Bine ati venit la jocul Pong!");
	printf("\nSelectati modul de joc prin apasarea tastei corespunzatoare");
	printf("\nTasta 1 ->Player vs Player");
	printf("\nTasta 2 ->Player vs Computer");

}


