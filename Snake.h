#include "LTexture.h"
extern LTexture gSnakeTexture;
extern LTexture gTailTexture;
extern LTexture gFoodTexture;
extern LTexture gPlayerTexture;
extern LTexture gPlayerTailTexture;
using namespace std;
 bool checkCollision( SDL_Rect a, SDL_Rect b );

class Food
{
    public:
Food();
void render();
void spawnFood(vector <int> tailX, vector <int> tailY,vector <SDL_Rect> stail ,int playerX, int playerY, vector <SDL_Rect> opponent1, vector <SDL_Rect> opponent2,vector <SDL_Rect> opponent3);
SDL_Rect GetCollider();

//get food locations
int GetPosX();
int GetPosY();

    private:
		int mPosX, mPosY;
		SDL_Rect mCollider;
};

class Snake
{
    public:
static const int SNAKE_VEL =SNAKE_WIDTH;
Snake();
virtual	void render(){};
virtual void move(Food& sfood){};


};





class Tail : public Snake
{
    public:
Tail();
void enlarge(SDL_Rect& head,int velx,int vely);
void update(SDL_Rect& head,int velx,int vely);
//virtual void move( SDL_Rect& wall );
virtual	void render();
vector <int> GetPosX();
vector <int> GetPosY();
vector <int> GetVelX();
vector <int> GetVelY();
vector<SDL_Rect> GetCollider();
    private:
        vector<int> mtPosX, mtPosY;
		vector<int> mtVelX, mtVelY;
		vector<SDL_Rect> mtCollider;
};

class compTail : public Snake
{
    public:
compTail();
void enlarge(SDL_Rect& head,int velx,int vely);
void update(SDL_Rect& head,int velx,int vely);
//virtual void move( SDL_Rect& wall );
virtual	void render();
vector <int> GetPosX();
vector <int> GetPosY();
vector <int> GetVelX();
vector <int> GetVelY();
vector<SDL_Rect> GetCollider();
    private:
        vector<int> mtPosX, mtPosY;
		vector<int> mtVelX, mtVelY;
		vector<SDL_Rect> mtCollider;
};



class Head : public Snake
{
    public:

Head();
void handleEvent( SDL_Event& e );
virtual	void render();
virtual bool move( Food& sfood,Tail& stail,int &, vector <SDL_Rect> opponent1, vector <SDL_Rect> opponent2,vector <SDL_Rect> opponent3 );
bool checkCollisionFood( Food &food );
bool checkCollisionTail( vector <SDL_Rect> tail );
bool checkCollisionAI(vector <SDL_Rect> aiTail);
int GetPosX();
int GetPosY();
int GetVelX();
int GetVelY();

SDL_Rect GetCollider();
    private:
		int mPosX, mPosY;
		int mVelX, mVelY;
		SDL_Rect mCollider;
};

class compHead : public Snake
{
    public:

compHead();
compHead(int xloc, int yloc, int xvel, int yvel); //xloc and vloc are multiples of snake width
void handleEvent( Food& food,int frame );
virtual	void render();
virtual bool move( Food& sfood,compTail& stail,int &,vector <SDL_Rect> opponent1, vector <SDL_Rect> opponent2,vector <SDL_Rect> opponent3  );
bool checkCollisionFood( Food &food );
bool checkCollisionTail( vector <SDL_Rect> tail );
int GetPosX();
int GetPosY();
int GetVelX();
int GetVelY();

void goUp();
void goDown();
void goLeft();
void goRight();

SDL_Rect GetCollider();
    private:
        bool m_nearFood ;
		int mPosX, mPosY;
		int mVelX, mVelY;
		SDL_Rect mCollider;
		//define viewfinder coords
		int xMax, yMax, xMin, yMin;
};
