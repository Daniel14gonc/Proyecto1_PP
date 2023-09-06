/*
* Integrantes:
*   - Sebastian Aristondo 20880
*   - Daniel Gonzalez 20293
*
* Descripcion: Programa secuencial para mostrar un fonde de pantalla
*   con tematica basada en el juego de Snake. 
*/


#include <SDL2/SDL.h>
#include <random>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <chrono>


#define WIDTH 1000
#define HEIGHT 1000
#define SQUARE_SIZE 10
using namespace std;



class Serpiente {

    public:
        int len;
        int* x;
        int* y;
        int color[4];
    
    Serpiente(int largo_serpiente) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution_x(10, WIDTH);
        int initial_x = distribution_x(gen) * SQUARE_SIZE;
        std::uniform_int_distribution<> distribution_y(10, HEIGHT);
        int initial_y = distribution_y(gen) * SQUARE_SIZE;
        len = largo_serpiente;
        x = new int[largo_serpiente];
        y = new int[largo_serpiente];
        std::uniform_int_distribution<> distribution_pos(0, 1);
        int latest = 0;
        for (int i = 0; i < largo_serpiente; i++) {
            x[i] = initial_x % WIDTH;
            y[i] = initial_y % HEIGHT;
            
        }
        

        std::uniform_int_distribution<> distribution_color(0, 255);
        std::uniform_int_distribution<> distribution_intensidad(0, 1);

        color[0] = distribution_color(gen);
        color[1] = distribution_color(gen);
        color[2] = distribution_color(gen);
        color[3] = distribution_intensidad(gen);
    }
    void colision(){
        len -=1;
    }

    void movimiento(int mov){

        if (mov == 1 && x[0]+SQUARE_SIZE != x[1]){// Movimiento a la derecha
            for (int i = len-1; i > 0; i--)
            {
                x[i] = x[i-1];
                y[i] = y[i-1];
            }
            
            x[0] = (x[0] + SQUARE_SIZE)%WIDTH;
        }
        if (mov == 2 && x[0]-SQUARE_SIZE != x[1]){// Movimiento a la izquierda
            for (int i = len-1; i > 0; i--)
            {
                x[i] = x[i-1];
                y[i] = y[i-1];
            }
            x[0] = (x[0] - SQUARE_SIZE)%WIDTH;
        }
        if (mov == 3 && y[0]+SQUARE_SIZE != y[1]){// Movimiento hacia abajo
            for (int i = len-1; i > 0; i--)
            {
                x[i] = x[i-1];
                y[i] = y[i-1];
            }
            y[0] = (y[0] + SQUARE_SIZE)%HEIGHT;

        }

        if (mov == 4 && y[0]-SQUARE_SIZE != y[1]) { // Movimiento hacia arriba
            for (int i = len-1; i > 0; i--)
            {
                x[i] = x[i-1];
                y[i] = y[i-1];
            }
            y[0] = (y[0] - SQUARE_SIZE)%HEIGHT;
        }

        if (x[0] < 0) {
            x[0] = WIDTH - SQUARE_SIZE;
        }
        if (y[0] < 0) {
            y[0] = HEIGHT - SQUARE_SIZE;
        }
    }

    
};
// Generacion de serpientes 
int spawn_snakes(Serpiente* serpientes[], int x, int largo_serpiente, int cantidad_serpientes) {
    int cont = 0;
    for (int i = x; i < x + 5 && i < cantidad_serpientes; i++) {
        Serpiente* serpiente = new Serpiente(largo_serpiente);
        serpientes[i] = serpiente;
        cont += 1;
    }

    return cont;
}


