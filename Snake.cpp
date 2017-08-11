#include "Snake.h"
#include <stdlib.h>
extern SDL_Window* gWindow ;
//The window renderer
extern SDL_Renderer* gRenderer;
extern TTF_Font* gfont ;

Snake::Snake(){};

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}





Head::Head()
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

	//Set collision box dimension
	mCollider.w = SNAKE_WIDTH;
	mCollider.h = SNAKE_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

int Head::GetPosX(){return Head::mPosX ;}
int Head::GetPosY(){return Head::mPosY ;}
int Head::GetVelX(){return Head::mVelX ;}
int Head::GetVelY(){return Head::mVelY ;}
SDL_Rect Head::GetCollider(){return Head::mCollider;}

void Head::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= SNAKE_VEL; mVelX = 0 ;if(mVelY ==0){mVelY = SNAKE_VEL;} break;
            case SDLK_DOWN: mVelY += SNAKE_VEL; mVelX = 0;if(mVelY ==0){mVelY = -SNAKE_VEL;} break;
            case SDLK_LEFT: mVelX -= SNAKE_VEL; mVelY =0 ; if(mVelX ==0){mVelX = SNAKE_VEL;} break;
            case SDLK_RIGHT: mVelX += SNAKE_VEL; mVelY = 0;if(mVelX ==0){mVelX = -SNAKE_VEL;} break;
        }
    }

}

 bool Head::move( Food& sfood,Tail& stail,int &nbrtails, vector <SDL_Rect> opponent1, vector <SDL_Rect> opponent2,vector <SDL_Rect> opponent3 )
{
    //Move the dot left or right
    bool hit = false ;
    mPosX += mVelX;
	mCollider.x = mPosX;

    //If the dot collided or went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + SNAKE_WIDTH > SCREEN_WIDTH )  )
    {
hit = true ;

    }

    //Move the dot up or down
    mPosY += mVelY;
	mCollider.y = mPosY;

    //If the dot collided or went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + SNAKE_HEIGHT > SCREEN_HEIGHT )  )
    {
        hit = true ;

    }
        if( checkCollisionTail(stail.GetCollider())  )
    {
        hit = true ;

    }

    //include colliding with ai tail:
    if(checkCollisionAI(opponent1)){
        hit = true;
    }
        if(checkCollisionAI(opponent2)){
        hit = true;
    }
        if(checkCollisionAI(opponent3)){
        hit = true;
    }


    if(checkCollisionFood(  sfood ))
    {
sfood.spawnFood(stail.GetPosX(),stail.GetPosY(),stail.GetCollider(),this->mPosX,this->mPosY,  opponent1, opponent2,opponent3);
nbrtails++ ;
stail.enlarge(Head::mCollider,Head::mVelX,Head::mVelY);
    }
stail.update(Head::mCollider,Head::mVelX,Head::mVelY) ;

return hit ;
}


 void Head::render()
{
    //Show the dot
	gPlayerTexture.render( Head::mPosX, Head::mPosY );
}

bool Head::checkCollisionFood( Food& food ){
bool hit = checkCollision(Head::mCollider,food.GetCollider() );
return hit ;
}



bool Head::checkCollisionTail( vector <SDL_Rect> tail ){
bool hit = false ;
if ((int)tail.size() > 1 ){
for (int i=3; i<(int)tail.size(); i++){

    hit = (hit || checkCollision(mCollider,tail[i]) ) ;
}
}
return hit ;
}

//check for collision with single ai (can expand to include all AI later, also include head collisions)
bool Head::checkCollisionAI(vector <SDL_Rect> aiTail){
    bool hit = false;

    for(int i=1; i< (int)aiTail.size(); i++){
        hit = (hit || checkCollision(mCollider, aiTail[i]) );
    }

    if(hit == true){
        cout << "AI snake hit its tail" << endl;
    }
    return hit ;
}







Food::Food()
{
    //Initialize the offsets
    mPosX = SCREEN_WIDTH/2;  mPosX = (int)mPosX/32  ; mPosX *= 32 ;
    mPosY = SCREEN_HEIGHT/2; mPosY = (int)mPosY/32  ; mPosY *= 32 ;

			mCollider.x = mPosX;
			mCollider.y = mPosY;
	//Set collision box dimension
	mCollider.w = SNAKE_WIDTH;
	mCollider.h = SNAKE_HEIGHT;
}
 void Food::render()
{
    //Show the dot
	gFoodTexture.render( Food::mPosX, Food::mPosY );
}

