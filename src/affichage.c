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




int affichage() {

//////Init
    if(SDL_Init (SDL_INIT_VIDEO)==-1) {
        WARNING_MSG("Impossible d'initialiser la SDL : %s\n",SDL_GetError());
        return -1;
    }
    if(TTF_Init() == -1) {
        WARNING_MSG("Impossible d'initialiser TTF_Init : %s\n", TTF_GetError());
        return -1;
    }
    if(TE_Init() == -1) {
        WARNING_MSG("Impossible d'initialiser TTF_Init : %s\n", TTF_GetError());
        return -1;
    }

    SDL_WM_SetCaption("Emulateur MIPS",NULL);
    SDL_WM_SetIcon(SDL_LoadBMP(ICON), NULL);

    SDL_Surface* ecran;
    int WINDOW_H =DEFAULT_WINDOW_H;
    int WINDOW_W =DEFAULT_WINDOW_W;

    ecran= SDL_SetVideoMode(WINDOW_W, WINDOW_H, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE); //|SDL_RESIZABLE
    if(ecran ==NULL) {
        WARNING_MSG("Impossible d'initialiser la fenetre graphique : %s",SDL_GetError());
        return -1;
    }


//////Vars
    SDL_Event event;
    int boucle = 1;
    int layout = 0;
    uint32_t tick;

    TTF_Font *font;
    font=TTF_OpenFont("UbuntuMono-R.ttf", 14);
    TTF_Font *titlefont;
    titlefont=TTF_OpenFont("test.otf", 18);
    TTF_SetFontStyle(titlefont, TTF_STYLE_BOLD/*|TTF_STYLE_UNDERLINE*/);
    char input[INPUT_SIZE];
    char normalized_input[INPUT_SIZE];

    int res = 0;

    char string_pipeline [32768];
    char string_registers [32768];
    char string_disasm [32768];
    char string_memory [32768];
//////////

//////Colors
    uint32_t white=SDL_MapRGB(ecran->format, 255,255,255);
    uint32_t black=SDL_MapRGB(ecran->format, 0,0,0);
    uint32_t grey=SDL_MapRGB(ecran->format, 70,70,70);
    uint32_t softgrey=SDL_MapRGB(ecran->format, 150,150,150);

    uint32_t red=SDL_MapRGB(ecran->format, 255,0,0);
    uint32_t green=SDL_MapRGB(ecran->format, 0,255,0);
    uint32_t blue=SDL_MapRGB(ecran->format, 0,0,255);

    SDL_Color fontwhite=(SDL_Color) {
        255,255,255,0
    };
    SDL_Color fontblack=(SDL_Color) {
        0,0,0,0
    };
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
    SDL_Rect titlerect;


    //TextEditions definitions
    TextEdition pipelinete;
    {
        memset(&pipelinete, 0, sizeof(TextEdition));
        pipelinete.blitStyle = TE_BLITSTYLE_BLENDED;
        pipelinete.colorBG = (SDL_Color) {
            15,15,15,0
        };
        pipelinete.colorFGSelect = fontwhite;
    }
    TextEdition registerte;
    {
        memset(&registerte, 0, sizeof(TextEdition));
        registerte.blitStyle = TE_BLITSTYLE_BLENDED;
        registerte.colorBG = (SDL_Color) {
            15,15,15,0
        };
        registerte.colorFGSelect = fontwhite;
    }
    TextEdition memoryte;
    {
        memset(&memoryte, 0, sizeof(TextEdition));
        memoryte.blitStyle = TE_BLITSTYLE_BLENDED;
        memoryte.colorBG = (SDL_Color) {
            15,15,15,0
        };
        memoryte.colorFGSelect = fontwhite;
    }
    TextEdition disasmte;
    {
        memset(&disasmte, 0, sizeof(TextEdition));
        disasmte.blitStyle = TE_BLITSTYLE_BLENDED;
        disasmte.colorBG = (SDL_Color) {
            15,15,15,0
        };
        disasmte.colorFGSelect = fontwhite;
    }
    TextEdition consolete;
    {
        memset(&consolete, 0, sizeof(TextEdition));
        consolete.blitStyle = TE_BLITSTYLE_BLENDED;
        consolete.colorBG = (SDL_Color) {
            15,15,15,0
        };
        consolete.colorFGSelect = fontwhite;
    }
///////////


    //Nested Function pour mise à jour du texte
    void redraw () {
        //Mise à jour de l'affichage
        stringPipeline (string_pipeline);
        stringRegisters (string_registers);
        stringDisasm (textStart,textEnd-textStart+1,string_disasm);
        stringMemory (string_memory);

        TE_SetEditionText(&pipelinete,string_pipeline);
        TE_SetEditionText(&registerte,string_registers);
        TE_SetEditionText(&disasmte,string_disasm);
        TE_SetEditionText(&memoryte,string_memory);
    }





    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_SetEventFilter(FilterEvents);
    while (boucle)
    {
        tick = SDL_GetTicks();

        if (layout==0) {	//Initialisation or after a resize


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



            //Text Rendering
            TE_NewTextEdition(&pipelinete, 32768, pipelinetext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_BLITRGBA  | TE_STYLE_READONLY /*| TE_STYLE_AUTOJUMP*/ );
            TE_NewTextEdition(&registerte, 32768, registertext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_BLITRGBA  | TE_STYLE_READONLY | TE_STYLE_AUTOJUMP );
            TE_NewTextEdition(&memoryte, 32768, memorytext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA  | TE_STYLE_READONLY );
            TE_NewTextEdition(&disasmte, 32768, disasmtext, font, fontblack, TE_STYLE_MULTILINE | TE_STYLE_HSCROLL | TE_STYLE_VSCROLL | TE_STYLE_BLITRGBA /*| TE_STYLE_JUSTDISPLAY* | TE_STYLE_READONLY */);
            TE_NewTextEdition(&consolete, 1024, consoletext, font, fontblack, TE_STYLE_HSCROLL | TE_STYLE_BLITRGBA );

            redraw();

            layout=1;
        }




        //IO Event handler
        while(SDL_WaitEvent(&event)) {
            switch(event.type)
            {
            case SDL_QUIT:
                boucle = 0;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    boucle=0;
                    break;

                case SDLK_UP:
                    //printf("TEST\n");
                    break;

                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    strcpy(input,consolete.text);
                    TE_SetEditionText(&consolete,"");


                    //Envoi de la commande
                    string_standardise(input,normalized_input);     //On normalise l'entree - echappement, commentaires, etc
                    string_standardise(normalized_input,input);

                    printf("%s%s\n",PROMPT_STRING, input );
                    if(strcmp(input,"")!=0) res=decrypt(input); 	//Anti-Sigsegv
                    redraw();
                    break;

                case SDLK_F1:
                    strcpy(input,"run");
                    printf("%s%s\n",PROMPT_STRING, input );
                    decrypt(input);
                    redraw();
                    break;

                case SDLK_F2:
                    strcpy(input,"step");
                    printf("%s%s\n",PROMPT_STRING, input );
                    decrypt(input);
                    redraw();
                    break;

                case SDLK_F3:
                    strcpy(input,"step into");
                    printf("%s%s\n",PROMPT_STRING, input );
                    decrypt(input);
                    redraw();
                    break;

                default:
                    break;
                }
                break;

            case SDL_VIDEORESIZE:
                //printf("W: %d\tH: %d\n",event.resize.w,event.resize.h);
                WINDOW_W=event.resize.w;
                WINDOW_H=event.resize.h;

                if(WINDOW_W>MIN_WINDOW_W || WINDOW_H>MIN_WINDOW_H) {	//Taille minimale
                    //Redefinition du layout

                    ecran= SDL_SetVideoMode(WINDOW_W, WINDOW_H, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE); //|SDL_RESIZABLE
                    layout=0;

                }
                break;

                //default:
                //printf("Unhandled Event : %d\n",event.type);
                //break;
            }
            break;
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
        Draw_Line (ecran, pipelinetext.x, pipelinetext.y+3*15, pipelinetext.x+pipelinetext.w-2,pipelinetext.y+3*15, grey);	//STAGE Separation line (horizontal)

        Draw_Rect (ecran, registertitle.x, registertitle.y, registertitle.w-1, registertitle.h-1, black);
        Draw_Rect (ecran, registertext.x,registertext.y,registertext.w-1,registertext.h-1, black);

        Draw_Rect (ecran, consoletitle.x, consoletitle.y, consoletitle.w-1, consoletitle.h-1, black);
        Draw_Rect (ecran, consoletext.x,consoletext.y,consoletext.w-1,consoletext.h-1, black);

        Draw_Rect (ecran, memorytitle.x, memorytitle.y, memorytitle.w-1, memorytitle.h-1, black);
        Draw_Rect (ecran, memorytext.x,memorytext.y,memorytext.w-1,memorytext.h-1, black);
        Draw_Line (ecran, memorytext.x+82,memorytext.y,memorytext.x+82,memorytext.y+memorytext.h-2, grey);	//ADDRESS Separation line

        Draw_Rect (ecran, disasmtitle.x, disasmtitle.y, disasmtitle.w-1, disasmtitle.h-1, black);
        Draw_Rect (ecran, disasmtext.x,disasmtext.y,disasmtext.w-1,disasmtext.h-1, black);
        Draw_Line (ecran, disasmtext.x+20,disasmtext.y,disasmtext.x+20,disasmtext.y+disasmtext.h-2, grey);	//Break Separation line
        Draw_Line (ecran, disasmtext.x+50,disasmtext.y,disasmtext.x+50,disasmtext.y+disasmtext.h-2, grey);	//PC Separation line
        Draw_Line (ecran, disasmtext.x+90,disasmtext.y,disasmtext.x+90,disasmtext.y+disasmtext.h-2, grey);	//ADDRESS Separation line

        //Fill back blanks
        Draw_Rect(ecran, pipelinetitle.x+1, pipelinetitle.y+pipelinetitle.h-2 ,pipelinetitle.w-3,2,white);
        Draw_Rect(ecran, registertitle.x+1, registertitle.y+registertitle.h-2 ,registertitle.w-3,2,white);
        Draw_Rect(ecran, disasmtitle.x+1, disasmtitle.y+disasmtitle.h-2 ,disasmtitle.w-3,2,white);
        Draw_Rect(ecran, memorytitle.x+1, memorytitle.y+memorytitle.h-2 ,memorytitle.w-3,2,white);
        Draw_Rect(ecran, consoletitle.x+1, consoletitle.y+consoletitle.h-2 ,consoletitle.w-3,2,white);

        //Titles
        /*
        RenderText (ecran, pipelinetitle.x,pipelinetitle.y,pipelinetitle.w,pipelinetitle.h, " Pipeline State : ", titlefont, fontblack);
        RenderText (ecran, registertitle.x,registertitle.y,registertitle.w,registertitle.h, " Registers : ", titlefont, fontblack);
        RenderText (ecran, memorytitle.x,memorytitle.y,memorytitle.w,memorytitle.h, " Memory : ", titlefont, fontblack);
        RenderText (ecran, disasmtitle.x,disasmtitle.y,disasmtitle.w,disasmtitle.h, " Main Program : ", titlefont, fontblack);
        RenderText (ecran, consoletitle.x,consoletitle.y,consoletitle.w,consoletitle.h, " Console : ", titlefont, fontblack);
        */
        title = TTF_RenderText_Blended(titlefont, " Pipeline State : ", fontblack);
        SDL_BlitSurface(title, NULL, ecran, &pipelinetitle);
        SDL_FreeSurface(title);
        title = TTF_RenderText_Blended(titlefont, " Registers : ", fontblack);
        SDL_BlitSurface(title, NULL, ecran, &registertitle);
        SDL_FreeSurface(title);
        title = TTF_RenderText_Blended(titlefont, " Memory : ", fontblack);
        SDL_BlitSurface(title, NULL, ecran, &memorytitle);
        SDL_FreeSurface(title);
        title = TTF_RenderText_Blended(titlefont, " Main program : ", fontblack);
        SDL_BlitSurface(title, NULL, ecran, &disasmtitle);
        SDL_FreeSurface(title);
        title = TTF_RenderText_Blended(titlefont, " Console : ", fontblack);
        SDL_BlitSurface(title, NULL, ecran, &consoletitle);
        SDL_FreeSurface(title);

        //Buttons
        Button (ecran, 5,  5,  65, 20, event, 0);
        RenderText (ecran, 8,7,60,20, " F1 Run ", font, fontblack);

        Button (ecran, 80,  5,  65, 20, event, 1);
        RenderText (ecran, 80,7,60,20, " F2 Step ", font, fontblack);

        Button (ecran, 155,  5,  65, 20, event, 2);
        RenderText (ecran, 155,7,65,20, " F3 Into ", font, fontblack);



        //TextEditions Event handlers
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
        if((1000/MAX_FPS) > SDL_GetTicks() - tick) {
            //printf("t: %d\n", tick );
            SDL_Delay((1000/MAX_FPS - (SDL_GetTicks()-tick))*1);
        }
        //printf("FPS : %lf\n", (double)1000/(SDL_GetTicks()-tick) );
///////////////////


        if (res==2) {
            break;	//Sortie du programme
        }


        //Mise a jour
        SDL_Flip(ecran);
    }


//////Quitting
    TTF_CloseFont(font);
    TTF_CloseFont(titlefont);

    TE_DeleteTextEdition(&pipelinete);
    TE_DeleteTextEdition(&registerte);
    TE_DeleteTextEdition(&memoryte);
    TE_DeleteTextEdition(&disasmte);
    TE_DeleteTextEdition(&consolete);

    SDL_FreeSurface(ecran);

    TE_Quit();
    TTF_Quit();
    SDL_Quit();

    INFO_MSG("Window Closed");
    return res;
}









