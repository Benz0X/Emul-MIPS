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
#include "common/notify.h"


int UpdateRect(SDL_Rect* rect, int x, int y, int w, int h){
	(*rect).x = x;
	(*rect).y = y;
	(*rect).w = w;
	(*rect).h = h;
	return 0;
}

int affichage(){


	char* string = "\
Français (complet)\n\
\n\
En informatique, une zone de texte ou un champ de saisie (en anglais text box) est un élément de base des interfaces graphiques (un widget ou contrôle) qui permet de proposer à l'utilisateur d'entrer du texte, par exemple pour qu'il soit ensuite utilisé par un programme, ou envoyé à un site web dans le cas d'un formulaires de page Web.\n\
Graphiquement une zone de texte ressemble à un rectangle de taille quelconque à l'intérieur duquel se trouve le texte saisi par l'utilisateur. Les bordures de la zone de texte sont en général bien mises en évidence par un effet de relief.\n\
\n\
On peut distinguer deux types de zones de texte :\n\
    les zones de texte dans lesquelles on ne peut rentrer qu'une seule ligne de texte. Il s'agit par exemple des champs de recherche comme celui d'un moteur de recherche.\n\
    les zones de texte dans lesquelles on peut rentrer un texte complet avec éventuellement des retours à la ligne. Celles-ci sont généralement équipées de barres de défilement lorsqu'elles ne sont pas assez grandes pour afficher tout le texte. Certaines d'entre elles, utilisées conjointement avec d'autres widgets,\n\
    permettent même de mettre en forme le texte saisi, c'est-à-dire de le souligner, le mettre en gras, etc.\n\
\n\
Habituellement lorsque l'utilisateur passe le pointeur de sa souris au-dessus d'une zone de texte, celui-ci change de forme (il se transforme en une sorte de I majuscule) pour indiquer à l'utilisateur qu'il peut cliquer pour que la zone de texte obtienne le focus.\n\
Si l'utilisateur effectue cette dernière opération, un curseur se met à clignoter pour lui indiquer l'endroit où le texte qu'il va éventuellement taper au clavier sera inséré.\n\
\n\
Les zones de texte sont le plus souvent vides lorsque l'utilisateur les découvre. Toutefois leur contenu peut être modifié par le programme. Cela peut permettre par exemple de remplir une zone de texte prévue pour permettre à l'utilisateur de répondre à une question dont la réponse est souvent la même.\n\
Ceci lui évitera ainsi de la remplir et donc de gagner du temps.\n\
Les zones de texte peuvent également être verrouillées en écriture pour empêcher les utilisateurs d'y écrire quoi que ce soit ou de modifier une réponse. Cette possibilité permet d'indiquer à l'utilisateur que sous certaines conditions, non remplies en l'occurrence, il pourrait modifier le texte du champs.\n\
En général, il est possible d'effectuer des opérations de copier-coller dans les zones de texte.\n\
\n\
\n\
English (short)\n\
\n\
A text box, text field or text entry box is a kind of widget used when building a graphical user interface (GUI). A text box's purpose is to allow the user to input text information to be used by the program.\n\
User-interface guidelines recommend a single-line text box when only one line of input is required, and a multi-line text box only if more than one line of input may be required. Non-editable text boxes can serve the purpose of simply displaying text.\n\
\n\
A typical text box is a rectangle of any size, possibly with a border that separates the text box from the rest of the interface. Text boxes may contain zero, one, or two scrollbars. Text boxes usually display a text cursor (commonly a blinking vertical line), indicating the current region of text being edited.\n\
It is common for the mouse cursor to change its shape when it hovers over a text box.";

//////Init
	if(SDL_Init (SDL_INIT_VIDEO)==-1){
		WARNING_MSG("Impossible d'initialiser la SDL : %s\n",SDL_GetError());
		return -1;
	}
	if(TTF_Init() == -1){
	    WARNING_MSG("Impossible d'initialiser TTF_Init : %s\n", TTF_GetError());
	    return -1;
	}

	SDL_WM_SetCaption("Emulateur MIPS",NULL);
	SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);

	SDL_Surface* ecran;
	int WINDOW_H =DEFAULT_WINDOW_H;
	int WINDOW_W =DEFAULT_WINDOW_W;

	ecran= SDL_SetVideoMode(WINDOW_W, WINDOW_H, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE); //|SDL_RESIZABLE
	
	if(ecran ==NULL){
		WARNING_MSG("Impossible d'initialiser la fenetre graphique : %s",SDL_GetError());
		return -1;
	}

	
