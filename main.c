
#include<stdio.h>           // Standard input output library
#include <stdlib.h>         // Stadart Library
                            
#include <time.h>           // For initialization for randum number generator
#include <math.h>           // For mathematical functions

#include <SDL2/SDL.h>       // SDL2 for graphical window

#include <unistd.h>

#define MAX(x, y) ((x > y) ? x : y)
#define MIN(x, y) ((x < y) ? x : y)

// Window Values
#define CAM_WIDTH 1280      // Possible Values: 640, 1280
#define CAM_HEIGHT 840      // Possible Values: 320, 840

// Grid Values
#define RECT_WIDTH 8
#define RECT_HEIGHT 8

#define ROWS CAM_HEIGHT/RECT_WIDTH
#define COLUMNS CAM_WIDTH/RECT_WIDTH

#define GRID_SIZE ROWS*COLUMNS

#define BG_SHADE 0

#define NUM_THREADS 4

typedef struct Pixel
{
    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char A;
} Pixel;

char buf_index;
void *buffer_grid[2];

int pitch;

float ColorMask[3] = {0.2, 0.6, 0.9};

SDL_Window *gameWindow;
SDL_Renderer *gameRenderer;
SDL_Texture *gameTexture;

float Rand01()
{
    return (float)(rand()%1000)/1000;
}

void GridUpdate(Pixel* tempGrid, double deltaTime)
{
    // Updating every agent
    for (int i = 0; i < GRID_SIZE; i++)
    {
        Pixel* pix = &tempGrid[i];
        pix->R = 255;
        pix->G = 0;
        pix->B = 0;
    }
}

int Update(double deltaTime)
{
    if (SDL_LockTexture(gameTexture, NULL, &buffer_grid[buf_index], &pitch) < 0)
        return -1;

    Pixel* tempGrid = (Pixel*)buffer_grid[buf_index]; 

    GridUpdate(tempGrid, deltaTime);

    SDL_UnlockTexture(gameTexture);

    return 0;
}

void Draw()
{
    // Renders texture to screen
    SDL_RenderCopy(gameRenderer, gameTexture, NULL, NULL);
    SDL_RenderPresent(gameRenderer);
    
    // Change Buffer
    buf_index = !buf_index;
}


int GameWindow()
{
    // Initialize random number generator
    srand(time(NULL));

    // Initialize window
    SDL_Init(SDL_INIT_VIDEO);

    // Creating window
    gameWindow = SDL_CreateWindow("Window",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                CAM_WIDTH,
                                CAM_HEIGHT,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    // Checking if window is created
    if (gameWindow == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return -1;
    }

    // Creating renderer window
    gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);

    // Creating streaming texture
    gameTexture = SDL_CreateTexture(gameRenderer, 
                                    SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    CAM_WIDTH/RECT_WIDTH,
                                    CAM_HEIGHT/RECT_HEIGHT); 
    

    // General Event Structure
    SDL_Event e;            
                            
    char quit = 0;

    clock_t time1 = clock();
    clock_t time2 = clock();
    double deltaTime = 0;
    double fps = 0;

    float time = 0;

    // Event loop
    while (quit == 0){

        time2 = clock();
        deltaTime = (double)(time2-time1)/CLOCKS_PER_SEC;
        fps = 1/deltaTime;

        // Wait before start
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = 1;

            if (e.type == SDL_MOUSEBUTTONDOWN)
                quit = 1;
        }

        Update(deltaTime);
        
        Draw();

        printf("FPS:%f\r", fps);
        fflush(stdout);

        time1 = time2;
    }
    printf("\n");

    // Destroy texture
    SDL_DestroyTexture(gameTexture);

    // Destroying rederer
    SDL_DestroyRenderer(gameRenderer);

    // Destroying window
    SDL_DestroyWindow(gameWindow);

    // Quitting SDL ...
    SDL_Quit();

    return 0;
}

int main(void)
{
    int ExitCode = GameWindow();
    return ExitCode;
}
