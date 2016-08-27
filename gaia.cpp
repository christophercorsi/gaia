#include <cstdio>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>

#include <SDL.h>

#include "simulation.h"
#include "utilities.h"

Random random_generator;

// One discrete time step is a Metric Month (a tenth of a year).

u64 person_counter = 0;
Person* newGenesisPerson(World& world) {
  const float width = world.getWidth(), height = world.getHeight();
  const bool is_male = random_generator.uniform_u32(0, 1) == 0;

  auto* person = new Person {
    .self_it = std::list<Person>::iterator(),
    .id = person_counter++,
    .age = 0,
    .lifetime = random_generator.uniform_u32(10_years, 90_years),
    .is_male = is_male,
    .father = 0,
    .mother = 0,
    .x = random_generator.uniform_i32(0,width-1),
    .y = random_generator.uniform_i32(0,height-1),
    .food_stock = random_generator.uniform_f32(4, 3_years)
  };

  return person;
}

int main(int argc, char **argv) {
  printf("Gaia!\n");

  int width = 256, height = 256;
  auto world = World(width, height);

  const auto n_starting_people = 1000000u;
  auto simulation = Simulation(n_starting_people, [&world]() -> Person*{
    return newGenesisPerson(world);
  }, world);

  ///////////
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
  	log("SDL_Init Error: %s", SDL_GetError())
  	return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Gaia", 100, 100, 1024, 1024, SDL_WINDOW_SHOWN);
  if (window == nullptr){
  	log("SDL_CreateWindow Error: %s", SDL_GetError());
  	SDL_Quit();
  	return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr){
  	SDL_DestroyWindow(window);
  	log("SDL_CreateRenderer Error: %s", SDL_GetError());
  	SDL_Quit();
  	return 1;
  }

  bool is_running = true;
  while(is_running) {

    // Advance simulation
    simulation.step();

    // Title update (TODO : IMGUI)
    {
      static u64 ticks = 0;

      u64 now = SDL_GetTicks();
      u64 delta = now - ticks;
      ticks = now;

      std::stringstream title;
      title << "Gaia - [Time step " << simulation.getTimeStep() << "] ";
      title << "[" << (u64)(simulation.getPeople().size() / delta) << " agent updates/ms] ";
      title << "[" << simulation.getPeople().size() << " agents remain]";
      SDL_SetWindowTitle(window, title.str().c_str());
    }

    // Even handling
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch(e.type){
        case SDL_QUIT:
          is_running = false;
          break;
        case SDL_KEYDOWN:
          if(e.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
          break;
        default: break;
      }
    }

    // Drawing
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear(renderer);

    const World& world(simulation.getWorld());
    for(int j=0; j<height; ++j)
      for(int i=0; i<width; ++i) {

        float food = world.get_food_read()(i,j) * 0.001f;
        float population = logf(1.f + world.get_population()(i,j)) * 0.6f;

        const u8 food_color = (u8)(std::max(0.f, std::min(1.f, food)) * 255);
        const u8 population_color = (u8)(std::max(0.f, std::min(1.f, population)) * 255);
        SDL_SetRenderDrawColor(renderer, food_color, population_color, 0, 255);

        const int scale = 4;
        SDL_Rect rect {scale * i + 2, scale * j + 2, scale - 2, scale - 2};
        SDL_RenderFillRect(renderer, &rect);
      }

    SDL_RenderPresent(renderer);
    //SDL_Delay(250);
  }


  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
