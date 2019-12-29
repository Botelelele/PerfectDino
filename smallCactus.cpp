#include "global.h"
#include "smallCactus.h"
#include "Dino.h"
namespace enemies
{
	smallCactus::smallCactus() :type(KILLER_SCACTUS), pos(), visible(0), sprite(nullptr),ID(fps.framecount)
	{
		pos = { (float)((SCREEN_WIDTH * 1.2) + 100.0),(float)(SCREEN_HEIGHT * 0.625) + (float)(SCREEN_HEIGHT / 8.0)- (float)(SCREEN_HEIGHT / 9.0),(float)(SCREEN_WIDTH / 17.0),(float)(SCREEN_HEIGHT / 9.0) };
	}
	void smallCactus::handleAnimation()
	{
		if(!visible)
		{
			if (pos.x > -100.0 && pos.x < SCREEN_WIDTH + obstacle_velocity * -3)
			{
				visible = 1;
				sprite = ::smallCactus;
			}
		}
		move();
	}
	bool smallCactus::collide(const Dino& d)
	{
		return collision(pos, d.Dino_pos);
	}
	bool smallCactus::useless()
	{
		return (pos.x < -100);
	}
	void smallCactus::move()
	{
		pos.x += obstacle_velocity;
	}
	void smallCactus::render()
	{
		if (visible)
		{
			SDL_SetRenderDrawColor(::render, 0, 0, 0, 0xff);
			SDL_RenderDrawRectF(::render, &pos);
			SDL_RenderCopyF(::render, sprite, nullptr, &pos);
		}
	}
}