int RenderText (SDL_Surface* ecran, int x,int y,int w,int h, char* string, TTF_Font* font, SDL_Color fontcolor) {
    SDL_Surface* title;
    SDL_Rect titlerect;

    UpdateRect(&titlerect, x,y,w,h);
    title = TTF_RenderText_Blended(font, string, fontcolor);
    SDL_BlitSurface(title, NULL, ecran, &titlerect);
    SDL_FreeSurface(title);

    return 0;
}






//Fonctions
int FilterEvents(const SDL_Event *event) {
    int x,y;
    if ( event->type == SDL_MOUSEMOTION && !(SDL_GetMouseState(&x, &y)&SDL_BUTTON(SDL_BUTTON_LEFT)) ) {
        return(0);    /* Drop it, we've handled it */
    }
    return(1);
}



int Button (SDL_Surface* ecran, int x, int y, int w, int h, SDL_Event event, int fn ) {
//Colors
    uint32_t color=SDL_MapRGB(ecran->format, 200,200,200);
    uint32_t hovercolor=SDL_MapRGB(ecran->format, 80,80,80);
    uint32_t clickedcolor=SDL_MapRGB(ecran->format, 50,50,50);

    uint32_t white=SDL_MapRGB(ecran->format, 255,255,255);
    uint32_t black=SDL_MapRGB(ecran->format, 0,0,0);
    SDL_Color fontblack=(SDL_Color) {
        0,0,0,0
    };
///////


    Draw_FillRect (ecran, x,y,w,h, color);
    Draw_Rect (ecran, x,y,w,h, white);
    Draw_Rect (ecran, x,y,w-1,h-1, black);

    //SDL_Surface* title;
    //title = TTF_RenderText_Blended(titlefont, " Text ", fontblack);
    //SDL_BlitSurface(title, NULL, ecran, {x,y,w,h});


//Button
    int posx, posy;
    char input[10];
    switch(event.type)
    {
    case SDL_MOUSEMOTION:
        posx = event.motion.x;
        posy = event.motion.y;

        if(posx > x && posx < x+w && posy > y && posy < y+h) {
            Draw_FillRect (ecran, x+1,y+1,w-2,h-2, hovercolor);

            if(SDL_GetMouseState(&posx, &posy)&SDL_BUTTON(SDL_BUTTON_LEFT)) {
                Draw_FillRect (ecran, x+1,y+1,w-2,h-2, clickedcolor);
                switch (fn) {
                case 0:
                    strcpy(input,"run");
                    break;

                case 1:
                    strcpy(input,"step");
                    break;

                case 2:
                    strcpy(input,"step into");
                    break;
                }
                printf("%s%s\n",PROMPT_STRING, input );
                return decrypt(input);
            }
        }
        break;
    }
    return 0;
}











