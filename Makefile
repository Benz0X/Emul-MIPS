TARGET=projet



# noms des executables utilisés durant la compilation/edition des liens
CC=`which gcc`
LD=`which gcc`
RM=`which rm` -f

#options de compilation/edition des liens
INCLUDE=-I$(INCDIR)

SDL=-LSDL/lib -lSDLmain -lSDL -lSDL2 -lSDL_draw -lSDL_ttf
SDL_PHELMA=-L$(SDLDIR)/lib -lSDLmain -lSDL -lSDL_ttf -lSDL_image -lSDL_phelma -lSDL_draw


CFLAGS=-Wall $(INCLUDE) 
LFLAGS_PHELMA=-lreadline -lm -lcurses $(SDL_PHELMA)
LFLAGS_HOME=-lreadline -lm -lcurses $(SDL)
CFLAGS_DBG=$(CFLAGS) -g -c -DVERBOSE -I$(SDLDIR)/include/SDL -I$(SDLDIR)/include
CFLAGS_RLS=$(CFLAGS)




# definition des repertoires de source/destination
SRCDIR=src
INCDIR=include
DOCDIR=doc
SDLDIR=/users/prog1a/C/librairie/2011

# les fichiers dont on peut se débarasser
GARBAGE=*~ $(SRCDIR)/*~ $(INCDIR)/*~ $(TESTDIR)/*~ $(SRCDIR)/*.orig $(INCDIR)/*.orig

# ou se trouve les sources (i.e., le *.c)
SRC=$(wildcard $(SRCDIR)/*.c)

# les objets avec l'option DEBUG s'appeleront fichier.dbg.o
# ceux de la release fichier.rls.o
OBJ_DBG=$(SRC:.c=.dbg.o)
OBJ_RLS=$(SRC:.c=.rls.o)

# 1er target (celui executé par défaut quand make est lancé sans nom de cible) 
# affiche l'aide
all : 
	@echo ""
	@echo "Usage:"
	@echo ""
	@echo "make debug   => build DEBUG   version"
	@echo "make release => build RELEASE version"
	@echo "make doc     => produce the doxygen documentation"
	@echo "make clean   => clean everything"
	@echo "make tarball => produce archive .tar.gz in ../ directory"

debug   : $(OBJ_DBG)
	$(LD) $^ $(LFLAGS_PHELMA) -o $(TARGET)

debug2   : $(OBJ_DBG)
	$(LD) $^ $(LFLAGS_HOME) -o $(TARGET)

release : $(OBJ_RLS)
	$(LD) $^ $(LFLAGS) -o $(TARGET)


%.rls.o : %.c
	$(CC) $< $(CFLAGS_RLS) -c -o $(basename $<).rls.o

%.dbg.o : %.c
	$(CC) $< $(CFLAGS_DBG) -c -o $(basename $<).dbg.o


clean : 
	$(RM) $(TARGET) $(SRCDIR)/*.o $(GARBAGE) 
	$(RM) -r $(DOCDIR)/*	

# créé l'archive à envoyer à votre tuteur (pas de rar SVP! et interdiction absolu d'envoyer autre chose qu'une archive, il ne doit y avoir qu'un seul fichier joint dans l'e-mail !)
archive : 
	make clean 
	tar -czvf ../$(notdir $(PWD) )-`whoami`-`date +%d-%m-%H-%M`.tgz .
	echo "Fichier archive ../emulMips-`whoami`-`date +%d-%m-%H-%M`.tgz genere"
