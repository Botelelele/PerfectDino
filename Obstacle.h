#pragma once
#include "global.h"
#include "Dino.h"
extern class Dino;
namespace enemies
{
	class Obstacle
	{
	public:
		Uint32 ID;
		int type;
		SDL_FRect pos;
		bool visible;
		SDL_Texture* sprite;
		Obstacle():type(-1),pos(),visible(0),sprite(nullptr),ID(0) {}
		virtual ~Obstacle() {}
		virtual void handleAnimation() = 0;
		virtual bool collide(const Dino& d) = 0;
		virtual bool useless() = 0;
		virtual void move() = 0;
		virtual void render() = 0;
		virtual float& returnX() { return pos.x; }
		virtual float& returnY() { return pos.y; }
		virtual float& returnW() { return pos.w; }
		virtual float& returnH() { return pos.h; }
		virtual int& returnType() { return type; }
		virtual Uint32& returnID() { return ID; }
	};
}

