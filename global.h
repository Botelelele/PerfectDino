#pragma once
#include <iostream>
#include <cstdlib>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2/SDL2_framerate.h>
#include <algorithm>
#include <string>
#include <vector>
#include "Obstacle.h"
namespace enemies {//
}

#define KILLER_BIRD_TOP 0
#define KILLER_BIRD_MID 1
#define KILLER_BIRD_BOT 2
#define KILLER_BCACTUS 3
#define KILLER_SCACTUS 4
#define KILLER_BIRD 5

const float gravity = 10.0;
const int population_size = 1000;

extern class Dino;
extern class enemies::Obstacle;

extern Uint16 SCREEN_WIDTH;
extern Uint16 SCREEN_HEIGHT;

extern float obstacle_velocity;

extern SDL_Window* window;
extern SDL_Renderer* render;
extern FPSmanager fps;

extern TTF_Font* font;
/*our sprites needed to make our "game" atleast a bit estetic*/
extern SDL_Texture* bird1;
extern SDL_Texture* bird2;

extern SDL_Texture* dino1;
extern SDL_Texture* dino2;
extern SDL_Texture* dinoJumping;
extern SDL_Texture* dinoCrouching1;
extern SDL_Texture* dinoCrouching2;

extern SDL_Texture* smallCactus;

extern SDL_Texture* bigCactus;
/*textures meant to contain text*/
extern SDL_Texture* score;
extern SDL_Texture* generation;

extern std::vector<enemies::Obstacle*>obstacles;
extern std::vector<Dino*>players;

extern Dino frankenstein;
extern int scoreObst[5];

SDL_Texture* loadTexture(std::string path);
SDL_Texture* renderText(std::string text, Uint8 r, Uint8 g, Uint8 b);

Dino* fittest(std::vector<Dino*>&players);

bool alive(const std::vector<Dino*>& players);//whether or not current generations is still alive

void captivate_gene(const Dino*donor,int type);//captivates gene and implant to our frankenstein

void mutate(Dino* object, int type = rand() % 5);//randomizes given parameter

bool collision(const SDL_FRect& a, const SDL_FRect& b);//whether or not two boxes collide with each other

void init();//initializes anything needed for this project

void free();//deallocates memory

void freeTexture(SDL_Texture* text);//deallocates memory of the SDL2 Texture

Uint32 max(Uint32 a, Uint32 b); //simillar to std::max

void create_generation(std::vector<Dino*>& players);//creates another generation of Dinos