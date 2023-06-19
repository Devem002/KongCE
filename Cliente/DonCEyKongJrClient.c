
// gcc -Ilibs/Include -Llibs/lib -o game DonCEyKongJrClient.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

//Estructura de datos para la identificación de las lianas y plataformas
typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} Line;

Line platforms[] = {
    {0, 863, 271, 863},
    {192, 607, 383, 607},
    {393, 797, 503, 797},
    {190, 447, 318, 447},
    {552, 830, 632, 830},
    {680, 796, 792, 796},
    {842, 765, 952, 765},
    {768, 543, 1024, 543},
    {608, 286, 829, 286},
    {283, 253, 606, 253},
    {287, 158, 382, 158}
};

Line lianas[] = {
    {80, 799, 80, 288},
    {176, 767, 176, 288},
    {272, 798, 272, 640},
    {272, 606, 272, 480},
    {432,703 ,432 ,288},
    {560 ,575 ,560 ,288},
    {656 ,702 ,656 ,320},
    {752 ,639 ,752 ,320},
    {848 ,702 ,848 ,577},
    {943 ,702 ,943 ,577},
    {848 ,543 ,848 ,192},
    {943 ,543 ,943 ,192},
    {431 ,191 ,432 ,127}
};

// Variables para la posición de Donkey Kong Jr.
int dk_x = 200;
int dk_y = 200;

// Variable para la vida de Donkey Kong Jr.
int life = 3;

// Variable para el puntaje
int score = 0;

// Variable para la victoria
bool win = false;

// Variables para la animación de Donkey Kong Jr.
int dk_frame = 0;
bool dk_facing_right = true;
bool dk_moving = false;
bool dk_flip = false;
bool dk_init = true;

// Variables para controlar el tiempo entre frames de animación
Uint32 last_frame_time = 0;
Uint32 frame_duration = 100;

// Variables para el salto de Donkey Kong Jr.
bool dk_jumping = false;
int dk_jump_start_y = 0;
int dk_jump_speed = 10;

// Función para detectar si Donkey Kong Jr. está en contacto con una plataforma
bool is_on_platform(int dk_x, int dk_y, Line platforms[], int num_platforms) {
    // Recorrer el array de plataformas
    for (int i = 0; i < num_platforms; i++) {
        // Verificar si la posición de Donkey Kong Jr. está dentro de los límites de la plataforma
        if (dk_x >= platforms[i].x1 && dk_x <= platforms[i].x2 && dk_y == platforms[i].y1) {
            // Si está en contacto con la plataforma, devolver true
            return true;
        }
    }
    // Si no está en contacto con ninguna plataforma, devolver false
    return false;
}

// Función para detectar si Donkey Kong Jr. está en contacto con una liana
bool is_on_liana(int dk_x, int dk_y, Line lianas[], int num_lianas) {
    // Recorrer el array de lianas
    for (int i = 0; i < num_lianas; i++) {
        // Verificar si la posición de Donkey Kong Jr. está dentro de los límites de la liana
        if (dk_x == lianas[i].x1 && dk_y >= lianas[i].y1 && dk_y <= lianas[i].y2) {
            // Si está en contacto con la liana, devolver true
            return true;
        }
    }
    // Si no está en contacto con ninguna liana, devolver false
    return false;
}

