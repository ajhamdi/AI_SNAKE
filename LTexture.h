#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cctype>
#include <string>
#include <iomanip>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

//const int SCREEN_WIDTH = 1920;
//const int SCREEN_HEIGHT =1440;
		//The dimensions of the snake
const int SNAKE_WIDTH = 32;
const int SNAKE_HEIGHT = 32;
const int SCREEN_WIDTH = 45*SNAKE_WIDTH;
const int SCREEN_HEIGHT =30 * SNAKE_WIDTH;
//const int SCREEN_WIDTH = 40*SNAKE_WIDTH;
//const int SCREEN_HEIGHT = 30*SNAKE_HEIGHT;
const int SMARTNESS = 20 ; // default 20
const int GAME_DELAY = 60;
const int PREDICTABILITY = 10; // default 20




class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};
