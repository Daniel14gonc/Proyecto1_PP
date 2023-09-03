#include <SDL2/SDL.h>

int main(int argc, char* args[]) {
    // Inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL no pudo inicializarse! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Crea una ventana
    SDL_Window* ventana = SDL_CreateWindow("Calculadora de FPS en SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (ventana == NULL) {
        printf("No se pudo crear la ventana! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Crea un renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("No se pudo crear el renderer! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // ...

    // Tu bucle principal del juego

    // Variables para calcular FPS
    int frames = 0;
    Uint32 startTime = SDL_GetTicks();

    bool quit = false;
    while (!quit) {
        // Procesa eventos
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Tu lógica de juego y renderizado aquí

        // Incrementa el contador de frames
        frames++;

        // Calcula el tiempo transcurrido
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - startTime;

        // Si ha pasado 1 segundo, calcula los FPS
        if (deltaTime >= 1000) {
            float fps = frames / (deltaTime / 1000.0f);
            printf("FPS: %.2f\n", fps);

            // Reinicia las variables
            startTime = currentTime;
            frames = 0;
        }
    }

    // Libera recursos y cierra SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return 0;
}