SDL_Rect Food::GetCollider(){return Food::mCollider;}

void Food::spawnFood(vector <int> tailX, vector <int> tailY,vector <SDL_Rect> stail, int playerX, int playerY, vector <SDL_Rect> opponent1, vector <SDL_Rect> opponent2,vector <SDL_Rect> opponent3 ) {
	//Get a valid spawn for the food which is not on top of a tail or player block
	bool valid = false;
	int x = 0;
	int y = 0;
	while (!valid) {
//		srand(time(NULL));
		x = (rand() % (SCREEN_WIDTH-SNAKE_WIDTH));
		y = (rand() % (SCREEN_HEIGHT-SNAKE_HEIGHT));
		SDL_Rect sfood ; sfood.h = SNAKE_HEIGHT ; sfood.w = SNAKE_WIDTH ; sfood.x = x ; sfood.y  = y ;
		valid = true;

		//Check all tail blocks and player block
		for (int i = 0; i < stail.size(); i++) {

			if ((x == tailX[i] && y == tailY[i]) || (x == playerX && y == playerY) || checkCollision(stail[i],sfood) ) {
				valid = false;
			}

		}
		for(int i=0; i < (int) opponent1.size(); i++ ){
            if(checkCollision(sfood, opponent1[i])){
                valid = false;
            }
		}

		for(int i=0; i <(int) opponent2.size(); i++ ){
            if(checkCollision(sfood, opponent2[i])){
                valid = false;
            }
		}

		for(int i=0; i <(int) opponent3.size(); i++ ){
            if(checkCollision(sfood, opponent3[i])){
                valid = false;
            }
		}
	}
	x =(int)x /SNAKE_WIDTH; x*=SNAKE_WIDTH;
	y =(int)y /SNAKE_HEIGHT; y*=SNAKE_HEIGHT ;
	//Create and return a pair of the two position values
mPosX = x ; mPosY = y ;
mCollider.x = x ; mCollider.y = y ;

}

Tail::Tail()
{
    vector <int> temp(1,0);


    //Initialize the offsets
    mtPosX = temp;
    mtPosY = temp;
    mtVelX = temp;
    mtVelY = temp;
vector <SDL_Rect> rect(1);
mtCollider = rect ;
//           	cout<<"OOOOOOOOOOOOOOOOOOOO";
//			cin.get();
	//Set collision box dimension
	mtCollider[0].w = SNAKE_WIDTH;
	mtCollider[0].h = SNAKE_HEIGHT;
	mtCollider[0].x = mtPosX[0];
	mtCollider[0].y = mtPosY[0];

    //Initialize the velocity

}

int Food::GetPosX(){return Food::mPosX ;}
int Food::GetPosY(){return Food::mPosY ;}





vector <int> Tail::GetPosX(){return Tail::mtPosX ;}
vector <int> Tail::GetPosY(){return Tail::mtPosY ;}
vector <int> Tail::GetVelX(){return Tail::mtVelX ;}
vector <int> Tail::GetVelY(){return Tail::mtVelY ;}
vector<SDL_Rect> Tail::GetCollider(){return Tail::mtCollider;}

void Tail::render()
{
    for(int i = 1 ; i<int(mtCollider.size()) ; i++){
       gPlayerTailTexture.render( mtPosX[i], mtPosY[i] );
    }
	}

//virtual void Tail::\move( SDL_Rect& wall )
//{
//    //Move the dot left or right
//    mPosX += mVelX;
//	mCollider.x = mPosX;
//
//    //If the dot collided or went too far to the left or right
//    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision( mCollider, wall ) )
//    {
//        //Move back
//        mPosX -= mVelX;
//		mCollider.x = mPosX;
//    }
//
//    //Move the dot up or down
//    mPosY += mVelY;
//	mCollider.y = mPosY;
//
//    //If the dot collided or went too far up or down
//    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( mCollider, wall ) )
//    {
//        //Move back
//        mPosY -= mVelY;
//		mCollider.y = mPosY;
//    }
//}
//
void Tail::enlarge(SDL_Rect& head,int velx,int vely){
mtPosX.push_back(mtPosX.back());
mtPosY.push_back(mtPosY.back());
mtVelX.push_back(mtVelX.back());
mtVelY.push_back(mtVelY.back());
mtCollider.push_back(mtCollider.back());
mtCollider[0] = head;
mtPosX[0] = head.x - velx ;
mtPosY[0] = head.y - vely ;
mtVelX[0] = velx ;
mtVelY[0] = vely ;
mtCollider[0].x = mtPosX[0];
mtCollider[0].y = mtPosY[0];
}
void Tail::update(SDL_Rect& head,int velx,int vely){
mtCollider.insert(mtCollider.begin(),head) ;
mtPosX.insert(mtPosX.begin(),head.x) ;
mtPosY.insert(mtPosY.begin(),head.y) ;
mtVelX.insert(mtVelX.begin(),velx) ;
mtVelY.insert(mtVelY.begin(),vely) ;
//    				cout<<mtPosX[0]<<mtPosX[];
//				cin.get();
mtCollider.pop_back() ;
mtPosX.pop_back() ;
mtPosY.pop_back() ;
mtVelX.pop_back() ;
mtVelY.pop_back() ;
//    				cout<<"OOOOOOOOOOOOO";
//				cin.get();
}

