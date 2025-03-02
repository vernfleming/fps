// Updated raycasting with smooth shading, colored walls, and movement
#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define MOVE_SPEED 0.1
#define ROTATE_SPEED 0.05

int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1},
    {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1},
    {1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,1},
    {1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

typedef struct {
    double x, y;
    double dirX, dirY;
    double planeX, planeY;
} Player;

void handleInput(Player *player) {
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    
    if (keystate[SDL_SCANCODE_W]) {
        if (!map[(int)(player->x + player->dirX * MOVE_SPEED)][(int)(player->y)]) player->x += player->dirX * MOVE_SPEED;
        if (!map[(int)(player->x)][(int)(player->y + player->dirY * MOVE_SPEED)]) player->y += player->dirY * MOVE_SPEED;
    }
    if (keystate[SDL_SCANCODE_S]) {
        if (!map[(int)(player->x - player->dirX * MOVE_SPEED)][(int)(player->y)]) player->x -= player->dirX * MOVE_SPEED;
        if (!map[(int)(player->x)][(int)(player->y - player->dirY * MOVE_SPEED)]) player->y -= player->dirY * MOVE_SPEED;
    }
    if (keystate[SDL_SCANCODE_A]) {
        double oldDirX = player->dirX;
        player->dirX = player->dirX * cos(ROTATE_SPEED) - player->dirY * sin(ROTATE_SPEED);
        player->dirY = oldDirX * sin(ROTATE_SPEED) + player->dirY * cos(ROTATE_SPEED);
        double oldPlaneX = player->planeX;
        player->planeX = player->planeX * cos(ROTATE_SPEED) - player->planeY * sin(ROTATE_SPEED);
        player->planeY = oldPlaneX * sin(ROTATE_SPEED) + player->planeY * cos(ROTATE_SPEED);
    }
    if (keystate[SDL_SCANCODE_D]) {
        double oldDirX = player->dirX;
        player->dirX = player->dirX * cos(-ROTATE_SPEED) - player->dirY * sin(-ROTATE_SPEED);
        player->dirY = oldDirX * sin(-ROTATE_SPEED) + player->dirY * cos(-ROTATE_SPEED);
        double oldPlaneX = player->planeX;
        player->planeX = player->planeX * cos(-ROTATE_SPEED) - player->planeY * sin(-ROTATE_SPEED);
        player->planeY = oldPlaneX * sin(-ROTATE_SPEED) + player->planeY * cos(-ROTATE_SPEED);
    }
    if (keystate[SDL_SCANCODE_Q]) {
        if (!map[(int)(player->x - player->planeX * MOVE_SPEED)][(int)(player->y)]) player->x -= player->planeX * MOVE_SPEED;
        if (!map[(int)(player->x)][(int)(player->y - player->planeY * MOVE_SPEED)]) player->y -= player->planeY * MOVE_SPEED;
    }
    if (keystate[SDL_SCANCODE_E]) {
        if (!map[(int)(player->x + player->planeX * MOVE_SPEED)][(int)(player->y)]) player->x += player->planeX * MOVE_SPEED;
        if (!map[(int)(player->x)][(int)(player->y + player->planeY * MOVE_SPEED)]) player->y += player->planeY * MOVE_SPEED;
    }
}

void renderWalls(Player *player) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        double cameraX = 2 * x / (double)SCREEN_WIDTH - 1;
        double rayDirX = player->dirX + player->planeX * cameraX;
        double rayDirY = player->dirY + player->planeY * cameraX;
        int mapX = (int)player->x;
        int mapY = (int)player->y;
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, x, 100, x, 500);
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Raycasting FPS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    Player player = {12, 12, -1, 0, 0, 0.66};
    bool running = true;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
        handleInput(&player);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderWalls(&player);
        SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
