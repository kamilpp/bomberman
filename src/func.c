#include "func.h"

void initialize(void)
{
    printf ("Initializing Allegro...\n");

    allegro_init();						    //inicjacja allegro
    install_keyboard();						//instalacja klawiatury
    set_color_depth(32);					//ustalenie głębi kolorów
    set_gfx_mode(GFX_AUTODETECT_WINDOWED,WIDTH,HEIGHT,0,0);	    //ustawienie rozmiaru okna
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "");	    //instalacja dzwięku
    set_volume(255, 255);					//ustawienie głośności
    install_timer();						//instalacja timera
    install_int_ex(increment_speed, BPS_TO_TIMER(BPS));		    //instalacja funkcji timera

    srand(time(NULL));						//aktywacja losowości

    load_addons(); 						    //wczytanie grafiki, dzwięku...
    create_bufors(); 						//stworzenie vars.buforów
}  //initialize

void exit_game(void)
{
    printf ("Terminating application...\n");

    remove_int(increment_speed);
    destroy_addons();
    allegro_exit();
} //exit_game

void show_error(char *message)
{
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message(message);
    exit_game();
}

void increment_speed(void)
{
    timer_speed++;
} END_OF_FUNCTION(increment_speed);
LOCK_FUNCTION(increment_speed); //bloka funkcji

void load_addons(void)
{
    printf ("Loading bitmaps...\n");

    vars.bit_player		= load_bmp("img/lud.bmp", 	default_palette);
    vars.bit_backg 		= load_bmp("img/back.bmp", 	default_palette);
    vars.bit_trap		= load_bmp("img/blank.bmp", default_palette);
    vars.bit_wall		= load_bmp("img/boki.bmp", 	default_palette);
    vars.bit_header		= load_bmp("img/up.bmp", 	default_palette);
    vars.bit_brick 		= load_bmp("img/brick.bmp",	default_palette);
    vars.bit_explo 		= load_bmp("img/fire.bmp",	default_palette);
    vars.bit_bomb[0]	= load_bmp("img/bomb1.bmp",	default_palette);
    vars.bit_bomb[1]	= load_bmp("img/bomb2.bmp",	default_palette);
    vars.bit_bomb[2]	= load_bmp("img/bomb3.bmp",	default_palette);
    vars.bit_dead 		= load_bmp("img/dead.bmp",	default_palette);
    vars.bit_bomberman  = load_bmp("img/bamb.bmp",  default_palette);
    vars.bit_keyboard2  = load_bmp("img/klawa2.bmp",  default_palette);
    vars.bit_keyboard3  = load_bmp("img/klawa3.bmp",  default_palette);

    vars.bit_promo_bomb  = load_bmp("img/more_bomb.bmp",	default_palette);
    vars.bit_promo_speed = load_bmp("img/speed.bmp",	default_palette);
    vars.bit_promo_range = load_bmp("img/dist.bmp",	default_palette);

    printf ("Loading sounds...\n");

    vars.sam_main 	    = load_sample("sound/bezerra.wav");
    vars.sam_round 	    = load_sample("sound/ready.wav");
    vars.sam_dead 	    = load_sample("sound/dead.wav");
    vars.sam_bomb       = load_sample("sound/bomb.wav");

    printf ("Loading fonts...\n");

    vars.fon_menu        = load_font("font/8x16.pcx", NULL, NULL);
    vars.fon_submenu     = load_font("font/txt_hud.pcx", NULL, NULL);
    vars.fon_round       = load_font("font/font_large.pcx", NULL, NULL);
    vars.fon_score       = load_font("font/ex03.pcx", NULL, NULL);

    printf ("Validating loaded resources...\n");

    if (!vars.bit_player    || !vars.bit_backg  || !vars.bit_wall   || !vars.bit_header
    || !vars.bit_trap       || !vars.sam_main   || !vars.bit_explo  || !vars.fon_menu
    || !vars.bit_promo_bomb || !vars.fon_round  || !vars.fon_score  || !vars.sam_main
    || !vars.bit_promo_speed|| !vars.sam_round  || !vars.sam_dead
    || !vars.bit_promo_range|| !vars.sam_bomb   || !vars.bit_keyboard2
    || !vars.fon_submenu    || !vars.bit_keyboard3)
        show_error("Error: loading files failed.");

} //load_addons

void create_bufors(void)
{
    printf ("Creating vars.bufors...\n");

    vars.bufor 	    = create_bitmap(WIDTH,HEIGHT);
    vars.bufor_const	= create_bitmap(WIDTH,HEIGHT);

    if (!vars.bufor || !vars.bufor_const)
        show_error("Error: making vars.bufors failed.");

} //create_bufors