int UpdateRect(SDL_Rect* rect, int x, int y, int w, int h) {
    (*rect).x = x;
    (*rect).y = y;
    (*rect).w = w;
    (*rect).h = h;
    return 0;
}

int stringRegisters(char* string) {
    int i;
    char name[INPUT_SIZE];
    char tmp[50];
    sprintf(string,"");	//Remise à 0

    for(i=0; i<NBREG+2; i++) {
        parseReg(i,name);               //Recuperation du nom complet
        if(i%4==0) {
            strcat(string, "\n");   //Affichage 4 par ligne
        }
        sprintf(tmp, "%5s: %-5d ",name,reg_mips[i]);   //Affichage du registre
        strcat(string,tmp);
    }
    parseReg(PC,name);
    sprintf(tmp, "%5s: 0x%X ",name,reg_mips[PC]);   //Affichage du PC en hexa
    strcat(string,tmp);
    return 0;
}

int stringMemory(char* string) {
    char tmp[128];
    sprintf(string,"");	//Remise à 0

    if(memory==NULL) {
        sprintf(string,"            No memory loaded");
        return -1;
    }

    int k;
    uint32_t start_addr, size;
    uint32_t i=0;
    uint32_t current_addr;
    int value;


    for (k = 0; k < memory->nseg; k++) {

        if(memory->seg[k].name[0]!='l' && memory->seg[k].name[0]!='[' ) {	//Affichage de libc.seg et de [seg] désactivé
            start_addr = memory->seg[k].start._32;
            size = memory->seg[k].size._32;

            sprintf(tmp,"\n\n  %s", memory->seg[k].name);
            strcat(string,tmp);
            current_addr=start_addr;
            i=0;

            while (i<size) {	//Affichage du segment
                if (i%16==0) {
                    sprintf(tmp,"\n 0x%8.8X  ",current_addr);
                    strcat(string,tmp);
                }

                if(memRead(current_addr,0,&value)==0) { //on verifie qu'il soit dans une plage memoire valide
                    sprintf(tmp,"%2.2X ",value);
                    strcat(string,tmp);
                }
                else strcat(string,"   ");

                current_addr++;
                i++;
            }


        }
    }
    return 0;
}

