#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <windows.h>
#include "Entity.hpp"
#include "renderwindow.hpp"
#include <vector>

#include <chrono>
#include <thread>

std::vector<std::pair<int,int>> randomSetup(int n);//defined at the bottom

const int arrlen1 = (int) 32;
const int arrlen2 = (int) 32;

const int SCREEN_WIDTH = arrlen1*32;
const int SCREEN_HEIGHT = arrlen2*32;

int main(int argc, char *argv[]){

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		printf("SDL couldn't initialise. ERROR: %s\n", SDL_GetError());
	}

    if(!IMG_Init(IMG_INIT_PNG)){
		printf("SDL_image couldn't initialise. ERROR: %s\n", SDL_GetError());
	}

    renderwindow window("game", SCREEN_WIDTH, SCREEN_HEIGHT);

    //loading textures
    SDL_Texture* organism = window.LoadTextureMe("..\\assets\\organism.png");//the asset doesn't load unless you run the binary from with the bin file open. Running bin\conway.exe command in the terminal gives asset could not be loaded error.
	
	//defines starting state

	//std::vector <std::pair <int,int>> setup = {{3,3},{4,3},{5,4},{4,4}}; used this to test before implementing random startup
	std::vector <std::pair <int,int>> setup = randomSetup(arrlen1*arrlen2*0.5);

	//initialising game map using the setup vector
	Entity maparr[arrlen1][arrlen2];
	for (int i = 0; i < arrlen1; i++){
			for (int j = 0; j < arrlen2; j++){
        		maparr[i][j] = Entity(i*32, j*32, organism,false);//this is for only on and off of only one entity.	
				for (int k = 0; k < setup.size(); k++){
					if (i == setup[k].first && j == setup[k].second){
						maparr[i][j].exist = 1;
					}
				}
			}
		}

	//variables for game loop
    bool running = true;
	SDL_Event event;
	//pause event
	SDL_Event temp;
	int i1,j1 = 0;
	//game loop
	while(running){
		int status = 0;

		SDL_PollEvent(&event);
		if(event.type == SDL_KEYDOWN){
			if (event.key.keysym.sym == SDLK_c){
				while (event.key.keysym.sym == SDLK_c){
					SDL_PollEvent(&temp);
					if (temp.type == SDL_KEYUP && temp.key.keysym.sym == SDLK_c){
						break;
					}
				}
			}	
		}

		if(event.type == SDL_QUIT){
			window.fillcolour(255,0,0,255);
            window.renderclear();
            window.renderdisplay();
			
			SDL_Delay(100);
			
			running = false;
			break;
		}
        window.renderclear();
		for (int i = 0; i < arrlen1; i++){
			for (int j = 0; j < arrlen2; j++){
				//neighbour check (checks the spaces in the array around (x,y))
				maparr[i][j].neighbourcount = 0;
  				for(int x = i-1; x < i+2; x++){
     				for(int y = j-1; y < j+2; y++){
		        		if(x>=0 && y>=0 && x<arrlen1 && y<arrlen2){
                			if(maparr[x][y].exist){maparr[i][j].neighbourcount++;}
           				}
        			}
   				 }
				//makes sure the element itself isn't part of the neighbourcount
    			maparr[i][j].neighbourcount = maparr[i][j].neighbourcount - (int) maparr[i][j].exist;


				//render all existing blobs
				if (maparr[i][j].exist == 1){
					window.render(maparr[i][j]);
					status++;
				}
			}
		}
		
		//update with game logic
		for (int i = 0; i < arrlen1; i++){
			for (int j = 0; j < arrlen2; j++){
				maparr[i][j].update();
			}
		}

		//per frame stuff
        window.renderdisplay();

		//slows down the framerate to make it look nicer
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

    window.cleanup();
	SDL_Quit();

	return 0;
}

//randomises the starting system state
std::vector<std::pair<int,int>> randomSetup(int n){
	std::vector<std::pair<int,int>> result;
	for (int i = 0; i < n; i++){
		std::pair <int,int> temp = {rand() % arrlen1 , rand() % arrlen2};
		result.push_back(temp);
	}
	return result;
}