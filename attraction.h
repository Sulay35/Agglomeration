#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
//Screen dimension constants

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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