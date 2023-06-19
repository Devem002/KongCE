// Cabezera encargada de contener las constantes de DonCEyKongJrClient.c

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

// Variables para la animación de Donkey Kong Jr.
int dk_frame = 0;
bool dk_facing_right = true;
bool dk_moving = false;
bool dk_flip = false;
bool dk_init = true;

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

//Estructura encargada de las frutas
typedef struct {
    int x;
    int y;
    bool active;
} Fruit;

Fruit fruits[5];

// Generar frutas de manera aleatoria en las coordenadas de las lianas
void generate_fruits(){
    for (int i = 0; i < 5; i++) {
        int liana_index = rand() % (sizeof(lianas) / sizeof(lianas[0]));
        fruits[i].x = lianas[liana_index].x1;
        fruits[i].y = rand() % (lianas[liana_index].y1 - lianas[liana_index].y2 + 1) + lianas[liana_index].y2;
        fruits[i].active = true;
    }
}

// Variables para controlar el tiempo entre frames de animación
Uint32 last_frame_time = 0;
Uint32 frame_duration = 100;

//Estructura encargada de los cocodrilos
typedef struct {
    int x;
    int y;
    int liana_index;
    bool active;
    bool moving_up;
    int frame;
} Crocodile;

Crocodile crocodiles[5];

//Función encargada de crear los cocodrilos de forma aleatoria en las lianas
void generate_cocodriles(){
    for (int i = 0; i < 5; i++) {
        crocodiles[i].liana_index = rand() % (sizeof(lianas) / sizeof(lianas[0]) - 3);
        crocodiles[i].x = lianas[crocodiles[i].liana_index].x1;
        crocodiles[i].y = rand() % (lianas[crocodiles[i].liana_index].y1 - lianas[crocodiles[i].liana_index].y2 + 1) + lianas[crocodiles[i].liana_index].y2;
        crocodiles[i].active = true;
        crocodiles[i].moving_up = true;
        crocodiles[i].frame = 0;
    }
}

// Variable para el estado de daño de Donkey Kong Jr.
bool damage = false;