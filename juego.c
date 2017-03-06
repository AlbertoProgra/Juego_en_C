//Bibliotecas a importar dentro del programa
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>

//Constantes del programa
#define FPS 30.0 
#define SCREEN_W 1024 //Ancho de la pantalla
#define SCREEN_H 640 //Alto de la pantalla
#define VELX_NAVE 10 //Velocidad de la nave en X
#define VELY_NAVE 7.5 //Velocidad de la nave en Y
#define VEL_BITTYS 13 //Velocidad de los enemigos
#define CANT_ENEMI 9 //Cantidad de enemigos

//Enumera los nombres de variable segun un indice que empiezara en 0   
enum KEYS{
    UP,   //0
    DOWN, //1
    LEFT, //2
    RIGHT, //3
    SPACE //4
};

//Arreglo creado para efectuar una correspondencia entre KEYS y su posicion correspondiente dentro de teclas[] 
int teclas[5] = {0, 0, 0, 0, 0};

//Struct para el fondo del juego
typedef struct fondo {
    int op;
    ALLEGRO_BITMAP *fondog; //Imagen a renderizar
} fondo_f;

//Struct para el jugador
typedef struct jugador {
    int x; //Posicion x de la nave
    int y; //Posicion y de la nave
    ALLEGRO_BITMAP *nave; //Imagen a renderizar
} jugador_t;

//Struct para el enemigo
typedef struct bitty {
    int x; //Posicion en x
    int y; //Posicion en y
    int vida; //Puntos de vida [Segun el tipo]
    int tipo; //Tipo de enemigo
    int aux1; //auxiliar de movimiento 1
    int aux2; //auxiliar de movimiento 2
    int func; //Numero de funcion para el movimiento
    ALLEGRO_BITMAP *bittys; //Imagen [Segun el tipo]
} enemigo_s;

//Struct para los disparos
typedef struct bullet{
    int x; //Posicion en x
    int y; //Posicion en y
    int vel_y; //Velocidad de los disparos
    bool used; //Valida si esta en uso o no
} shoot_b;

//Funcion que dibuja el menu
void dibujarMenu(fondo_f *fondo){
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(fondo->fondog, 0, 0, 0);
    al_flip_display();
}

void moverMenu(fondo_f *fondo, int m){
    if(m == 2 && fondo->op < 3){
        fondo->op += 1;
    }
    else if(m == 1 && fondo->op > 1) {
        fondo->op -= 1;
    }
    switch(fondo->op){
        case 1:
            fondo->fondog = al_load_bitmap("fondo_menu_nuevo_juego.jpg");
            break;
        case 2:
            fondo->fondog = al_load_bitmap("fondo_menu_como_se_juega.jpg");
            break;
        case 3:
            fondo->fondog = al_load_bitmap("fondo_menu_salir.jpg");
            break;
        case 4:
            fondo->fondog = al_load_bitmap("fondo_instrucciones.jpg");
            break;
    }
}

//Funcion que ayuda a dibujar el juego
void dibujarJuego(jugador_t *jugador, enemigo_s *bitty[], shoot_b *bullet[], fondo_f *fondo, enemigo_s *bossy) {
    int i;

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(fondo->fondog, 0, 0, 0);
    al_draw_bitmap(jugador->nave, jugador->x, jugador->y, 0);

    if(bossy->vida != 0){
        al_draw_bitmap(bossy->bittys, bossy->x, bossy->y, 0);
        //Dibujar balas del boss
    }
    else{
        for(i=0; i<CANT_ENEMI; i++){
            if(bitty[i]->vida){
                al_draw_bitmap(bitty[i]->bittys, bitty[i]->x, bitty[i]->y, 0);
            }
        }
    }
    
    for(i=0; i<5; i++){
        if(bullet[i]->used){
            al_draw_filled_circle(bullet[i]->x, bullet[i]->y, 4, al_map_rgb(0, 0, 0));
            al_draw_filled_circle(SCREEN_W - 5 -(10*i), SCREEN_H -5, 4, al_map_rgb(255, 0, 0));
        }
        else{
            al_draw_filled_circle(SCREEN_W - 5 -(10*i), SCREEN_H -5, 4, al_map_rgb(0, 0, 0));
        }
    }
   al_flip_display();
}

