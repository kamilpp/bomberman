#include "core.h"
#include "stack.h"
#include "func.h"

stack *bomb_stack = NULL;	    //stos z bombami do zdetonowania

void read_settings(void)
{
    printf ("Reading settings...\n");

    FILE *ini = NULL;
    ini = fopen("config.ini", "r");
    if (ini == NULL) show_error("Loading settings failed.");
    char word[100] = {0};
//    int pos = 0;
//    char znak;
//    while (fscanf(ini,"%s",word) != EOF)
//    {
//        fscanf(ini, "%c",&znak);
//        printf("znak %c\n",znak);
//        if (word[0] == ';')
//        {
//            pos += strlen(word) + 2;
//            printf ("dodajem %d\n",strlen(word) + 2);
//            continue;
//        }
//        int i = 0;
//        while (word[++i] != '=');
//        word[i] = '\0'; i++;
//        int val = (int)word[i] - '0';
//        if (!strcmp("DEATH",word))
//        {
//            config.death_time = val;
//            config.death_pos = pos + i;
//        }
//        if (!strcmp("RANGE",word)) config.dist = val;
//        if (!strcmp("NUMBER",word)) config.numb = val;
//        REP(i,100) word[i] = '\0';
//        pos += strlen(word);
//    }
    while (fscanf(ini,"%s",word) != EOF)
    {
        int i = 0;
        while (word[++i] != '=');
        word[i++] = '\0';
        int val = (int)word[i] - '0';
        if (word[i + 1] != '\0')
        {
            val *= 10;
            val += (int)word[i + 1] - '0';
        }
        if (!strcmp("DEATH",word)) config.death_time = val;
        if (!strcmp("RANGE",word)) config.dist = val;
        if (!strcmp("NUMBER",word)) config.numb = val;
        if (!strcmp("INVISIBLE",word)) config.invi = val;
        if (!strcmp("SPEED",word)) config.speed = val;
        REP(i,100) word[i] = '\0';
    }
    fclose(ini);


    //printf ("%d %d %d\n",death,range,number,speed);
    p[1].pos.x = FS;
    p[1].pos.y = FS;
    p[2].pos.x = (SIZE_X - 2) * FS;
    p[2].pos.y = (SIZE_Y - 2) * FS;
    p[3].pos.x = (SIZE_X - 2) * FS;
    p[3].pos.y = FS;

    FOR(i,1,3)
    {
        p[i].nr = i;
        p[i].speed = config.speed * 0.2;
        p[i].timer = 0;
        p[i].status = 0;
        p[i].bomb.dist = config.dist;
        p[i].bomb.used = 0;
        p[i].bomb.numb = config.numb;
        p[i].bomb.status = 1;
    }

    if (PL == 2)
    {
        p[2].keys[left] 	= KEY_LEFT;
        p[2].keys[right] 	= KEY_RIGHT;
        p[2].keys[up] 	    = KEY_UP;
        p[2].keys[down] 	= KEY_DOWN;
        p[2].keys[plant] 	= KEY_RCONTROL;
    }
    else
    {
        p[3].keys[left] 	= KEY_1_PAD;
        p[3].keys[right] 	= KEY_3_PAD;
        p[3].keys[up] 	    = KEY_5_PAD;
        p[3].keys[down] 	= KEY_2_PAD;
        p[3].keys[plant] 	= KEY_RIGHT;

        p[2].keys[left] 	= KEY_K;
        p[2].keys[right] 	= KEY_SEMICOLON;
        p[2].keys[up] 	    = KEY_O;
        p[2].keys[down] 	= KEY_L;
        p[2].keys[plant] 	= KEY_SPACE;
    }

    p[1].keys[left] 	= KEY_A;
    p[1].keys[right] 	= KEY_D;
    p[1].keys[up] 	    = KEY_W;
    p[1].keys[down] 	= KEY_S;
    p[1].keys[plant] 	= KEY_LCONTROL;

    bomb_stack = NULL;
} //read_settings