int main(int argc, char* argv[]) {
    // Inicializar SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    // Crear ventana
    SDL_Window *window = SDL_CreateWindow(
        "Donkey Kong Jr.",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1024,
        960,
        SDL_WINDOW_SHOWN
    );

    // Crear renderizador
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    // Cargar imagen de fondo
    SDL_Surface *background_surface = IMG_Load("Sprites/Background.png");
    SDL_Texture *background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
    SDL_FreeSurface(background_surface);

    // Cargar hoja de sprites
    SDL_Surface *spritesheet_surface = IMG_Load("Sprites/SpriteSheet.png");
    SDL_SetColorKey(spritesheet_surface, SDL_TRUE, SDL_MapRGB(spritesheet_surface->format, 0, 0, 0));
    SDL_Texture *spritesheet_texture = SDL_CreateTextureFromSurface(renderer, spritesheet_surface);
    SDL_FreeSurface(spritesheet_surface);

     // Bucle principal del juego
     bool running = true;
     while (running) {
         // Manejar eventos
         dk_moving = false;
         SDL_Event event;
         while (SDL_PollEvent(&event)) {
             if (event.type == SDL_QUIT) {
                 running = false;
             } else if (event.type == SDL_KEYDOWN) {
                 switch (event.key.keysym.sym) {
                     case SDLK_w:
                         // Iniciar salto si Donkey Kong Jr. está en contacto con una plataforma
                         if (is_on_platform(dk_x, dk_y, platforms, sizeof(platforms) / sizeof(platforms[0]))) {
                             dk_jumping = true;
                             dk_jump_start_y = dk_y;
                         }
                         break;
                     case SDLK_a:
                         dk_x -= 1;
                         dk_facing_right = false;
                         dk_moving = true;
                         break;
                     case SDLK_s:
                         dk_y += 1;
                         dk_moving = true;
                         break;
                     case SDLK_d:
                         dk_x += 1;
                         dk_facing_right = true;
                         dk_moving = true;
                         break;
                 }
             }
         }

         const Uint8 *state = SDL_GetKeyboardState(NULL);
         if (state[SDL_SCANCODE_W]) {
             // Iniciar salto si Donkey Kong Jr. está en contacto con una plataforma
             if (is_on_platform(dk_x, dk_y, platforms, sizeof(platforms) / sizeof(platforms[0]))) {
                 dk_jumping = true;
                 dk_jump_start_y = dk_y;
             }
         }
         if (state[SDL_SCANCODE_A]) {
             dk_x -= 1;
             dk_facing_right = false;
             dk_moving = true;
         }
         if (state[SDL_SCANCODE_S]) {
             dk_y += 1;
             dk_moving = true;
         }
         if (state[SDL_SCANCODE_D]) {
             dk_x += 1;
             dk_facing_right = true;
             dk_moving = true;
         }

         // Actualizar posición de Donkey Kong Jr. mientras está saltando
         if (dk_jumping) {
             int jump_height = dk_jump_speed * dk_jump_speed;
             int jump_progress = dk_jump_start_y - dk_y;
             if (jump_progress < jump_height) {
                 // Subir
                 dk_y -= dk_jump_speed;
             } else if (jump_progress < 2 * jump_height) {
                 // Bajar
                 dk_y += dk_jump_speed;
             } else {
                 // Detener salto
                 dk_jumping = false;
             }
         }

         // Hacer caer a Donkey Kong Jr. si no está en contacto con una plataforma
         else if (!dk_jumping && !is_on_platform(dk_x, dk_y, platforms, sizeof(platforms) / sizeof(platforms[0]))) {
             dk_y += 1;
         }

         // Limpiar pantalla
         SDL_RenderClear(renderer);

         // Dibujar imagen de fondo
         SDL_RenderCopy(renderer, background_texture, NULL, NULL);

         // Dibujar sprite de Donkey Kong Jr. si todavía tiene vida
         if (life > 0) {
             int sprite_x, sprite_y;
             if (!dk_moving) {
                 dk_frame = 0;
             }
             if (dk_init){
                sprite_x = 35; 
                sprite_y = 1; 
                if (state[SDL_SCANCODE_A] | state[SDL_SCANCODE_D] || state[SDL_SCANCODE_W]){
                    dk_init = false;
                }
             }
             else if (dk_facing_right) {
                dk_flip = true;
                 if (dk_frame == 0) {
                     sprite_x = 69;
                     sprite_y = 1;
                 } else if (dk_frame == 1) {
                     sprite_x = 103;
                     sprite_y = 1;
                 } else {
                     sprite_x = 137; 
                     sprite_y = 1; 
                 }
             } else { 
                dk_flip = false; 
                 if (dk_frame == 0) { 
                     sprite_x = 69; 
                     sprite_y = 1; 
                 } else if (dk_frame == 1) { 
                     sprite_x = 103; 
                     sprite_y = 1; 
                 } else { 
                     sprite_x = 137; 
                     sprite_y = 1; 
                 } 
             } 
             SDL_Rect srcrect = {sprite_x, sprite_y, 32, 16}; 
             SDL_Rect dstrect = {dk_x -72, dk_y -36,115.2 ,57.6}; 
             if (dk_flip) { 
                SDL_RenderCopyEx(renderer, spritesheet_texture, &srcrect, &dstrect, 0, NULL, SDL_FLIP_HORIZONTAL); 
            } else { 
                SDL_RenderCopy(renderer, spritesheet_texture, &srcrect, &dstrect); 
    } 
         }

        // Actualizar animación de Donkey Kong Jr.
        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_frame_time >= frame_duration) {
            dk_frame = (dk_frame +1) %3;
            last_frame_time = current_time;
        }

        // Presentar en pantalla
        SDL_RenderPresent(renderer);

       //Agregar retraso
        SDL_Delay(2);
    }

    // Destruir ventana y cerrar SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
