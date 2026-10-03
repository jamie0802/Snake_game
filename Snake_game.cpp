#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define DIRECTION 224
#define UP 72 
#define DOWN 80 
#define LEFT 75 
#define RIGHT 77
#define HEIGHT 25
#define WIDTH 80
#define SLEEP_TIME 150
#define SIZE 6
#define LENGTH 2000
#define SNAKE 1
#define EGG 2
#define WALL 3

using namespace std;

void HideCursor();
void gotoxy(int, int); 

void HideCursor(){
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void gotoxy(int x, int y){
    COORD  loc;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE); 
    loc.X = x;
    loc.Y = y;
    SetConsoleCursorPosition(hOuput,loc);
} 



class Egg;

class Object{
protected:
	static char map[25][80];	
public:
	virtual void Move(){
		cout<<"Snake overriding"<<endl;
	}

	virtual int CheckCollision(Egg *egg){
		cout<<"Snake overridng"<<endl;
	}
	
	virtual int GetIsEaten(){
		cout<<"Egg overriding"<<endl;
	}
};

char Object::map[25][80] = {0};

class Snake:public Object{
private:
    int snakeX;  
    int snakeY; 
    int tempX;  
    int tempY; 
	int *bodyX = new int [LENGTH]; 
	int *bodyY = new int [LENGTH]; 
	int flag;
	int head = -1;
	int tail = -1;
	
    int direction;
    char HeadShape;
    int BodyShape;
    int isAlive;
	int length; //蛇身目前的長度 
	
public:
    Snake();
	void ChangeShape(char skin);
	void ChangeShape(int n);
    void Move(); //判斷你按下是不是方向鍵、按的是哪個方向？ 
    void Forward();
	int IsFull();
	int IsEmpty();	
	void Enqueue(); //蛇身往前推 
	void Dequeue(); //減掉蛇尾
	void PrintBody();
    int CheckCollision(Egg *egg); //覆寫 偵測蛇是否碰到邊界 
    int IsAlive();
    
    int GetX();
    int GetY();
    int GetbodyX(int a);
    int GetbodyY(int b);
    int GetHead();
    int GetTail();
    //int GetLength();
};

class Egg:public Object{
private:
    int eggX;
    int eggY;
    char shape;
    int isEaten;
public:
    Egg(Snake *snake);
    void ChangeShape(char skin);
    void CheckProduce(Snake *snake); 	
    void SetIsEatenTo1();
    void SetIsEatenTo0();
    int GetIsEaten();
	int GetX();
	int GetY();	
};

class Wall:public Object{
public:
	Wall();
};

class GM{
private:
    Object *LinkMap[2000];
	int ObjectMap[2000];

	Wall *wall = new Wall;
	Snake *snake = new Snake;
	Egg *egg = new Egg(snake);
	
	int SnakeX;
	int SnakeY;
	int EggX;
	int EggY;

	
public:
    GM();
    ~GM();
    void Detect(); //地圖上每一格偵測
};

/*Snake's function*/

Snake::Snake():length(0),tempX(0),tempY(0),direction(DOWN),isAlive(1),flag(0){
	
	
	for(int i=-1;i<LENGTH;i++){ 
		bodyX[i] = 0;
        bodyY[i] = 0;
	}
	
	snakeX = rand()%(WIDTH-10)+5;
    snakeY = rand()%(HEIGHT-10)+5;
           
    ChangeShape('S');
	ChangeShape(115);
	 
    gotoxy(snakeX,snakeY);
    cout<<HeadShape;
}

void Snake::ChangeShape(char skin){
	HeadShape = skin;
}

void Snake::ChangeShape(int n){
	BodyShape = n;
}

void Snake::Move(){  
    unsigned char key;
        
    if(kbhit()){
        key = getch();
        switch(key){
            case DIRECTION:
                switch(key = getch()){
                    case UP:
                        direction = UP;
                        break;
                    case DOWN:
                        direction = DOWN;
                        break;
                    case LEFT:
                        direction = LEFT;
                        break;
                    case RIGHT:
                        direction = RIGHT;
                        break;
                }
        }
    }
    
    Forward(); 
    Sleep(SLEEP_TIME);
}

void Snake::Forward(){
    gotoxy(snakeX,snakeY);
    cout<<' ';
    
    tempX = snakeX;
    tempY = snakeY;
    
	if(head>(-1)){
		Enqueue();
    	Dequeue();
	}
		
	switch(direction){
        case UP:
            if(snakeY>0) snakeY--;
            break;
        case DOWN:
            if(snakeY<HEIGHT) snakeY++;
            break;      
        case LEFT:
            if(snakeX>0) snakeX--;
            break;      
        case RIGHT:
            if(snakeX<WIDTH) snakeX++;
            break;          
    }
    
	PrintBody();
	
	gotoxy(snakeX,snakeY); 
    cout<<HeadShape;
}

int Snake::IsFull(){
	if((head==tail)&&(flag==1)){
		return 1;
	}else{
		return 0;
	}
}

int Snake::IsEmpty(){
	if((head==tail)&&(flag==0)){
		return 1;
	}else{
		return 0;
	}
}

