#ifndef _affichage_h
#define _affichage_h

#define MAX_FPS 60
#define DEFAULT_WINDOW_W 800
#define DEFAULT_WINDOW_H 600
#define MIN_WINDOW_W 640
#define MIN_WINDOW_H 480


#define OFFSET 5
#define TITLE_H 20
#define CONSOLE_H 16

int affichage();

//int UpdateRect(SDL_Rect* rect, int x, int y, int w, int h);

int stringPipeline(char* string);
int stringRegisters(char* string);
int stringDisasm(uint32_t start_addr,uint32_t size, char* string);
int stringMemory(char* string);

#endif