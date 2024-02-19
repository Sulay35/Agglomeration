#include "../include/attraction.h"
/**
 * TODO :
 * - grid of pointer instead of object
 * - Agglomerations
 * - Vector.distance(v1, v2)
 * - draw according to speed
 * - constraint pos to windows size
 * - * dt pour le movement
*/


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
	const siv::PerlinNoise::seed_type seed1 = 123456u;
	const siv::PerlinNoise perlinX{ seed1 };
    const siv::PerlinNoise::seed_type seed2 = -654321u;
    const siv::PerlinNoise perlinY{ seed2 };

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
	int flowRes = 40;
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

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x0F);
		SDL_RenderClear(renderer);

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
				people.clear();
				for(int j = 0; j < population; j++){
					c =  Cell(Vector2(
						(float) (rand()%SCREEN_WIDTH),
						(float) (rand()%SCREEN_HEIGHT))
					);
					people.push_back(c);
				}
			}

			if(dt_click > 0.5f && buttons[Buttons::LEFT_CLICK]){
                // create a new entity at mouse pos
			}

			cCenter.pos = mousePos;

			auto t3 = std::chrono::high_resolution_clock::now();
			tf = std::chrono::duration<float, std::chrono::milliseconds::period>(t3 - t0).count();

			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x2f);

			/*
			SDL_Rect fRect;
			fRect.w = SCREEN_WIDTH;
			fRect.x = 0;
			fRect.y = 0;
			fRect.h = SCREEN_HEIGHT;
			SDL_RenderFillRect(renderer, &fRect);
			*/
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			//Draw the flow grid with lines
			SDL_SetRenderDrawColor(renderer, 0xEE, 0x00, 0x00, 0x30);
			for(FlowPoint& fPoint : flowPointArray){
				Vector2 r;
				//r.x = mousePos.x - fPoint.pos.x;
				//r.y = mousePos.y - fPoint.pos.y;
                //r.x = (float) perlin.normalizedOctave1D(fPoint.pos.x/SCREEN_WIDTH + tf*.00001, 7);
                //r.y = (float) perlin.normalizedOctave1D(fPoint.pos.y/SCREEN_HEIGHT  + tf*.00001, 7);
				r.x = (float) perlinX.octave2D_11((fPoint.pos.x/flowRes) * tf*.000001,(fPoint.pos.y/flowRes) * tf*.0000001, 5);
				r.y = (float) perlinY.octave2D_11((fPoint.pos.x/flowRes) * tf*.0000001,(fPoint.pos.y/flowRes) * tf*.0000001, 5);
				fPoint.vel = r;
				fPoint.vel = fPoint.vel.normalized();
				SDL_RenderDrawLineF(renderer, fPoint.pos.x, fPoint.pos.y, fPoint.pos.x + fPoint.vel.x * 20, fPoint.pos.y + fPoint.vel.y * 20);
			}

			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
			for(int i = 0; i < population; i++){
				for(FlowPoint fPoint : flowPointArray){
					Vector2 r;
					r.x = people[i].pos.x - fPoint.pos.x;
					r.y = people[i].pos.y - fPoint.pos.y;

					float d = r.norm();
					if(d >= 0.01 && d <= 50){
						Vector2 force =fPoint.vel;
						//force.setMag(1/d);
						people[i].applyForce(&force);
					}

				}
                people[i].keep_in_bounds(0,SCREEN_WIDTH, 0, SCREEN_HEIGHT);
				people[i].Update(dt);

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
				r = {(float) attractP->pos.x-w/2, (float) attractP->pos.y-h/2, w, h};
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