//Funcion_1_enemigo: controla el movimiento del enemigo
void primeraEq(enemigo_s *bitty){

        switch(bitty->aux1){
            case 0:
                if (bitty->x <= (SCREEN_W - 52 - 4.0)){
                    bitty->x += VEL_BITTYS;
                }
                else{
                    if(bitty->y == 52*5){
                        bitty->x = 0;
                        bitty->y = 0;
                        bitty->aux1 = 0;
                        bitty->aux2 = 0;
                        bitty->func = 2;
                    }
                    else{
                        bitty->aux1 = 2;
                        bitty->aux2 = 0;
                    }
                } 
                break;
            case 1:
                if (bitty->x >= (2.0)){
                    bitty->x -= VEL_BITTYS;
                }
                else{
                    if(bitty->y == 52*5){
                        bitty->x = 979;
                        bitty->y = -52;
                        bitty->aux1 = 0;
                        bitty->aux2 = 0;
                        bitty->func = 2;
                    }
                    else{
                        bitty->aux1 = 2;
                        bitty->aux2 = 1;
                    }
                }
                break;
            case 2:
                bitty->y += VEL_BITTYS;
                if (bitty->y % 52 == 0){
                    if(bitty->aux2){
                        bitty->aux1 = 0;    
                    }
                    else{
                        bitty->aux1 = 1;
                    }
                }
                break;
        }
}

//Funcion_2_enemigo: controla el movimiento del enemigo_2
void segundaEq(enemigo_s *bitty){

        switch(bitty->aux1){
            case 0:
                if (bitty->y <= (SCREEN_H/2 - 52 - 4.0)){
                    bitty->y += VEL_BITTYS;
                }
                else{
                    if(bitty->x == 52*20){
                        bitty->x = 0;
                        bitty->y = -52;
                        bitty->aux1 = 0;
                        bitty->aux2 = 0;
                        bitty->func = 1;
                    }
                    else{
                        bitty->aux1 = 2;
                        bitty->aux2 = 0;
                    }
                } 
                break;
            case 1:
                if (bitty->y > 0){
                    bitty->y -= VEL_BITTYS;
                }
                else{
                    if(bitty->x == 0){
                        bitty->x = 0;
                        bitty->y = 0;
                        bitty->aux1 = 0;
                        bitty->aux2 = 0;
                        bitty->func = 2;
                    }
                    else{
                        bitty->aux1 = 2;
                        bitty->aux2 = 1;
                    }
                }
                break;
            case 2:
                bitty->x += VEL_BITTYS;
                if (bitty->x % 104 == 0){
                    if(bitty->aux2){
                        bitty->aux1 = 0;    
                    }
                    else{
                        bitty->aux1 = 1;
                    }
                }
                break;
        }
}



//Funcion_1_jugador: modifica el movimiento del avion en el eje -y
void moverArriba(jugador_t *jugador) {
    jugador->y -= VELY_NAVE;
}

//Funcion_2_jugador: modifica el movimiento del avion en el eje +y
void moverAbajo(jugador_t *jugador) {
    jugador->y += VELY_NAVE;
}

//Funcion_3_jugador: modifica el movimiento del avion en el eje +x
void moverDerecha(jugador_t *jugador) {
    jugador->x += VELX_NAVE;
    jugador->nave = al_load_bitmap("nave_d.png");
}

//Funcion_3_jugador: modifica el movimiento del avion en el eje -x
void moverIzquierda(jugador_t *jugador) {
    jugador->x -= VELX_NAVE;
    jugador->nave = al_load_bitmap("nave_i.png");
}

//Funcion_1_disparo: alinea el disparo con el avion en el eje x
void creaDisparo(shoot_b *bullet, jugador_t *jugador){
    bullet->y = jugador->y;
    bullet->x = jugador->x + 20.5;
    bullet->used = true;
}