void save_settings(void)
{
    printf ("Saving settings...\n");

    FILE *ini = NULL;
    //ini = fopen("config.ini", "r+");
    ini = fopen("config.ini", "w");
    if (ini == NULL) show_error("Saving settings failed.");
//    rewind(ini);
//    printf("pos %d\n",config.death_pos);
//    fseek (ini, config.death_pos, SEEK_SET);
    //printf ("%d\n",config.death_pos);
   // char znak[2] = {0};
   // znak[0] = config.death_time;
   // fputs ("SAF",ini);
    //fprintf(ini,"%d",config.death_time);
    fprintf(ini,"DEATH=%d\nRANGE=%d\nNUMBER=%d\nSPEED=%d\nINVISIBLE=%d\n",config.death_time,config.numb,config.dist,config.speed,config.invi);
    fclose(ini);
}
BITMAP *change_to_bitmap(int i, int j, int frame)
{
    int start = map[i][j].start;
    int bonus = map[i][j].bonus;

    switch (map[i][j].background)
    {
        case b_brick:
            return vars.bit_brick;
            break;
        case b_explo:
            return vars.bit_explo;
            break;
        case b_backg:
            if (bonus == 1) return vars.bit_promo_bomb;
            if (bonus == 2) return vars.bit_promo_speed;
            if (bonus == 3) return vars.bit_promo_range;
            return vars.bit_backg;
            break;
        case b_bomb:
            frame += start;
            frame %= BPS;
            if (frame < BPS / 5) 		return vars.bit_bomb[0];
            if (frame < BPS / 5 * 2) 	return vars.bit_bomb[1];
            if (frame < BPS / 5 * 3) 	return vars.bit_bomb[2];
            if (frame < BPS / 5 * 4) 	return vars.bit_bomb[1];
            return vars.bit_bomb[0];
            break;
        default:
            return vars.bit_backg;
    }
} //change_to_bitmap

int check_colision(int j, int x, int y) //funkcja sprawdzająca kolizje z przeciwnikiem
{
    FOR(i,1,PL)
    {
        if (i == j) continue;
        if (p[i].status) continue;

        if (x > p[i].pos.x - FS + PLAYER_TOLERANCY && x < p[i].pos.x + FS - PLAYER_TOLERANCY && y > p[i].pos.y - FS + PLAYER_TOLERANCY && y < p[i].pos.y + FS - PLAYER_TOLERANCY)
            return 1;
    }
    return 0;
} //check_colision


void move_player (_player *player, _action act, int val)
{
    if (DEB) printf("Akcja: %d, wartosc: %d aktualna poz %d %d\n",act,val,player->pos.x,player->pos.y);
    if (act == right)
    {
        int m = player->pos.x + FS - 1 + val; //docelowa pozycja prawej krawędzi ludka
        if (check_colision(player->nr, player->pos.x + val, player->pos.y)) //jeśli nie koliduje z przeciwnikiem
            return;
        if (field_floor(m) <= m && m < field_floor(m) + FS/2) //jeśli postać wchodzi na nowe pole (do połowy długości FS)
        {
            if (field_floor(m) != field_floor(m - val) && !check_pos(player->pos.x)) //jeśli zmienia pole i nie jest na skrzyżowaniu siatek to musi być
                player->pos.x = field_floor(m - val);
            else if (check_way(m, player->pos.y)) //jeśli pole jest wolne to przemieszczamy
                player->pos.x += val;
        }
        else //jeśli się tylko przesuwa na swoim polu o trochę - przemieszczamy
            player->pos.x += val;
    }
    if (act == left)
    {
        int m = player->pos.x - val;
        if (check_colision(player->nr, player->pos.x - val, player->pos.y))
            return;
        if (field_floor(m) + FS - 1 >= m && m >= field_floor(m) + FS/2)
        {
            if (field_floor(m) != field_floor(player->pos.x) && !check_pos(player->pos.x))
                player->pos.x = field_floor(player->pos.x);
            else if (check_way(m, player->pos.y))
                player->pos.x -= val;
        }
        else
            player->pos.x -= val;
    }
    if (act == up)
    {
        int m = player->pos.y - val;
        if (check_colision(player->nr, player->pos.x, player->pos.y - val)) return;
        if (field_floor(m) + FS - 1 >= m && m >= field_floor(m) + FS/2)
        {
            if (field_floor(m) != field_floor(player->pos.y) && !check_pos(player->pos.y))
                player->pos.y = field_floor(player->pos.y);
            else if (check_way(player->pos.x, m))
                player->pos.y -= val;
        }
        else
            player->pos.y -= val;
    }
    if (act == down)
    {
        int m = player->pos.y + FS - 1 + val;
        if (check_colision(player->nr, player->pos.x, player->pos.y + val))
            return;
        if (field_floor(m) <= m && m < field_floor(m) + FS/2)
        {
            if (field_floor(m) != field_floor(m - val) && !check_pos(player->pos.y))
                player->pos.y = field_floor(m - val);
            else if (check_way(player->pos.x, m))
                player->pos.y += val;
        }
        else
            player->pos.y += val;
    }
    int m = center(player->pos.x) * FS;
    int n = center(player->pos.y) * FS;

    if (map[m / FS][n / FS].bonus && player->pos.x > m - BONUS_TOLERANCY && player->pos.x < m + BONUS_TOLERANCY && player->pos.y > n - BONUS_TOLERANCY && player->pos.y < n + BONUS_TOLERANCY)
    {
        if (map[m / FS][n / FS].bonus == 1)
            player->bomb.numb += 1;
        if (map[m / FS][n / FS].bonus == 2)
            player->speed += 0.2;
        if (map[m / FS][n / FS].bonus == 3)
            player->bomb.dist += 1;;

        map[m/ FS][n / FS].bonus = 0;
    }
    if (DEB) printf ("Po akcji\n");
} //move_player


