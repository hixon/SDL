/*
Logic behind getting the blit info is the following:
we need to keep an arry of all characters that are typed and displayed to the screen.

foreach value we need to go 16 pixels to the right (for one line) to print the next character

we also need to get the correct image from the file based on 

NEXT STEPS
- keep track of entered characters in an array (2d)
- see how many values will fit per row
- work on wrapping to next line
*/

#include <stdio.h>
#include <SDL.h>

bool init();

bool loadMedia();

void close();

int SCREEN_WIDTH = 512;
int SCREEN_HEIGHT = 512;

SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gHelloWorld = NULL;

SDL_Renderer* renderer = NULL;

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL not loaded: %s", SDL_GetError());
		success = false;
	}
	else {
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window not created: %s", SDL_GetError());
			success = false;
		}
		else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	return success;
}

bool loadMedia()
{
	bool success = true;

	gHelloWorld = SDL_LoadBMP("Font.bmp");

	if (gHelloWorld == NULL) {
		printf("did not load bmp file: %s", SDL_GetError());
		success = false;
	}	

	return success; 
}

void close()
{
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

int wmain(int argc, char* argv[])
{
	if (!init()) {
		printf("cannot init");
	}
	else {
		if (!loadMedia()) {
			printf("cannot load media");
		}
		else {
			bool quit = false;

			SDL_Event event;
			int index = -16;
			bool lastCharBackSpace = false;

			uint32_t rowNumber = 0;
			uint32_t colNumber = 0;

			while (!quit) {
				SDL_Rect charToWrite;

				while (SDL_PollEvent(&event) != 0) {
					if (event.type == SDL_QUIT) {
						quit = true;
					}

					else if (event.key.keysym.scancode >= 4 && event.key.keysym.scancode <= 39 && event.type == SDL_KEYDOWN) {
						/*
						a-z = 4 - 29 which maps to 16-
						*/
						
						charToWrite.x = ((event.key.keysym.scancode - 3) * 16);
						charToWrite.y = 0;
						charToWrite.w = 16;
						charToWrite.h = 16;													

 						if (!lastCharBackSpace) {
							index += 16;								

							if (colNumber < 31) {
								++colNumber;
							}
							else {
								colNumber = 0;
								++rowNumber;
								index = 0;
							}
						}	
						else {
							lastCharBackSpace = false;
						}						
					}
					
					else if (event.key.keysym.scancode == 44 && event.type == SDL_KEYDOWN) {
						charToWrite.x = 0;
						charToWrite.y = 0;
						charToWrite.w = 16;
						charToWrite.h = 16;

						if (!lastCharBackSpace) {
							index += 16;							

							if (colNumber < 31) {
								++colNumber;
							}
							else {
								colNumber = 0;
								++rowNumber;
								index = 0;
							}
						}
						else {
							lastCharBackSpace = false;
						}						
					}

					else if (event.type = SDLK_BACKSPACE && event.type == SDL_KEYDOWN) {
						charToWrite.x = 0;
						charToWrite.y = 0;
						charToWrite.w = 16;
						charToWrite.h = 16;

						if (lastCharBackSpace) {
							index -= 16;							

							if (colNumber > 0) {
								--colNumber;
							}
							else {
								colNumber = 31;
								index = 31 * 16;
								--rowNumber;								
							}
						}
						else{
							lastCharBackSpace = true;
						}										
					}
				}					

				SDL_Rect output;
				output.x = index;
				output.y = rowNumber*16;
				output.h = 16;
				output.w = 16;

				if (SDL_BlitSurface(gHelloWorld, &charToWrite, gScreenSurface, &output) == 0) {
					SDL_UpdateWindowSurface(gWindow);					
				}							

			}						
		}
	}

	close();

	return (0);
}