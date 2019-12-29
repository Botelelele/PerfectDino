#pragma once
#include "global.h"
class Dino;
using MFP = void (Dino::*)();
class Node
{
public:
	float activation;
	MFP pointer;
	Node();
	~Node();
	void call(float c);
};
class Dino
{
	Uint32 first_frame;
	int counterC;
	bool jump_possible;
	int counter;
	int killer;
public:
	Node reactions[5];
	float time;
	float y_vel;
	bool duck;
	int score;
	bool dead;
	bool init;
	
	SDL_FRect Dino_pos;
	SDL_Texture* texture;
	Dino();
	~Dino()
	{
		texture = nullptr;
	}
	void big_jump();
	void small_jump();
	void crouch();
	void bias();
	void handle_event(const Uint32& frame);
	void render(SDL_Renderer* render);
	void initialize(const Uint32& fram);
	void kill(int type);
	void change_animation();
	void callAI(int type,Uint32 y, Uint32 dist);
	void seed();
	int& expose_killer() { return killer; }
	void update();
};