void make_action (_player *player)
{
    //printf("%d %d %d %d\n",key[KEY_RIGHT], key[p[i].keys[right]], key[KEY_UP], key[KEY_DOWN]);
    if (player->status) return;

    if (key[player->keys[left]] + key[player->keys[right]] + key[player->keys[up]] + key[player->keys[down]] == -1) //wduszony jest 1 klawisz
    {
        player->timer += player->speed;
        if (player->timer >= 1.0)      //możliwość przejścia
        {
            int val = 0;
            while(player->timer >= 1.0) //obliczamy o ile bedzie ludek przesuniety
            {
                val++;
                player->timer -= 1.0;
            }

            if (key[player->keys[right]]) //jeśli jest wduszony klawisz w prawo
            {
                if (!check_pos(player->pos.y)) //jeśli się nie da iść bezpośrednio w prawo
                {
                    if (check_way(player->pos.x + FS, player->pos.y))       //jeśli można wyrównać do siatki w górę (a potem w prawo)
                        move_player(player, up, val);
                    if (check_way(player->pos.x + FS, player->pos.y + FS))  //jeśli można wyrównać do siatki w górę (a potem w prawo)
                        move_player(player, down, val);
                }
                else
                    move_player(player, right, val);
            }
            if (key[player->keys[left]])
            {
                if (!check_pos(player->pos.y))
                {
                    if (check_way(player->pos.x - FS, player->pos.y))
                        move_player(player, up, val);
                    if (check_way(player->pos.x - FS, player->pos.y + FS))
                        move_player(player, down, val);
                }
                else
                    move_player(player, left, val);
            }
            if (key[player->keys[down]])
            {
                if (!check_pos(player->pos.x))
                {
                    if (check_way(player->pos.x, player->pos.y + FS))
                        move_player(player, left, val);
                    if (check_way(player->pos.x + FS, player->pos.y + FS))
                        move_player(player, right, val);
                }
                else
                    move_player(player, down, val);
            }
            if (key[player->keys[up]])
            {
                if (!check_pos(player->pos.x))
                {
                    if (check_way(player->pos.x, player->pos.y - FS))
                        move_player(player, left, val);
                    if (check_way(player->pos.x + FS, player->pos.y - FS))
                        move_player(player, right, val);
                }
                else
                    move_player(player, up, val);
            }
        }
    }
} //make_action

void make_field_explo(int i, int j)
{
    map[i][j].background = b_explo;
    if (map[i][j].time) map[i][j].bonus = 0;
    map[i][j].time = map[i][j].start = 0;

    if (SOUND_ON) play_sample(vars.sam_bomb, VOLUME_EXPLO, 0, 1000, 0);
} //make_field_explo

int bomb_check(int x, int y)
{
    _background bgd = map[x][y].background;
    if (bgd == b_explo) make_field_explo(x,y);
    else if (bgd == b_backg)
    {
        make_field_explo(x,y);
        return 0;
    }
    else if (bgd == b_brick) make_field_explo(x,y);
    else if (bgd == b_bomb) push(&bomb_stack, x, y);
    return 1;
} //bomb_check

