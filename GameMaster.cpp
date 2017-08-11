#include "GameMaster.h"
extern SDL_Window* gWindow ;
//The window renderer
extern SDL_Renderer* gRenderer;
extern TTF_Font* gfont ;
extern LTexture gSnakeTexture;
extern LTexture gTailTexture;
extern LTexture gFoodTexture;
extern LTexture gPlayerTexture;
extern LTexture gPlayerTailTexture;

/*** the Actual implementation of the game ***/
GameMaster::GameMaster(){

gameOff = false;
gameCont = true ;
GameScore = 0 ;
nbrTails = 1 ;
frameCount = 1 ;
}
bool GameMaster::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load press texture
	if( !gSnakeTexture.loadFromFile( "snake_head.png" ) )
	{
		printf( "Failed to load snake texture!\n" );
		success = false;
	}

		if( !gTailTexture.loadFromFile( "snake_body.png" ) )
	{
		printf( "Failed to load tail texture!\n" );
		success = false;
	}
		if( !gFoodTexture.loadFromFile( "food.png" ) )
	{
		printf( "Failed to load tail texture!\n" );
		success = false;
		}
        if( !gPlayerTexture.loadFromFile( "player_head.png" ) )
	{
		printf( "Failed to load tail texture!\n" );
		success = false;
	}
	    if( !gPlayerTailTexture.loadFromFile( "player_body.png" ) )
	{
		printf( "Failed to load tail texture!\n" );
		success = false;
	}
	 gfont = TTF_OpenFont("arial.ttf", 12);
	if (gfont == NULL) {
     	printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
	//	gfont not loaded? print an error and return
		cout << "\nFont loading error" << endl;
		success = false;
	}
	return success;
}

bool GameMaster::init()
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
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}

	}

	return success;
}

void GameMaster::runGame(){
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen

			Head mysnake1;
			Tail mytail;
			//Head mysnake2;
			//Tail mytail2;

			//add computer players
			compHead myCompHead(4,15,SNAKE_HEIGHT,0);
			compTail myCompTail;

			compHead myCompHead2(18,20,0,SNAKE_WIDTH);
			compTail myCompTail2;

            compHead myCompHead3(10,10,0,SNAKE_WIDTH);
			compTail myCompTail3;
            Food myfood1;
			//Set the wall
            srand(time(NULL));

			//While application is running
			while( !gameOff )
			{
            myCompHead.handleEvent(myfood1,frameCount);
            myCompHead2.handleEvent(myfood1,frameCount);
            myCompHead3.handleEvent(myfood1,frameCount);


				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						gameOff = true;
						gameCont = false ;
					}

					//Handle input for the dot
					mysnake1.handleEvent( e );
				}

				//Move the snake and record if there was a hit
				if (gameOff ) {break ; }
				gameOff = mysnake1.move( myfood1,mytail,GameMaster::nbrTails, myCompTail.GetCollider(), myCompTail2.GetCollider(),myCompTail3.GetCollider() ) ||
				 myCompHead.move(myfood1, myCompTail, GameMaster::nbrTails, myCompTail.GetCollider(), myCompTail2.GetCollider(),myCompTail3.GetCollider()) || myCompHead2.move(myfood1, myCompTail2, GameMaster::nbrTails, myCompTail.GetCollider(), myCompTail2.GetCollider(),myCompTail3.GetCollider()) ||
				 myCompHead3.move(myfood1, myCompTail3, GameMaster::nbrTails, myCompTail.GetCollider(), myCompTail2.GetCollider(),myCompTail3.GetCollider()) ;
                frameCount++ ;
                GameScore = nbrTails -1 ;
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render wall
//				SDL_SetRenderDrawColor( gRenderer, 0x00, 0xF0, 0xF0, 0x0F );
//				SDL_RenderFillRect( gRenderer, &mfood );

				//Render dot
				mytail.render();
				mysnake1.render();
                myfood1.render();
//                mysnake2.render();
//                mytail2.render();
                myCompHead.render();
                myCompTail.render();

                myCompHead2.render();
                myCompTail2.render();
                myCompHead3.render();
                myCompTail3.render();
				//Update screen
				SDL_RenderPresent( gRenderer );
				SDL_Delay(GAME_DELAY);
			}
		}
	}

	//Free resources and close SDL
	close();

}


void GameMaster::close()
{
	//Free loaded images
	gSnakeTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


void GameMaster::gameOver(SDL_Renderer* renderer, SDL_Event event,  int tailLength) {
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color White = { 255, 255, 255 };
	SDL_Color Black = { 0, 0, 0 };
	//Get the gfont used for displaying text


	SDL_Surface* gameover = TTF_RenderText_Solid(gfont, "Game Over", Red);
	SDL_Surface* retry = TTF_RenderText_Solid(gfont, "Press Enter to retry, HAMDI", White);

	//c_str() allows normal strings to be rendered here, otherwise the rendered text would have to be a const char* type
	//Const char* values cannot be changed after initialization, but we need dynamic text here
	//We re-render the score in the game over screen so the player can still see their score
	string scoremessage = "Score: " + NumberToString(tailLength * 10) ;
	SDL_Surface* score = TTF_RenderText_Solid(gfont,scoremessage.c_str() , Black);
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	gameoverRect.w = 200;
	gameoverRect.h = 100;
	gameoverRect.x = ((SCREEN_WIDTH) / 2)-(gameoverRect.w/2);
	gameoverRect.y = ((SCREEN_HEIGHT) / 2)-(gameoverRect.h/2)-50;
	retryRect.w = 300;
	retryRect.h = 50;
	retryRect.x = ((SCREEN_WIDTH) / 2) - ((retryRect.w / 2));
	retryRect.y = (((SCREEN_HEIGHT) / 2) - ((retryRect.h / 2))+150);
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((SCREEN_WIDTH) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
	//Close gfont before finishing so this gfont doesn't collide with any other function's gfont
	TTF_CloseFont(gfont);

	//Show game over screen while space has not been pressed
	while (true) {
		SDL_RenderPresent(renderer);
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				return;
			}
		}
	}
}



string NumberToString ( int Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }
