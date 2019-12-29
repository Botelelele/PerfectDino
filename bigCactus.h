#pragma once
#include "global.h"
#include "Obstacle.h"
namespace enemies
{
	class bigCactus : public Obstacle
	{
	public:
		Uint32 ID;
		int type;
		SDL_FRect pos;
		bool visible;
		SDL_Texture* sprite;
		bigCactus();
		virtual ~bigCactus() {}
		virtual void handleAnimation();
		virtual bool collide(const Dino& d);
		virtual bool useless();
		virtual void move();
		virtual void render();
		virtual float& returnX() { return pos.x; }
		virtual float& returnY() { return pos.y; }
		virtual float& returnW() { return pos.w; }
		virtual float& returnH() { return pos.h; }
		virtual int& returnType() { return type; }
		virtual Uint32& returnID() { return ID; }
	};
}

