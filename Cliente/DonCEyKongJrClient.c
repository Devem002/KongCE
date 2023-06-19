
// gcc -Ilibs/Include -Llibs/lib -o game DonCEyKongJrClient.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
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
    {0, 863, 274, 863},
    {188, 607, 387, 607},
    {389, 797, 507, 797},
    {186, 447, 322, 447},
    {548, 830, 636, 830},
    {676, 796, 796, 796},
    {838, 765, 956, 765},
    {764, 543, 1026, 543},
    {604, 286, 833, 286},
    {279, 253, 610, 253},
    {283, 158, 386, 158}
};

Line lianas[] = {
    {80, 799, 80, 288},
    {176, 799, 176, 288},
    {272, 798, 272, 640},
    {272, 606, 272, 480},
    {432, 750,432 ,288},
    {560 ,799 ,560 ,288},
    {656 ,799 ,656 ,320},
    {752 ,740 ,752 ,320},
    {848 ,702 ,848 ,577},
    {943 ,702 ,943 ,577},
    {848 ,543 ,848 ,192},
    {943 ,543 ,943 ,192},
    {431 ,191 ,432 ,100}
};

// Variables para la posición de Donkey Kong Jr.
int dk_x = 50;
int dk_y = 848;

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
int dk_jump_speed = 100;

// Función para detectar si Donkey Kong Jr. está en contacto con una plataforma
bool is_on_platform(int dk_x, int dk_y, Line platforms[], int num_platforms) {
    // Recorrer el array de plataformas
    for (int i = 0; i < num_platforms; i++) {
        // Verificar si la posición de Donkey Kong Jr. está dentro de los límites de la plataforma
        if (dk_x >= platforms[i].x1+10 && dk_x <= platforms[i].x2+10 && dk_y == platforms[i].y1) {
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
        // Verificar si la posición de Donkey Kong Jr. está cerca de la liana
        if (dk_x >= lianas[i].x1 - 15 && dk_x <= lianas[i].x1 + 25 && dk_y <= lianas[i].y1 && dk_y >= lianas[i].y2) {
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
    Mix_VolumeMusic(10);

    Mix_PlayMusic(music, -1);

    // Cargar efecto de sonido de victoria
    Mix_Chunk *win_sound = Mix_LoadWAV("Music/Win.mp3");
    if (win_sound == NULL) {
        printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
    // Reducir el volumen del efecto de sonido 
    Mix_VolumeChunk(win_sound, 10);
    // Cargar efecto de sonido de derrota
    Mix_Chunk *lose_sound = Mix_LoadWAV("Music/Lose.mp3");
    if (lose_sound == NULL) {
        printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
    // Reducir el volumen del efecto de sonido 
    Mix_VolumeChunk(lose_sound, 10);


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

    // Cargar fuente
    TTF_Font *font = TTF_OpenFont("Fonts/AGENCYB.TTF", 28);
    if (font == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(1);
    }

    // Establecer color del texto (blanco)
    SDL_Color textColor = {255, 255, 255};

    // Renderizar texto del puntaje
    char scoreText[64];
    sprintf(scoreText, "%d", score);
    SDL_Surface *scoreSurface = TTF_RenderText_Blended(font, scoreText, textColor);
    SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    int scoreWidth = scoreSurface->w;
    int scoreHeight = scoreSurface->h;
    SDL_FreeSurface(scoreSurface);

    // Renderizar texto de las vidas
    char lifeText[64];
    sprintf(lifeText, "%d", life);
    SDL_Surface *lifeSurface = TTF_RenderText_Blended(font, lifeText, textColor);
    SDL_Texture *lifeTexture = SDL_CreateTextureFromSurface(renderer, lifeSurface);
    int lifeWidth = lifeSurface->w;
    int lifeHeight = lifeSurface->h;
    SDL_FreeSurface(lifeSurface);


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
        if (state[SDL_SCANCODE_W]) {
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
        if (state[SDL_SCANCODE_A]) {
            dk_x -= 1;
            dk_facing_right = false;
            dk_moving = true;
        }
        if (state[SDL_SCANCODE_S]) {
            // Bajar liana si Donkey Kong Jr. está en contacto con una liana
            if (is_on_liana(dk_x, dk_y, lianas, sizeof(lianas) / sizeof(lianas[0]))) {
                dk_y += 2;
                dk_moving = true;
            }
        }
        if (state[SDL_SCANCODE_D]) {
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

        // Dibujar sprite de Donkey Kong Jr. si todavía tiene vida
        if (life > 0) {
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
        if(dk_y > 900) {
            // Restar 1 a la vida de Donkey Kong Jr.
            life -= 1;

            //Actualización del texto
            sprintf(lifeText, "%d", life);
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
            if (life <= 0) {
                // Mostrar ventana emergente con el mensaje "Game Over"
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Has perdido todas tus vidas", window);

                // Terminar el juego
                running = false;
            }
        }

        //Se encargar de revisar la victoria de Donkey Kong Jr.
        if(dk_y >= 150 && dk_y <= 260 && dk_x >= 150 && dk_x <= 360) {
            // Actualizar puntaje
            score += 100;

            // Volver a renderizar texto del puntaje
            sprintf(scoreText, "%d", score);
            scoreSurface = TTF_RenderText_Blended(font, scoreText, textColor);
            scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
            scoreWidth = scoreSurface->w;
            scoreHeight = scoreSurface->h;
            SDL_FreeSurface(scoreSurface);

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

            // Mostrar ventana emergente con el mensaje "Victoria"
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Victoria", "Felicidades sigue jugando por más puntos", window);
            Mix_ResumeMusic();
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

    //Liberar musica
    Mix_FreeMusic(music);
    Mix_FreeChunk(win_sound);
    Mix_FreeChunk(lose_sound);

    Mix_Quit();


    // Destruir ventana y cerrar SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
