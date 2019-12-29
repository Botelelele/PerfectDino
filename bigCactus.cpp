#include "global.h"
#include "bigCactus.h"
#include "Dino.h"
namespace enemies
{
	bigCactus::bigCactus() :type(KILLER_BCACTUS), pos(), visible(0), sprite(nullptr),ID(fps.framecount)
	{
		pos = { (float)((SCREEN_WIDTH * 1.2) + 100.0),(float)(SCREEN_HEIGHT * 0.625) + (float)(SCREEN_HEIGHT / 8.0) - (float)(SCREEN_HEIGHT / 5.0),(float)(SCREEN_WIDTH / 12.0),(float)(SCREEN_HEIGHT / 5.0) };
	}
	void bigCactus::handleAnimation()
	{
		if (!visible)
		{
			if (pos.x > -100.0 && pos.x < SCREEN_WIDTH + obstacle_velocity * -3.0)
			{
				visible = 1;
				sprite = ::bigCactus;
			}
		}
		move();
	}
	bool bigCactus::collide(const Dino& d)
	{
		return collision(pos, d.Dino_pos);
	}
	bool bigCactus::useless()
	{
		return (pos.x < -100);
	}
	void bigCactus::move()
	{
		pos.x += obstacle_velocity;
	}
	void bigCactus::render()
	{
		if (visible)
		{
			SDL_SetRenderDrawColor(::render, 0, 0, 0, 0xff);
			SDL_RenderDrawRectF(::render, &pos);
			SDL_RenderCopyF(::render, sprite, nullptr, &pos);
		}
	}
}