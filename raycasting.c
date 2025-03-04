#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 1000
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define FOV 3.14159 / 3.0  // 60-degree field of view
#define MAX_DEPTH 16

// 2D Map (1 = wall, 0 = empty space)
int map[MAP_WIDTH][MAP_HEIGHT] = {
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},  
    {1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1},  
    {1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1},  
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},  
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},  
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},  
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}  
};

// Player position and direction
float playerX = 3.0, playerY = 3.0;
float playerAngle = 0.0;

// Initialize SDL
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Initialize SDL
int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Simple FPS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
if (!window) {
  printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
  return 0;
 }

renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
if (!renderer) {
  printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
  return 0;
 }

return 1;
}

// Raycasting function
void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  for (int x = 0; x < SCREEN_WIDTH; x++) {
    float rayAngle = (playerAngle - FOV / 2.0) + ((float)x / SCREEN_WIDTH) * FOV;
    float rayX = cos(rayAngle);
    float rayY = sin(rayAngle);

    float distance = 0;
    int hitWall = 0;

    while (!hitWall && distance < MAX_DEPTH) {
      int testX = (int)(playerX + rayX * distance);
      int testY = (int)(playerY + rayY * distance);

      if (testX < 0 || testX >= MAP_WIDTH || testY < 0 || testY >= MAP_HEIGHT) {
	hitWall = 1;
	distance = MAX_DEPTH;
      } else if (map[testX][testY] == 1) {
	hitWall = 1;
      } else {
	distance += 0.1;
      }
    }

    int lineHeight = (int)(SCREEN_HEIGHT / (distance + 0.1));
    int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawStart < 0) drawStart = 0;
    int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
    if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

    SDL_SetRenderDrawColor(renderer, 255 - (distance * 15), 255 - (distance * 15), 255 - (distance * 15), 255);
    SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
  }

  SDL_RenderPresent(renderer);
}

// Handle player movement
void movePlayer(float moveX, float moveY) {
  if (map[(int)(playerX + moveX)][(int)(playerY)] == 0) playerX += moveX;
  if (map[(int)(playerX)][(int)(playerY + moveY)] == 0) playerY += moveY;
}

// Main game loop
void gameLoop() {
  int running = 1;
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) running = 0;
    }

    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_W]) movePlayer(cos(playerAngle) * 0.1, sin(playerAngle) * 0.1);
    if (keystate[SDL_SCANCODE_S]) movePlayer(-cos(playerAngle) * 0.1, -sin(playerAngle) * 0.1);
    if (keystate[SDL_SCANCODE_A]) playerAngle -= 0.05;
    if (keystate[SDL_SCANCODE_D]) playerAngle += 0.05;

    render();
    SDL_Delay(16);
  }
}

// Clean up SDL
void cleanup() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

// Main function
int main() {
  if (!initSDL()) return 1;

  gameLoop();
  cleanup();

  return 0;
}
