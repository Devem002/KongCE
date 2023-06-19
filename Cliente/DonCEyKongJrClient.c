
// gcc -Ilibs/Include -Llibs/lib -o game DonCEyKongJrClient.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include "Constantes.h"

int main(int argc, char* argv[]) {
    
    // Inicializar SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    // Abrir joystick
    SDL_Joystick *joystick = SDL_JoystickOpen(0);

    // Inicializar SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
    // Inicializar SDL_ttf
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }

    // Cargar musica
    Mix_Music *music = Mix_LoadMUS("Music/SoundTrack.mp3");
    if (music == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    // Reducir el volumen de la música 
    Mix_VolumeMusic(20);

    Mix_PlayMusic(music, -1);

    // Cargar efecto de sonido de victoria
    Mix_Chunk *win_sound = Mix_LoadWAV("Music/Win.mp3");
    if (win_sound == NULL) {
        printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
    // Reducir el volumen del efecto de victoria 
    Mix_VolumeChunk(win_sound, 10);

    // Cargar efecto de sonido de derrota
    Mix_Chunk *lose_sound = Mix_LoadWAV("Music/Lose.mp3");
    if (lose_sound == NULL) {
        printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
    // Reducir el volumen del efecto de derrota 
    Mix_VolumeChunk(lose_sound, 10);

    // Cargar efecto de sonido de comer
    Mix_Chunk *bite_sound = Mix_LoadWAV("Music/Bite.mp3");
    if (lose_sound == NULL) {
        printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
    // Reducir el volumen del efecto de derrota 
    Mix_VolumeChunk(bite_sound, 10);


    // Crear ventana
    SDL_Window *window = SDL_CreateWindow(
        "Donkey Kong Jr.",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1024,
        960,
        SDL_WINDOW_SHOWN
    );
    //Ventana seleccion de jugadores
    SDL_Window* auxWindow = SDL_CreateWindow("Ventana Auxiliar", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_SHOWN);
    SDL_Renderer* auxRenderer = SDL_CreateRenderer(auxWindow, -1, SDL_RENDERER_ACCELERATED);

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

    // Cargar fuente
    TTF_Font *font = TTF_OpenFont("Fonts/AGENCYB.TTF", 28);
    if (font == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(1);
    }

    // Establecer color del texto (blanco)
    SDL_Color textColor = {255, 255, 255};
    
    //Bucle encargado de relizar la confirmación de si serán 2 o un jugador los que participaran en el juego.
    bool on = true;
    while (on) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                on = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= 50 && mouseX <= 150 && mouseY >= 50 && mouseY <= 100) {
                    num_players = 1;
                    on = false;
                }
                else if (mouseX >= 50 && mouseX <= 150 && mouseY >= 110 && mouseY <= 160) {
                    num_players = 2;
                    on = false;
                }
            }
        }
        SDL_SetRenderDrawColor(auxRenderer, 255, 255, 255, 255);
        SDL_RenderClear(auxRenderer);

        // Dibujar texto
        TTF_Font* font = TTF_OpenFont("Fonts/AGENCYB.TTF", 20);
        SDL_Color color = { 0, 0, 0 };
        SDL_Surface* surface = TTF_RenderText_Solid(font, "Cuantos jugadores?", color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(auxRenderer, surface);
        SDL_Rect textRect = { 50, 20, surface->w, surface->h };
        SDL_RenderCopy(auxRenderer, texture, NULL, &textRect);

        // Dibujar botón "1 jugador"
        SDL_SetRenderDrawColor(auxRenderer, 0, 255, 0, 255);
        SDL_Rect buttonRect1 = { 50, 50, 100, 50 };
        SDL_RenderFillRect(auxRenderer, &buttonRect1);

        // Dibujar texto "1"
        SDL_Surface* surface1 = TTF_RenderText_Solid(font, "1", color);
        SDL_Texture* texture1 = SDL_CreateTextureFromSurface(auxRenderer, surface1);
        int textWidth1 = surface1->w;
        int textHeight1 = surface1->h;
        SDL_Rect textRect1 = { buttonRect1.x + (buttonRect1.w - textWidth1) / 2, buttonRect1.y + (buttonRect1.h - textHeight1) / 2, textWidth1, textHeight1 };
        SDL_RenderCopy(auxRenderer, texture1, NULL, &textRect1);

        // Dibujar botón "2 jugadores"
        SDL_SetRenderDrawColor(auxRenderer, 0, 0, 255, 255);
        SDL_Rect buttonRect2 = { 50, 110, 100, 50 };
        SDL_RenderFillRect(auxRenderer, &buttonRect2);

        // Dibujar texto "2"
        SDL_Surface* surface2 = TTF_RenderText_Solid(font, "2", color);
        SDL_Texture* texture2 = SDL_CreateTextureFromSurface(auxRenderer, surface2);
        int textWidth2 = surface2->w;
        int textHeight2 = surface2->h;
        SDL_Rect textRect2 = { buttonRect2.x + (buttonRect2.w - textWidth2) / 2, buttonRect2.y + (buttonRect2.h - textHeight2) / 2, textWidth2, textHeight2 };
        SDL_RenderCopy(auxRenderer, texture2, NULL, &textRect2);

        SDL_RenderPresent(auxRenderer);
    }
    SDL_DestroyRenderer(auxRenderer);
    SDL_DestroyWindow(auxWindow);

    // Crear jugadores
    for (int i = 0; i < num_players; i++) {
        players[i].score = 0;
        players[i].life = 3;
    }
    
    // Renderizar texto del puntaje
    char scoreText[64];
    sprintf(scoreText, "%d", players[turn].score);
    SDL_Surface *scoreSurface = TTF_RenderText_Blended(font, scoreText, textColor);
    SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    int scoreWidth = scoreSurface->w;
    int scoreHeight = scoreSurface->h;
    SDL_FreeSurface(scoreSurface);

    // Renderizar texto de las vidas
    char lifeText[64];
    sprintf(lifeText, "%d", players[turn].life);
    SDL_Surface *lifeSurface = TTF_RenderText_Blended(font, lifeText, textColor);
    SDL_Texture *lifeTexture = SDL_CreateTextureFromSurface(renderer, lifeSurface);
    int lifeWidth = lifeSurface->w;
    int lifeHeight = lifeSurface->h;
    SDL_FreeSurface(lifeSurface);

    // Renderizar texto del jugador que esta jugando
    char turnText[64];
    sprintf(turnText, "1");
    SDL_Surface *turnSurface = TTF_RenderText_Blended(font, turnText, textColor);
    SDL_Texture *turnTexture = SDL_CreateTextureFromSurface(renderer, turnSurface);
    int turnWidth = turnSurface->w;
    int turnHeight = turnSurface->h;
    SDL_FreeSurface(turnSurface);

    //LLamado a generar frutas
    generate_fruits();

    //Llamado a generar cocodrilos
    generate_cocodriles();

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
                        }// Subir liana si Donkey Kong Jr. está en contacto con una liana
                        else if (is_on_liana(dk_x, dk_y, lianas, sizeof(lianas) / sizeof(lianas[0]))) {
                            dk_y -= 0.5;
                            dk_moving = true;
                        }
                        break;
                    case SDLK_a:
                        dk_x -= 1;
                        dk_facing_right = false;
                        dk_moving = true;
                        break;
                    case SDLK_s:
                        // Bajar liana si Donkey Kong Jr. está en contacto con una liana
                        if (is_on_liana(dk_x, dk_y, lianas, sizeof(lianas) / sizeof(lianas[0]))) {
                            dk_y += 2;
                            dk_moving = true;
                        }
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
        if (state[SDL_SCANCODE_W] || SDL_JoystickGetButton(joystick, 4)) {
            // Iniciar salto si Donkey Kong Jr. está en contacto con una plataforma
            if (is_on_platform(dk_x, dk_y, platforms, sizeof(platforms) / sizeof(platforms[0]))) {
                dk_jumping = true;
                dk_jump_start_y = dk_y;
             }
             // Subir liana si Donkey Kong Jr. está en contacto con una liana
            else if (is_on_liana(dk_x, dk_y, lianas, sizeof(lianas) / sizeof(lianas[0]))) {
                dk_y -= 0.5;
                dk_moving = true;
            }
        }
        if (state[SDL_SCANCODE_A] || SDL_JoystickGetButton(joystick, 7)) {
            dk_x -= 1;
            dk_facing_right = false;
            dk_moving = true;
        }
        if (state[SDL_SCANCODE_S] || SDL_JoystickGetButton(joystick, 6)) {
            // Bajar liana si Donkey Kong Jr. está en contacto con una liana
            if (is_on_liana(dk_x, dk_y, lianas, sizeof(lianas) / sizeof(lianas[0]))) {
                dk_y += 2;
                dk_moving = true;
            }
        }
        if (state[SDL_SCANCODE_D] || SDL_JoystickGetButton(joystick, 5)) {
            dk_x += 1;
            dk_facing_right = true;
            dk_moving = true;
        }

        // Evitar que Donkey Kong Jr. se salga de los límites de la pantalla
        if (dk_x < 0) {
            dk_x = 0;
        }
        if (dk_x > 1024) {
            dk_x = 1024;
        }
        if (dk_y < 0) {
            dk_y = 0;
        }
        if (dk_y > 960) {
            dk_y = 960;
        }

        // Actualizar posición de Donkey Kong Jr. mientras está saltando
        if (dk_jumping && !is_on_liana(dk_x, dk_y, lianas, sizeof(lianas) / sizeof(lianas[0]))) {
            is_on_platform(dk_x, dk_y, platforms, sizeof(platforms) / sizeof(platforms[0]));
            int jump_height = dk_jump_speed * dk_jump_speed;
            int jump_progress = dk_jump_start_y - dk_y;
            if (jump_progress < jump_height) {
                // Subir
                dk_y -= dk_jump_speed;
            } else if (jump_progress < 2 * jump_height) {
                // Bajar
                dk_y += dk_jump_speed;
            } 
        }
        SDL_Delay(1.65);

        // Hacer caer a Donkey Kong Jr. si no está en contacto con una plataforma
        if ((!dk_jumping && !is_on_platform(dk_x, dk_y, platforms, sizeof(platforms) / sizeof(platforms[0]))) && !is_on_liana(dk_x, dk_y, lianas, sizeof(lianas) / sizeof(lianas[0]))) {
            dk_y += 1;
        }else{
            dk_jumping = false; 
        }

        // Mover cocodrilos
        for (int i = 0; i < 5; i++) {
            if (crocodiles[i].active) {
                if (crocodiles[i].moving_up) {
                    crocodiles[i].y -= 1;
                    if (crocodiles[i].y <= lianas[crocodiles[i].liana_index].y2) {
                        crocodiles[i].moving_up = false;
                    }
                } else {
                    crocodiles[i].y += 1;
                    if (crocodiles[i].y >= lianas[crocodiles[i].liana_index].y1) {
                        crocodiles[i].moving_up = true;
                    }
                }
            }
        }

        // Detectar si Donkey Kong Jr. ha tocado un cocodrilo
        for (int i = 0; i < 5; i++) {
            if (crocodiles[i].active && dk_x >= crocodiles[i].x - 15 && dk_x <= crocodiles[i].x + 25 && dk_y <= crocodiles[i].y && dk_y >= crocodiles[i].y - 50) {
                // Actualizar estado de daño
                damage = true;
            }
        }

        // Detectar si Donkey Kong Jr. ha tocado una fruta
        for (int i = 0; i < 5; i++) {
            if (fruits[i].active && dk_x >= fruits[i].x - 15 && dk_x <= fruits[i].x + 25 && dk_y <= fruits[i].y && dk_y >= fruits[i].y - 50) {
                // Desactivar la fruta
                fruits[i].active = false;

                //Reproducir sonido de muerte
                Mix_PlayChannel(-1, bite_sound, 0);

                // Actualizar puntaje
                players[turn].score += 100;
                }}

        // Limpiar pantalla
        SDL_RenderClear(renderer);

        // Dibujar imagen de fondo
        SDL_RenderCopy(renderer, background_texture, NULL, NULL);
        
        // Mostrar texto del puntaje
        SDL_Rect scoreRect = {740, 155, scoreWidth, scoreHeight};
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

        // Mostrar texto de las vidas
        SDL_Rect lifeRect = {905, 155, lifeWidth, lifeHeight};
        SDL_RenderCopy(renderer, lifeTexture, NULL, &lifeRect);

        // Mostrar texto del turno del jugador
        SDL_Rect turnRect = {685, 155, turnWidth, turnHeight};
        SDL_RenderCopy(renderer, turnTexture, NULL, &turnRect);

        // Dibujar cocodrilos
        for (int i = 0; i < 5; i++) {
            if (crocodiles[i].active) {
                int sprite_cocox;
                int sprite_cocoy;
                if(crocodiles[i].frame == 0){
                    sprite_cocox = 145;
                    sprite_cocoy = 55;
                }else{
                    sprite_cocox = 163;
                    sprite_cocoy = 55;
                }
                
                SDL_Rect srcrect = {sprite_cocox, sprite_cocoy, 16, 16};
                SDL_Rect dstrect = {crocodiles[i].x -16, crocodiles[i].y -16, 32, 32};
                SDL_RenderCopy(renderer, spritesheet_texture, &srcrect, &dstrect);
            }
        }


        // Dibujar frutas
        for (int i = 0; i < 5; i++) {
            if (fruits[i].active) {
                int sprite_fruitx = 1;
                int sprite_fruity = 73;
                SDL_Rect srcrect = {sprite_fruitx, sprite_fruity, 16, 16};
                SDL_Rect dstrect = {fruits[i].x -16, fruits[i].y -16, 32, 32};
                SDL_RenderCopy(renderer, spritesheet_texture, &srcrect, &dstrect);
            }
        }

        // Dibujar sprite de Donkey Kong Jr. si todavía tiene vida
        if (players[turn].life > 0) {
            int sprite_x, sprite_y;
            if (!dk_moving) {
                dk_frame = 0;
            }
            if (dk_init){
                sprite_x = 35; 
                sprite_y = 1; 
                if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D] || state[SDL_SCANCODE_W]){
                    dk_init = false;
                }
            }
            else if (is_on_liana(dk_x, dk_y, lianas, sizeof(lianas) / sizeof(lianas[0]))) {
                // Seleccionar sprite para subir o bajar liana
                if (dk_frame == 0) {
                    sprite_x = 239;
                    sprite_y = 1;
                } else {
                    sprite_x = 273;
                    sprite_y = 1;
                }}
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
        //Se encargar de revisar la muerte de Donkey Kong Jr.
        if(dk_y > 900 || damage) {
            damage = false;
            
            //Resetear los cocodrilos
                for (int i = 0; i < 5; i++) {
            crocodiles[i].active = false;
            }
            generate_cocodriles();

            // Restar 1 a la vida de Donkey Kong Jr.
            players[turn].life -= 1;

            //Actualización del texto
            sprintf(lifeText, "%d", players[turn].life);
            lifeSurface = TTF_RenderText_Blended(font, lifeText, textColor);
            lifeTexture = SDL_CreateTextureFromSurface(renderer, lifeSurface);
            lifeWidth = lifeSurface->w;
            lifeHeight = lifeSurface->h;
            SDL_FreeSurface(lifeSurface);

            //Reproducir sonido de muerte
            Mix_PlayChannel(-1, lose_sound, 0);

            // Mostrar sprite de muerte durante un segundo
            SDL_RenderCopy(renderer, background_texture, NULL, NULL);
            int sprite_x = 409;
            int sprite_y = 1;
            SDL_Rect srcrect = {sprite_x, sprite_y, 32, 16};
            SDL_Rect dstrect = {dk_x -72, dk_y -36,115.2 ,57.6};
            SDL_RenderCopy(renderer, spritesheet_texture, &srcrect, &dstrect);
            SDL_RenderPresent(renderer);
            SDL_Delay(500);
            SDL_RenderCopy(renderer, background_texture, NULL, NULL);

            sprite_x = 443;
            srcrect.x = sprite_x;
            SDL_RenderCopy(renderer, spritesheet_texture, &srcrect, &dstrect);
            SDL_RenderPresent(renderer);
            SDL_Delay(500);

            // Reiniciar variables del juego
            dk_x = 50;
            dk_y = 848;
            dk_frame = 0;
            dk_facing_right = true;
            dk_moving = false;
            dk_jumping = false;
            dk_jump_start_y = 0;

            // Verificar si Donkey Kong Jr. ha perdido todas sus vidas
            if (players[turn].life <= 0) {
                // Mostrar ventana emergente con el mensaje "Game Over"
                char message[64];
                if (turn == 0){
                    sprintf(message,"Has perdido todas tus vidas. Tuviste un puntaje de: %d, Jugador 1", players[turn].score);
                }else{
                    sprintf(message,"Has perdido todas tus vidas. Tuviste un puntaje de: %d, Jugador 2", players[turn].score);
                }
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", message, window);

                // Terminar el juego
                if((turn == 0 && num_players == 1)||turn == 1){
                    running = false;
                }else{
                    turn = 1;
                    // Actualización del texto del jugador que esta jugando
                    sprintf(turnText, "2");
                    turnSurface = TTF_RenderText_Blended(font, turnText, textColor);
                    turnTexture = SDL_CreateTextureFromSurface(renderer, turnSurface);
                    turnWidth = turnSurface->w;
                    turnHeight = turnSurface->h;
                    SDL_FreeSurface(turnSurface);
                }
                
            }
        }

        //Se encargar de revisar la victoria de Donkey Kong Jr.
        if(dk_y >= 150 && dk_y <= 260 && dk_x >= 150 && dk_x <= 360) {
            // Actualizar puntaje
            players[turn].score += 1000;

            //Reproducir sonido de victoria
            Mix_PauseMusic();
            Mix_PlayChannel(-1, win_sound, 0);

            // Mostrar sprite de victoria durante un segundo
            SDL_RenderCopy(renderer, background_texture, NULL, NULL);
            int sprite_x = 35;
            int sprite_y = 1;
            SDL_Rect srcrect = {sprite_x, sprite_y, 32, 16};
            SDL_Rect dstrect = {dk_x -72, dk_y -36,115.2 ,57.6};
            SDL_RenderCopy(renderer, spritesheet_texture, &srcrect, &dstrect);
            SDL_RenderPresent(renderer);
            SDL_Delay(500);
            SDL_RenderCopy(renderer, background_texture, NULL, NULL);

            sprite_x = 375;
            srcrect.x = sprite_x;
            SDL_RenderCopy(renderer, spritesheet_texture, &srcrect, &dstrect);
            SDL_RenderPresent(renderer);
            SDL_Delay(500);

            // Reiniciar variables del juego
            dk_x = 50;
            dk_y = 848;
            dk_frame = 0;
            dk_facing_right = true;
            dk_moving = false;
            dk_jumping = false;
            dk_jump_start_y = 0;
            //LLamado a generar frutas
            generate_fruits();

            // Mostrar ventana emergente con el mensaje "Victoria"
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Victoria", "Felicidades, sigue jugando por más puntos", window);
            Mix_ResumeMusic();
        }

        // Volver a renderizar texto del puntaje
            sprintf(scoreText, "%d", players[turn].score);
            scoreSurface = TTF_RenderText_Blended(font, scoreText, textColor);
            scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            scoreWidth = scoreSurface->w;
            scoreHeight = scoreSurface->h;
            SDL_FreeSurface(scoreSurface);
        
        // Actualizar animación de Donkey Kong Jr.
        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_frame_time >= frame_duration) {
            dk_frame = (dk_frame +1) %3;
            last_frame_time = current_time;
        }
        
        // Actualizar animación de los cocodrilos
        for (int i = 0; i < 5; i++) {
            if (crocodiles[i].active) {
                crocodiles[i].frame = (crocodiles[i].frame + 1) % 2;
            }
        }

        // Presentar en pantalla
        SDL_RenderPresent(renderer);

       //Agregar retraso
        SDL_Delay(2);
    }

    //Liberar musica
    Mix_FreeMusic(music);
    Mix_FreeChunk(win_sound);
    Mix_FreeChunk(lose_sound);
    Mix_FreeChunk(bite_sound);

    Mix_Quit();

    // Cerrar joystick
    SDL_JoystickClose(joystick);


    // Destruir ventana y cerrar SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
