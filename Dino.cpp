#include "global.h"
#include "Dino.h"
Node::Node():activation(15.0-(rand()%50-25.0)/10.0)
{
	pointer = &Dino::bias;
}
Node::~Node()
{
	pointer = nullptr;
}
Dino::Dino():killer(-1),time(0.0),y_vel(0.0),first_frame(0), jump_possible(1), counter(0), duck(0), score(0), dead(0), init(0), counterC(0)
{
	Dino_pos = { (float)(SCREEN_WIDTH * 0.125),(float)(SCREEN_HEIGHT * 0.625),66.7,100.0 };
	texture = nullptr;
}        
void Dino::big_jump()
{
	jump_possible = 0;
	Dino_pos.y -= 30;
	counter = 8;
}
void Dino::small_jump()
{
	jump_possible = 0;
	Dino_pos.y -= 20;
	counter = 6;
}
void Dino::crouch()
{
	jump_possible = 0;
	Dino_pos.h = Dino_pos.h / 2;
	Dino_pos.y = Dino_pos.y + Dino_pos.h;
	duck = 1;
	counterC = 20;
}
void Dino::bias()
{

}
void Dino::handle_event(const Uint32& frame)
{
	if (!dead)
	{
		if (Dino_pos.y + Dino_pos.h >= (SCREEN_HEIGHT*0.75))
		{
			y_vel = 0;
			time = 0;
			if (!duck)
			{
				jump_possible = 1;
			}
			Dino_pos.y = (SCREEN_HEIGHT * 0.75) - Dino_pos.h;
		}
		else
		{
			if (counter!=0)
			{
				time = 0.05;
				Dino_pos.y = std::min(Dino_pos.y + gravity, (float)(SCREEN_HEIGHT * 0.75) - Dino_pos.h);
			}
			else
			{
				y_vel = gravity*time;
				Dino_pos.y = std::min(Dino_pos.y + y_vel, (float)(SCREEN_HEIGHT * 0.75) - Dino_pos.h);
				time += 0.12;
			}
		}
		if (counter > 0)
		{
			counter--;
			Dino_pos.y -= 33;
		}
		if (counterC > 0)
		{
			duck = 1;
			counterC--;
			if (counterC==0)
			{
				duck = 0;
				Dino_pos.h = Dino_pos.h * 2;
				Dino_pos.y = (SCREEN_HEIGHT * 0.75) - Dino_pos.h;
				this->texture = dino1;
			}
		}
		score = frame - first_frame;
	}
	change_animation();
}
void Dino::render(SDL_Renderer* render)
{
	if (!dead&&texture!=nullptr)
	{
		SDL_SetRenderDrawColor(render, 0, 0, 0, 0xff);
		SDL_RenderDrawRectF(render, &Dino_pos);
		SDL_RenderCopyF(render, texture, nullptr, &Dino_pos);
	}
}
void Dino::initialize(const Uint32& fram)
{
	if (!init)
	{
		init = 1;
		first_frame = fram;
		texture = dino1;
	}
}
void Dino::kill(int type)
{
	this->dead = 1;
	this->killer = type;
}
void Dino::change_animation()
{
	if (jump_possible)
	{
		if (texture == dino1)
		{
			texture = dino2;
		}
		else
		{
			texture = dino1;
		}
	}
	else
	{
		if(duck)
		{
			if (texture == dinoCrouching1)
			{
				texture = dinoCrouching2;
			}
			else
			{
				texture = dinoCrouching1;
			}
		}
		else
		{
			texture = dinoJumping;
		}
	}
}
void Dino::callAI(int type, Uint32 y, Uint32 dist)
{
	float effective_distance = -1.0*dist / obstacle_velocity;
	if (this->jump_possible)
	{
		switch (type)
		{
		case KILLER_BIRD_TOP:
		{
			if (reactions[0].activation > effective_distance)
			{
				(*this.*(reactions[0].pointer))();
			}
			break;
		}
		case KILLER_BIRD_MID:
		{
			if (reactions[1].activation > effective_distance)
			{
				(*this.*(reactions[1].pointer))();
			}
			break;
		}
		case KILLER_BIRD_BOT:
		{
			if (reactions[2].activation > effective_distance)
			{
				(*this.*(reactions[2].pointer))();
			}
			break;
		}
		case KILLER_BIRD:
		{
			if (y == (int)(SCREEN_HEIGHT * 0.47))
			{
				if (reactions[0].activation > effective_distance)
				{
					(*this.*(reactions[0].pointer))();
				}
			}
			else if(y== (int)(SCREEN_HEIGHT * 0.5625))
			{
				if (reactions[1].activation > effective_distance)
				{
					(*this.*(reactions[1].pointer))();
				}
			}
			else
			{
				if (reactions[2].activation > effective_distance)
				{
					(*this.*(reactions[2].pointer))();
				}
			}
			break;
		}
		case KILLER_BCACTUS:
		{
			if (reactions[3].activation > effective_distance)
			{
				(*this.*(reactions[3].pointer))();
			}
			break;
		}
		case KILLER_SCACTUS:
		{
			if (reactions[4].activation > effective_distance)
			{
				(*this.*(reactions[4].pointer))();
			}
			break;
		}
		}
	}
}
void Dino::seed()
{
	for (size_t i = 0; i < 5; i++)
	{
		this->reactions[i] = frankenstein.reactions[i];
	}
	int random_int = rand() % 101;
	if (random_int >= 100)//mutates 5 times
	{
		mutate(this,0);
		mutate(this,1);
		mutate(this,2);
		mutate(this,3);
		mutate(this,4);
	}
	else if (random_int >= 98)//mutates 4 times
	{
		int a = rand() % 5;
		for (int i = 0; i < 5; i++)
		{
			if (i != a)
			{
				mutate(this, i);
			}
		}
	}
	else if (random_int >= 88)//mutates 3 times
	{
		int a = rand() % 5;
		int b(a);
		while (b == a)
		{
			b = rand() % 5;
		}
		for (int i = 0; i < 5; i++)
		{
			if (i != a&&i!=b)
			{
				mutate(this, i);
			}
		}
	}
	else if (random_int > 53)//mutates twice
	{
		int a = rand() % 5;
		int b(a), c(a);
		while (b == a)
		{
			b = rand() % 5;
		}
		while (c == a)
		{
			c = rand() % 5;
		}
		for (int i = 0; i < 5; i++)
		{
			if (i != a && i != b&&i!=c)
			{
				mutate(this, i);
			}
		}
	}
	else if(random_int>5)//mutates once
	{
		mutate(this);
	}
}
void Dino::update()
{
	Dino_pos = { (float)(SCREEN_WIDTH * 0.125),(float)(SCREEN_HEIGHT * 0.625),66.7,100.0};
}