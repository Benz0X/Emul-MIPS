#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_draw.h>
#include <SDL_phelma.h>

#include "textedition.h"


#include "affichage.h"
#include "define.h"
#include "fonctions.h"
#include "user_int.h"
#include "common/notify.h"


int UpdateRect(SDL_Rect* rect, int x, int y, int w, int h){
	(*rect).x = x;
	(*rect).y = y;
	(*rect).w = w;
	(*rect).h = h;
	return 0;
}

int affichage(){

//////Init
	if(SDL_Init (SDL_INIT_VIDEO)==-1){
		WARNING_MSG("Impossible d'initialiser la SDL : %s\n",SDL_GetError());
		return -1;
	}
	if(TTF_Init() == -1){
	    WARNING_MSG("Impossible d'initialiser TTF_Init : %s\n", TTF_GetError());
	    return -1;
	}
	if(TE_Init() == -1){
	    WARNING_MSG("Impossible d'initialiser TTF_Init : %s\n", TTF_GetError());
	    return -1;
	}

	SDL_WM_SetCaption("Emulateur MIPS",NULL);
	SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);

	SDL_Surface* ecran;
	int WINDOW_H =DEFAULT_WINDOW_H;
	int WINDOW_W =DEFAULT_WINDOW_W;

	ecran= SDL_SetVideoMode(WINDOW_W, WINDOW_H, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE); //|SDL_RESIZABLE
	int test= SDL_EnableKeyRepeat(0*SDL_DEFAULT_REPEAT_DELAY/100, SDL_DEFAULT_REPEAT_INTERVAL/100);
	if(ecran ==NULL){
		WARNING_MSG("Impossible d'initialiser la fenetre graphique : %s",SDL_GetError());
		return -1;
	}

	
//////Vars	
	int boucle = 1;
	int layout = 0;
	uint32_t tick;
    TTF_Font *font; font=TTF_OpenFont("UbuntuMono-R.ttf", 14);
    TTF_Font *titlefont; titlefont=TTF_OpenFont("test.otf", 18);
    TTF_SetFontStyle(titlefont, TTF_STYLE_BOLD/*|TTF_STYLE_UNDERLINE*/);
    char input[INPUT_SIZE];
    char normalized_input[INPUT_SIZE];
    int res = 0;
//////////    

//////Colors
	uint32_t white=SDL_MapRGB(ecran->format, 255,255,255);
	uint32_t black=SDL_MapRGB(ecran->format, 0,0,0);
	uint32_t grey=SDL_MapRGB(ecran->format, 70,70,70);
	uint32_t softgrey=SDL_MapRGB(ecran->format, 150,150,150);

	uint32_t red=SDL_MapRGB(ecran->format, 255,0,0);
	uint32_t green=SDL_MapRGB(ecran->format, 0,255,0);
	uint32_t blue=SDL_MapRGB(ecran->format, 0,0,255);

	SDL_Color fontwhite=(SDL_Color){255,255,255,0};
    SDL_Color fontblack=(SDL_Color){0,0,0,0};
/////////////

//////Items
	//Layout
	SDL_Rect box;

	SDL_Rect pipelinetitle;
	SDL_Rect pipelinetext;

	SDL_Rect registertitle;
	SDL_Rect registertext;
	
	SDL_Rect memorytitle;
	SDL_Rect memorytext;

	SDL_Rect disasmtitle;
	SDL_Rect disasmtext;

	SDL_Rect consoletitle;
	SDL_Rect consoletext;

	//Titles
	SDL_Surface* title;

	//TextEditions
	TextEdition pipelinete;{ 
		memset(&pipelinete, 0, sizeof(TextEdition));
		pipelinete.blitStyle = TE_BLITSTYLE_BLENDED;
	    pipelinete.colorBG = (SDL_Color){15,15,15,0};
	    pipelinete.colorFGSelect = fontwhite;
	}
	TextEdition registerte;{ 
		memset(&registerte, 0, sizeof(TextEdition));
		registerte.blitStyle = TE_BLITSTYLE_BLENDED;
	    registerte.colorBG = (SDL_Color){15,15,15,0};
	    registerte.colorFGSelect = fontwhite;
	}
	TextEdition memoryte;{ 
		memset(&memoryte, 0, sizeof(TextEdition));
		memoryte.blitStyle = TE_BLITSTYLE_BLENDED;
	    memoryte.colorBG = (SDL_Color){15,15,15,0};
	    memoryte.colorFGSelect = fontwhite;
	}
	TextEdition disasmte;{ 
		memset(&disasmte, 0, sizeof(TextEdition));
		disasmte.blitStyle = TE_BLITSTYLE_BLENDED;
	    disasmte.colorBG = (SDL_Color){15,15,15,0};
	    disasmte.colorFGSelect = fontwhite;
	} 
	TextEdition consolete;{ 
		memset(&consolete, 0, sizeof(TextEdition));
		consolete.blitStyle = TE_BLITSTYLE_BLENDED;
	    consolete.colorBG = (SDL_Color){15,15,15,0};
	    consolete.colorFGSelect = fontwhite;
	}  
