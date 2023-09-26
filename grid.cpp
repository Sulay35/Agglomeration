#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <random>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

/**
 * TODO : 
 * - grid of pointer instead of object 
 * - Agglomerations
*/

class Vector2{
	public:
		Vector2()
			: x(0.0f), y(0.0f)
		{}
		Vector2(int x, int y)
			: x(x), y(y)
		{}

		Vector2 operator+(Vector2 const& rhs)
		{
			return Vector2(x+rhs.x, y+rhs.y);
		}	
		Vector2& operator +=(Vector2 const& rhs)
		{
			x+=rhs.x;
			y+=rhs.y;
			return *this;
		}
		Vector2 operator*(int rhs)
		{
			return Vector2(x*rhs, y*rhs);
		}	
		int x,y;
};

class Vector2f{
	public:
		Vector2f()
			: x(0.0f), y(0.0f)
		{}
		Vector2f(float x, float y)
			: x(x), y(y)
		{}

		Vector2f operator+(Vector2f const& rhs)
		{
			return Vector2f(x+rhs.x, y+rhs.y);
		}	
		Vector2f& operator +=(Vector2f const& rhs)
		{
			x+=rhs.x;
			y+=rhs.y;
			return *this;
		}
		Vector2f operator*(float rhs)
		{
			return Vector2f(x*rhs, y*rhs);
		}	
		float x,y;
};

enum Direction{
	N,
	NE,
	E,
	SE,
	S,
	SO,
	O,
	NO
};

class Cell{
	public:

		Cell(Vector2 pos, int st, int d) : pos(pos), state(st), dir(d)
		{}

		Cell(Vector2 pos) : pos(pos)
		{
			state = (int)rand()%2;
			dir = (int)rand()%8;
		}

		Cell(){
			pos.x = 0.0f;
			pos.y = 0.0f;
			state = (int)rand()%2;
			dir = (int)rand()%8;
		}

		void Update(){
			Vector2 vDir;
			if(dir == 0){
				vDir.x = 0;
				vDir.y = -1;
			}else if (dir == 1){
				vDir.x = 1;
				vDir.y = -1;
			}else if (dir == 2){
				vDir.x = 1;
				vDir.y = 0;
			}else if (dir == 3){
				vDir.x = 1;
				vDir.y = 1;
			}else if (dir == 4){
				vDir.x = 0;
				vDir.y = 1;
			}else if (dir == 5){
				vDir.x = -1;
				vDir.y = 1;
			}else if (dir == 6){
				vDir.x = -1;
				vDir.y = 0;
			}else if (dir == 7){
				vDir.x = -1;
				vDir.y = -1;
			}
			pos += vDir;
		}
		Vector2 pos;
		int dir; // direction [0-7]
		int state;
};

enum Buttons{
	SPACE
};

/**
 * Start up and initialize SDL
 *  
 * Initialize SDL its window, a renderer and a screen surface for it
 * 
 * @param gW a pointer to the Window 
 * @param gR a pointer to the Renderer 
 * @param gSS a pointer to the Screen Surface
 * 
 * @returns success true if has successfully been initialized, false otherwise
 * 
*/
bool init(SDL_Window* & gW, SDL_Renderer* & gR, SDL_Surface* & gSS);


/**
 * Frees media and shuts down SDL
 * 
 * @param gW a pointer to the Window 
 * @param gR a pointer to the Renderer 
 * 
*/
void close(SDL_Window* & gW, SDL_Renderer* & gR, TTF_Font * & font);

/**
 *  Loads a texture
 * 
 * @param gR a pointer to the Renderer 
 * @param path the path to the texture file
 * 
*/
SDL_Texture* loadTexture(SDL_Renderer* sR, std::string path);

bool init(SDL_Window* & gW, SDL_Renderer* & gR, SDL_Surface* & gSS)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gW = SDL_CreateWindow( "Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gW == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gR = SDL_CreateRenderer(gW, -1,SDL_RENDERER_ACCELERATED);
			if(gR == NULL){
				printf( "Renderer could not be initialized! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else{
				// init TTF 
				if(TTF_Init() < 0){
					printf( "TTF could not be initialized! TTF Error: %s\n", TTF_GetError());
					success = false;
				}else{
					printf("Everything initialized!");
					gSS = SDL_GetWindowSurface(gW);
				}
			}
		}
	}

	return success;
}

void close(SDL_Window* & gW, SDL_Renderer* & gR, TTF_Font* & font)
{
	SDL_DestroyRenderer(gR);
	SDL_DestroyWindow( gW );

	gW = NULL;
	gR = NULL;

	//Quit SDL subsystems
	TTF_CloseFont(font);
	font = NULL;
	
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(SDL_Renderer* sR, std::string path )
{
	//The final optimized image
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Convert surface to screen format
		newTexture = SDL_CreateTextureFromSurface( sR, loadedSurface);
		if( newTexture == NULL )
		{
			printf( "Unable to create texture %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}


int main( int argc, char* args[] )
{
	SDL_Window* window = NULL;
		
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;
	
	SDL_Texture* gTexture = NULL;
	SDL_Renderer* renderer = NULL;

	TTF_Font* mainFont = TTF_OpenFont("DejaVuSansMono.ttf", 36);

	float dt = 0.0f;
	bool buttons[4] = {};

	int population = 10000;

	std::vector<Cell> people; 

	Cell c;
	for(int j = 0; j < population; j++){
		c =  Cell(Vector2(
			(int) rand()%SCREEN_WIDTH,
			(int) rand()%SCREEN_WIDTH)
		);
		people.push_back(c);
	}

	//Start up SDL and create window
	if(!init(window, renderer, screenSurface))
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Main loop flag
		bool running = true;

		//Event handler
		SDL_Event e;

		//While application is running
		while(running)
		{
			auto t1 = std::chrono::high_resolution_clock::now();
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					running = false;
				}
				else if(e.type == SDL_KEYDOWN){
					if(e.key.keysym.sym == SDLK_SPACE){
						buttons[Buttons::SPACE] = true;
					}
				}
				else if(e.type == SDL_KEYUP){
					if(e.key.keysym.sym == SDLK_SPACE){
						buttons[Buttons::SPACE] = false;
					}
				}
			}
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			// Draw the grid
			for(int i = 0; i < population; i++){
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderDrawPoint(renderer, people[i].pos.x, people[i].pos.y);
			}
			SDL_RenderPresent(renderer);

			// compute the time delta  
			auto t2 = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::chrono::milliseconds::period>(t2 - t1).count();
		}
	}

	//Free resources and close SDL
	close(window, renderer, mainFont);

	return 0;
}
