/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include "GameMaster.h"


using namespace std;
extern LTexture gSnakeTexture;
extern LTexture gTailTexture;
extern LTexture gFoodTexture;

//The window we'll be rendering to

int main( int argc, char* args[] )
{
    int cont ;
do
{
GameMaster Snake_Game ;
Snake_Game.runGame();
cout<< "\n The score is .......  "<<Snake_Game.GameScore <<endl ;
cont = Snake_Game.gameCont ;
SDL_Delay(1500);
}while(cont);
	return 0;
}
