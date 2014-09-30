#ifndef _user_int_h
#define _user_int_h

typedef enum {
    LOAD,
    EXIT,
    DISP,
    DISASM,
    SET,
    ASSERT,
    DEBUG,
    RESUME,
    RUN,
    STEP,
    BREAK,
    UNKNOWN
} command ;


command decrypt(char input []);//doit decrypter mot par mot, faudrait une fonction get-word qui ne renvoi rien si c'est des comment




#endif
