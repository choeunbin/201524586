#include <stdio.h>
#include <unistd.h> //for sleep()
#include <stdlib.h>
#include <time.h>   //for random number creation
#include "kbhit.h"

#define LEFT 97		//key 'a'
#define RIGHT 100	//key 'd'
#define UP 119		//key 'w'
#define DOWN 115	//key 's'
#define PAUSE 112	//key 'p'
#define ESC 27		//key 'esc'

#define MAP_Start_X 3
#define MAP_Start_Y 2
#define MAP_X 30
#define MAP_Y 20

int x[100], y[100];
int length;		//the length of snake
int speed;
int score;
int dir; 		//direction to move
int key;
int food_x, food_y;	//place where the food is 
int best_score = 0;	//the highest score
int last_score = 0;

void draw_map(void); //draw game map
void title(void);
void reset(void);
void food(void);
void pause_game(void);
void move(int dir);
void game_over(void);

void gotoxy (int x, int y, char* s){
   printf("\033[%d;%df", y, 2*x);
   fflush(stdout);
   printf("%s",s);
}

int main(){
   int x,y;
   
   title();
   

   while(1){
	if(kbhit())    key=getch(); 

	usleep(speed);

	switch(key) {
	   case LEFT:
	   case RIGHT:
	   case UP:
           case DOWN:
		if((dir==LEFT&&key!=RIGHT)||(dir==RIGHT&&key!=LEFT)||(dir==UP&&key!=DOWN)||(dir==DOWN&&key!=UP))
		   dir = key;
		key = 0; 
	   break;
	   case PAUSE:
		pause_game();
	   break;
	   case ESC:
		exit(0);
	}
	move(dir);
   }

   return 0;

}
//end of main





void draw_map(void){
   int i, j;
   for(i=0; i<MAP_X; i++){
	gotoxy(MAP_Start_X+i, MAP_Start_Y, "■  ");
   }
   for(i=MAP_Start_Y+1; i<MAP_Start_Y + MAP_Y -1; i++){
	gotoxy(MAP_Start_X, i, "■  ");
	gotoxy(MAP_Start_X + MAP_X-1, i, "■  ");
   }
   for(i=0; i<MAP_X; i++){
	gotoxy(MAP_Start_X+i, MAP_Start_Y+MAP_Y-1, "■  ");
   }
} //draw the edge of map


void title(void){
   int i, j;

   draw_map();
   for( i=MAP_Start_Y+1; i<MAP_Start_Y + MAP_Y-1; i++){
	for(j=MAP_Start_X + 1; j<MAP_Start_X+MAP_X -1; j++) gotoxy(j,i,"  ");
   }
   gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+5,"┌-------------------------┐");
   gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+6,"│        SNAKE GAME       │");
   gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+7,"└-------------------------┘");
   gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+9," <PRESS ANY KEY TO START>");
   gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+11,"  ⊙ Move  :       ↑ (w)      ");
   gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+12,"            ← (a) ↓ (s) → (d)");
   gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+13,"  ⊙ Pause : P             ");
   gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+14,"  ⊙ Quit  : ESC           ");

   while(1){
	if(kbhit()){
	   key=getch();
	   if(key==ESC) exit(0);
	   else break;
	}
	gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+9, "< PRESS ANY KET TO START >");
	usleep(400000);
	gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+9, "                          ");
	usleep(400000);

   }
   reset();
}


void reset(void){
   int i;
   system("clear");
   draw_map();
   while (kbhit()) getch();

   dir = LEFT;			//initialize the direction
   speed = 100000;		//initialize the speed
   length = 5;			//initialize the length
   score = 0;			//initialize the score to zero
   for (i=0; i<length; i++){
	x[i]=MAP_X/2 + i;
	y[i]=MAP_Y/2;
	gotoxy(MAP_Start_X+x[i], MAP_Start_Y+y[i], "◇ ");
   }	// the tail of snake

   gotoxy(MAP_Start_X + x[0], MAP_Start_Y+y[0], "⊙ ");	//the head
   food();
}

void food(void){
   int i;
   int food_crush = 0; //The value is 1 when food is on the snake
   int r =0; // value for rando number creation

   gotoxy(MAP_Start_X, MAP_Start_Y+MAP_Y, "  Your Score: ");
   printf("%3d,   Last Score: %3d,   Best Score: %3d", score, last_score, best_score);

   while(1){
	food_crush = 0;
	srand((unsigned)time(NULL)+r);
	food_x=(rand()%(MAP_X-2))+1;
	food_y=(rand()%(MAP_Y-2))+1;

	for(i=0; i<length; i++){
	   if(food_x==x[i]&&food_y==y[i]){
		food_crush = 1;
		r++;
		break;
	   }  
	}	//when eat the food
	
	if(food_crush==1) continue;

	gotoxy(MAP_Start_X+food_x, MAP_Start_Y+food_y, "♥ ");
	speed-=3000;
	break;
   }
}

void pause_game(void){
   while(1){
	if(key==PAUSE){
	   gotoxy(MAP_Start_X+(MAP_X/2)-11, MAP_Start_Y, "< PAUSE : Press Any Key To Resume the Game! >");
	   usleep(400000);
	   gotoxy(MAP_Start_X+(MAP_X/2)-11, MAP_Start_Y, "                                             ");
	   usleep(400000);
	}
	
	else{
	   draw_map();
	   return;
	}
	if(kbhit()) key=getch();
   }
}


void move(int dir){
   int i;
   
   printf("\b "); //for delete input direction
   if (x[0]==food_x&&y[0]==food_y){
	score += 10;
	food();
	length++;
	x[length-1] = x[length-2];
	y[length-1] = y[length-2];
   }
   if (x[0]==0||x[0]==MAP_X-1 || y[0]==0 || y[0]==MAP_Y-1){
	game_over();
	return;
   }
   for (i=1; i<length; i++){
	if(x[0]==x[i]&&y[0]==y[i]){
	   game_over();
	   return;
	}
   }

   gotoxy(MAP_Start_X+x[length-1], MAP_Start_Y+y[length-1], "  ");
   for(i=length-1; i>0; i--){
	x[i]=x[i-1];
	y[i]=y[i-1];
   }
   gotoxy(MAP_Start_X+x[0], MAP_Start_Y+y[0], "◇ ");
   if (dir==LEFT)	--x[0];
   if (dir==RIGHT)	++x[0];
   if (dir==UP)		--y[0];
   if (dir==DOWN)	++y[0];
   gotoxy(MAP_Start_X+x[i], MAP_Start_Y+y[i], "⊙ ");
}

void game_over(void) {
   gotoxy(MAP_Start_X+(MAP_X/2)-6, MAP_Start_Y+5,"┌----------------------┐");
   gotoxy(MAP_Start_X+(MAP_X/2)-6, MAP_Start_Y+6,"│   G A M E  O V E R   │");
   gotoxy(MAP_Start_X+(MAP_X/2)-6, MAP_Start_Y+7,"└----------------------┘");
   gotoxy(MAP_Start_X+(MAP_X/2)-5, MAP_Start_Y+8,"   Your Score : ");
   printf("%d", last_score=score);

   gotoxy(MAP_Start_X+(MAP_X/2)-8, MAP_Start_Y+12," Press Any Key to Restart the Game! ");

   if (score>best_score){
	best_score = score;
	gotoxy(MAP_Start_X+(MAP_X/2)-7, MAP_Start_Y+10," ★ You Are The Best!!★  ");
   }

   while (kbhit()) getch();
   key=getch();
   title();
}


