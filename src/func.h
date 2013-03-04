#ifndef func_h
#define fucn_h

#include "header.h"

void initialize(void);              //inicjalizacja gry
void exit_game(void);               //finalizowanie gry

void show_error(char *message);     //obsługa błędów

void increment_speed(void);         //obsługa timera
void load_addons(void);             //wczytywanie ...
void create_bufors(void);           //tworzenie buforów
void destroy_addons(void);          //niszczenie bitmap, samples, fonts i BUFORÓW...

int check_pos(int z);               //czy znajduje się na skrzyżowaniu siatki dla jednej współrzędnej
int field_floor(int x);             //"siatkowa" podłoga
int check_way(int x, int y);        //sprawdza czy można wejść na dane pole
int center(int x);                  //transferuje lewy gorny punkt pola na środek i zwraca wartość we współrzędnych tablicowych

void create_default_map(void);       //rysowanie domyślnej mapy

#endif
