#ifndef core_h
#define core_h

#include "header.h"

void read_settings(void);                   //wczytanie ustawień
void save_settings(void);                   //zapisanie ustawień

BITMAP *change_to_bitmap(int i, int j, int frame);          //zamiana nazwy tła na wskaźnik na to tło

int check_colision(int j, int x, int y);    //funkcja sprawdzająca kolizje z przeciwnikiem
void move_player (_player *player, _action act, int val);   //przesuń gracza w odpowiednią stronę o val
void make_action (_player *player);         //sprawdz dostępne akcje gracza

void make_field_explo(int i, int j);        //stworzenie wybuchu na danym (jednym) polu
int bomb_check(int x, int y);               //sprawdzenie czy jest bomba
void make_explosion(int i, int j);          //tworzenie wybuchu bomby

int check_game_over(void);                  //sprawdzanie stanu rozgrywki
void check_player_burned (int i, int j);    //sprawdzanie przy wybuchu czy ogień nie dosięga playera

void update();
void draw();                                //rysowanie wszystkiego

void check_pauses(void);                    //sprawdzanie wduszenia pauzy
void run_death_mode(void);                  //losowanie bomb w trybie nagłej śmierci
void start_game(int mode);                  //rozpoczęcie gry
void show_options();                        //dodatkowe opcje
void show_players_options(int menu);        //wybór liczby graczy
void display_menu(void);                    //wyświetlenie menu

#endif
