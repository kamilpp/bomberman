#include "func.h"
#include "core.h"

int PL = 2;
int RAND_BONUS = 45;            //liczba bonusów na mapie MAX VALUE < RAND_NUM
int RAND_NUM = 80;              //maksymalna liczba wylosowanych kafelek MAX VALUE ~110
const int WIDTH 	= 760; 	    //wysokość okna
const int HEIGHT	= 600; 	    //szerokość okna
const int HEADER_HEIGHT	= 80;  	//wysokość headera
const int FS = 40;		        //wymiar pojedyńczego pola
const int BPS = 100;		    //liczba klatek na sekundę

volatile long timer_speed; 	    //obsługa klatek
LOCK_VARIABLE(timer_speed); 	//blokada zmiennej

_resources vars;                //struktura zawierająca bitmapy, samples, fonty
_config config;
_field map[SIZE_X][SIZE_Y]; 	//SIZE_X pól po osi X, SIZE_Y po Y
_player p[4];		        //deklaracja profili graczy
int frame;			            //zmiena pomocnicza do obsługi animacji

int main()
{
    initialize();
    display_menu();

    exit_game();
    return 0;
}
END_OF_MAIN();


