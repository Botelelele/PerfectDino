#include "global.h"
#include "Obstacle.h"


Uint16 SCREEN_WIDTH=800;
Uint16 SCREEN_HEIGHT=800;

float obstacle_velocity = -20.0;

SDL_Window* window = nullptr;
SDL_Renderer* render = nullptr;

FPSmanager fps;

TTF_Font* font = nullptr;

SDL_Texture* bird1 = nullptr;
SDL_Texture* bird2 = nullptr;
SDL_Texture* dino1 = nullptr;
SDL_Texture* dino2 = nullptr;
SDL_Texture* dinoJumping = nullptr;
SDL_Texture* dinoCrouching1 = nullptr;
SDL_Texture* dinoCrouching2 = nullptr;
SDL_Texture* smallCactus = nullptr;
SDL_Texture* bigCactus = nullptr;

SDL_Texture* score = nullptr;
SDL_Texture* generation=nullptr;

std::vector<enemies::Obstacle*>obstacles;
std::vector<Dino*>players;

Dino frankenstein;
int scoreObst[5];


void init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	
	srand(SDL_GetTicks());

	window = SDL_CreateWindow("Perfect Dinosaur", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE

	);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	font = TTF_OpenFont("data/arial.ttf", 150);

	bird1 = loadTexture("data/berd.png");
	bird2 = loadTexture("data/berd2.png");
	dino1 = loadTexture("data/dinorun1.png");
	dino2 = loadTexture("data/dinorun2.png");
	dinoJumping = loadTexture("data/dinoJump.png");
	dinoCrouching1 = loadTexture("data/dinoDuck1.png");
	dinoCrouching2 = loadTexture("data/dinoDuck2.png");
	smallCactus = loadTexture("data/cactusSmall.png");
	bigCactus = loadTexture("data/cactusBig.png");


	SDL_initFramerate(&fps);
	fps.rate = 60;

	scoreObst[0] = -99999;
	scoreObst[1] = -99999;
	scoreObst[2] = -99999;
	scoreObst[3] = -99999;
	scoreObst[4] = -99999;
}

void free()
{
	freeTexture(score);
	freeTexture(generation);
	freeTexture(bird1);
	freeTexture(bird2);
	freeTexture(dino1);
	freeTexture(dino2);
	freeTexture(dinoJumping);
	freeTexture(dinoCrouching1);
	freeTexture(dinoCrouching2);
	freeTexture(smallCactus);
	freeTexture(bigCactus);

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	if (!obstacles.empty())
	{
		for (size_t i = 0; i < obstacles.size(); i++)
		{
			if (obstacles[i] != nullptr)
			{
				delete (obstacles[i]);
			}
		}
	}
	if (!players.empty())
	{
		for (size_t i = 0; i < players.size(); i++)
		{
			if (players[i]!=nullptr)
			{
				delete (players[i]);
			}
		}
	}
}

void freeTexture(SDL_Texture* text)
{
	if (text != nullptr)
	{
		SDL_DestroyTexture(text);
		text = nullptr;
	}
}

Uint32 max(Uint32 a, Uint32 b)
{
	return (a > b) ? a : b;
}

void create_generation(std::vector<Dino*>&players)
{
	
	if (!players.empty())
	{
		for (size_t i = 0; i < players.size(); i++)
		{
			if (players[i] != nullptr)
			{
				delete (players[i]);
			}
		}
		players.erase(players.begin(), players.end());
	}
	for (int i = 0; i < population_size-1; i++)
	{
		players.push_back(nullptr);
		players.back() = new Dino;
		players.back()->initialize(fps.framecount);
		players.back()->seed();
	}
	players.push_back(nullptr);
	players.back() = new Dino;
	*players.back() = frankenstein;
	players.back()->initialize(fps.framecount);
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(render, loadedSurface);
		if (newTexture == nullptr)
		{
			SDL_Log("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

SDL_Texture* renderText(std::string text, Uint8 r, Uint8 g, Uint8 b)
{
	if (font != nullptr)
	{
		SDL_Color color = { r,g,b };
		color.a = 0xff;
		SDL_Surface* s = TTF_RenderText_Blended(font, text.c_str(), color);
		SDL_Texture* result = SDL_CreateTextureFromSurface(render, s);
		SDL_FreeSurface(s);
		return result;
	}
	return nullptr;
}

Dino* fittest(std::vector<Dino*>& players)
{
	int score = players[0]->score;
	size_t it = 0;
	for (size_t i = 1; i < players.size(); i++)
	{
		if (players[i]->score > score)
		{
			score = players[i]->score;
			it = i;
		}
	}
	return players[it];
}

bool alive(const std::vector<Dino*>& players)
{
	for (size_t i = 0; i < players.size(); i++)
	{
		if (!players[i]->dead)
		{
			return 1;
		}
	}
	return 0;
}

void captivate_gene(const Dino* donor, int type)
{
	frankenstein.reactions[type] = donor->reactions[type];
}

void mutate(Dino* object, int type)
{
	if (rand() % 100 > 50)
	{
		MFP old = object->reactions[type].pointer;
		while (object->reactions[type].pointer == old)
		{
			switch (rand() % 4)
			{
			case 0:
			{
				object->reactions[type].pointer = &Dino::bias;
				break;
			}
			case 1:
			{
				object->reactions[type].pointer = &Dino::small_jump;
				break;
			}
			case 2:
			{
				object->reactions[type].pointer = &Dino::big_jump;
				break;
			}
			case 3:
			{
				object->reactions[type].pointer = &Dino::crouch;
				break;
			}
			}
		}
	}
	else
	{
		object->reactions[type].activation = std::max(5.0, object->reactions[type].activation + (rand() % 101) / 50.0 - 10.0);
	}
}

bool collision(const SDL_FRect& a, const SDL_FRect& b)
{
	if (a.y + a.h <= b.y)
	{
		return false;
	}
	if (a.y >= b.y + b.h)
	{
		return false;
	}
	if (a.x + a.w <= b.x)
	{
		return false;
	}
	if (a.x >= b.x + b.w)
	{
		return false;
	}
	return true;
}