int main(int argc, char* argv[]) {
    double FRAMES_PROMEDIO = 0.0;
    int cantidad_serpientes = 10;
    int largo_serpiente = 4;
    bool valid = false;

    // Validacion y almacenamiento de argumentos de linea de comandos para cantidad de serpientes
    // y su largo.
    if (argc == 3) {
        cantidad_serpientes = strtol(argv[1], NULL, 10);
        largo_serpiente = strtol(argv[2], NULL, 10);

        if (cantidad_serpientes > 0 && largo_serpiente > 0) {
            valid = true;
        }
    }
    
    
    auto inicio = std::chrono::high_resolution_clock::now();

    // Creacion de canvas para almacenar las posiciones de las serpientes.
    int canvas[WIDTH/SQUARE_SIZE][HEIGHT/SQUARE_SIZE];
    for (int i = 0; i < WIDTH/SQUARE_SIZE; i++) {
        for (int j = 0; j < HEIGHT/SQUARE_SIZE; j++){
            canvas[i][j] = -1;
        }
    }

    
    if (valid)
    {
        // Inicializacion de SDL para mostrar las serpientes.
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Window* window = SDL_CreateWindow("SDL Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        bool quit = false;
        SDL_Event e;

        // SDL_Rect squareRect = { 100, 100, 100, 100 };
        // int velocity = 1;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(1, 4);
        
        Serpiente* serpientes[cantidad_serpientes];

        int cont = 0;
        int current = 0;

        int frames = 0;
        Uint32 startTime = SDL_GetTicks();

        // Main loop
        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            // En cada iteracion del main loop se crean nuevas serpientes.
            if (cont % 1000 == 0 && current < cantidad_serpientes) {
                current += spawn_snakes(serpientes, current, largo_serpiente, cantidad_serpientes);
            }

            // Si la cantidad de serpientes llega a su limite, se acaba el programa.
            if (current == cantidad_serpientes) {
                quit = true;
            }

            // Limpiar renderer
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            // Ciclo que evalua colisiones entre serpientes.
            for (int i = 0; i < current; i++) {
                Serpiente* serpiente = serpientes[i];
                int len_serpiente = serpiente->len;
                // Se realiza un ciclo por cada elemento del cuerpo de la serpiente. Se evalua si la cabeza
                // tiene una colision con el cuerpo de alguna otra y se actualiza la posicion de la serpiente
                // en el canvas, ademas de renderizar.
                for (int j = 0; j < len_serpiente; j++) {

                    int valid_len = serpientes[i]->len;
                    // En caso ya no se deba renderiza porque el largo es menor a j.
                    if (j >= valid_len) {
                        continue;
                    }
                    

                    int x = serpiente->x[j];
                    int y = serpiente->y[j];     

                    // Verificacion de colision de serpiente.
                    if (j==0 && canvas[y/SQUARE_SIZE][x/SQUARE_SIZE] != i && canvas[y/SQUARE_SIZE][x/SQUARE_SIZE] != -1) {
                        // Se obtiene la cola de la serpiente, que es lo que se debe eliminar.
                        int x_eliminada = serpiente->x[valid_len-1];
                        int y_eliminada = serpiente->y[valid_len-1];
                        canvas[y_eliminada/SQUARE_SIZE][x_eliminada/SQUARE_SIZE] = -1;
                        // Se le indica a la serpiente que tuvo una colision.
                        serpiente->colision();
                        SDL_Rect rect_elim = {x_eliminada, y_eliminada, SQUARE_SIZE, SQUARE_SIZE};
                        SDL_RenderFillRect(renderer, &rect_elim);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        
                    }
                    else
                    {
                        // Se actualiza la posicion de la serpiente en canvas.
                        canvas[y/SQUARE_SIZE][x/SQUARE_SIZE] = i;
                        int x_cola = serpiente->x[valid_len-1];
                        int y_cola = serpiente->y[valid_len-1];
                        canvas[y_cola/SQUARE_SIZE][x_cola/SQUARE_SIZE] = -1; 


                        SDL_Rect rect1 = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                        SDL_RenderFillRect(renderer, &rect1);
                        // Se obtienen los colores de la serpiente para renderizar.
                        int r = serpiente->color[0];
                        int g = serpiente->color[1];
                        int b = serpiente->color[2];
                        int a = serpiente->color[3];
                        if(j!=0 and j%2==0)
                        {
                            r = r+25;
                            g = g-5;
                            b = b-5;
                        }
                        else if (j == 0) {
                            r = r+5;
                            g = g-5;
                            b = b-5;
                            a = a/2;
                        }
                        SDL_SetRenderDrawColor(renderer, r, g, b, a);
                    }
                }

                // Se le asigna un movimiento random a la serpiente.
                serpiente->movimiento(distribution(gen));
            }


            // Update the screen
            SDL_RenderPresent(renderer);

            // Delay for smooth animation
            SDL_Delay(20);

            // Reset the drawing color to white
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

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

                if (0 == FRAMES_PROMEDIO) {
                    FRAMES_PROMEDIO = fps;
                }
                else
                {
                    FRAMES_PROMEDIO = (FRAMES_PROMEDIO + fps) / 2.0;
                }
            }
        }

        for (int i = 0; i < cantidad_serpientes; i++) {
            delete serpientes[i];
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio);
        cout << "\nTiempo de ejecución: " << duracion.count()/1000.0 << " segundos" << endl;
        cout << "FPS promedio: " << FRAMES_PROMEDIO << endl;
    }
    else
    {
        cout << "Revise que esta ingresando correctamente los parámetros" << endl;
    }
    return 0;
}