void make_explosion(int i, int j)
{
    make_field_explo(i,j);
    p[map[i][j].owner].bomb.used--;

    FOR(k,1,p[map[i][j].owner].bomb.dist)
        if (bomb_check(i - k, j)) break;
    FOR(k,1,p[map[i][j].owner].bomb.dist)
        if (bomb_check(i + k, j)) break;
    FOR(k,1,p[map[i][j].owner].bomb.dist)
        if (bomb_check(i, j - k)) break;
    FOR(k,1,p[map[i][j].owner].bomb.dist)
        if (bomb_check(i, j + k)) break;

    if (!empty(bomb_stack))
    {
        i = top(bomb_stack).val_x;
        j = top(bomb_stack).val_y;
        pop(&bomb_stack); //najpierw pop bo się zapętli jak wywołamy make_ex
        make_explosion(i,j);
    }
} //make_explosion


void plant_bombs (int t)
{
    FOR(i,1,PL)
    {
        if (!p[i].status &&                              //gracz żyje
            key[p[i].keys[plant]] && 		            //wduszony przycisk stawiania bomby
            p[i].bomb.status &&  			            //został wcześniej puszczony
            p[i].bomb.used < p[i].bomb.numb &&  	    //mamy jeszcze wolne bomby
            map[center(p[i].pos.x)][center(p[i].pos.y)].background != b_bomb)  //nie ma już tam bomby
        {
            printf ("Player %d bomb's planted at %d %d...\n",i,center(p[i].pos.x),center(p[i].pos.y));

            p[i].bomb.used++;
            map[center(p[i].pos.x)][center(p[i].pos.y)].background = b_bomb;
            map[center(p[i].pos.x)][center(p[i].pos.y)].time = 1;
            map[center(p[i].pos.x)][center(p[i].pos.y)].owner = i;
            map[center(p[i].pos.x)][center(p[i].pos.y)].start = t;
            map[center(p[i].pos.x)][center(p[i].pos.y)].bonus = 0;
            p[i].bomb.status = 0;
        }
        else if (!key[p[i].keys[plant]] && !p[i].bomb.status)
            p[i].bomb.status = 1;
    }
}

int check_game_over(void)
{
    FOR(i,1,PL)
    {
        if (p[i].status == 1)
        {
            printf ("%d %d\n",i,p[i].status);
            printf("Player %d dead...\n",i);
            if (PL == 2) stop_sample(vars.sam_main);
            if (SOUND_ON) play_sample(vars.sam_dead, VOLUME_DEAD, 0, 1000, 0);
            p[i].status = 2;
        }
    }

    int tmp = 0;
    if (p[1].status) tmp++;
    if (p[2].status) tmp++;
    if (p[3].status) tmp++;

    if (tmp >= PL - 1) draw();

    if (PL == 2)
    {
        if (p[1].status && p[2].status)
            textprintf_centre_ex(screen, vars.fon_score, (int)WIDTH / 2, (int)HEIGHT / 2, RED, -1, "DRAW");
        else if (p[1].status || p[2].status)
        {
            textprintf_centre_ex(screen, vars.fon_score, (int)WIDTH / 2, (int)HEIGHT / 2, RED, -1, "PLAYER %d!",(p[1].status)?2:1);
            p[(p[1].status)?2:1].wins++;
        }
        else return 1;
    }
    else
    {
        if (tmp == 3)
            textprintf_centre_ex(screen, vars.fon_score, (int)WIDTH / 2, (int)HEIGHT / 2, RED, -1, "DRAW");
        else if (tmp == 2)
        {
            if (!p[1].status) tmp = 1;
            else if (!p[2].status) tmp = 2;
            else tmp = 3;
            textprintf_centre_ex(screen, vars.fon_score, (int)WIDTH / 2, (int)HEIGHT / 2, RED, -1, "PLAYER %d!",tmp);
            p[tmp].wins++;
        }
        else return 1;
    }

    rest(3000);
    return 0;
} //check_game_over

void check_player_burned (int i, int j)
{
    FOR(k,1,PL)
        if (!p[k].status && i * FS - FS + BURNED_TOLERANCY < p[k].pos.x && p[k].pos.x < i * FS + FS - BURNED_TOLERANCY && j * FS - FS + BURNED_TOLERANCY < p[k].pos.y && p[k].pos.y < j * FS + FS - BURNED_TOLERANCY)
            p[k].status = 1;
} //check_players_burned

