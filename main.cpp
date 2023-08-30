#include <SDL2/SDL.h>
#include <random>
#include <iostream>
#include <stdio.h>
#include <vector>


#define WIDTH 400
#define HEIGHT 400
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
        int initial_x = distribution_x(gen) * SQUARE_SIZE;
        std::uniform_int_distribution<> distribution_y(10, HEIGHT);
        int initial_y = distribution_y(gen) * SQUARE_SIZE;
        len = l;
        x = new int[l];
        y = new int[l];
        std::uniform_int_distribution<> distribution_pos(0, 1);
        int latest = 0;
        cout << initial_x % WIDTH << endl;
        cout << initial_y % HEIGHT << endl;
        for (int i = 0; i < l; i++) {
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
            x[0] = WIDTH - x[0];
        }
        if (y[0] < 0) {
            y[0] = HEIGHT - y[0];
        }
    }

    
};

int spawn_snakes(Serpiente* serpientes[], int x, int l, int n) {
    int cont = 0;
    for (int i = x; i < x + 5 && i < n; i++) {
        Serpiente* serpiente = new Serpiente(l);
        serpientes[i] = serpiente;
        cont += 1;
    }

    return cont;
}


int main(int argc, char* argv[]) {
    int n = 10;
    int l = 4;
    bool valid = false;
    if (argc == 3) {
        n = strtol(argv[1], NULL, 10);
        l = strtol(argv[2], NULL, 10);

        if (n > 0 && l > 0) {
            valid = true;
        }
    }
    
    int canvas[WIDTH/SQUARE_SIZE][HEIGHT/SQUARE_SIZE];
    for (int i = 0; i < WIDTH/SQUARE_SIZE; i++) {
        for (int j = 0; j < HEIGHT/SQUARE_SIZE; j++){
            canvas[i][j] = -1;
        }
    }

    
    if (valid)
    {
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
        
        Serpiente* serpientes[n];
        // for (int i = 0; i < n; i++) {
        //     Serpiente* serpiente = new Serpiente(l);
        //     serpientes[i] = serpiente;
        //     cout << "creada" << endl;
        // }

        int cont = 0;
        int current = 0;

        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            if (cont % 1000 == 0 && current < n) {
                current += spawn_snakes(serpientes, current, l, n);
            }

            // Clear the renderer
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            for (int i = 0; i < current; i++) {
                Serpiente* serpiente = serpientes[i];
                int len_serpiente = serpiente->len;
                for (int j = 0; j < len_serpiente; j++) {

                    int valid_len = serpientes[i]->len;
                    if (j >= valid_len) {
                        continue;
                    }
                    

                    int x = serpiente->x[j];
                    int y = serpiente->y[j];     
                    if (j==0 && canvas[y/SQUARE_SIZE][x/SQUARE_SIZE] != i && canvas[y/SQUARE_SIZE][x/SQUARE_SIZE] != -1) {
                        cout << "valid_len " << valid_len << endl;
                        cout << "colision " << i << endl;
                        int x_eliminada = serpiente->x[valid_len-1];
                        int y_eliminada = serpiente->y[valid_len-1];
                        canvas[y_eliminada/SQUARE_SIZE][x_eliminada/SQUARE_SIZE] = -1;
                        serpiente->colision();
                        SDL_Rect rect_elim = {x_eliminada, y_eliminada, SQUARE_SIZE, SQUARE_SIZE};
                        SDL_RenderFillRect(renderer, &rect_elim);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                        continue;
                        
                    }
                    else
                    {
                        canvas[y/SQUARE_SIZE][x/SQUARE_SIZE] = i;
                        int x_cola = serpiente->x[valid_len-1];
                        int y_cola = serpiente->y[valid_len-1];
                        canvas[y_cola/SQUARE_SIZE][x_cola/SQUARE_SIZE] = -1; 


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
                }


                serpiente->movimiento(distribution(gen));
            }


            // Update the screen
            SDL_RenderPresent(renderer);

            // Delay for smooth animation
            SDL_Delay(65);

            // Reset the drawing color to white
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    else
    {
        cout << "Revise que esta ingresando correctamente los parámetros" << endl;
    }
    return 0;
}