
#include<stdio.h>           // Standard input output library
                            
#include <time.h>           // For initialization for randum number generator

#include <SDL2/SDL.h>       // SDL2 for graphical window


#define CAM_WIDTH 1280      
#define CAM_HEIGHT 840   

#define ROWS 1920
#define COLUMNS 1080

#define GRID_SIZE ROWS*COLUMNS

#define NUM_BUFFERS 2


typedef struct Pixel
{
    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char A;
} Pixel;

char buf_index;
void *bufferGrid[2];
int pitch;

SDL_Window *gameWindow;
SDL_Renderer *gameRenderer;
SDL_Texture *gameTexture;


float Rand01()
{
    return (float)(rand()%1000)/1000;
}

void GridUpdate(Pixel* tempGrid, Pixel* lastGrid, double deltaTime)
{
    // Update Grid Here
    for (int i = 0; i < GRID_SIZE; i++)
    {
        Pixel* pix = &tempGrid[i];
        Pixel* lastPix = &lastGrid[i];

        char alive = rand()%2;

        pix->R = alive*255;
        pix->G = alive*255;
        pix->B = alive*255;
    }
}

int Update(double deltaTime)
{

    // Locking Texture to current buffer
    if (SDL_LockTexture(gameTexture, NULL, &bufferGrid[0], &pitch) < 0)
    {
        printf("Could not lock texture: %s\n", SDL_GetError());
        return -1;
    }

    Pixel* lastGrid = (Pixel*)bufferGrid[1]; 

    Pixel* grid = (Pixel*)bufferGrid[0];

    GridUpdate(grid, lastGrid, deltaTime);

    memcpy(lastGrid, grid, GRID_SIZE*sizeof(Pixel));

    // Unlocking Texture

    SDL_UnlockTexture(gameTexture);

    return 0;
}

void Draw()
{
    // Renders texture to screen
    SDL_RenderCopy(gameRenderer, gameTexture, NULL, NULL);
    SDL_RenderPresent(gameRenderer);

    // Change Buffer
}

int InitSDL()
{ 
    // Initialize window
    SDL_Init(SDL_INIT_VIDEO);

    // Creating window
    gameWindow = SDL_CreateWindow("Window",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  CAM_WIDTH,
                                  CAM_HEIGHT,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (gameWindow == NULL) 
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return -1;
    }

    // Creating renderer window
    gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);

    if (gameRenderer == NULL)
    {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return -1;
    }

    // Creating streaming texture
    gameTexture = SDL_CreateTexture(gameRenderer, 
                                    SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    ROWS,
                                    COLUMNS); 
    
    if (gameTexture == NULL)
    {
        printf("Could not create texture: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

int GameWindow()
{
    // Initialize random number generator
    srand(time(NULL));

    for (int i = 0; i < NUM_BUFFERS; i++)
        bufferGrid[i] = (int*)calloc(GRID_SIZE, sizeof(Pixel));

    if (InitSDL() < 0)
        return -1;

    // General Event Structure
    SDL_Event e;            
                            
    clock_t time1 = clock();
    clock_t time2 = clock();

    double deltaTime = 0;
    double fps = 0;
    char quit = 0;

    // Event loop
    while (quit == 0){
        time2 = clock();

        deltaTime = (double)(time2-time1)/CLOCKS_PER_SEC;
        fps = 1/deltaTime;

        // Wait before start
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    quit = 1;
                    break;

                case SDL_KEYDOWN:
                    // Quitting if Esc is pressed
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                        quit = 1;
                    break;
            }
        }

        // Updating Grid
        if (Update(deltaTime) < 0)
            return -1;
        
        // Drawing Grid
        Draw();

//        printf("FPS:%f\r", fps);
//        fflush(stdout);

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