void destroy_addons(void)
{
    printf ("Destroying vars.bufors...\n");

    destroy_bitmap(vars.bufor);
    destroy_bitmap(vars.bufor_const);

    printf ("Destroying bitmaps...\n");

    destroy_bitmap(vars.bit_player);
    destroy_bitmap(vars.bit_backg);
    destroy_bitmap(vars.bit_trap);
    destroy_bitmap(vars.bit_wall);
    destroy_bitmap(vars.bit_header);
    destroy_bitmap(vars.bit_brick);
    destroy_bitmap(vars.bit_explo);
    destroy_bitmap(vars.bit_dead);
    REP(i,3) destroy_bitmap(vars.bit_bomb[i]);

    destroy_bitmap(vars.bit_promo_speed);
    destroy_bitmap(vars.bit_promo_bomb);
    destroy_bitmap(vars.bit_promo_range);

    printf ("Stopping and destroying sounds...\n");

    //stop_sample(vars.sam_main_song);
    destroy_sample(vars.sam_main);
    destroy_sample(vars.sam_round);
    destroy_sample(vars.sam_bomb);
    destroy_sample(vars.sam_dead);

    printf ("Destroying fonts...\n");

    destroy_font(vars.fon_menu);
    destroy_font(vars.fon_score);
    destroy_font(vars.fon_round);
    destroy_font(vars.fon_submenu);
} //destroy_addons



int check_pos (int z)
{
    if (field_floor(z) == z)
        return 1;
    return 0;
} //check_pos

int check_way (int x, int y) //czy może iść w tym kierunku
{
    if (map[x / FS][y / FS].background == b_backg || map[x / FS][y / FS].background == b_explo)
        return 1;
    return 0;
} //check_way

int field_floor(int x)
{
    return x / FS * FS;
} //field_floor

int center(int x) //transferuje lewy gorny punkt pola na środek i zwraca wartość we współrzędnych tablicowych
{
    return field_floor(x + FS/2) / FS;
} //center

void create_default_map(void) //wywoływana raz na początku lvla
{
    printf ("Creating default map...\n");

    //tło domyślnie - wartość 0; kilka rund = czyszczenie
    REP(i,SIZE_X) REP(j,SIZE_Y)
    {
        map[i][j].background = b_backg; //clean
        map[i][j].bonus = map[i][j].time = map[i][j].start = map[i][j].owner = 0;
    }

    //ściany
    REP(i,SIZE_X) map[i][0].background = map[i][SIZE_Y - 1].background = b_wall;
    REP(i,SIZE_Y) map[0][i].background = map[SIZE_X - 1][i].background = b_wall;

    //kafelki ograniczające
    for (int i = 2; i < SIZE_X - 1; i += 2) for (int j = 2; j < SIZE_Y - 1; j += 2)
	    map[i][j].background = b_trap;


    printf ("Drawing default map to vars.bufor_const...\n");

    clear_to_color(vars.bufor_const, DEF_COLOR);    //czyścimy vars.bufor_const
    draw_sprite(vars.bufor_const, vars.bit_header, 0, 0);         //dodajemy header
//    textprintf_centre_ex(vars.bufor_const, vars.fon_round, (int)WIDTH / 2 - 50, 20, RED, -1, "Player 1");
//    textprintf_centre_ex(vars.bufor_const, vars.fon_round, (int)WIDTH / 2 + 50, 20, RED, -1, "Player 2");
    if (PL == 2) textprintf_centre_ex(vars.bufor_const, vars.fon_score, (int)WIDTH / 2,      35, RED, -1, "%d : %d", p[1].wins, p[2].wins);
    else textprintf_centre_ex(vars.bufor_const, vars.fon_score, (int)WIDTH / 2,      35, RED, -1, "%d : %d : %d", p[1].wins, p[2].wins, p[3].wins);

    REP(i,SIZE_X) REP(j,SIZE_Y)
    {
        if (map[i][j].background == b_backg) 	draw_sprite(vars.bufor_const, vars.bit_backg,   i * FS, j * FS + HEADER_HEIGHT);        //rysujemy tło
        if (map[i][j].background == b_trap) 	draw_sprite(vars.bufor_const, vars.bit_trap,    i * FS, j * FS + HEADER_HEIGHT);        //rysujemy pola ograniczające
        if (map[i][j].background == b_wall) 	draw_sprite(vars.bufor_const, vars.bit_wall,    i * FS, j * FS + HEADER_HEIGHT);        //rysujemy ściany
    }

    printf ("Randomize bricks...\n");

    int tmp = 0;
    while(1)
    {
        int i = rand()%SIZE_X;
        int j = rand()%SIZE_Y;
        if ( (map[i][j].background == b_backg)   &&   (i > 2 || j > 2)   &&   (i < (SIZE_X - 3)  ||  j < (SIZE_Y - 3)) && (i < (SIZE_X - 3) || j > 2) )
        {
            map[i][j].background = b_brick;
            tmp++;
        }
        if (tmp >= RAND_NUM) break;
    }

    if (RAND_BONUS > RAND_NUM)
        show_error("Error: number of bonuses is too big.");

    tmp = 0;
    while(1) //losowanie bonusów
    {
        int i = rand()%SIZE_X;
        int j = rand()%SIZE_Y;
        if (map[i][j].background == b_brick)
        {
            map[i][j].bonus = rand()%3 + 1;
            //printf ("wylosowano %d %d\n",i,j);
            tmp++;
        }
        if (tmp >= RAND_BONUS)
            break;
    }
} //create_default_map
