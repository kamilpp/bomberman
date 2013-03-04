#ifndef header_h
#define header_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro.h>

#define FOR(x, b, e) for(int x=(b); x<=(e); ++x)
#define FORD(x, b, e) for(int x=(b); x>=(e); x)
#define REP(x, n) for(int x=0; x<(n); ++x)
#define VAR(v,n) typeof(n) v=(n)
#define ST first
#define ND second
#define MP make_pair
#define MX (long long int)(10e9 + 1)
#define MXS (int)(10e6 + 5)
#define sgn(x) (x<0)?(-1):1
#define abs(x) (((x)<0)?(-(x)):(x))
#define min(a,b)  (a<b)?(a):(b)
#define max(a,b) (a<b)?(b):(a)
#define MALLOC(typ, ilosc) (typ*)malloc((ilosc)*sizeof(typ))
#define swap(x,y) {typeof(y) tmp = y; y = x; x = tmp; }

#define SIZE_X 19
  //ilość pól na osi X
#define SIZE_Y 13
  //ilość pól na osi Y
#define BURNED_TOLERANCY 12
  //tolerancja w pixelach na zabicie przez wybuch
#define BONUS_TOLERANCY 18
  //tolerancja przy łapaniu bonusa
#define PLAYER_TOLERANCY 5
  //tolerancja przy mijaniu przeciwnika

#define DEF_COLOR    makecol(255,0,255)
#define BLUE         makecol(0,0,255)
#define GREEN        makecol(0,255,0)
#define RED          makecol(255,0,0)
#define BLACK        makecol(0, 0, 0)

#define SOUND_ON 1
#define VOLUME_MAIN         200
#define VOLUME_BACKGROUND   40
#define VOLUME_READY        200
#define VOLUME_EXPLO        20
#define VOLUME_DEAD         255

#define DEB  0
#define DEB2 0
#define DEB3 0

extern int PL;
extern int RAND_BONUS;
extern int RAND_NUM;

extern const int WIDTH;
extern const int HEIGHT;
extern const int HEADER_HEIGHT;
extern const int FS;
extern const int BPS;
extern volatile long timer_speed;

typedef struct
{
    //żeby nie bawić się w externy dodając nowe bitmapy itd.
    BITMAP
        *bufor, 			//główny bufor
        *bufor_const,		//bufor z wyrysowanym wszystkim oprócz ludków, kafelek i bomb
        *bit_backg,
        *bit_trap,
        *bit_wall,
        *bit_header, 		//tło pod nagłówek z czasem
        *bit_brick,
        *bit_bomb[3], 		//3 klatki do animacji bomby
        *bit_player,
        *bit_dead,
        *bit_bomberman,     //obrazek tła
        *bit_explo,         //wybuch

        *bit_keyboard2,
        *bit_keyboard3,

        *bit_promo_speed,	//bonus - prędkość
        *bit_promo_bomb,	//bonus - liczba bomb
        *bit_promo_range;	//bonus - zasięg bomb


    SAMPLE
        *sam_main,          //muzyka podczas rozgrywki
        *sam_round,         //podczas rozpoczynania rundy
        *sam_bomb,          //wybuch bomby
        *sam_dead;          //

    FONT
        *fon_menu,
        *fon_submenu,
        *fon_score,
        *fon_round;
} _resources;

typedef struct
{
    int x, y; //pozycja na osi x i y
} _position;

typedef struct
{
    int used; 	//ile zostało użytych
    int numb; 	//ilość dostępnych bomb
    int dist; 	//zasięg rażenia w polach
    int status; //czy zwolniono klawisz - można posadzić nową
} _bomb;	//podstruktura player'a

typedef enum
{
    nope,  	//brak akcji w tej turze
    left,
    right,
    up,
    down,
    plant 	//sadzimy bombe
} _action;

typedef struct
{
    _position pos;	//pozycja
    double speed;	//predkosc poruszania sie ludzika
    double timer; 	//zmienna pomocnicza do obliczania prędkości, kiedy przekroczy 1 zmniejszamy o 1 i przesuwamy ludzika o 1 pixel
    _bomb bomb;	    //podklasa dla bomb
    int keys[10]; 	//mapowanie klawiszy
    int nr; 		//numer playera
    int status;     //0 - żyje
    int wins;       //liczba zwycięstw
} _player;


typedef struct
{
    int death_time;
    int invi;
    int numb;
    int dist;
    int speed;
} _config;

typedef enum
{
    b_backg, 	//tło
    b_trap,  	//kafelki ograniczające
    b_wall,   	//ściany
    b_brick, 	//mur
    b_bomb,	    //bomba
    b_explo, 	//wybuch
    b_bonus 	//bonus
} _background;

typedef struct
{
    _background background; 	//
    int bonus; 			        //czy jest ukryty bonus
    int time; 			        //czas do wybuchu bomby lub numer klatki wybuchu; 0 default
    int start;			        //moment zasadzenia
    int owner;			        //kogo bomba - potrzebne do sprawdzenia np. zasięgu
} _field;

extern _field map[SIZE_X][SIZE_Y];
extern _player p[4];
extern int frame;
extern _resources vars;
extern _config config;

#endif

/*
      KEY_A ... KEY_Z,
      KEY_0 ... KEY_9,
      KEY_0_PAD ... KEY_9_PAD,
      KEY_F1 ... KEY_F12,

      KEY_ESC, KEY_TILDE, KEY_MINUS, KEY_EQUALS,
      KEY_BACKSPACE, KEY_TAB, KEY_OPENBRACE, KEY_CLOSEBRACE,
      KEY_ENTER, KEY_COLON, KEY_QUOTE, KEY_BACKSLASH,
      KEY_BACKSLASH2, KEY_COMMA, KEY_STOP, KEY_SLASH,
      KEY_SPACE,

      KEY_INSERT, KEY_DEL, KEY_HOME, KEY_END, KEY_PGUP,
      KEY_PGDN, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,

      KEY_SLASH_PAD, KEY_ASTERISK, KEY_MINUS_PAD,
      KEY_PLUS_PAD, KEY_DEL_PAD, KEY_ENTER_PAD,

      KEY_PRTSCR, KEY_PAUSE,

      KEY_ABNT_C1, KEY_YEN, KEY_KANA, KEY_CONVERT, KEY_NOCONVERT,
      KEY_AT, KEY_CIRCUMFLEX, KEY_COLON2, KEY_KANJI,

      KEY_LSHIFT, KEY_RSHIFT,
      KEY_LCONTROL, KEY_RCONTROL,
      KEY_ALT, KEY_ALTGR,
      KEY_LWIN, KEY_RWIN, KEY_MENU,
      KEY_SCRLOCK, KEY_NUMLOCK, KEY_CAPSLOCK

      KEY_EQUALS_PAD, KEY_BACKQUOTE, KEY_SEMICOLON, KEY_COMMAND
*/