//void Tail::move(SDL_Rect& head){
//mtPosX.push_back(mtPosX.back());
//mtPosY.push_back(mtPosY.back());
//mtCollider.push_back(mtCollider.back());
//mtCollider[0] = head;
//mtPosX[0] = head.x ;
//mtPosY[0] = head.y ;
//}






//define computer copies of the functions
compHead::compHead()
{
    //Initialize the offsets
    mPosX = 4*SNAKE_WIDTH;
    mPosY = 15*SNAKE_HEIGHT;

	//Set collision box dimension
	mCollider.w = SNAKE_WIDTH;
	mCollider.h = SNAKE_HEIGHT;

    //Initialize the velocity
    mVelX = 30;
    mVelY = 0;

    int xMin = 1;
    int xMax = 2;
    int yMin = 3;
    int yMax = 4;
}

compHead::compHead(int xloc, int yloc, int xvel, int yvel){
    //Initialize the offsets
    mPosX = xloc*SNAKE_WIDTH;
    mPosY = yloc*SNAKE_HEIGHT;

	//Set collision box dimension
	mCollider.w = SNAKE_WIDTH;
	mCollider.h = SNAKE_HEIGHT;

    //Initialize the velocity
    mVelX = xvel;
    mVelY = yvel;

    int xMin = 1;
    int xMax = 2;
    int yMin = 3;
    int yMax = 4;

    m_nearFood = false ;
}




int compHead::GetPosX(){return compHead::mPosX ;}
int compHead::GetPosY(){return compHead::mPosY ;}
int compHead::GetVelX(){return compHead::mVelX ;}
int compHead::GetVelY(){return compHead::mVelY ;}


void compHead::goUp(){mVelX = 0; mVelY = -SNAKE_VEL;}
void compHead::goDown(){mVelX = 0; mVelY = SNAKE_VEL;}
void compHead::goRight(){mVelX = SNAKE_VEL; mVelY = 0;}
void compHead::goLeft(){mVelX = -SNAKE_VEL; mVelY = 0;}



SDL_Rect compHead::GetCollider(){return compHead::mCollider;}

