#include <stdio.h>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <random>
#include "attraction.h"

# include "./PerlinNoise/PerlinNoise.hpp"

/**
 * TODO :
 * - grid of pointer instead of object
 * - Agglomerations
 * - Vector.distance(v1, v2)
 * - draw according to speed
 * dt
 * - use fouble instead of float
 * - constraint pos to windows size
 *
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

		Vector2 operator*(float rhs) const
		{
			return Vector2(x*rhs, y*rhs);
		}

		Vector2 operator/(float rhs) const
		{
			return Vector2(x/rhs, y/rhs);
		}

		Vector2 operator*=(float rhs) const
        {
			return Vector2(x*rhs, y*rhs);
		}

		Vector2 operator/=(float rhs) const
        {
			return Vector2(x/rhs, y/rhs);
		}

		Vector2 normalized(){
			Vector2 normalized = (*this)/norm();
			return normalized;
		}

		void setMag(float magnitude){
			if(magnitude!=0.0f)
				*this = this->normalized() * magnitude;
			else
				*this = Vector2(0.0f, 0.0f);
			//printf("\t%f\tf = %f \n",magnitude, this->norm());
		}

		void limit(float limit){
			if(this->norm() > limit){
				setMag(limit);
			}
		}
		float norm() const
        {
			return sqrt(x*x + y*y);
		}

		void add(Vector2 *v2)
        {
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

/*
 * A Flowpoint is a fixed point with a velocity. It can affect neighboring entities
 */
class FlowPoint{
	public:
		explicit FlowPoint(Vector2 pos) : pos(pos)
        {
			vel = Vector2(0.0f,0.0f);
		}
		FlowPoint(Vector2 pos, Vector2 vel) : pos(pos), vel(vel){}
		FlowPoint(){
			pos = Vector2(0.0f,0.0f);
			vel = Vector2(0.0f,0.0f);
		}
	Vector2 pos;
	Vector2 vel;
};

/**
 * Cell is a living pixel, it has a position and a velocity, a state
 *
 * @param pos initial position
 * @param vel initial velocity
 * @param st initial state
 * @note it can be initialized with or without initial values, for the latter,
 * random values are given or 0,0 for its position
*/
class Cell{
	public:

		Cell(Vector2 pos, Vector2 vel, int st, int d) : pos(pos), vel(vel), state(st)
		{}

		explicit Cell(Vector2 pos) : pos(pos)
		{
			vel = Vector2(-1 + rand()%3, -1 +rand()%3);
			state = (int)rand()%2;
		}

		Cell(){
			pos.x = 0.0f;
			pos.y = 0.0f;
			vel = Vector2(-1 + rand()%3, -1 +rand()%3);
			state = (int)rand()%2;
		}

        /*
         * Update the cell : acceleration is added to velocity, and limits are applied
         *
         */
		void Update(float dt){
			vel.add(&acc);
			acc.limit(maxSpeed);
			vel.limit(maxSpeed);
			//printf("v = %f \n", acc.norm());
			pos.add(&vel);
		}

        void keep_in_bounds(float minX, float maxX, float minY, float maxY){
            if(pos.x >= maxX)
                pos.x = minX;
            else if(pos.x <= minX)
                pos.x = maxX;
            if(pos.y >= maxY)
                pos.y = minY;
            else if(pos.y <= minY)
                pos.y = maxY;
        }

