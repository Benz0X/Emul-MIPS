#ifndef _affichage_h
#define _affichage_h

#include <SDL/SDL.h>
#include <SDL_ttf.h>

//Window Defines
#define MAX_FPS 30
#define DEFAULT_WINDOW_W 800
#define DEFAULT_WINDOW_H 600
#define MIN_WINDOW_W 640
#define MIN_WINDOW_H 480
#define ICON "icon.bmp"

//Layout Defines
#define OFFSET 5			//Marge autour des bords de la fenêtre
#define TITLE_H 20			//Hauteur d'un titre
#define CONSOLE_H 16		//Hauteur de la console


int affichage();			//Boucle principale de l'affichage


int FilterEvents(const SDL_Event *event) ;																				// Permet de filtrer les evenements MouseMotion (indirectement permet de Flip quand necessaire)
int UpdateRect(SDL_Rect* rect, int x, int y, int w, int h);																// "Macro"
int Button (SDL_Surface* ecran, int x, int y, int w, int h, SDL_Event event, int fn );									// Element graphique
int RenderText (SDL_Surface* ecran, int x,int y,int w,int h, char* string, TTF_Font* font, SDL_Color fontcolor);		// Allège le blit de texte

//Permet de générer les strings à afficher
int stringPipeline(char* string);
int stringRegisters(char* string);
int stringDisasm(uint32_t start_addr,uint32_t size, char* string);
int stringMemory(char* string);

#endif