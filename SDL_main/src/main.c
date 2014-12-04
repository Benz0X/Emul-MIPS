#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL_ttf.h>

#include "textedition.h"





int main(){

//////Init
	if(SDL_Init (SDL_INIT_VIDEO)==-1){
		printf("Impossible d'initialiser la SDL : %s\n",SDL_GetError());
		return -1;
	}
	if(TTF_Init() == -1){
	    printf("Impossible d'initialiser TTF_Init : %s\n", TTF_GetError());
	    return -1;
	}
	if(TE_Init() == -1){
	    printf("Impossible d'initialiser TE_Init\n");
	    return -1;
	}

	SDL_WM_SetCaption("Test SDL_EnableKeyRepeat",NULL);
	//SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);

	SDL_Surface* ecran;
	int WINDOW_H =600;
	int WINDOW_W =800;

	ecran= SDL_SetVideoMode(WINDOW_W, WINDOW_H, 32, SDL_HWSURFACE|SDL_DOUBLEBUF); //|SDL_RESIZABLE
	
	if(ecran ==NULL){
		printf("Impossible d'initialiser la fenetre graphique : %s\n",SDL_GetError());
		return -1;
	}

	
//////Vars	
	int boucle = 1;
	uint32_t tick;
    TTF_Font *font; font=TTF_OpenFont("default.ttf", 12);
//////////    

//////Colors
	uint32_t white=SDL_MapRGB(ecran->format, 255,255,255);
	uint32_t black=SDL_MapRGB(ecran->format, 0,0,0);
	uint32_t grey=SDL_MapRGB(ecran->format, 70,70,70);

	uint32_t red=SDL_MapRGB(ecran->format, 255,0,0);
	uint32_t green=SDL_MapRGB(ecran->format, 0,255,0);
	uint32_t blue=SDL_MapRGB(ecran->format, 0,0,255);

	SDL_Color fontwhite=(SDL_Color){255,255,255,0};
    SDL_Color fontblack=(SDL_Color){0,0,0,0};
/////////////

//////Items
    SDL_Rect box = {100,100,600,400};

	//TextEditions
	TextEdition te;{ 
		memset(&te, 0, sizeof(TextEdition));
		te.blitStyle = TE_BLITSTYLE_BLENDED;
	    te.colorBG = (SDL_Color){15,15,15,0};
	}
///////////



	//Text Rendering
	TE_NewTextEdition(&te, 1000, box, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_BLITRGBA | TE_STYLE_AUTOJUMP );
	TE_SetEditionText(&te,"Test : ");

	SDL_EnableKeyRepeat(0*SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
   
    while (boucle)
    {
    	tick = SDL_GetTicks();

    	
		//IO Event handler
		SDL_Event event;
	    while(SDL_PollEvent(&event)){
	        switch(event.type)
	        {
	            case SDL_QUIT:
	                boucle = 0;
	                printf("QUIT\n");
	            break;

	            case SDL_KEYDOWN:
	            	switch (event.key.keysym.sym){
	            		case SDLK_ESCAPE:
	            			boucle=0;
	            			printf("QUIT\n");
	            		break;

	            	    case SDLK_RIGHT:
	            			printf("Test SDL_EnableKeyRepeat\n");
	            		break;


	            		default:
	            		break;
	            	}
	            break;

	            default:
	            	//printf("Unhandled Event : %d\n",event.type);
	            break;
	        }

	    }

///////////////Main
        SDL_FillRect (ecran, NULL, grey);

        TE_HoldTextEdition(&te, event);
        TE_DisplayTextEdition(&te);
///////////////////


/////////FPS Caping
		if((1000/60) > SDL_GetTicks() - tick){
			//printf("t: %d\n", tick );
			SDL_Delay((1000/60 - (SDL_GetTicks()-tick))*1);
		}
		//printf("FPS : %lf\n", (double)1000/(SDL_GetTicks()-tick) );
///////////////////

		//Mise a jour
		SDL_Flip(ecran);
    }


//////Quitting
    TTF_CloseFont(font);

	TE_DeleteTextEdition(&te);
	SDL_FreeSurface(ecran);
	
	TE_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}
