#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600
#define SPEED 500

typedef struct{
	SDL_Rect rect;
}Stone;

typedef struct{
	SDL_Rect rect;
}Bullet;

Bullet *loadBullet(SDL_Rect player){
	Bullet *bullet=malloc(sizeof(Bullet));
	bullet->rect.w=7; bullet->rect.h=7;
	bullet->rect.x=player.x+player.w/2-bullet->rect.w/2;
	bullet->rect.y=player.y-player.h/2+bullet->rect.h/2;
	return bullet;
}

Stone *spawnStone(int x){
	Stone *stone=malloc(sizeof(Stone));
	stone->rect.w=24; stone->rect.h=24;
	stone->rect.x=x;
	stone->rect.y=0;
	return stone;
}

void stoneMotion(Stone *stone,int score){
	stone->rect.y+=(score+SPEED-100)/60;
}

void main(){
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){
		printf("Error: %s\n",SDL_GetError());
		exit(1);
	}

	SDL_Window *win=SDL_CreateWindow("Space Shooter",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,0);
	Uint32 flags=SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;
       SDL_Renderer *rend=SDL_CreateRenderer(win,-1,flags);	

       SDL_ShowCursor(SDL_DISABLE);

       SDL_SetRenderDrawColor(rend,0,0,0,0);
       SDL_RenderClear(rend);

       SDL_Rect player;
       player.w=48; player.h=48;
       float pos_x=(WIDTH-player.w)/2; float pos_y=HEIGHT-2*player.h;

       SDL_SetRenderDrawColor(rend,168,60,50,0);
       SDL_RenderFillRect(rend,&player);
       SDL_RenderDrawRect(rend,&player);
       SDL_RenderPresent(rend);


       Stone *stone=spawnStone(100);
       Bullet *bullet;

       int close=0;
       int score=0;
       int shoot=0;
       while(!close){
	       SDL_Event e;
	       while(SDL_PollEvent(&e)){
		       switch(e.type){
			       case SDL_QUIT:
				      close=1;
				      break; 
			       case SDL_MOUSEMOTION:
				      player.x=e.motion.x;
				      break;
		               case SDL_MOUSEBUTTONDOWN:

				      shoot=1;
				      break;

		       }	
	       }

	  //     player.x=pos_x; 
	         player.y=pos_y;
	   

	       stoneMotion(stone,score);

	       if(!shoot){
		       bullet=loadBullet(player);
	       }
	       if(shoot){
	               if(bullet->rect.y>0){
				bullet->rect.y-=SPEED/20;		       
		       }
		       if(bullet->rect.y<0){
			        free(bullet);
				bullet=loadBullet(player);
		       		shoot=0;
		       }
	       }

	       if(shoot&&bullet->rect.x<=stone->rect.x+stone->rect.w && bullet->rect.x>=stone->rect.x&&bullet->rect.y<=stone->rect.y+stone->rect.h&&bullet->rect.y>=stone->rect.y-stone->rect.h){
	           score+=5;
		   free(stone);
		   srand(time(0));
       		   stone=spawnStone(rand()%WIDTH);
                   system("clear");
		   printf("score: %d\n",score);
	       }

	       if(stone->rect.y>HEIGHT){
	       	  free(stone);
                  srand(time(0));
                  stone=spawnStone(rand()%WIDTH);
		  
	       }

	       if(stone->rect.x<=player.x+player.w&&stone->rect.x>=player.x&&stone->rect.y<=player.y+player.h&&stone->rect.y>=player.y){
	       	close=1;

	       }

	       SDL_SetRenderDrawColor(rend,0,0,0,0);
	       SDL_RenderClear(rend);
	       
	       SDL_SetRenderDrawColor(rend,66, 135, 245,0);
	       SDL_RenderFillRect(rend,&stone->rect);
	       SDL_RenderDrawRect(rend,&stone->rect);

	       SDL_SetRenderDrawColor(rend,205, 219, 44,0);
	       SDL_RenderFillRect(rend,&bullet->rect);
	       SDL_RenderDrawRect(rend,&bullet->rect);
	       //SDL_RenderPresent(rend); 
  	       SDL_SetRenderDrawColor(rend,168,60,50,0);
	       SDL_RenderFillRect(rend,&player);
	       SDL_RenderDrawRect(rend,&player);
	       SDL_RenderPresent(rend);

	       SDL_Delay(1000/60);
       }

//       SDL_Delay(5000);

       SDL_DestroyRenderer(rend);
       SDL_DestroyWindow(win);
       SDL_Quit();
}