int stringPipeline(char* string) {
    if(memory==NULL) {
        sprintf(string,"  No memory loaded");
        return -1;
    }

    char tmp[128];
    sprintf(string,"");	//Remise à 0

    //Affichage des differents blocks
    sprintf(string,"  Stage Fetch\t          Stage Decode\t          Stage Execute\t          Stage Memory\t          Stage Write Back\n");

    sprintf(tmp,"  %8.8X\t             %8.8X\t              %8.8X\t               %8.8X\t              %8.8X\n",vpipeline[IF].ins.value,vpipeline[ID].ins.value,vpipeline[EX].ins.value,vpipeline[MEM].ins.value,vpipeline[WB].ins.value);
    strcat(string,tmp);

    sprintf(tmp,"          \t           ->Decoding\t              %s\t                    \t %s\t                \t%s\n\n",dico_data[vpipeline[EX].dico_entry].name,dico_data[vpipeline[MEM].dico_entry].name,dico_data[vpipeline[WB].dico_entry].name);
    strcat(string,tmp);


    sprintf(tmp,"  Temporary Values :                \t              %d\t                  \t %d\t                \t%d\n\n",vpipeline[EX].tmp,vpipeline[MEM].tmp,vpipeline[WB].tmp);
    strcat(string,tmp);

    sprintf(tmp,"  Clock Count : %d      Clock time : %d\n", nbcycle, clocktime);
    strcat(string,tmp);

    return 0;
}