void update (int frame)
{
    plant_bombs(frame);
    make_action(&p[1]);
    make_action(&p[2]);
    if (PL == 3) make_action(&p[3]);
} //update

void draw()
{
    draw_sprite(vars.bufor, vars.bufor_const, 0, 0);  //podstawiamy podstawowy szkic

    REP(i,SIZE_X) REP(j,SIZE_Y)
    {
        if (map[i][j].background == b_explo)
        {
            if (++map[i][j].time >= BPS/3)
                map[i][j].background = b_backg;
            else
                check_player_burned(i, j);
        } //sprawdzanie kolizji oraz odliczanie czasu
    }

    //CHECK SHALL NOT PASS!
//    FOR(i,1,PL)
//    {
//        int x = p[i].pos.x + FS/2;
//        int y = p[i].pos.y + FS/2;
//        x /= FS;
//        y /= FS;
//
//        if (!shall_pass && map[x - 1][y].background != b_backg && map[x + 1][y].background != b_backg && map[x][y - 1].background != b_backg && map[x][y + 1].background != b_backg)
//        {
//            play_sample(vars.sam_gandalf_pass, 255, 0, 1000, 0);
//            shall_pass = 2;
//        }
//    }

    REP(i,SIZE_X) REP(j,SIZE_Y)
    {
        if (map[i][j].background == b_bomb && ++map[i][j].time >= 3 * BPS)
            make_explosion(i,j);

        if (map[i][j].background != b_trap &&   //kafelki ograniczające już narysowane
            map[i][j].background != b_wall &&
            map[i][j].background != b_bomb &&
            map[i][j].background != b_explo)    //eksplozja musi być na ludku - rysujemy później
        {
           draw_sprite(vars.bufor, change_to_bitmap(i,j, frame), i * FS, j * FS + HEADER_HEIGHT); //rysujemy
        }
    }
    FOR(i,1,PL)
        if (p[i].status) masked_blit(vars.bit_dead, vars.bufor, 0, 0, p[i].pos.x, p[i].pos.y + HEADER_HEIGHT, FS, FS);

    if (!config.invi) REP(i,SIZE_X) REP(j,SIZE_Y)
        if (map[i][j].background == b_bomb)    //eksplozja musi być na ludku - rysujemy później
           draw_sprite(vars.bufor, change_to_bitmap(i,j, frame), i * FS, j * FS + HEADER_HEIGHT); //rysujemy


    FOR(i,1,PL)
        if (!p[i].status) masked_blit(vars.bit_player, vars.bufor, 0, 0, p[i].pos.x, p[i].pos.y + HEADER_HEIGHT, FS, FS);

    REP(i,SIZE_X) REP(j,SIZE_Y)
        if (map[i][j].background == b_explo)
            draw_sprite(vars.bufor, change_to_bitmap(i,j, frame), i * FS, j * FS + HEADER_HEIGHT); //eksplozja ma być NA ludku, reszta pod

    blit(vars.bufor, screen, 0, 0, 0, 0, WIDTH,HEIGHT);
} //draw

void check_pauses(void)
{
    if (key[KEY_P]) //PAUZA
    {
        printf ("Game paused...\n");

        int tmp = timer_speed;  //zapamiętujemy stan timera
        textout_centre_ex(vars.bufor, vars.fon_menu, "GAME PAUSED", (int)WIDTH / 2, (int)HEIGHT / 2, RED, -1);
        masked_blit(vars.bufor, screen, 0, 0, 0, 0, WIDTH,HEIGHT);

        while(key[KEY_P]);      //zwolnienie klawisza
        while (!key[KEY_P]);     //wznowienie na ponowne przyduszenie
        //    rest(50);

        printf ("Game resumsed...\n");

        while (key[KEY_P]);     //zwolnienie klawisza
        timer_speed = tmp;      //przywrócenie stanu timera
    }
} //check_pauses


void run_death_mode()
{
    while(1)
    {
        int i = rand()%SIZE_X;
        int j = rand()%SIZE_Y;
        if (map[i][j].background == b_backg)
        {
            map[i][j].background = b_bomb;
            map[i][j].owner = 0;
            map[i][j].time = 1;
            map[i][j].start = 0;
            map[i][j].bonus = 0;
            p[0].bomb.dist = rand()%5 + 1;
            break;
        }
    }
} //run_death_mode