///////////


    
    while (boucle)
    {
    	tick = SDL_GetTicks();

    	if (layout==0){	//Initialisation or after a resize
    		//Layout
		    UpdateRect(&box, OFFSET,30,WINDOW_W-2*OFFSET,WINDOW_H-30-OFFSET-TITLE_H-CONSOLE_H);

		    UpdateRect(&pipelinetitle, box.x,box.y,box.w,TITLE_H);
			UpdateRect(&pipelinetext, box.x,box.y+TITLE_H,box.w,(box.h*15/100)-TITLE_H);

			UpdateRect(&registertitle, box.x,pipelinetext.y+pipelinetext.h+(box.h*1/100),box.w/2,TITLE_H);
			UpdateRect(&registertext, box.x,registertitle.y+registertitle.h,box.w/2,(box.h*41/100)-TITLE_H);

			UpdateRect(&memorytitle, box.x,registertext.y+registertext.h,box.w/2,TITLE_H);
			UpdateRect(&memorytext, box.x,memorytitle.y+TITLE_H,box.w/2,(box.h*41/100)-TITLE_H);

			UpdateRect(&disasmtitle, box.x+box.w/2,pipelinetext.y+pipelinetext.h+(box.h*1/100),box.w/2,TITLE_H);
			UpdateRect(&disasmtext, box.x+box.w/2,disasmtitle.y+disasmtitle.h,box.w/2,(box.h*82/100)-TITLE_H-1);

			UpdateRect(&consoletitle, box.x,WINDOW_H-TITLE_H-CONSOLE_H-5,box.w,TITLE_H);
			UpdateRect(&consoletext, box.x,WINDOW_H-CONSOLE_H-5,box.w,CONSOLE_H);

			//printf("box : %d, %d, %d, %d\n", box.x, box.y, box.w, box.h);
			//printf("disasmtext : %d, %d, %d, %d\n", disasmtext.x, disasmtext.y, disasmtext.w, disasmtext.h);



			//Text Rendering
			TE_NewTextEdition(&pipelinete, 1024, pipelinetext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_BLITRGBA  | TE_STYLE_READONLY | TE_STYLE_AUTOJUMP );
   			TE_SetEditionText(&pipelinete,"pipeline");

   			TE_NewTextEdition(&registerte, 1024, registertext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_BLITRGBA  | TE_STYLE_READONLY | TE_STYLE_AUTOJUMP );
   			TE_SetEditionText(&registerte,"register");

   			TE_NewTextEdition(&memoryte, 4096, memorytext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA  | TE_STYLE_READONLY | TE_STYLE_AUTOJUMP );
   			TE_SetEditionText(&memoryte,"memory");   
  
   			TE_NewTextEdition(&disasmte, 4096, disasmtext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_HSCROLL | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA /*| TE_STYLE_JUSTDISPLAY* | TE_STYLE_READONLY */| TE_STYLE_AUTOJUMP );
   			TE_SetEditionText(&disasmte,"disasm");   

   			TE_NewTextEdition(&consolete, 1024, consoletext, font, fontblack, TE_STYLE_HSCROLL | TE_STYLE_BLITRGBA );
   			TE_SetEditionText(&consolete,"console");  

    		layout=1;
    	}




		//IO Event handler
		SDL_Event event;
	    while(SDL_PollEvent(&event)){
	        switch(event.type)
	        {
	            case SDL_QUIT:
	                boucle = 0;
	                INFO_MSG("Window Closed");
	            break;

	            case SDL_KEYDOWN:
	            	switch (event.key.keysym.sym){
	            		case SDLK_ESCAPE:
	            			boucle=0;
	            			printf("QUIT\n");
	            		break;

	            		case SDLK_UP:
	            			//printf("TEST\n");
	            		break;

	            		case SDLK_RETURN:
	            		case SDLK_KP_ENTER:
	            			strcpy(input,consolete.text);
	            			TE_SetEditionText(&consolete,"");  


	            			INFO_MSG("Console entry : %s", input);
	            			string_standardise(input,normalized_input);     //On normalise l'entree - echappement, commentaires, etc
           					string_standardise(normalized_input,input);
           					res=decrypt(input);
	            		break;

	            		default:
	            		break;
	            	}
	            break;

	            case SDL_VIDEORESIZE:
	            	//printf("W: %d\tH: %d\n",event.resize.w,event.resize.h);
	            	WINDOW_W=event.resize.w;
	            	WINDOW_H=event.resize.h;
	            	ecran= SDL_SetVideoMode(WINDOW_W, WINDOW_H, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE); //|SDL_RESIZABLE

	            	//Redefinition du layout
	            	layout=0;
	            	SDL_Flip(ecran);
	            break;

	            default:
	            	//printf("Unhandled Event : %d\n",event.type);
	            break;
	        }

	    }

///////////////Main
	    //Background
        SDL_FillRect (ecran, NULL, softgrey);
		

		//Rectangles
		SDL_FillRect (ecran, &pipelinetitle, white);
        SDL_FillRect (ecran, &pipelinetext, white);
		SDL_FillRect (ecran, &registertitle, white);
        SDL_FillRect (ecran, &registertext, white);
        SDL_FillRect (ecran, &memorytitle, white);
        SDL_FillRect (ecran, &memorytext, white);
        SDL_FillRect (ecran, &disasmtitle, white);
        SDL_FillRect (ecran, &disasmtext, white);
        SDL_FillRect (ecran, &consoletitle, white);
        SDL_FillRect (ecran, &consoletext, white);

                

        //Lines
		Draw_Rect (ecran, pipelinetitle.x, pipelinetitle.y, pipelinetitle.w-1, pipelinetitle.h-1, black);
        Draw_Rect (ecran, pipelinetext.x, pipelinetext.y, pipelinetext.w-1, pipelinetext.h-1, black);
		Draw_Rect (ecran, registertitle.x, registertitle.y, registertitle.w-1, registertitle.h-1, black);
        Draw_Rect (ecran, registertext.x,registertext.y,registertext.w-1,registertext.h-1, black);
        Draw_Rect (ecran, memorytitle.x, memorytitle.y, memorytitle.w-1, memorytitle.h-1, black);
        Draw_Rect (ecran, memorytext.x,memorytext.y,memorytext.w-1,memorytext.h-1, black);
        Draw_Rect (ecran, disasmtitle.x, disasmtitle.y, disasmtitle.w-1, disasmtitle.h-1, black);
        Draw_Rect (ecran, disasmtext.x,disasmtext.y,disasmtext.w-1,disasmtext.h-1, black);
        Draw_Rect (ecran, consoletitle.x, consoletitle.y, consoletitle.w-1, consoletitle.h-1, black);
        Draw_Rect (ecran, consoletext.x,consoletext.y,consoletext.w-1,consoletext.h-1, black);

        //Fill back blanks
        Draw_Rect(ecran, pipelinetitle.x+1, pipelinetitle.y+pipelinetitle.h-2 ,pipelinetitle.w-3,2,white);
        Draw_Rect(ecran, registertitle.x+1, registertitle.y+registertitle.h-2 ,registertitle.w-3,2,white);
        Draw_Rect(ecran, disasmtitle.x+1, disasmtitle.y+disasmtitle.h-2 ,disasmtitle.w-3,2,white);
        Draw_Rect(ecran, memorytitle.x+1, memorytitle.y+memorytitle.h-2 ,memorytitle.w-3,2,white);
        Draw_Rect(ecran, consoletitle.x+1, consoletitle.y+consoletitle.h-2 ,consoletitle.w-3,2,white);

        //Titles
		title = TTF_RenderText_Blended(titlefont, " Pipeline State : ", fontblack);
		SDL_BlitSurface(title, NULL, ecran, &pipelinetitle);
		title = TTF_RenderText_Blended(titlefont, " Registers : ", fontblack);
		SDL_BlitSurface(title, NULL, ecran, &registertitle);
		title = TTF_RenderText_Blended(titlefont, " Memory : ", fontblack);
		SDL_BlitSurface(title, NULL, ecran, &memorytitle);
		title = TTF_RenderText_Blended(titlefont, " Main program : ", fontblack);
		SDL_BlitSurface(title, NULL, ecran, &disasmtitle);
		title = TTF_RenderText_Blended(titlefont, " Console : ", fontblack);
		SDL_BlitSurface(title, NULL, ecran, &consoletitle);

		//TextEdition
        TE_HoldTextEdition(&pipelinete, event);
        TE_DisplayTextEdition(&pipelinete);
        TE_HoldTextEdition(&registerte, event);
        TE_DisplayTextEdition(&registerte);
        TE_HoldTextEdition(&memoryte, event);
        TE_DisplayTextEdition(&memoryte);
        TE_HoldTextEdition(&disasmte, event);
        TE_DisplayTextEdition(&disasmte);
        TE_HoldTextEdition(&registerte, event);
        TE_DisplayTextEdition(&registerte);
        TE_HoldTextEdition(&consolete, event);
        TE_DisplayTextEdition(&consolete);
///////////////////


/////////FPS Caping
		if((1000/MAX_FPS) > SDL_GetTicks() - tick){
			//printf("t: %d\n", tick );
			SDL_Delay((1000/MAX_FPS - (SDL_GetTicks()-tick))*1);
		}
		//printf("FPS : %lf\n", (double)1000/(SDL_GetTicks()-tick) );
///////////////////

		//Mise a jour
		SDL_Flip(ecran);
    }


//////Quitting
    TTF_CloseFont(font);
    TTF_CloseFont(titlefont);

	TE_DeleteTextEdition(&registerte);
	TE_DeleteTextEdition(&memoryte);
	TE_DeleteTextEdition(&disasmte);

	SDL_FreeSurface(title);
	SDL_FreeSurface(ecran);
	
	TE_Quit();
	TTF_Quit();
	SDL_Quit();
	return res;
}