		void attract(Cell * object){
			Vector2 r;
			//r.x = object->pos.x - pos.x;
			//r.y = object->pos.y - pos.y;
			r.x = pos.x - object->pos.x;
			r.y = pos.y - object->pos.y;

			float d = r.norm();
			float G = 1.5f;
			float f;

			d*=d;
			d = d > 100.0f ? 100.0f : d;
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
	SPACE,
	R,
	LEFT_CLICK
};

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
		gW = SDL_CreateWindow("Attraction Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gW == nullptr )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gR = SDL_CreateRenderer(gW, -1,SDL_RENDERER_ACCELERATED);
			if(gR == nullptr){
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
    // Destroy instances
	SDL_DestroyRenderer(gR);
	SDL_DestroyWindow( gW );

	gW = nullptr;
	gR = nullptr;

	//Quit SDL subsystems
	TTF_CloseFont(font);
	font = nullptr;

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(SDL_Renderer* sR, std::string path )
{
	//The final optimized image
	SDL_Texture* newTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == nullptr )
	{
		printf( "unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Convert surface to screen format
		newTexture = SDL_CreateTextureFromSurface( sR, loadedSurface);
		if( newTexture == nullptr )
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
	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{ seed };

	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screenSurface = nullptr;

	SDL_Texture* gTexture = nullptr;
	SDL_Renderer* renderer = nullptr;

	TTF_Font* mainFont = TTF_OpenFont("DejaVuSansMono.ttf", 36);

    // Time management
	float dt = 0.0f;
	float tf = 0.0f;
	float dt_click = 0.0f;
	auto t0 = std::chrono::high_resolution_clock::now();

    // Position of the mouse
    Vector2 mousePos;

    // Button states are kept in a table
	bool buttons[4] = {};

	bool gravity = true;

    // Number of cells to spawn
	int population = 500;


    // Vector to contain population
	std::vector<Cell> people;

    // Vector to contain attraction points of the world
	std::vector<Cell*> attractionPoints;

    // Spawn all population
	Cell c;
	for(int j = 0; j < population; j++){
		c =  Cell(Vector2(
                (float) (rand()%SCREEN_WIDTH),
			(float) (rand()%SCREEN_HEIGHT))
		);

		// A random neighbor
        // Add to the people vector
		people.push_back(c);
	}

    // a center cell
	Cell cCenter = Cell(Vector2(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f));
	attractionPoints.push_back(&cCenter);

	// FlowGrid
	int flowRes = 50;
	std::vector<FlowPoint> flowPointArray;
	FlowPoint fp;

	for(int i = 0; i < SCREEN_WIDTH; i+=flowRes){
		for(int j = 0; j < SCREEN_HEIGHT; j+=flowRes){
		fp = FlowPoint(
			Vector2((float) i,(float) j)
		);
		flowPointArray.push_back(fp);
		}
	}

	//Start up SDL and create window
	if(!init(window, renderer, screenSurface))
	{
		printf( "(-) Failed to initialize\n" );
	}
	else
	{
		bool running = true;
		SDL_Event e;

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
					if(e.key.keysym.sym == SDLK_r){
						buttons[Buttons::R] = true;
					}
				}
				else if(e.type == SDL_KEYUP){
					if(e.key.keysym.sym == SDLK_SPACE){
						buttons[Buttons::SPACE] = false;
					}
					if(e.key.keysym.sym == SDLK_r){
						buttons[Buttons::R] = false;
					}
				}
				else if (e.type == SDL_MOUSEBUTTONDOWN){
					if(e.button.button == SDL_BUTTON_LEFT){
						buttons[Buttons::LEFT_CLICK] = true;
					}
				}
				else if (e.type == SDL_MOUSEBUTTONUP){
					if(e.button.button == SDL_BUTTON_LEFT){
						buttons[Buttons::LEFT_CLICK] = false;
					}
				}
			}

			// INPUT
			int a,b;
			SDL_GetMouseState(&a, &b);
			mousePos.x = a;
			mousePos.y = b;

			if(buttons[Buttons::R]){

				//for(int j = 0; j < population; j++){
					people.clear();
				//}
				for(int j = 0; j < population; j++){
					c =  Cell(Vector2(
						(float) (rand()%SCREEN_WIDTH),
						(float) (rand()%SCREEN_WIDTH))
					);
					// A random neighbor
					people.push_back(c);
				}
			}

			if(dt_click > 0.5f && buttons[Buttons::LEFT_CLICK]){
                // create a new entity at mouse pos
			}

			cCenter.pos = mousePos;

			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			//SDL_SetRenderDrawColor(renderer, 0xD0, 0xE7, 0xD2, 0x01);
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x0F);
			SDL_RenderClear(renderer);

			//Draw the flow grid with lines
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xAC);
            auto t3 = std::chrono::high_resolution_clock::now();
            tf = std::chrono::duration<float, std::chrono::milliseconds::period>(t3 - t0).count();
			for(FlowPoint& fPoint : flowPointArray){
				Vector2 r;
				//r.x = mousePos.x - fPoint.pos.x;
				//r.y = mousePos.y - fPoint.pos.y;
				r.x = (float) perlin.normalizedOctave1D(fPoint.pos.x/SCREEN_WIDTH + tf*.0001, 2);
				r.y = (float) perlin.normalizedOctave1D(fPoint.pos.y/SCREEN_HEIGHT  + tf*.0001, 2);
				fPoint.vel =  r;
                fPoint.vel = fPoint.vel.normalized();
				SDL_RenderDrawLineF(renderer, fPoint.pos.x, fPoint.pos.y, fPoint.pos.x + fPoint.vel.x * 20, fPoint.pos.y + fPoint.vel.y * 20);
			}

			// Draw the grid
			// 618264
			//SDL_SetRenderDrawColor(renderer, 0x61, 0x82, 0x64, 0xFF);
			//SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			for(int i = 0; i < population; i++){
				for(FlowPoint fPoint : flowPointArray){
						/*printf("FlowPoint : (%3.f, %3.f) v=(%.2f, %.2f)\n",
						fPoint.pos.x,
						fPoint.pos.y,
						fPoint.vel.x,
						fPoint.vel.y
						);
						 */

					Vector2 r;
					r.x = people[i].pos.x - fPoint.pos.x;
					r.y = people[i].pos.y - fPoint.pos.y;

					float d = r.norm();
					if(d >= 0.01 && d <= 50){
						Vector2 force =fPoint.vel;
						force.setMag(1.0f/(d));
						people[i].applyForce(&force);
					}

				}
				people[i].Update(dt);
                people[i].keep_in_bounds(0,SCREEN_WIDTH, 0, SCREEN_HEIGHT);

                // Draw a square
                SDL_Rect fRect;
/*                fRect.x = people[i].pos.x;
                float rem = fRect.x%5;
                if(rem < 2.5f)
                    fRect.x = fRect.x - rem;
                else
                    fRect.x = fRect.x + (5 - rem);

                fRect.y = people[i].pos.y;

                rem = fRect.y%5;
                if(rem < 2.5f)
                    fRect.y = fRect.y - rem;
                else
                    fRect.y = fRect.y + (5 - rem);
*/
                fRect.w = 5;
                fRect.x = people[i].pos.x;
                fRect.y = people[i].pos.y;
                fRect.h = 5;
                SDL_RenderFillRect(renderer, &fRect);

                // Draw a pixel
                //SDL_RenderDrawPointF(renderer, people[i].pos.x, people[i].pos.y);
			}

			SDL_FRect r;
			float w, h;
			w = h = 10.0f;

			for(Cell* attractP : attractionPoints){
				r = {attractP->pos.x-w/2, attractP->pos.y-h/2, w, h};
				SDL_RenderFillRectF(renderer, &r);
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