void compHead::handleEvent(Food& food, int frame )
{
    int viewDist = SMARTNESS * SNAKE_WIDTH;

    bool goingRight = compHead::mVelX > 0;
    bool goingLeft = compHead::mVelX < 0;
    bool goingUp = compHead::mVelY < 0;
    bool goingDown = compHead::mVelY > 0;

    //set view boundaries

    if(goingRight){
        //cout << "velocity positive x" << endl;
        xMax = compHead::mPosX + 2*viewDist;
        xMin = compHead::mPosX;
        yMax = compHead::mPosY + viewDist;
        yMin = compHead::mPosY - viewDist;

    }
    else if(goingLeft){
        //cout << "you shouldn't see me" << endl;
        xMax = compHead::mPosX;
        xMin = compHead::mPosX - 2*viewDist;
        yMax = compHead::mPosY + viewDist;
        yMin = compHead::mPosY - viewDist;

    }
    else if(goingUp){
        xMax = compHead::mPosX + viewDist;
        xMin = compHead::mPosX - viewDist;
        yMax = compHead::mPosY;
        yMin = compHead::mPosY - 2*viewDist;

    }
    else if(goingDown){
        xMax = compHead::mPosX + viewDist;
        xMin = compHead::mPosX - viewDist;
        yMax = compHead::mPosY + 2*viewDist;
        yMin = compHead::mPosY;

    }
    SDL_Rect field_of_vision ;
            field_of_vision.h = 2*viewDist ; field_of_vision.w = 2*viewDist ; field_of_vision.x = xMin ; field_of_vision.y = yMin ;

    //cout << xMax <<" " << xMin <<" "<< yMax << " "<< yMin << endl;
    m_nearFood = checkCollision(food.GetCollider(),field_of_vision) ;
//    bool nearFood  foodX > xMin && foodX < xMax && foodY > yMin && foodY < yMax;

    //avoiding walls:
    int wallMargin = 2*SNAKE_WIDTH;
    bool nearTopWall = compHead::mPosY <= wallMargin;
    bool nearBotWall = SCREEN_HEIGHT - compHead::mPosY <= wallMargin;
    bool nearRightWall = SCREEN_WIDTH - compHead::mPosX <= wallMargin;
    bool nearLeftWall = compHead::mPosX <= wallMargin;


    if( nearRightWall && goingRight ){
        if(nearBotWall){
            goUp();
        }
        else if(nearTopWall){
            goDown();
        }
        else{
            if(rand()%10 >= 5){
                goUp();
            }
            else{
                goDown();
            }
        }
    }

    if(nearLeftWall && goingLeft){
        if(nearBotWall){
            goUp();
        }
        else if(nearTopWall){
            goDown();
        }
        else{
            if(rand()%10 >= 5){
                goUp();
            }
            else{
                goDown();
            }
        }
    }

    if(nearBotWall && goingDown){
        if(nearRightWall){
            goLeft();
        }
        else if(nearLeftWall){
            goRight();
        }
        else{
            if(rand()%10 >= 5){
                goLeft();
            }
            else{
                goRight();
            }
        }
    }

    if(nearTopWall && goingUp){
         if(nearRightWall){
            goLeft();
        }
        else if(nearLeftWall){
            goRight();
        }
        else{
            if(rand()%10 >= 5){
                goLeft();
            }
            else{
                goRight();
            }
        }
    }



    //loop to detect food:
    int foodX = food.GetPosX();
    int foodY = food.GetPosY();
    //cout << "food coords: " << foodX << " " <<foodY << endl;


    if(m_nearFood){
        if(goingRight || goingLeft){
            if(abs(mPosX-foodX) <= 0){
                if(foodY > mPosY){
                    goDown();
                }
                else if(foodY <= mPosY){
                    goUp();
                }
            }
        }

        if(goingUp || goingDown){
            if(abs(mPosY-foodY) <= 0){
                if(foodX > mPosX){
                    goRight();
                }
                else if(foodX <= mPosX){
                    goLeft();
                }
            }
        }

    }

    //need to add more dynamic behavior inside the playing field. use 2.5 secs for when not near wall
    bool nearAnyWall = nearBotWall || nearLeftWall || nearRightWall || nearTopWall;
    if(!nearAnyWall && !m_nearFood && (frame%PREDICTABILITY == 0)){

//        cout << "random off-wall movement" << endl;
        int randMove = rand() % 100;
            if(goingUp || goingDown){
                if(randMove < 50){
                    goLeft();
                }
                else{
                    goRight();
                }
            }
            if(goingLeft || goingRight){
                if(randMove < 50){
                    goUp();
                }
                else{
                    goDown();
                }
            }
    }
    //add random behavior every 5 secs

    else if (nearAnyWall && (frame%15 == 0)){

//        cout << "random wall movement" << endl;

        //check that we aren't near food first:

        if(nearRightWall && (goingUp || goingDown)){
            goLeft();
        }
        else if(nearBotWall && (goingRight || goingLeft)){
            goUp();
        }
        else if(nearLeftWall && (goingUp || goingDown)){
            goRight();
        }
        else if(nearTopWall && (goingRight || goingLeft)){
            goDown();
        }

    }


}


bool compHead::move( Food& sfood, compTail& stail, int &nbrtails, vector <SDL_Rect> opponent1, vector <SDL_Rect> opponent2,vector <SDL_Rect> opponent3 )
{
    //Move the dot left or right
    bool hit = false ;
    mPosX += mVelX;
	mCollider.x = mPosX;

    //If the dot collided or went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + SNAKE_WIDTH > SCREEN_WIDTH )  )
    {
hit = true ;

    }

    //Move the dot up or down
    mPosY += mVelY;
	mCollider.y = mPosY;

    //If the dot collided or went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + SNAKE_HEIGHT > SCREEN_HEIGHT )  )
    {
        hit = true ;

    }
//        if( checkCollisionTail(stail.GetCollider()) )
//    {
//        hit = true ;
//
//    }
    if(compHead::checkCollisionFood(  sfood ))
    {
sfood.spawnFood(stail.GetPosX(),stail.GetPosY(),stail.GetCollider(),this->mPosX,this->mPosY, opponent1,  opponent2, opponent3);
//nbrtails++ ;
stail.enlarge(compHead::mCollider,compHead::mVelX,compHead::mVelY);
    }
stail.update(compHead::mCollider,compHead::mVelX,compHead::mVelY) ;

return hit ;
}


 void compHead::render()
{
    //Show the dot
	gSnakeTexture.render( compHead::mPosX, compHead::mPosY );

//	show field of vision
//	gSnakeTexture.render( xMin, yMin );
//	gSnakeTexture.render( xMin, yMax );
//	gSnakeTexture.render( xMax, yMin );
//	gSnakeTexture.render( xMax, yMax );




}

