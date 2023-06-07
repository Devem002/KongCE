
// gcc -Ilibs/Include -Llibs/lib -o game DonCEyKongJrClient.c -lmingw32 -lSDL2main -lSDL2

#include <SDL2/SDL.h>

int main(int argc, char** argv) {
    // Inicializar SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Crear la ventana
    SDL_Window* window = SDL_CreateWindow("Donkey Kong Jr.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

    // Crear el renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Mantener la ventana abierta hasta que se cierre
    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Limpiar el renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Renderizar los elementos gráficos
        // Aquí puedes agregar el código para dibujar los sprites, lianas, etc.

        // Actualizar la ventana
        SDL_RenderPresent(renderer);
    }

    // Liberar recursos y cerrar SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}