void start_game(int mode)
{
    if (!mode)  printf ("\nStarting arena mode...\n");
    else printf("\nStarting death mode...\n");

    int round = 1, death_time = 0;      //utrzowenie zmiennych rozgrywki - nr rundy, timer do losowania bomb
    p[1].wins = p[2].wins = p[3].wins = 0;

    while (mode != 3 && p[1].wins != 3 && p[2].wins != 3 && p[3].wins != 3)    //dopóki ktoś nie wygrał
    {
        if (key[KEY_ESC]) break;

        //WYŚWIIETLENIE EKRANU POCZĄTKU RUNDY I PRZYGOTOWANIE MAPY
        clear_to_color(vars.bufor, makecol(0, 0, 0));
        if (round == 1)
        {
            if (PL == 2) draw_sprite(vars.bufor, vars.bit_keyboard2, 0, 400);
            else draw_sprite(vars.bufor, vars.bit_keyboard3, 0, 400);
        }
        textprintf_centre_ex(vars.bufor, vars.fon_round, (int)WIDTH / 2, (int)HEIGHT / 2 - 25, RED, -1, "ROUND %d", round);
        if (PL == 2) textprintf_centre_ex(vars.bufor, vars.fon_score, (int)WIDTH / 2, (int)HEIGHT / 2, RED, -1, "%d : %d", p[1].wins, p[2].wins);
        else textprintf_centre_ex(vars.bufor, vars.fon_score, (int)WIDTH / 2, (int)HEIGHT / 2, RED, -1, "%d : %d : %d", p[1].wins, p[2].wins, p[3].wins);
        masked_blit(vars.bufor, screen, 0, 0, 0, 0, WIDTH,HEIGHT);

        if (SOUND_ON) play_sample(vars.sam_round, VOLUME_READY, 0, 900, 0);
        create_default_map();                   //wyczyszczenie i stworzenie szkicu mapy
        read_settings();                        //
        rest(2000);
        if (round == 1) rest(1500);             //dodatkowe 1,5s na zapoznanie się z klawąw
        stop_sample(vars.sam_round);

        timer_speed = 1;                        //ustalenie początkowej wartości timera
        if (SOUND_ON) play_sample(vars.sam_main, VOLUME_BACKGROUND, 0, 1000, 1); //odpalenie muzyki tła
        frame = 0;

        //GŁÓWNA PĘTLA ROZGRYWKI
        while(check_game_over())
        {
            while( timer_speed > 0 )
            {
                update(frame);
                if (mode == 1 && !frame && ++death_time >= config.death_time)
                {
                    death_time = 0;
                    run_death_mode();
                }
                draw();

                if (key[KEY_T] && key[KEY_Y] && p[1].bomb.numb < 16)
                {
                    FOR(i,1,PL)
                    {
                        p[i].bomb.numb += 15;
                        p[i].bomb.dist += 30;
                        p[i].speed += 1.0;
                    }
                }

                timer_speed--;
                if( ++frame > BPS ) frame = 0;
            } //while timer

            if (key[KEY_ESC])
            {
                int tmp = timer_speed;  //zapamiętujemy stan timera
                textprintf_centre_ex(screen, vars.fon_score, (int)WIDTH / 2, (int)HEIGHT / 2 - 20, RED, -1, "EXIT?");
                textprintf_centre_ex(screen, vars.fon_round, (int)WIDTH / 2, (int)HEIGHT / 2 + 20, RED, -1, "Y/N");
                while (!key[KEY_Y] && !key[KEY_N]);
                if (key[KEY_Y])
                {
                    mode = 3;
                    break;
                }
                else
                {
                    timer_speed = tmp;
                    draw();
                }
            }
            check_pauses();
        } //while main

        stop_sample(vars.sam_main);  //zatrzymanie muzyki tła
        round++;                //zwiększenie licznika rund
    } //while roumds

    printf ("Finalizing arena/death mode...\n");

    if (mode != 3)
    {
        clear_to_color(vars.bufor, makecol(0, 0, 0));
        masked_blit(vars.bit_bomberman, vars.bufor, 0, 0, 50, 200, WIDTH,HEIGHT);
        textprintf_centre_ex(vars.bufor, vars.fon_score, (int)WIDTH / 2 + 100, (int)HEIGHT / 2, RED, -1, "PLAYER %d WINS!", (p[1].wins == 3) ? 1 : ((p[2].wins == 3) ? 2 : 3));
        masked_blit(vars.bufor, screen, 0, 0, 0, 0, WIDTH,HEIGHT);
        rest(3000);
    }
} //start_game