int stringDisasm(uint32_t start_addr,uint32_t size, char* string) {
    char tmp[128];
    sprintf(string,"");	//Remise à 0


    if(memory==NULL) {
        sprintf(string,"             No memory loaded");
        return -1;
    }
    if (start_addr%4!=0) WARNING_MSG("%X mod 4 != 0, read instruction may not be alligned with real instruction",start_addr);
    uint32_t i=0;
    uint32_t current_addr=start_addr;
    instruction current_instr;
    uint32_t instr_value;
    int text_ident,libctext_ident;
    int seg,k;

    //get .text scnidx in order to show only .text label
    for (k = 1; k < symtab.size; ++k) {
        if(!strcmp(symtab.sym[k].name,".text")) {
            text_ident=symtab.sym[k].scnidx;
            break;
        }
    }
    for (k = 1; k < libcsymtab.size; ++k) {
        if(!strcmp(libcsymtab.sym[k].name,".text")) {
            libctext_ident=libcsymtab.sym[k].scnidx;
            break;
        }
    }

    while (i<size) {

        seg=get_seg_from_adress(current_addr,memory);
        //printf("%d %d\n",i,j );
        if( seg>=0 && (!strcmp(memory->seg[seg].name,".text")||!strcmp(memory->seg[seg].name,"libc.text"))) {

            getInstr(current_addr,&current_instr);
            memcpy(&instr_value,&current_instr,4);
            //Affichage des breakpoints
            if(present(current_addr,breaklist)!=NULL) {
                sprintf(tmp,"\n X  ");
                strcat(string,tmp);
            } else {
                sprintf(tmp,"\n    ");
                strcat(string,tmp);
            }
            //affichage de PC
            if(current_addr==reg_mips[PC]) {
                sprintf(tmp,"-> ");
                strcat(string,tmp);
            } else {
                sprintf(tmp,"   ");
                strcat(string,tmp);
            }

            //Afficahge de l'adresse
            sprintf(tmp,"\t%X  %8.8X     ",current_addr,instr_value);
            strcat(string,tmp);


            int dico_entry=0;
            while((instr_value & dico_data[dico_entry].mask)!= dico_data[dico_entry].instr) {
                dico_entry++;
            }
            if (dico_entry>=nbinstr)
            {
                WARNING_MSG("invalid instruction at adress %X",current_addr);
                printf("\n");
                return -1;
            }
            else {

                //affichage des étiquettes en début de ligne dans .text
                for (k = 1; k < symtab.size; ++k)
                {
                    if(((current_addr-memory->seg[seg].start._32)==symtab.sym[k].addr._32) && (symtab.sym[k].type != section) && ( strcmp(memory->seg[get_seg_from_adress(current_addr,memory)].name,".text")==0) && ((symtab.sym[k].scnidx == text_ident) ) ) {
                        sprintf(tmp,"%s: ",symtab.sym[k].name);
                        strcat(string,tmp);
                        break;
                    }
                }
                //affichage des étiquettes en début de ligne dans libc.text
                for (k = 1; k < libcsymtab.size; ++k)
                {
                    if(((current_addr-memory->seg[seg].start._32)==libcsymtab.sym[k].addr._32) && (libcsymtab.sym[k].type != section) && ( strcmp(memory->seg[get_seg_from_adress(current_addr,memory)].name,"libc.text")==0) && ((libcsymtab.sym[k].scnidx == libctext_ident)) ) {
                        sprintf(tmp,"%s: ",libcsymtab.sym[k].name);
                        strcat(string,tmp);
                        break;
                    }
                }

                sprintf(tmp,"%s",dico_data[dico_entry].name );
                strcat(string,tmp);
            }
            switch (dico_data[dico_entry].type) {

            case 0: 									//R TYPE

                switch (dico_data[dico_entry].nb_arg) {
                case 3:
                    //3 ARGS
                    //printf("%s, %s, %s\n",dico_data[dico_entry].argname[0],dico_data[dico_entry].argname[1],dico_data[dico_entry].argname[2]);
                    // RS RT RD
                    if(!strcmp("RS",dico_data[dico_entry].argname[0]) && !strcmp("RT",dico_data[dico_entry].argname[1]) && !strcmp("RD",dico_data[dico_entry].argname[2])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rd,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rs,regname2);
                        char regname3[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rt,regname3);

                        sprintf(tmp," %s, %s, %s",regname1,regname2,regname3);
                        strcat(string,tmp);
                    }
                    //RT RD SA
                    else if(!strcmp("RT",dico_data[dico_entry].argname[0]) && !strcmp("RD",dico_data[dico_entry].argname[1]) && !strcmp("SA",dico_data[dico_entry].argname[2])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rd,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rt,regname2);
                        char regname3[MAX_NAME_SIZE];
                        parseReg(current_instr.r.sa,regname3);

                        sprintf(tmp," %s, %s, %s",regname1,regname2,regname3);
                        strcat(string,tmp);

                    }
                    else {
                        WARNING_MSG("Unknown arg 1-3 for R command");
                        printf("\n");
                        return -1;
                    }
                    break;
                case 2: 	//2 arg
                    // RS RT
                    if(!strcmp("RS",dico_data[dico_entry].argname[0]) && !strcmp("RT",dico_data[dico_entry].argname[1])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rs,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rt,regname2);

                        sprintf(tmp," %s, %s",regname1,regname2);
                        strcat(string,tmp);

                    }
                    else if(!strcmp("RD",dico_data[dico_entry].argname[0]) && !strcmp("RS",dico_data[dico_entry].argname[1])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rd,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rs,regname2);

                        sprintf(tmp," %s, %s",regname1,regname2);
                        strcat(string,tmp);

                    }
                    else if(!strcmp("RT",dico_data[dico_entry].argname[0]) && !strcmp("RD",dico_data[dico_entry].argname[1])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rd,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rt,regname2);

                        sprintf(tmp," %s, %s",regname1,regname2);
                        strcat(string,tmp);

                    }
                    else {
                        WARNING_MSG("Unknown arg 1 or 2 for R command");
                        printf("\n");
                        return -1;
                    }

                    break;

                case 1:     //1 arg
                    if(!strcmp("RS",dico_data[dico_entry].argname[0])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rs,regname1);
                        sprintf(tmp," %s",regname1);
                        strcat(string,tmp);
                    }
                    else if(!strcmp("RD",dico_data[dico_entry].argname[0])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rd,regname1);
                        sprintf(tmp," %s",regname1);
                        strcat(string,tmp);
                    }
                    else {
                        WARNING_MSG("Unknown arg for R command");
                        printf("\n");
                        return -1;
                    }

                    break;

                case 0:

                    break;

                default:
                    WARNING_MSG("Unknown R command");
                    printf("\n");
                    return -1;
                    break;
                }
                break;


            case 1:
                //i type
                switch (dico_data[dico_entry].nb_arg) {
                case 3:
                    //3arg
                    if(!strcmp("RS",dico_data[dico_entry].argname[0]) && !strcmp("RT",dico_data[dico_entry].argname[1]) && !strcmp("IM",dico_data[dico_entry].argname[2])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rt,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rs,regname2);

                        sprintf(tmp," %s, %s, %d",regname1,regname2,current_instr.i.immediate);
                        strcat(string,tmp);

                    }
                    else if(!strcmp("RS",dico_data[dico_entry].argname[0]) && !strcmp("RT",dico_data[dico_entry].argname[1]) && !strcmp("OFFSET",dico_data[dico_entry].argname[2])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rs,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rt,regname2);

                        sprintf(tmp," %s, %s, %d",regname1,regname2,4*current_instr.i.immediate);
                        strcat(string,tmp);

                        for (k = 1; k < symtab.size; ++k) {
                            if(((current_addr+ 4 + 4*current_instr.i.immediate-memory->seg[seg].start._32)==symtab.sym[k].addr._32)&&(symtab.sym[k].type != section)&& (symtab.sym[k].scnidx == text_ident) && ( strcmp(memory->seg[get_seg_from_adress(current_addr,memory)].name,".text")==0)) {
                                sprintf(tmp," <%s>",symtab.sym[k].name);
                                strcat(string,tmp);
                                break;
                            }
                        }
                        for (k = 1; k < libcsymtab.size; ++k) {
                            if(((current_addr+ 4 + 4*current_instr.i.immediate-memory->seg[seg].start._32)==libcsymtab.sym[k].addr._32)&&(libcsymtab.sym[k].type != section)&& (libcsymtab.sym[k].scnidx == libctext_ident) && ( strcmp(memory->seg[get_seg_from_adress(current_addr,memory)].name,"libc.text")==0)) {
                                sprintf(tmp," <%s>",libcsymtab.sym[k].name);
                                strcat(string,tmp);
                                break;
                            }
                        }
                    }
                    else if(!strcmp("BASE",dico_data[dico_entry].argname[0]) && !strcmp("RT",dico_data[dico_entry].argname[1]) && !strcmp("OFFSET",dico_data[dico_entry].argname[2])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rt,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rs,regname2);
                        //BASE est a l'addresse de rs
                        sprintf(tmp," %s, %d(%s)",regname1,current_instr.i.immediate,regname2);
                        strcat(string,tmp);

                    }
                    else {
                        WARNING_MSG("Unknown arg 1-3 for I command : %s %s %s", dico_data[dico_entry].argname[0],dico_data[dico_entry].argname[1],dico_data[dico_entry].argname[2]);
                        printf("\n");
                        return -1;
                    }

                    break;
                case 2:
                    if(!strcmp("RS",dico_data[dico_entry].argname[0]) && !strcmp("OFFSET",dico_data[dico_entry].argname[1])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rs,regname1);

                        sprintf(tmp," %s, %d",regname1,4*current_instr.i.immediate);
                        strcat(string,tmp);

                        for (k = 1; k < symtab.size; ++k) {
                            if(((current_addr+ 4 + 4*current_instr.i.immediate-memory->seg[seg].start._32)==symtab.sym[k].addr._32)&&(symtab.sym[k].type != section)&& (symtab.sym[k].scnidx == text_ident) && ( strcmp(memory->seg[get_seg_from_adress(current_addr,memory)].name,".text")==0) ) {
                                sprintf(tmp," <%s>",symtab.sym[k].name);
                                strcat(string,tmp);
                                break;
                            }
                        }
                        for (k = 1; k < libcsymtab.size; ++k) {
                            if(((current_addr+ 4 + 4*current_instr.i.immediate-memory->seg[seg].start._32)==libcsymtab.sym[k].addr._32)&&(libcsymtab.sym[k].type != section)&& (libcsymtab.sym[k].scnidx == text_ident) && ( strcmp(memory->seg[get_seg_from_adress(current_addr,memory)].name,"libc.text")==0) ) {
                                sprintf(tmp," <%s>",libcsymtab.sym[k].name);
                                strcat(string,tmp);
                                break;
                            }
                        }

                    }
                    else if(!strcmp("RT",dico_data[dico_entry].argname[0]) && !strcmp("IM",dico_data[dico_entry].argname[1])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rt,regname1);

                        sprintf(tmp," %s, %d",regname1,current_instr.i.immediate);
                        strcat(string,tmp);

                    }
                    else {
                        WARNING_MSG("Unknown arg 1 or 2 for I command");
                        printf("\n");
                        return -1;
                    }

                    break;
                default:
                    WARNING_MSG("Wrong arg number for I command");
                    printf("\n");
                    return -1;
                    break;


                }
                break;

            case 2:
                //type J
                if(!strcmp("TARGET",dico_data[dico_entry].argname[0])) {



                    sprintf(tmp," 0x%4.8X",((current_addr & 0xF0000000) | 4*current_instr.j.target));
                    strcat(string,tmp);
                    int32_t temp=((current_addr & 0xF0000000) | 4*current_instr.j.target);
                    for (k = 1; k < symtab.size; ++k) {
                        if(((temp-textStart) == symtab.sym[k].addr._32) && (symtab.sym[k].type != section) && (symtab.sym[k].scnidx == text_ident) && ( strcmp(memory->seg[get_seg_from_adress(current_addr,memory)].name,".text")==0) ) {
                            sprintf(tmp," <%s>",symtab.sym[k].name);
                            strcat(string,tmp);
                            break;
                        }
                    }
                    //affichage des étiquettes dans la libc
                    for (k = 1; k < libcsymtab.size; ++k) {
                        //printf("\nsymb adress : %d, total=%X,addr=%X",libcsymtab.sym[k].addr._32,((current_addr & 0xF0000000) | 4*current_instr.j.target-libcTextStart),((current_addr & 0xF0000000) | 4*current_instr.j.target));
                        //printf("addr %X\n",libcTextStart);
                        if(((temp - libcTextStart) == libcsymtab.sym[k].addr._32) && (libcsymtab.sym[k].type != section) && (libcsymtab.sym[k].scnidx == text_ident)&& ( strcmp(memory->seg[get_seg_from_adress(current_addr,memory)].name,"libc.text")==0)) {
                            sprintf(tmp," <%s>",libcsymtab.sym[k].name);
                            strcat(string,tmp);
                            break;
                        }
                    }


                }
                else {
                    WARNING_MSG("Unknown J command");
                    printf("\n");
                    return -1;
                }

                break;

            default:

                WARNING_MSG("Unknown type");
                printf("\n");
                return -1;
                break;

            }
        }
        current_addr+=4;
        i+=4;
    }
    sprintf(tmp,"\n");
    strcat(string,tmp);
    return 0;
}