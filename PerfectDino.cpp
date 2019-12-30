#include "global.h"
#include "Dino.h"
#include "Bird.h"
#include "smallCactus.h"
#include "bigCactus.h"
int main(int argc, char* argv[])
{
	init();
	SDL_Event event;
	SDL_Rect score_pos = {10,10,100,100};
	SDL_Rect gen_pos = { (SCREEN_WIDTH*5/6),10,80,80 };
	const Uint8* keystate = nullptr;
	enemies::Obstacle* temp_ob=nullptr;
	enemies::Obstacle* nearest_obst=nullptr;
	//creating first generation of our useless Dinos
	for (int i = 0; i < population_size; i++)
	{
		players.push_back(nullptr);
		players.back() = new Dino;
		players.back()->initialize(fps.framecount);
		mutate(players.back());
	}
	bool leave = 0;
	bool clicked_U = 0;
	bool clicked_PLUS = 0;
	bool clicked_MINUS = 0;
	int gen_counter=1;
	Uint32 former_ID = 0;
	int former_type = 0;
	while (!leave)
	{
		{//events
			//SDL2 Events
			if (SDL_PollEvent(&event))
			{
				while (SDL_PollEvent(&event))
				{
					if (event.type == SDL_QUIT)
					{
						leave = 1;
					}
				}
			}

			obstacle_velocity = (float)std::max(-20-std::round(fittest(players)->score/150.0),-40.0);

			//getting keyboard states
			{
				keystate = SDL_GetKeyboardState(nullptr);

				if (keystate[SDL_SCANCODE_K] && !players.empty())
				{
					players.back()->kill(-1);
					delete players.back();
					players.erase(players.end());
				}
				else if (keystate[SDL_SCANCODE_U] && !clicked_U)
				{
					clicked_U = 1;
					//uber mode incoming
				}
				else if (keystate[SDL_SCANCODE_KP_PLUS] && !clicked_PLUS)//incresing FPS by 10
				{
					clicked_PLUS = 1;
					fps.rate += 10;
				}
				else if (keystate[SDL_SCANCODE_KP_MINUS] && !clicked_MINUS)//decreasing FPS by 10
				{
					clicked_MINUS = 1;
					fps.rate = max(30, fps.rate - 10);
				}
				else if (keystate[SDL_SCANCODE_ESCAPE])
				{
					leave = 1;
				}
				if (!keystate[SDL_SCANCODE_U] && clicked_U)
				{
					clicked_U = 0;
				}
				if (!keystate[SDL_SCANCODE_KP_PLUS] && clicked_PLUS)
				{
					clicked_PLUS = 0;
				}
				if (!keystate[SDL_SCANCODE_KP_MINUS] && clicked_MINUS)
				{
					clicked_MINUS = 0;
				}
			}

			//creating good for nothing obstacles
			{
				if (obstacles.empty() || obstacles.back()->visible)
				{
					switch (rand() % 6)
					{
					case 0:
					{
						obstacles.push_back(temp_ob);
						obstacles.back() = new enemies::smallCactus;
						break;
					}
					case 1:
					{
						obstacles.push_back(temp_ob);
						obstacles.back() = new enemies::Bird;
						break;
					}
					case 2:
					{
						obstacles.push_back(temp_ob);
						obstacles.back() = new enemies::bigCactus;
						break;
					}
					case 3:
					{
						obstacles.push_back(temp_ob);
						obstacles.back() = new enemies::Bird;
						break;
					}
					case 4:
					{
						obstacles.push_back(temp_ob);
						obstacles.back() = new enemies::bigCactus;
						break;
					}
					case 5:
					{
						obstacles.push_back(temp_ob);
						obstacles.back() = new enemies::smallCactus;
						break;
					}
					}
				}
			}

			float distance = 16000.0;

			//collsion detection
			{
				for (size_t i = 0; i < obstacles.size(); i++)
				{
					for (size_t j = 0; j < players.size(); j++)
					{
						if (!players[j]->dead && obstacles[i]->collide(*players[j]))
						{
							players[j]->kill(obstacles[i]->returnType());
						}
					}
				}
			}

			//precalculations for our AI
			{
				if (!::obstacles.empty())
				{
					distance = fittest(::players)->Dino_pos.x + fittest(::players)->Dino_pos.w;
					for (int w = 0; w < ::obstacles.size(); w++)
					{
						if (obstacles[w]->returnX()>(SCREEN_WIDTH * 0.125)+ (SCREEN_WIDTH /12.0))
						{
							nearest_obst = obstacles[w];
							distance = obstacles[w]->returnX() - (int)(SCREEN_WIDTH * 0.125) + (int)(SCREEN_WIDTH / 12.0);
							break;
						}
					}
					if (nearest_obst->returnID() != former_ID && alive(players))
					{
						if (scoreObst[former_type] < fittest(players)->score)
						{
							captivate_gene(fittest(players), former_type);
							scoreObst[former_type] = fittest(players)->score;
						}
					}
					former_ID = nearest_obst->returnID();
					former_type = nearest_obst->returnType();
				}
				else
				{
					nearest_obst = nullptr;
				}
			}

			//our stupid AI tries to control population full of Dinos
			{
				bool flag = 1;
				for (int i = 0; i < players.size(); i++)
				{
					if (!players[i]->dead)
					{
						flag = 0;
						if (nearest_obst!=nullptr)
						{
							players[i]->callAI(nearest_obst->returnType(), nearest_obst->returnY(), distance);
						}
						players[i]->handle_event(fps.framecount);
					}
				}
				//if all the dinoos are dead
				if (flag)
				{
					//time for the new generation
					::create_generation(players);
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
					obstacles.erase(obstacles.begin(), obstacles.end());
					gen_counter++;
				}
			}

			//getting rid of useless obstacles - already used ones
			{
				bool flag = 1;
				int q = 0;
				while (flag)
				{
					flag = 0;
					for (int i = q; i < obstacles.size(); i++)
					{
						if (::obstacles[i]->useless())
						{
							delete (obstacles[i]);
							obstacles.erase(obstacles.begin() + i);
							flag = 1;
							break;
						}
						q++;
					}
				}
			}

			//handling the animation of the obstacles
			for (int i = 0; i < obstacles.size(); i++)
			{
				::obstacles[i]->handleAnimation();
			}

		}

		{//rendering
			SDL_SetRenderDrawColor(render, 247, 0xff, 0xff, 0xff);
			SDL_RenderClear(render);
			SDL_SetRenderDrawColor(render, 0, 0, 0, 0xff);
			SDL_RenderDrawLineF(render, 0.0, SCREEN_HEIGHT*0.75, SCREEN_WIDTH, SCREEN_HEIGHT*0.75);
			for (size_t i = 0; i < players.size(); i++)
			{
				players[i]->render(render);
			}
			for (size_t i = 0; i < obstacles.size(); i++)
			{
				obstacles[i]->render();
			}
			if (!players.empty())
			{
				if (score != nullptr)
				{
					SDL_DestroyTexture(score);
				}
				if (generation != nullptr)
				{
					SDL_DestroyTexture(generation);
				}
				score = renderText(std::to_string(fittest(players)->score), 127, 127, 127);
				generation = renderText(std::to_string(gen_counter), 127, 127, 127);
				SDL_RenderCopy(render, score, nullptr, &score_pos);
				SDL_RenderCopy(render, generation, nullptr, &gen_pos);
			}
			SDL_RenderPresent(render);
		}
		//frame time managing
		SDL_framerateDelay(&fps);
	}
	free();
	return 0;
}