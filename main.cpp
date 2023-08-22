#include <SDL2/SDL.h>
#include <random>
#include <iostream>
#include <stdio.h>


#define WIDTH 800
#define HEIGHT 600
#define SQUARE_SIZE 10

using namespace std;


class Serpiente {

    public:
        int len;
        int* x;
        int* y;
        int color[4];
    
    Serpiente(int l) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution_x(10, WIDTH);
        int initial_x = distribution_x(gen);
        std::uniform_int_distribution<> distribution_y(10, HEIGHT);
        int initial_y = distribution_y(gen);
        len = l;
        x = new int[l];
        y = new int[l];
        for (int i = 0; i < l; i++) {
            x[i] = initial_x + i * SQUARE_SIZE;
            y[i] = initial_y;
        }
        

        std::uniform_int_distribution<> distribution_color(0, 255);
        std::uniform_int_distribution<> distribution_intensidad(0, 1);

        color[0] = distribution_color(gen);
        color[1] = distribution_color(gen);
        color[2] = distribution_color(gen);
        color[3] = distribution_intensidad(gen);
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
            x[0] = WIDTH - x[0];
        }
        if (y[0] < 0) {
            y[0] = HEIGHT - y[0];
        }
    }

    
};


int main(int argc, char* argv[]) {
    int n = 10;
    int l = 4;
    if (argc == 3) {
        n = strtol(argv[1], NULL, 10);
        l = strtol(argv[2], NULL, 10);
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool quit = false;
    SDL_Event e;

    SDL_Rect squareRect = { 100, 100, 100, 100 };
    int velocity = 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(1, 4);

    Serpiente* serpientes[n];
    for (int i = 0; i < n; i++) {
        Serpiente* serpiente = new Serpiente(l);
        serpientes[i] = serpiente;
    }

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < n; i++) {
            Serpiente* serpiente = serpientes[i];
            for (int j = 0; j < l; j++) {
                int x = serpiente->x[j];
                int y = serpiente->y[j];
                SDL_Rect rect1 = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                SDL_RenderFillRect(renderer, &rect1);
             // Draw the square
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


            serpiente->movimiento(distribution(gen));
        }


        // Update the screen
        SDL_RenderPresent(renderer);

        // Delay for smooth animation
        SDL_Delay(40);

        // Reset the drawing color to white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}