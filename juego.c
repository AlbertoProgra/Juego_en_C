#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>

#define FPS 30.0
#define CANTMOV 5
#define SCREEN_W 1300
#define SCREEN_H 920

enum KEYS{
    UP,   // 0
    DOWN, // 1
    LEFT, // 2
    RIGHT, // 3
    SPACE // 4
};

int teclas[5] = {0, 0, 0, 0, 0};

typedef struct jugador {
    int x; // posicion x de la nave
    int y; // posicion y de la nave
    ALLEGRO_BITMAP *nave; // imagen a renderizar
} jugador_t;

typedef struct fondo {
    ALLEGRO_BITMAP *fondog; // imagen a renderizar
}fondo_f;


typedef struct posBitty {
    int x;
    int y;
    int vida;
    int aux;
    ALLEGRO_BITMAP *bittys;
}enemigo;


typedef struct bullet{
    int x;
    int y;
    int vel_y;
} shoot_b;


// funcion ayuda que dibuja a nuestra navecita
void dibujarJugador(jugador_t *jugador, enemigo *posBitty, fondo_f *fondo) {

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(fondo->fondog, 0, 0, 0);
    al_draw_bitmap(jugador->nave, jugador->x, jugador->y, 0);
    al_draw_bitmap(posBitty->bittys, posBitty->x, posBitty->y, 0);
    
   al_flip_display();
}

void primeraEq(enemigo *posBitty){
    if (!posBitty->aux){
        if (posBitty->x <= (SCREEN_W -85 -20.0)){
            posBitty->x += 2.0;
        }
        else{
            posBitty->aux = 1;
        }   
    }
    else{
        if (posBitty->x >= (2.0)){
            posBitty->x -= 2.0;
        }
        else{
            posBitty->aux = 0;
        }  
    }
}

void segundaEq(enemigo *posBitty){

}

void moverArriba(jugador_t *jugador) {
    jugador->y -= 12.0;
}

void moverAbajo(jugador_t *jugador) {
    jugador->y += 12.0;
}

void moverDerecha(jugador_t *jugador) {
    jugador->x += 12.0;
    jugador->nave = al_load_bitmap("nave_d.png");
}

void moverIzquierda(jugador_t *jugador) {
    jugador->x -= 12.0;
    jugador->nave = al_load_bitmap("nave_i.png");
}

int main(int argc, char **argv) {
    // Nuestra pantalla
    ALLEGRO_DISPLAY *display = NULL;
    // Con esto podemos manejar eventos
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    // Timer para actulizar eventos
    ALLEGRO_TIMER *timer = NULL;

    al_init_primitives_addon();

    // Tratamos de inicializar allegro
    if(!al_init()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar allegro 5");
        return -1;
    }

    // Creamos un nuevo display de 1300x920 para empezar
    display = al_create_display(SCREEN_W, SCREEN_H);
    // Si no se pudo crear el display al_create_display devuelve false (0)
    if(!display) {
        fprintf(stderr, "%s\n", "No se pudo crear un display");
        return -1;
    }

    // Tratamos de agregar el addon de imagenes de allegro
    if(!al_init_image_addon()) {
        fprintf(stderr, "%s\n", "No se pudo inicializar el addon de imagenes");
        // tenemos que destruir el display que creamos
        al_destroy_display(display);
        return -1;
    }

    // Tratamos de instalar el teclado en allegro
    if(!al_install_keyboard()) {
        fprintf(stderr, "%s\n", "No se pudo instalar el teclado");
        // tenemos que destruir el display que creamos
        al_destroy_display(display);
        return -1;
    }

    // evitamos que se suspenda la computadora mientras esta el juego abierto
    al_inhibit_screensaver(1);
    // le ponemos un titulo a nuestro display
    al_set_window_title(display, "\t\t\t\t\t\t\t\t\t\t\t\t\t...........::::::::[ G     A     L     A     G     A ]::::::::...........");
    // al principio queremos que tenga fondo negro
    al_clear_to_color(al_map_rgb(0, 0, 0));
    // hacemos que se muestre lo que dibujamos
    //al_flip_display();

    // creamos el timer
    timer = al_create_timer(1.0 / FPS);

    // Creamos los eventos del juego
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // Creamos un jugador (miren como se usa malloc :) )
    // e inicializamos su posicion (620, 740)
    jugador_t *player = (jugador_t *)malloc(sizeof(jugador_t));
    player->nave = al_load_bitmap("nave.png");
    player->x = 620;
    player->y = 740;

    shoot_b *bullet[1];
    for (int i = 0; i < 1; i++)
    {
    bullet[i] = (shoot_b *)malloc(sizeof(shoot_b));
    bullet[i]->vel_y = 10;
    bullet[i]->y = 0;
    bullet[i]->x = 0;
    }

    void avion_dispara(shoot_b *bullet[], jugador_t *jugador){
        for (int i = 0; i <1; i++)
        {
            bullet[i]->y = jugador->y;
            bullet[i]->x = jugador->x;
        }
    }

    void activa_disparo(shoot_b *bullet[]){
        for (int i = 0; i < 1; i++)
        {
            al_draw_filled_circle(bullet[i]->x, bullet[i]->y, 6, al_map_rgb(0, 0, 0));
            bullet[i]->y -= bullet[i]->vel_y;
        }
        al_flip_display();
    }

    fondo_f *bg = (fondo_f *)malloc(sizeof(fondo_f));
    bg->fondog = al_load_bitmap("fondo.bmp");

    // si la imagen de la nave no se pudo cargar
    if(!player->nave) {
        fprintf(stderr, "%s\n", "No se pudo crear un display");
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        return -1;
    }

    enemigo *malo = (enemigo *)malloc(sizeof(enemigo));
    malo->bittys = al_load_bitmap("bitty.png");
    malo->x = 0;
    malo->y = 0;
    malo->aux = 0;

    // si la imagen de la nave no se pudo cargar
    if(!malo->bittys) {
        fprintf(stderr, "%s\n", "No se pudo crear un enemigo");
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
        return -1;
    }

    // dibujemos al jugador por primera vez
    dibujarJugador(player, malo, bg);

    // srand a un numero que tire el reloj
    srand(time(NULL));

    // comenzamos el timer
    al_start_timer(timer);

    // bandera para salir del juego se preciona escape
    int terminar = 0;

    // una variable que recibe eventos (?)
    ALLEGRO_EVENT ev;

    
    // loop del juego
    while(!terminar) {
        al_wait_for_event(event_queue, &ev);
        // si el evento es key_up
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
            }
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
                    avion_dispara(bullet, player);
                    break;
            }
        } else if(ev.type == ALLEGRO_EVENT_TIMER) {
            if(teclas[UP]){
                if (player->y >= 12.0){
                    moverArriba(player);
                }
            }
            else if(teclas[DOWN]){
                if (player->y <= (SCREEN_H -129 -4.0)){
                    moverAbajo(player);
                }
            }
            else if(teclas[LEFT]){
                if (player->x >= 12.0){
                    moverIzquierda(player);
                }
            }
            else if(teclas[RIGHT]){
                if (player->x <= (SCREEN_W -100 -4.0)){
                    moverDerecha(player);
                }
            }
            else if (teclas[SPACE])
            {
                activa_disparo(bullet);
            }
        }

        primeraEq(malo);

        
        // dibujamos al jugador
        dibujarJugador(player, malo, bg);
    }

    // siemple hay que limpiar memoria
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(player->nave);
    al_destroy_timer(timer);
    free(player);
    return 0;
}