bool compHead::checkCollisionFood( Food& food ){
bool hit = checkCollision(compHead::mCollider,food.GetCollider() );
return hit ;
}

bool compHead::checkCollisionTail( vector <SDL_Rect> tail ){
bool hit = false ;
if ((int)tail.size() > 3 ){
for (int i=3; i<(int)tail.size(); i++){

    hit = (hit || checkCollision(mCollider,tail[i]) ) ;
}
}
return hit ;
}



compTail::compTail()
{
    vector <int> temp(1,0);


    //Initialize the offsets
    mtPosX = temp;
    mtPosY = temp;
    mtVelX = temp;
    mtVelY = temp;
vector <SDL_Rect> rect(1);
mtCollider = rect ;
//           	cout<<"OOOOOOOOOOOOOOOOOOOO";
//			cin.get();
	//Set collision box dimension
	mtCollider[0].w = SNAKE_WIDTH;
	mtCollider[0].h = SNAKE_HEIGHT;
	mtCollider[0].x = mtPosX[0];
	mtCollider[0].y = mtPosY[0];

    //Initialize the velocity

}

vector <int> compTail::GetPosX(){return compTail::mtPosX ;}
vector <int> compTail::GetPosY(){return compTail::mtPosY ;}
vector <int> compTail::GetVelX(){return compTail::mtVelX ;}
vector <int> compTail::GetVelY(){return compTail::mtVelY ;}
vector<SDL_Rect> compTail::GetCollider(){return compTail::mtCollider;}

void compTail::render()
{
    for(int i = 1 ; i<int(mtCollider.size()) ; i++){
       gTailTexture.render( mtPosX[i], mtPosY[i] );
    }
	}

//virtual void Tail::\move( SDL_Rect& wall )
//{
//    //Move the dot left or right
//    mPosX += mVelX;
//	mCollider.x = mPosX;
//
//    //If the dot collided or went too far to the left or right
//    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || checkCollision( mCollider, wall ) )
//    {
//        //Move back
//        mPosX -= mVelX;
//		mCollider.x = mPosX;
//    }
//
//    //Move the dot up or down
//    mPosY += mVelY;
//	mCollider.y = mPosY;
//
//    //If the dot collided or went too far up or down
//    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ) || checkCollision( mCollider, wall ) )
//    {
//        //Move back
//        mPosY -= mVelY;
//		mCollider.y = mPosY;
//    }
//}
//
void compTail::enlarge(SDL_Rect& head,int velx,int vely){
mtPosX.push_back(mtPosX.back());
mtPosY.push_back(mtPosY.back());
mtVelX.push_back(mtVelX.back());
mtVelY.push_back(mtVelY.back());
mtCollider.push_back(mtCollider.back());
mtCollider[0] = head;
mtPosX[0] = head.x - velx ;
mtPosY[0] = head.y - vely ;
mtVelX[0] = velx ;
mtVelY[0] = vely ;
mtCollider[0].x = mtPosX[0];
mtCollider[0].y = mtPosY[0];
}
void compTail::update(SDL_Rect& head,int velx,int vely){
mtCollider.insert(mtCollider.begin(),head) ;
mtPosX.insert(mtPosX.begin(),head.x) ;
mtPosY.insert(mtPosY.begin(),head.y) ;
mtVelX.insert(mtVelX.begin(),velx) ;
mtVelY.insert(mtVelY.begin(),vely) ;
//    				cout<<mtPosX[0]<<mtPosX[];
//				cin.get();
mtCollider.pop_back() ;
mtPosX.pop_back() ;
mtPosY.pop_back() ;
mtVelX.pop_back() ;
mtVelY.pop_back() ;
//    				cout<<"OOOOOOOOOOOOO";
//				cin.get();
}