void Snake::Enqueue(){
	length++;
	if(!IsFull()){
		head = (head+1)%LENGTH;
	
		bodyX[head] = tempX;
		bodyY[head] = tempY;
	
		flag=1;
	}
	
}

void Snake::Dequeue(){
	int movedX;
	int movedY;
	
	if(!IsEmpty()){
		tail = (tail+1)%LENGTH;
		
		movedX = bodyX[tail];
		movedY = bodyY[tail];
			
		flag = 0;
	
		gotoxy(bodyX[tail],bodyY[tail]);
		cout<<' ';
	}
}

void Snake::PrintBody(){
	int i;
	
	if(!IsEmpty()){
		for(int i=tail+1;i<=head;i=(i+1)%LENGTH){
			gotoxy(bodyX[i],bodyY[i]);
			cout<<(char)BodyShape;
		}
	}
}

int Snake::CheckCollision(Egg *egg){
	if((snakeY==0)||(snakeY==HEIGHT)||(snakeX==0)||(snakeX==WIDTH)){ 
    	isAlive = 0;
	}
	
	for(int i=head;i>=tail;i--){  
        if((snakeX==bodyX[i])&&(snakeY==bodyY[i])){
            isAlive = 0;
            break;
        }
    }
    
    if((snakeX==egg->GetX())&&(snakeY==egg->GetY())){ 
    	Snake::Enqueue();
		egg->SetIsEatenTo1();
	}
    
}

int Snake::IsAlive(){
	return isAlive;
}

int Snake::GetX(){
	return snakeX;
}

int Snake::GetY(){
	return snakeY;
}

int Snake::GetbodyX(int a){
	return bodyX[a];
}

int Snake::GetbodyY(int b){
	return bodyY[b];
}

int Snake::GetHead(){
	return head;
}

int Snake::GetTail(){
	return tail;
}

/*
int Snake::GetLength(){
	return length;
}
*/

/*Egg's function*/

Egg::Egg(Snake *snake){
	eggX = rand()%(WIDTH-1)+1;
    eggY = rand()%(HEIGHT-1)+1;
        
	CheckProduce(snake);
                
	ChangeShape('E');
    gotoxy(eggX,eggY);
    cout<<shape;
}

void Egg::ChangeShape(char skin){
	shape = skin;
}

void Egg::CheckProduce(Snake *snake){
    if((eggY==0)||(eggY==HEIGHT)||(eggX==0)||(eggX==WIDTH)){
    	delete this;
    	Egg *egg = new Egg(snake);
    	gotoxy(eggX,eggY);
    	cout<<shape;
	}
	
	for(int i=snake->GetHead();i>=snake->GetTail();i--){
		if((eggX==snake->GetbodyX(i))&&(eggY==snake->GetbodyY(i))){
			delete this;
    		Egg *egg = new Egg(snake);
			gotoxy(eggX,eggY);
			cout<<shape;
			break;
		}
	}
}

int Egg::GetX(){
	return eggX;
}

int Egg::GetY(){
	return eggY;
}

int Egg::GetIsEaten(){
	return isEaten;
}

void Egg::SetIsEatenTo1(){
	isEaten = 1;
}

void Egg::SetIsEatenTo0(){
	isEaten = 0;
}

/*Wall's function*/

Wall::Wall(){
	for(int x=0;x<26;++x){
		for(int y=0;y<81;++y){
			if((x==0)||(x==25)||(y==0)||(y==80)){
				map[x][y]='+';
			}else{
				map[x][y]=' ';
			}
		}
	}
		
	for(int a=0;a<=25;a++){
		for(int b=0;b<=80;b++){
			cout<<map[a][b];
		}
		cout<<endl;
	}	
}

/*GM's function*/

GM::GM(){

	SnakeX = snake->GetX();
	SnakeY = snake->GetY();
		
	EggX = egg->GetX();
	EggY = egg->GetY();
	
	ObjectMap[SnakeX+80*(SnakeY-1)] = SNAKE;
    LinkMap[SnakeX+80*(SnakeY-1)] = snake;
				
	ObjectMap[EggX+80*(EggY-1)] = EGG;
    LinkMap[EggX+80*(EggY-1)] = egg;
	
}

GM::~GM(){
	system("cls");
	gotoxy(0,0);
	cout<<"You Dead!";
}

void GM::Detect(){
//地圖上每一格偵測
	int i=0;
	while(1){ 
		
		if(snake->IsAlive()==0){
			delete wall;
    		delete snake;
    		delete egg;
    		break;
		}
			
        switch(ObjectMap[i]){
            case SNAKE:
                LinkMap[i]->Move();
                LinkMap[i]->CheckCollision(egg);
                break;   
            case EGG:
            	if(LinkMap[i]->GetIsEaten()==1){
            		egg->SetIsEatenTo0();
            		delete egg;
            		Egg *egg = new Egg(snake);
				} 
                break;
			default:
                break;
        }
        
        i = (i+1)%LENGTH;
        
 	} 	
}

int main(){
    HideCursor();
    srand(time(NULL));
	
	GM *gm = new GM;
    gm->Detect();
    	
    delete gm;
    
    return 0;
}