void show_options()
{
    read_settings();
    int menu = 0; //wybrana opcja menu
    while(key[KEY_ENTER]);

    while(1)
    {
        while(!key[KEY_ENTER] && !key[KEY_LEFT] && !key[KEY_RIGHT])
        {
            clear_to_color(vars.bufor, makecol(0, 0, 0));
            //draw_sprite(vars.bufor, vars.bit_front, 0, 0);
            if (menu < 5) textprintf_centre_ex(vars.bufor, vars.fon_menu, (int)WIDTH / 2, (int)HEIGHT - 30,RED, -1, "Use right/left to increase/decrease...");
            textprintf_centre_ex(vars.bufor, vars.fon_menu, (int)WIDTH / 2, (int)HEIGHT / 2 - 75, (menu == 0)?(BLUE):(GREEN), -1, "DEATH MODE TIMER %d",config.death_time);
            textprintf_centre_ex(vars.bufor, vars.fon_menu, (int)WIDTH / 2, (int)HEIGHT / 2 - 45, (menu == 1)?(BLUE):(GREEN), -1, "PLAYERS BOMB RANGE %d",config.dist);
            textprintf_centre_ex(vars.bufor, vars.fon_menu, (int)WIDTH / 2, (int)HEIGHT / 2 - 15, (menu == 2)?(BLUE):(GREEN), -1, "PLAYERS BOMB NUMBER %d",config.numb);
            textprintf_centre_ex(vars.bufor, vars.fon_menu, (int)WIDTH / 2, (int)HEIGHT / 2 + 15, (menu == 3)?(BLUE):(GREEN), -1, "PLAYERS SPEED %d",config.speed);
            textprintf_centre_ex(vars.bufor, vars.fon_menu, (int)WIDTH / 2, (int)HEIGHT / 2 + 45, (menu == 4)?(BLUE):(GREEN), -1, "INVISIBLE BOMBS %s",(config.invi) ? "TRUE" : "FALSE");
            textprintf_centre_ex(vars.bufor, vars.fon_menu, (int)WIDTH / 2, (int)HEIGHT / 2 + 80, (menu == 5)?(RED):(GREEN), -1, "BACK");
            masked_blit(vars.bufor, screen, 0, 0, 0, 0, WIDTH,HEIGHT);

            //OBSŁUGA WYBORU OPCJI MENU
            if (key[KEY_DOWN])
            {
                if (++menu > 5) menu -= 6;
                while(key[KEY_DOWN]); //dopóki nie zwolnimy klawisza
            }
            if (key[KEY_UP])
            {
                if (--menu < 0) menu += 6;
                while(key[KEY_UP]);   //dopóki nie zwolnimy klawisza
            }

            if (menu < 5)
            {
                int x;
                if (key[KEY_LEFT]) x = -1;
                if (key[KEY_RIGHT]) x = 1;
                if (key[KEY_RIGHT] || key[KEY_LEFT]) switch(menu)
                {
                    case 0: config.death_time += x;
                            if (config.death_time >= 10) config.death_time = 10;
                            if (config.death_time <= 0) config.death_time = 1;
                            break;
                    case 1: config.dist += x;
                            if (config.dist >= SIZE_X - 2) config.dist = SIZE_X - 2;
                            if (config.dist == 0) config.dist = 1;
                            break;
                    case 2: config.numb += x;
                            if (config.numb >= 15) config.numb = 15;
                            if (config.numb == 0) config.numb = 1;
                            break;
                    case 3: config.speed += x;
                            if (config.speed >= 12) config.speed = 12;
                            if (config.speed <= 5) config.speed = 5;
                            break;
                    case 4: config.invi = (config.invi + 1) % 2;
                            break;
                } //switch
            }
        } //while ENTER - po wduszeniu akcja

        if (menu == 5 && key[KEY_ENTER])
        {
            save_settings();
            break;
        }
    }

} //show options