//////Vars	
	int boucle = 1;
	int layout = 0;
	uint32_t tick;
    TTF_Font *font; font=TTF_OpenFont("default.ttf", 12);
    TTF_Font *titlefont; titlefont=TTF_OpenFont("default.ttf", 16);
    TTF_SetFontStyle(titlefont, TTF_STYLE_BOLD|TTF_STYLE_UNDERLINE);
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
	//Layout
	SDL_Rect box;

	SDL_Rect registertitle;
	SDL_Rect registertext;
	
	SDL_Rect memorytitle;
	SDL_Rect memorytext;

	SDL_Rect disasmtitle;
	SDL_Rect disasmtext;

	//Titles
	SDL_Surface* title;

	//TextEditions
	TextEdition registerte;{ 
		memset(&registerte, 0, sizeof(TextEdition));
		registerte.blitStyle = TE_BLITSTYLE_BLENDED;
	    registerte.colorBG = (SDL_Color){15,15,15,0};
	}
	TextEdition memoryte;{ 
		memset(&memoryte, 0, sizeof(TextEdition));
		memoryte.blitStyle = TE_BLITSTYLE_BLENDED;
	    memoryte.colorBG = (SDL_Color){15,15,15,0};
	}
	TextEdition disasmte;{ 
		memset(&disasmte, 0, sizeof(TextEdition));
		disasmte.blitStyle = TE_BLITSTYLE_BLENDED;
	    disasmte.colorBG = (SDL_Color){15,15,15,0};
	}  
///////////



	//SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY/100, SDL_DEFAULT_REPEAT_INTERVAL/100);
    while (boucle)
    {
    	tick = SDL_GetTicks();

    	if (layout==0){	//Initialisation or after a resize
    		//Layout
		    UpdateRect(&box, WINDOW_W/100,WINDOW_H/100,WINDOW_W-WINDOW_W/50,WINDOW_H-WINDOW_H/50);
			UpdateRect(&registertitle, box.x,box.y,box.w/2,TITLE_H);
			UpdateRect(&registertext, box.x,box.y+TITLE_H,box.w/2,box.h/2-TITLE_H);
			UpdateRect(&memorytitle, box.x,box.y+box.h/2,box.w/2,TITLE_H);
			UpdateRect(&memorytext, box.x,memorytitle.y+TITLE_H,box.w/2,box.h/2-TITLE_H);
			UpdateRect(&disasmtitle, box.x+box.w/2,box.y,box.w/2,TITLE_H);
			UpdateRect(&disasmtext, box.x+box.w/2,box.y+TITLE_H,box.w/2,box.h-TITLE_H);

			//printf("box : %d, %d, %d, %d\n", box.x, box.y, box.w, box.h);
			//printf("disasmtext : %d, %d, %d, %d\n", disasmtext.x, disasmtext.y, disasmtext.w, disasmtext.h);

			//Title Rendering
			/*
*/

			//Text Rendering
   			TE_NewTextEdition(&registerte, 1000, registertext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_BLITRGBA  | TE_STYLE_READONLY | TE_STYLE_AUTOJUMP );
   			TE_SetEditionText(&registerte,string);

   			TE_NewTextEdition(&memoryte, 5000, memorytext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA  | TE_STYLE_READONLY | TE_STYLE_AUTOJUMP );
   			TE_SetEditionText(&memoryte,string);   
  
   			TE_NewTextEdition(&disasmte, 5000, disasmtext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_HSCROLL | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA /*| TE_STYLE_JUSTDISPLAY*/ | TE_STYLE_READONLY | TE_STYLE_AUTOJUMP );
   			TE_SetEditionText(&disasmte,string);   

    		layout=1;
    	}




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
        SDL_FillRect (ecran, NULL, grey);

		//Affichage du layout
        SDL_FillRect (ecran, &registertitle, red);
        SDL_FillRect (ecran, &registertext, white);
        SDL_FillRect (ecran, &memorytitle, red);
        SDL_FillRect (ecran, &memorytext, white);
        SDL_FillRect (ecran, &disasmtitle, red);
        SDL_FillRect (ecran, &disasmtext, white);

		title = TTF_RenderText_Blended(titlefont, "Registers : ", fontblack);
		SDL_BlitSurface(title, NULL, ecran, &registertitle);
		title = TTF_RenderText_Blended(titlefont, "Memory : ", fontblack);
		SDL_BlitSurface(title, NULL, ecran, &memorytitle);
		title = TTF_RenderText_Blended(titlefont, "Main program : ", fontblack);
		SDL_BlitSurface(title, NULL, ecran, &disasmtitle);



        TE_HoldTextEdition(&registerte, event);
        TE_DisplayTextEdition(&registerte);
        TE_HoldTextEdition(&memoryte, event);
        TE_DisplayTextEdition(&memoryte);
        TE_HoldTextEdition(&disasmte, event);
        TE_DisplayTextEdition(&disasmte);
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

	SDL_FreeSurface(ecran);
	
	TTF_Quit();
	SDL_Quit();
	return 0;
}
