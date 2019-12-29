#include "global.h"
#include "Bird.h"
#include "Dino.h"
namespace enemies
{
	Bird::Bird():type(KILLER_BIRD), pos(), visible(0), sprite(nullptr),ID(fps.framecount)
	{
		pos = { (float)((SCREEN_WIDTH * 1.2) + 100.0),500.0,40.0,80.0 };
		switch (fps.framecount % 3)
		{
		case 0:
		{
			pos.y = (float)(SCREEN_HEIGHT * 0.47);
			type = KILLER_BIRD_TOP;
			break;
		}
		case 1:
		{
			pos.y = (float)(SCREEN_HEIGHT * 0.55);
			type = KILLER_BIRD_MID;
			break;
		}
		case 2:
		{
			pos.y = (float)(SCREEN_HEIGHT * 0.63);
			type = KILLER_BIRD_BOT;
		}
		}
	}
	void Bird::handleAnimation()
	{
		if (visible)
		{
			if (sprite == bird1)
			{
				sprite = ::bird2;
			}
			else
			{
				sprite = ::bird1;
			}
		}
		else
		{
			if (pos.x > -100 && pos.x < SCREEN_WIDTH + obstacle_velocity * -3)
			{
				visible = 1;
				sprite = ::bird1;
			}
		}
		move();
	}
	bool Bird::collide(const Dino& d)
	{
		return collision(pos, d.Dino_pos);
	}
	bool Bird::useless()
	{
		return (pos.x < -100);
	}
	void Bird::move()
	{
		pos.x += obstacle_velocity;
	}
	void Bird::render()
	{
		if (visible)
		{
			SDL_SetRenderDrawColor(::render, 0, 0, 0, 0xff);
			SDL_RenderDrawRectF(::render, &pos);
			SDL_RenderCopyF(::render, sprite, nullptr, &pos);
		}
	}
}