void show_players_options(int mode)
{
    int menu = 0; //wybrana opcja menu
    while(key[KEY_ENTER]);

    while(1)
    {
        while(!key[KEY_ENTER])
        {
            clear_to_color(vars.bufor, makecol(0, 0, 0));
            //draw_sprite(vars.bufor, vars.bit_front, 0, 0);
            textprintf_centre_ex(vars.bufor, vars.fon_menu, (int)WIDTH / 2, (int)HEIGHT / 2 - 30, (menu == 0)?(RED):(GREEN), -1, "2 PLAYERS");
            textprintf_centre_ex(vars.bufor, vars.fon_menu, (int)WIDTH / 2, (int)HEIGHT / 2 + 0,  (menu == 1)?(RED):(GREEN), -1, "3 PLAYERS");
            textprintf_centre_ex(vars.bufor, vars.fon_menu,  (int)WIDTH / 2, (int)HEIGHT / 2 + 40, (menu == 2)?(RED):(GREEN), -1, "BACK");
            masked_blit(vars.bufor, screen, 0, 0, 0, 0, WIDTH,HEIGHT);

            //OBSŁUGA WYBORU OPCJI MENU
            if (key[KEY_DOWN])
            {
                if (++menu > 2) menu -= 3;
                while(key[KEY_DOWN]); //dopóki nie zwolnimy klawisza
            }
            if (key[KEY_UP])
            {
                if (--menu < 0) menu += 3;
                while(key[KEY_UP]);   //dopóki nie zwolnimy klawisza
            }
        } //while ENTER - po wduszeniu akcja
        if (!menu)
        {
            PL = 2;
            RAND_NUM = 60;
            RAND_BONUS = 35;
        }
        else if (menu == 1)
        {
            PL = 3;
            RAND_NUM = 85;
            RAND_BONUS = 50;
        }
        else break;

        start_game(mode);
    }

}

void display_menu()
{
    printf ("Displaying menu...\n");

    // if (SOUND_ON) play_sample(vars.sam_menu, VOLUME_MAIN, 0, 500, 1);
    // clear_to_color(vars.bufor, makecol(0, 0, 0));
    // draw_sprite(vars.bufor, vars.bit_front, 0, 0);
    // masked_blit(vars.bufor, screen, 0, 0, 0, 0, WIDTH,HEIGHT);
    // readkey();
    // stop_sample(vars.sam_menu);

    while(1)
    {
        int menu = 0; //wybrana opcja menu
        while(key[KEY_ENTER]);

        while(!key[KEY_ENTER])
        {
            clear_to_color(vars.bufor, makecol(0, 0, 0));
            //draw_sprite(vars.bufor, vars.bit_front, 0, 0);
            textout_centre_ex(vars.bufor, vars.fon_menu, "ARENA MODE",  (int)WIDTH / 2, (int)HEIGHT / 2 - 60,  (menu == 0)?(RED):(GREEN), -1);
            textout_centre_ex(vars.bufor, vars.fon_menu, "DEATH MODE",  (int)WIDTH / 2, (int)HEIGHT / 2 - 20,  (menu == 1)?(RED):(GREEN), -1);
            textout_centre_ex(vars.bufor, vars.fon_menu, "OPTIONS",     (int)WIDTH / 2, (int)HEIGHT / 2 + 20,  (menu == 2)?(RED):(GREEN), -1);
            textout_centre_ex(vars.bufor, vars.fon_menu, "EXIT",        (int)WIDTH / 2, (int)HEIGHT / 2 + 60,  (menu == 3)?(RED):(GREEN), -1);
            masked_blit(vars.bufor, screen, 0, 0, 0, 0, WIDTH,HEIGHT);

            //OBSŁUGA WYBORU OPCJI MENU
            if (key[KEY_DOWN])
            {
                if (++menu > 3) menu -= 4;
                while(key[KEY_DOWN]); //dopóki nie zwolnimy klawisza
            }
            if (key[KEY_UP])
            {
                if (--menu < 0) menu += 4;
                while(key[KEY_UP]);   //dopóki nie zwolnimy klawisza
            }
        } //while ENTER - po wduszeniu akcja

        if (menu < 2) show_players_options(menu);
        else if (menu == 2) show_options();
        else break; //EXIT GAME
    }
} //display_menu
