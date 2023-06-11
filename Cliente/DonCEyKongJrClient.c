
// gcc -Ilibs/Include -Llibs/lib -o game DonCEyKongJrClient.c -lmingw32 -lSDL2main -lSDL2

#include <SDL2/SDL.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    // Inicializar SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Crear ventana
    SDL_Window *window = SDL_CreateWindow(
        "Donkey Kong Jr.",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280,
        720,
        SDL_WINDOW_SHOWN
    );

    // Crear renderizador
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    // Establecer color de dibujo en blanco
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Limpiar renderizador
    SDL_RenderClear(renderer);

    // Variables para la posición del rectángulo
    int x = 640;
    int y = 360;

    // Variable para la vida de DK
    int life = 3;

    // Variable para el puntaje
    int score = 0;

    // Variable para la victoria
    bool win = false;

    // Posición del cuadro rojo
    int red_x = 200;
    int red_y = 200;

    // Posición del cuadro azul
    int blue_x = 400;
    int blue_y = 400;

    // Posición del cuadro amarillo
    int yellow_x = 600;
    int yellow_y = 600;

    // Bucle principal del juego
    bool running = true;
    while (running) {
        // Manejar eventos
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        y -= 10;
                        break;
                    case SDLK_a:
                        x -= 10;
                        break;
                    case SDLK_s:
                        y += 10;
                        break;
                    case SDLK_d:
                        x += 10;
                        break;
                }
            }
        }

        // Verificar colisión con el cuadro rojo
        if (x >= red_x - 25 && x <= red_x + 25 && y >= red_y - 25 && y <= red_y + 25) {
            life--;
            if (life == 0) {
                running = false;
            }
        }

        // Verificar colisión con el cuadro azul
        if (x >= blue_x - 25 && x <= blue_x + 25 && y >= blue_y - 25 && y <= blue_y + 25) {
            score++;
        }

        // Verificar colisión con el cuadro amarillo
        if (x >= yellow_x - 25 && x <= yellow_x + 25 && y >= yellow_y - 25 && y <= yellow_y + 25) {
            win = true;
            running = false;
        }

        // Limpiar pantalla
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Dibujar rectángulo negro si DK todavía tiene vida
        if (life > 0) {
            SDL_Rect rect = {x - 25, y - 25, 50, 50};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        // Dibujar cuadro rojo
        SDL_Rect red_rect = {red_x - 25, red_y - 25, 50, 50};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &red_rect);

        // Dibujar cuadro azul
        SDL_Rect blue_rect = {blue_x - 25, blue_y - 25, 50, 50};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &blue_rect);

        // Dibujar cuadro amarillo
        SDL_Rect yellow_rect = {yellow_x - 25, yellow_y - 25, 50, 50};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &yellow_rect);

        // Presentar en pantalla
        SDL_RenderPresent(renderer);
    }

    // Destruir ventana y cerrar SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}