//Funcion_2_disparo: modifica el movimiento del disparo en el eje -y
void moverDisparo(shoot_b *bullet, enemigo_s *bitty[]){
    if(bullet->y < 0){
        bullet->y = 0;
        bullet->x = 0;
        bullet->used = false;
    }
    else {
        int i;
        for(i=0; i<CANT_ENEMI;i++){
            if(bitty[i]->vida){
                if((bullet->y+2 <= bitty[i]->y + 52) && (bullet->y >= bitty[i]->y)){
                    if((bullet->x+2 >= bitty[i]->x) && (bullet->x <= bitty[i]->x + 52)){
                        bitty[i]->vida -= 1;
                        bullet->y = 0;
                        bullet->x = 0;
                        bullet->used = false;
                        if(bitty[i]->vida == 1){
                            bitty[i]->bittys = al_load_bitmap("bitty1.png");
                        } 
                    }
                }
            }
        }
        bullet->y -= bullet->vel_y;
    }
}

//Funcion main principal
int main(int argc, char **argv) {
    //Nuestra pantalla
    ALLEGRO_DISPLAY *display = NULL;


    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_SAMPLE *sample2 = NULL;
    ALLEGRO_SAMPLE *sample3 = NULL;

    //Con esto podemos manejar eventos
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    //Timer para actulizar eventos
    ALLEGRO_TIMER *timer = NULL;

    al_init_primitives_addon();

    //Tratamos de inicializar allegro
    if(!al_init()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar allegro 5");
        return -1;
    }

    if(!al_install_audio()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar el audio");
        return -1;
    }

    if(!al_init_acodec_addon()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar audio codecs");
        return -1;
    }

    if(!al_reserve_samples(1)) {
        fprintf(stderr, "%s\n", "No se pudo reservar la muestra de audio");
        return -1;
    }

    sample = al_load_sample( "KubbiEmber.wav" );
    sample2 = al_load_sample( "Chop_Suey_8_Bit.wav" );
    sample3 = al_load_sample( "KubbiEmber2.wav" );
    

    if(!sample) {
        fprintf(stderr, "%s\n", "No se pudo cargar el clip de audio");
        return -1;
    }

    //Creamos un nuevo display de 1024x640 para empezar
    display = al_create_display(SCREEN_W, SCREEN_H);
    //Si no se pudo crear el display al_create_display devuelve false (0)
    if(!display) {
        fprintf(stderr, "%s\n", "No se pudo crear un display");
        return -1;
    }

    //Tratamos de agregar el addon de imagenes de allegro
    if(!al_init_image_addon()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar el addon de imagenes");
        //Tenemos que destruir el display que creamos
        al_destroy_display(display);
        return -1;
    }

    //Tratamos de instalar el teclado en allegro
    if(!al_install_keyboard()) {
        fprintf(stderr, "%s\n", "No se pudo instalar el teclado");
        //Tenemos que destruir el display que creamos
        al_destroy_display(display);
        return -1;
    }

    //Evitamos que se suspenda la computadora mientras esta el juego abierto
    al_inhibit_screensaver(1);
    //Le ponemos un titulo a nuestro display
    al_set_window_title(display, "\t\t\t\t\t\t\t\t...........::::::::[  PAPER -> SH O O  T   E    R  ]::::::::...........");
    //Creamos el timer
    timer = al_create_timer(1.0 / FPS);

    //Creamos los eventos del juego
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    //Creamos un jugador e inicializamos su posicion en (532.5, 579)
    jugador_t *player = (jugador_t *)malloc(sizeof(jugador_t));
    player->nave = al_load_bitmap("nave.png");
    player->x = SCREEN_W/2 + 20.5;
    player->y = SCREEN_H - 61;

    //Creamos un arreglo de 5 disparos, inicializamos su posicion en (0,0) y llevamos control de cuantos (tenemos y restan) 
    shoot_b *bullet[5];
    int cont_b;
    for (cont_b = 0; cont_b < 5; cont_b ++){
        bullet[cont_b] = (shoot_b *)malloc(sizeof(shoot_b));
        bullet[cont_b]->vel_y = 10;
        bullet[cont_b]->y = 0;
        bullet[cont_b]->x = 0;
        bullet[cont_b]->used = 0;
    }
    cont_b = 0;

    //Creamos un fondo por default se inicializa su posicion en (0,0)
    fondo_f *bg = (fondo_f *)malloc(sizeof(fondo_f));
    bg->fondog = al_load_bitmap("fondo_menu_nuevo_juego.jpg");
    bg->op = 1;

    //Si la imagen de la nave no se pudo cargar
    if(!player->nave) {
        fprintf(stderr, "%s\n", "No se pudo crear un display");
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        return -1;
    }

    //Creamos un arreglo de enemigos
    enemigo_s *malo[CANT_ENEMI];
    int cont_e;
    for (cont_e = 0; cont_e < CANT_ENEMI; cont_e++){
        malo[cont_e] = (enemigo_s *)malloc(sizeof(enemigo_s));
        malo[cont_e]->bittys = al_load_bitmap("bitty.png");
        malo[cont_e]->x = cont_e*104;
        malo[cont_e]->y = -52;
        malo[cont_e]->aux1 = 0;
        malo[cont_e]->aux2 = 0;
        malo[cont_e]->vida = 2;
        malo[cont_e]->func = 1;
        //Si la imagen de la nave no se pudo cargar
        if(!malo[cont_e]->bittys) {
            fprintf(stderr, "%s\n", "No se pudo crear un enemigo");
            al_destroy_display(display);
            al_destroy_event_queue(event_queue);
            al_destroy_timer(timer);
            return -1;
        }
    }

    //Creamos al Boss
    enemigo_s *boss = (enemigo_s *)malloc(sizeof(enemigo_s));
    boss->bittys = al_load_bitmap("bitto.png");
    boss->x = SCREEN_W/2 - 94.5;
    boss->y = -157;
    boss->vida = 0;
    boss->func = 1;
    boss->aux1 = 0;

    //srand a un numero que tire el reloj
    srand(time(NULL));

    //Comenzamos el timer
    al_start_timer(timer);

    //Bandera para salir del juego se preciona escape
    int terminar = 0;

    //Una variable que recibe eventos (?)
    ALLEGRO_EVENT ev;

    void entradaBoss(enemigo_s *bossy){
    sample3 = NULL;
    al_play_sample(sample2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    if(bossy->y < 5){

        bossy->y += 1;
    }
    else{
        bossy->func = 2;
        bossy->aux1 = 1;
    }
}

void battleBoss(enemigo_s *bossy, jugador_t *jugador){
    if(jugador->x > bossy->x){
        bossy->x += 5;
    }else if(jugador->x < bossy->x){
        bossy->x -= 5;
    }
    if(bossy->aux1){
        if(jugador->y > bossy->y){
            bossy->y += 5;
            if(bossy->y == SCREEN_H/2 - 60){
                bossy->aux1 = 0;
            }
        }else if(jugador->y < bossy->y){
            bossy->y -= 5;
        }  
    }
    else{
        if(bossy->y > 5){
            bossy->y -= 5;
        }
        else{
            bossy->aux1 = 1;
        }
    }
      
}




    //Dibujamos el Menu
    int menu = 0;

    while(!menu){
        al_wait_for_event(event_queue, &ev);
        al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
        if(ev.type == ALLEGRO_EVENT_KEY_UP){
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    teclas[UP] = 0;
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[DOWN] = 0;
                    break;
                case ALLEGRO_KEY_SPACE:
                    teclas[SPACE] = 0;
                    break;
            }
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    if(bg->op != 4){
                        teclas[UP] = 1;
                        moverMenu(bg, 1);
                    }
                    break;
                case ALLEGRO_KEY_DOWN:
                    if(bg->op != 4){
                        teclas[DOWN] = 1;
                        moverMenu(bg, 2);
                    }
                    break;
                case ALLEGRO_KEY_SPACE:
                    
                    switch(bg->op){
                        case 1:
                            menu = 1;
                            break;
                        case 2:
                            bg->op = 4;
                            moverMenu(bg, 3);
                            break;
                        case 3:
                            menu = 1;
                            terminar = 1;
                            break;
                        case 4:
                            bg->op = 1;
                            moverMenu(bg, 0);
                            break;
                    }
                    break;
            }
            //Si el evento es Timer
        } else if(ev.type == ALLEGRO_EVENT_TIMER) {
            
        }
        
        dibujarMenu(bg);
    }

    if(!terminar){
        al_destroy_sample(sample);
        sample = NULL;
        al_play_sample(sample3, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
       bg->fondog = al_load_bitmap("fondo.jpg");
        //Dibujemos el juego por primera vez
        dibujarJuego(player, malo, bullet, bg, boss);
    }

    //Bandera para el Boss
        bool go_Boss = false; 
    
    //Loop del juego
    while(!terminar) {
        al_wait_for_event(event_queue, &ev);
        //Si el evento es key_up
        if(ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    terminar = 1;
                break;
                case ALLEGRO_KEY_UP:
                    teclas[UP] = 0;
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[DOWN] = 0;
                    break;
                case ALLEGRO_KEY_LEFT:
                    teclas[LEFT] = 0;
                    player->nave = al_load_bitmap("nave.png");
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[RIGHT] = 0;
                    player->nave = al_load_bitmap("nave.png");
                    break;
                case ALLEGRO_KEY_SPACE:
                    teclas[SPACE] = 0;
                    break;
            }
            //Si el evento es key_down
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    teclas[UP] = 1;
                    break;
                case ALLEGRO_KEY_DOWN:
                    teclas[DOWN] = 1;
                    break;
                case ALLEGRO_KEY_LEFT:
                    teclas[LEFT] = 1;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    teclas[RIGHT] = 1;
                    break;
                case ALLEGRO_KEY_SPACE:
                    teclas[SPACE] = 1;
                    cont_b ++;
                    if (cont_b <= 5){
                        creaDisparo(bullet[cont_b-1], player);
                    }
                    break;
            }
            //Si el evento es Timer
        } else if(ev.type == ALLEGRO_EVENT_TIMER) {
            if(teclas[UP]){
                if (player->y >= (SCREEN_H/2)){
                    moverArriba(player);
                }
            }
            else if(teclas[DOWN]){
                if (player->y <= (SCREEN_H -61 -4.0)){
                    moverAbajo(player);
                }
            }
            else if(teclas[LEFT]){
                if (player->x >= 12.0){
                    moverIzquierda(player);
                }
            }
            else if(teclas[RIGHT]){
                if (player->x <= (SCREEN_W -51)){
                    moverDerecha(player);
                }
            }
            int i;
            int cont_i = 0;
            
            for (i = 0; i < 5; i ++){
                if(bullet[i]->used) {
                    moverDisparo(bullet[i], malo);
                }
                else {
                    cont_i++;
                }
            }
            if(cont_i == 5){
                cont_b = 0;
            }
            
            int cont_muertos = 0;

            if(!go_Boss){
                
               for(i=0; i<CANT_ENEMI; i++){
                    if(malo[i]->func == 1){
                        primeraEq(malo[i]);
                    }
                    else if(malo[i]->func == 2){
                        segundaEq(malo[i]);
                    }

                    if(malo[i]->vida == 0){
                        cont_muertos++;
                    }
                }
                if (cont_muertos == 9){
                    go_Boss = true;
                    boss->vida = 50;
                }
            }
            else{
                al_destroy_sample(sample3);
                if(boss->func == 1){

                    entradaBoss(boss);
                }
                else {
                   battleBoss(boss, player); 
                }
            }

            
            i = 0;
            cont_i = 0;


        }
  
        //Recargamos el juego (del backBuffer al frontBuffer)
        dibujarJuego(player, malo, bullet, bg, boss);
    }

    //Se Limpia memoria (solo cuando el ciclo while termina)
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(player->nave);
    al_destroy_timer(timer);
    al_destroy_sample(sample);
    free(player);
    free(bg);
    return 0; //Como habito de buen programador
}
