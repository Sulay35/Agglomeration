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
 * - Vector.distance(v1, v2)
 * - Prog Dyn for distances (2D table with distances)
 * - desired velocity for inertia
*/

class Vector2{
	public:
		Vector2()
			: x(0.0f), y(0.0f)
		{}
		Vector2(float x, float y)
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
		Vector2 operator*(float rhs)
		{
			return Vector2(x*rhs, y*rhs);
		}	

		Vector2 operator/(float rhs)
		{
			return Vector2(x/rhs, y/rhs);
		}	

		Vector2 operator*=(float rhs){
			return Vector2(x*rhs, y*rhs);
		}

		Vector2 operator/=(float rhs){
			return Vector2(x/rhs, y/rhs);
		}

		Vector2 normalized(){
			Vector2 normalized = (*this)/norm();
			// printf("\tf_normalized = %f \n",normalized.norm());
			return normalized;

		}

		void setMag(float magnitude){
			if(magnitude!=0.0f)
				*this =  this->normalized() * magnitude;
			//printf("\t%f\tf = %f \n",magnitude, this->norm());
		}

		void limit(float limit){
			if(this->norm() > limit){
				setMag(limit);
			}
		}
		float norm(){
			return sqrt(x*x + y*y);
		}

		void add(Vector2 *v2){
			x += v2->x;
			y += v2->y;
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

		Cell(Vector2 pos, Vector2 vel, int st, int d) : pos(pos), vel(vel), state(st) 
		{}

		Cell(Vector2 pos) : pos(pos)
		{
			vel = Vector2(-1 + rand()%3, -1 +rand()%3);
			//vel = Vector2();
			state = (int)rand()%2;
		}

		Cell(){
			pos.x = 0.0f;
			pos.y = 0.0f;
			vel = Vector2(-1 + rand()%3, -1 +rand()%3);
			state = (int)rand()%2;
		}

		void Update(float dt){
			vel.add(&acc);
			acc.limit(maxSpeed);
			vel.limit(maxSpeed);
			//printf("v = %f \n", acc.norm());
			pos.add(&vel);
		}
		
		void attract(Cell * object){
			Vector2 r; 
			//r.x = object->pos.x - pos.x;
			//r.y = object->pos.y - pos.y;
			r.x = pos.x - object->pos.x;
			r.y = pos.y - object->pos.y;

			float d = r.norm();
			float G = 1.0f;
			float f;
			

			d*=d;
			d = d > 50.0f ? 50.0f : d;
			d = d < 5.0f ? 5.0f : d;

			Vector2 force; 
			force = r.normalized();

			// printf("f = %f \n", force.norm());
			f = G/d;
			// printf("d = %f, norm = %f, G/d =%f \n", d, r.norm(),f);
			force.setMag(G/d);

			// printf("\tf = %f \n", force.norm());

			object->applyForce(&force);
		}

		void applyForce(Vector2 *force){
			//printf("\t\tf = %f \n", force->norm());
			acc.add(force);
			//printf("\ta = %f \n", acc.norm());
		}

		Vector2 destination;

		Vector2 pos;
		Vector2 vel;
		Vector2 acc;

		float maxSpeed = 2.0f;

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
					printf("Everything initialized!\n");
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

	Vector2 mousePos;
	int population = 10000;

	std::vector<Cell> people; 

	Cell c;
	for(int j = 0; j < population; j++){
		c =  Cell(Vector2(
			(int) rand()%SCREEN_WIDTH,
			(int) rand()%SCREEN_WIDTH)
		);
		// A random neighbor
		people.push_back(c);
	}
	Cell cCenter = Cell(Vector2(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f));
	
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
					if(e.key.keysym.sym == SDLK_ESCAPE){
						running = false;
					}
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

			// INPUT
			int a,b;
			SDL_GetMouseState(&a, &b);
			mousePos.x = a;
			mousePos.y = b;

			if(buttons[Buttons::SPACE]){

				for(int j = 0; j < population; j++){
					people.clear();
				}
				for(int j = 0; j < population; j++){
					c =  Cell(Vector2(
						(int) rand()%SCREEN_WIDTH,
						(int) rand()%SCREEN_WIDTH)
					);
					// A random neighbor
					people.push_back(c);
				}
				}

			// neighbors 
			/*
			float d;
			for(int i = 0; i < population; i++){
				Vector2 v1 = people[i].pos;
				int jMin = 0;
				float dMin = MAXFLOAT;
				for(int j = 0; j < population; j++){
					Vector2 v2 = people[j].pos;
					if(i!=j){
						d = sqrt((v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y));
						if(d<dMin){
							jMin = j;
							dMin = d;
						}
					}
				}
				//people[i].destination = people[jMin].pos;
				people[i].destination = cCenter.pos;
			}
			for(int i = 0; i < population; i++){
			for(int j = 0; j < population; j++){
					if(i!=j)
					people[i].attract(people[j]);
				}
			}
			*/

			cCenter.pos = mousePos;

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x0F);
			SDL_RenderClear(renderer);

			SDL_Rect fRect;
			// Draw the grid
			for(int i = 0; i < population; i++){
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
				cCenter.attract(&people[i]);
				people[i].Update(dt);
				fRect.x = people[i].pos.x;
				fRect.y = people[i].pos.y;

				fRect.w = 5;
				fRect.h = 5;
				//SDL_RenderDrawRect(renderer, &fRect);

				SDL_RenderDrawPointF(renderer, people[i].pos.x, people[i].pos.y);
				//SDL_RenderDrawPointF(renderer, people[i].pos.x, people[i].pos.y);
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
