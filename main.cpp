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
        int x[4];
        int y[4];
    
    Serpiente() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution_x(10, WIDTH);
        int initial_x = distribution_x(gen);
        std::uniform_int_distribution<> distribution_y(10, HEIGHT);
        int initial_y = distribution_y(gen);
        x[0] = initial_x;
        x[1] = initial_x + SQUARE_SIZE;
        x[2] = initial_x + 2 * SQUARE_SIZE;
        x[3] = initial_x + 3 * SQUARE_SIZE;
        y[0] = initial_y;
        y[1] = initial_y;
        y[2] = initial_y;
        y[3] = initial_y;
    }

    void movimiento(int mov){

        if (mov == 1 and y[0]){// Movimiento a la derecha
            x[3] = x[2];
            y[3] = y[2];
            x[2] = x[1];
            y[2] = y[1];
            x[1] = x[0];
            y[1] = y[0];
            x[0] = x[0] + SQUARE_SIZE;
        }
        if (mov == 2){// Movimiento a la izquierda
            x[3] = x[2];
            y[3] = y[2];
            x[2] = x[1];
            y[2] = y[1];
            x[1] = x[0];
            y[1] = y[0];
            x[0] = x[0] - SQUARE_SIZE;
        }
        if (mov == 3){// Movimiento hacia abajo
            y[3] = y[2];
            x[3] = x[2];
            y[2] = y[1];
            x[2] = x[1];
            y[1] = y[0];
            x[1] = x[0];
            y[0] = y[0] + SQUARE_SIZE;

        }

        if (mov == 4) { // Movimiento hacia arriba
            y[3] = y[2];
            x[3] = x[2];
            y[2] = y[1];
            x[2] = x[1];
            y[1] = y[0];
            x[1] = x[0];
            y[0] = y[0] - SQUARE_SIZE;
        }
    }

    
};


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("SDL Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool quit = false;
    SDL_Event e;

    SDL_Rect squareRect = { 100, 100, 100, 100 };
    int velocity = 2;

    Serpiente serpiente;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Update position
        squareRect.y += velocity;

        // Draw the square
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &squareRect);

        // Update the screen
        SDL_RenderPresent(renderer);

        // Delay for smooth animation
        SDL_Delay(10);

        // Reset the drawing color to white
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}