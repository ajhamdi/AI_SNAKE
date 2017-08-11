#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include "Snake.h"
using namespace std;

//Scene textures


class GameMaster
{
private:
//SDL_Rect mfood ;
int nbrTails;
int frameCount ;

public:
int GameScore ;
GameMaster();
bool gameOff ;
bool gameCont ;
void runGame();
bool init();
bool loadMedia();
void close();
void gameOver(SDL_Renderer* renderer, SDL_Event event,  int tailLength);

};
string NumberToString ( int Number );
