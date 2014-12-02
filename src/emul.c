//Base de la machine virtuelle et chargement d'un fichier ELF




#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include "define.h"
#include "common/bits.h"
#include "common/notify.h"
#include "elf/elf.h"
#include "elf/syms.h"
#include "mem.h"
#include "emul.h"
#include "fonctions.h"
#include "elf/relocator.h"




//init des registres RAJOUTER HI LOW PC
int reg_mips[35];

//init de la memoire
mem memory=NULL;
stab symtab;
stab libcsymtab;

//init du dictionnaire
dico_info* dico_data=NULL;




#define LIBC_MEM_END 0xF000
#define PATH_TO_LIBC "include/libc/libc.so"

// nombre max de sections que l'on extraira du fichier ELF
#define NB_SECTIONS 4

// nom de chaque section
#define TEXT_SECTION_STR ".text"
#define RODATA_SECTION_STR ".rodata"
#define DATA_SECTION_STR ".data"
#define BSS_SECTION_STR ".bss"

//nom du prefix a appliquer pour la section
#define RELOC_PREFIX_STR ".rel"

int is_in_symbols(char* name, stab symtab) {
    int i;
    for (i=0; i<symtab.size; i++) {
        if (!strcmp(symtab.sym[i].name,name)) return 1;
    }
    return 0;
}

// Cette fonction calcule le nombre de segments a prevoir
// Elle cherche dans les symboles si les sections predefinies
// s'y trouve
// parametres :
//  symtab : la table des symboles
//
// retourne le nombre de sections trouvees

unsigned int get_nsegments(stab symtab,char* section_names[],int nb_sections) {
    unsigned int n=0;
    int i;
    for (i=0; i<nb_sections; i++) {
        if (is_in_symbols(section_names[i],symtab)) n++;
    }
    return n;
}



int elf_load_section_in_memory(FILE* fp, mem memory, char* scn,unsigned int permissions,unsigned long long add_start) {
    byte *ehdr    = __elf_get_ehdr( fp );
    byte *content = NULL;
    uint  textsz  = 0;
    vsize sz;
    vaddr addr;

    byte *useless = elf_extract_section_header_table( ehdr, fp );
    free( useless );

    if ( NULL == ehdr ) {
        WARNING_MSG( "Can't read ELF file" );
        return 1;
    }

    if ( 1 == attach_scn_to_mem(memory, scn, SCN_ATTR( WIDTH_FROM_EHDR( ehdr ), permissions ) ) ) {
        WARNING_MSG( "Unable to create %s section", scn );
        free( ehdr );
        return 1;
    }

    content = elf_extract_scn_by_name( ehdr, fp, scn, &textsz, NULL );
    if ( NULL == content ) {
        WARNING_MSG( "Corrupted ELF file" );
        free( ehdr );
        return 1;
    }

    switch( WIDTH_FROM_EHDR(ehdr) ) {
    case 32 :
        sz._32   = textsz/*+8*/; /* +8: In case adding a final sys_exit is needed */
        addr._32 = add_start;
        break;
    case 64 :
        sz._64   = textsz/*+8*/; /* +8: In case adding a final sys_exit is needed */
        addr._64 = add_start;
        break;
    default :
        WARNING_MSG( "Wrong machine width" );
        return 1;
    }

    if ( 1 == fill_mem_scn(memory, scn, sz, addr, content ) ) {
        free( ehdr );
        free( content );
        WARNING_MSG( "Unable to fill in %s segment", scn );
        return 1;
    }

    free( content );
    free( ehdr );

    return 0;
}


/*--------------------------------------------------------------------------  */
/**
 * @param fp le fichier elf original
 * @param seg le segment a reloger
 * @param mem l'ensemble des segments
 * @param endianness le boutisme du programme
 * @param symtab la table des symbole du programme
 * @param symtab_libc la table des symbole de la libc (NULL si inutile)
 * @param fp_libc le fichier elf de la libc (NULL si inutile)
 * @brief Cette fonction effectue la relocation du segment passe en parametres
 * @brief l'ensemble des segments doit deja avoir ete charge en memoire.
 *
 * VOUS DEVEZ COMPLETER CETTE FONCTION POUR METTRE EN OEUVRE LA RELOCATION !!
 */
void reloc_segment(FILE* fp, segment seg, mem memory,unsigned int endianness,stab* symtable,stab* symtab_libc,FILE* fp_libc) {
    byte *ehdr    = __elf_get_ehdr( fp );
    uint32_t  scnsz  = 0;
    Elf32_Rel *rel = NULL;
    char* reloc_name = malloc(strlen(seg.name)+strlen(RELOC_PREFIX_STR)+1);
    scntab section_tab;
    scntab section_tab_libc;

    // on recompose le nom de la section
    memcpy(reloc_name,RELOC_PREFIX_STR,strlen(RELOC_PREFIX_STR)+1);
    strcat(reloc_name,seg.name);

    // on recupere le tableau de relocation et la table des sections
    rel = (Elf32_Rel *)elf_extract_scn_by_name( ehdr, fp, reloc_name, &scnsz, NULL );
    elf_load_scntab(fp ,32, &section_tab);

    if (symtab_libc!=NULL && fp_libc!=NULL)
        elf_load_scntab(fp_libc ,32, &section_tab_libc);


    if (rel != NULL &&seg.content!=NULL && seg.size._32!=0) {
        if(verbose>0) {
            INFO_MSG("--------------Relocation de %s-------------------\n",seg.name) ;
            INFO_MSG("Nombre de symboles a reloger: %ld\n",scnsz/sizeof(*rel)) ;
        }

        //------------------------------------------------------

        int i=0;
        uint sym;
        uint type;
        uint info;
        uint offset;
        //display :
        if(verbose>0) {
            //printf("scnsz=%d\n", scnsz);
            //print_scntab(section_tab);
            printf("Offset    Info      Type            Sym.Value  Sym.Name\n");
            while(i<scnsz/sizeof(*rel)) {
                info=rel[i].r_info;
                offset=rel[i].r_offset;
                FLIP_ENDIANNESS(info) ;
                FLIP_ENDIANNESS(offset) ;
                sym=ELF32_R_SYM(info);
                type=ELF32_R_TYPE(info);
                if (type>32) {
                    WARNING_MSG("Unknown type : %d",type);
                }
                else {
                    printf("%08X  %08X  %-14s  %08X   %s\n",offset,info,MIPS32_REL[type],sym,(*symtable).sym[sym].name);
                    i++;
                }

            }
        }
        i=0;
        //------------------------------------------------------
        //Reloc :
        int A,V,P;
        //int segnum;
        uint32_t S;
        while(i<scnsz/sizeof(*rel)) {
            info=rel[i].r_info;
            offset=rel[i].r_offset;
            FLIP_ENDIANNESS(info) ;
            FLIP_ENDIANNESS(offset) ;
            sym=ELF32_R_SYM(info);
            type=ELF32_R_TYPE(info);
            //printf("Relocating symbol %d\n",i );
            //segnum=seg_from_scnidx((*symtable).sym[sym].scnidx,(*symtable),memory);
            //if(segnum==-1){
            //    WARNING_MSG("Couldn't resolve scndix correspondance");
            //    break;
            //}
            //S=memory->seg[segnum].start._32+(*symtable).sym[sym].addr._32;//a verif
            //printf("sym=%d, symbtable size=%d\n", sym,(*symtable).size);
            if(addr_from_symnb(sym, (*symtable), memory,&S)==-1) {
                WARNING_MSG("Trying to resolve scndix correspondance in libcsymtab");
            }

            P=seg.start._32+offset;
            memRead(P,1,&A);
            //printf("Relocation type %s\n",MIPS32_REL[type] );
            switch (type) {
            case 2:
                V=S+A;

                //printf("V= %X,S=%X,A=%X,P=%X\n",V,S,A,P);
                memWrite(P,1,V);
                break;
            case 4:
                V=(A&0xFC00000)|((((A<<2)|((P&0xF0000000)+S))>>2)&0x3FFFFFF);

                //printf("V= %X,S=%X,A=%X,P=%X\n",V,S,A,P);
                memWrite(P,1,V);
                break;
            case 5:
                ;
                uint nexttype=rel[i+1].r_info;
                uint nextoffset=rel[i+1].r_offset;
                FLIP_ENDIANNESS(nexttype);
                FLIP_ENDIANNESS(nextoffset);
                nexttype=ELF32_R_TYPE(nexttype);
                if(nexttype!=6) {
                    WARNING_MSG("R_MIPS_HI16 not followed by R_MIIPS_LO16 : %s",MIPS32_REL[nexttype]);
                }
                else {

                    int P2=seg.start._32+nextoffset,A2;
                    memRead(P2,1,&A2);
                    int AHL;
                    AHL=(A<<16)+(short)(A2);
                    //printf("A2=%X short A2=%X\n",A2, (short)A2 );
                    //printf("AHL : %X\n",AHL );
                    //printf("Total=%X AHL+S=%X, (AHL+S)&0xFFFF=%X, diff=%X\n",((AHL+S-(short)(AHL+S))>>16),AHL+S,(AHL+S)&0xFFFF,AHL+S-(short)AHL+S) ;
                    V=(A & 0xFFFF0000)|(  ((AHL+S-(short)(AHL+S))>>16)   &0xFFFF);

                    //printf("V= %X,S=%X,A=%X,A2=%X,P=%X,P2=%X, AHL=%X\n",V,S,A,A2,P,P2,AHL);
                    memWrite(P,1,V);
                }
                break;
            case 6:
                ;
                int previoustype=rel[i-1].r_info;
                int previousoffset=rel[i-1].r_offset;
                FLIP_ENDIANNESS(previoustype);
                FLIP_ENDIANNESS(previousoffset);
                previoustype=ELF32_R_TYPE(previoustype);
                if(previoustype!=5) {
                    WARNING_MSG("R_MIPS_LO16 not preceded by R_MIPS_HI16 : %s",MIPS32_REL[previoustype]);
                }
                else {

                    int32_t P2=seg.start._32+previousoffset,A2;
                    memRead(P2,1,&A2);
                    int32_t AHL=(A2<<16)+(short)(A);
                    V=(A&0xFFFF0000)|((short)(AHL+S)&0xFFFF);

                    //printf("V= %X,S=%X,A=%X,P=%X\n",V,S,A,P);
                    memWrite(P,1,V);
                }
                break;
            default:
                if (type>32) {
                    WARNING_MSG("Unknown type : %d, relocation impossible for element %d",type,i);
                }
                else {
                    WARNING_MSG("Unknown type : %s(code : %d), relocation impossible for element %d",MIPS32_REL[type],type,i);
                }
                break;
            }
            i++;
        }
        //------------------------------------------------------

    }
    del_scntab(section_tab);
    free( rel );
    free( reloc_name );
    free( ehdr );

}












int memRead(uint32_t start_addr,int type, int* value) {             //Lit la memoire et stock dans value
    if(memory==NULL) {
        WARNING_MSG("No memory loaded");
        return -1;
    }
    //if(start_addr>=0x4000 && start_addr<0x5000){printf("LECTURE DE RODATA\n");}
    int seg=get_seg_from_adress(start_addr,memory);
//printf("%d, current addr = 0x%X, start addr = 0x%X, size = %d",j,start_addr,memory->seg[j-1].start._32,memory->seg[j-1].size._32);
    if(type==0) { //type 0 pour byte
        if(seg>=0) {
            *value=memory->seg[seg].content[start_addr-memory->seg[seg].start._32];
            // printf(" \necriture\n");

        }
        else {
            return -1;
        }
    } else {
        if (seg>=0 && (start_addr+3 < memory->seg[seg].start._32+memory->seg[seg].size._32))
        {
            struct_word temp; //copier directement le tableau dans le utint ?
            temp.b1=memory->seg[seg].content[start_addr-memory->seg[seg].start._32];
            temp.b2=memory->seg[seg].content[start_addr-memory->seg[seg].start._32+1];
            temp.b3=memory->seg[seg].content[start_addr-memory->seg[seg].start._32+2];
            temp.b4=memory->seg[seg].content[start_addr-memory->seg[seg].start._32+3];
            memcpy(value,&temp,4);
        }
        else {
      //      printf("FAIL, %X \n", start_addr);
            return -1;
        }
    }
    //printf("0x%x\n",*value );
    return 0;
}

int memWrite(uint32_t start_addr,int type, int32_t value) {         // Ecrit value dans la memoire
    if(memory==NULL) {
        WARNING_MSG("No memory loaded");
        return -1;
    }
    int seg=get_seg_from_adress(start_addr,memory);


    if(type==0) {
        if(seg>=0) {
            memory->seg[seg].content[start_addr-memory->seg[seg].start._32]=value;
        }
        else {
            return -1;
        }
    } else {
        if (seg>=0 && (start_addr+3 < memory->seg[seg].start._32+memory->seg[seg].size._32))
        {
            struct_word temp;
            memcpy(&temp,&value,4); //copier directement le uint dans le tableau ?
            memory->seg[seg].content[start_addr-memory->seg[seg].start._32]=temp.b1;
            memory->seg[seg].content[start_addr-memory->seg[seg].start._32+1]=temp.b2;
            memory->seg[seg].content[start_addr-memory->seg[seg].start._32+2]=temp.b3;
            memory->seg[seg].content[start_addr-memory->seg[seg].start._32+3]=temp.b4;
        }
        else {
            return -1;
        }
    }
    return 0;
}

int memWriteChecked(uint32_t start_addr,int type, int32_t value) {         // Ecrit value dans la memoire
    if(memory==NULL) {
        WARNING_MSG("No memory loaded");
        return -1;
    }
    int seg=get_seg_from_adress(start_addr,memory);


    if(type==0) {
        if(seg>=0) {
            memory->seg[seg].content[start_addr-memory->seg[seg].start._32]=value;
        }
        else {
            return -1;
        }
    } else {
        if (seg>=0 && (start_addr+3 < memory->seg[seg].start._32+memory->seg[seg].size._32))
        {
            struct_word temp;
            memcpy(&temp,&value,4); //copier directement le uint dans le tableau ?
            memory->seg[seg].content[start_addr-memory->seg[seg].start._32]=temp.b1;
            memory->seg[seg].content[start_addr-memory->seg[seg].start._32+1]=temp.b2;
            memory->seg[seg].content[start_addr-memory->seg[seg].start._32+2]=temp.b3;
            memory->seg[seg].content[start_addr-memory->seg[seg].start._32+3]=temp.b4;
        }
        else {
            return -1;
        }
    }
    return 0;
}



void print_segment_raw_content(segment* seg) {      //Affiche un segment donne
    int k;
    int word =0;
    if (seg!=NULL && seg->size._32>0) {
        for(k=0; k<seg->size._32; k+=4) {
            if(k%16==0) printf("\n  0x%08x ",k);
            word = *((unsigned int *) (seg->content+k));
            FLIP_ENDIANNESS(word);
            printf("%08x ",	word);
        }
    }
}


int loadELF (char* name,int mode,uint32_t addr) {
    if(mode==1) {
        //printf("loading at adress :");
        textStart = addr;
        //printf("%X\n", textStart);
    }
    else {
        textStart=DEFAULT_S_ADDR;
    }
    if(textStart%0x1000>0) {
        textStart = textStart+0x1000-textStart%0x1000;
    }


    char* section_names[NB_SECTIONS]= {TEXT_SECTION_STR,RODATA_SECTION_STR,DATA_SECTION_STR,BSS_SECTION_STR};
    unsigned int segment_permissions[NB_SECTIONS]= {R_X,R__,RW_,RW_};
    unsigned int nsegments;
    int i=0,j=0;
    unsigned int type_machine;
    unsigned int endianness;   //little ou big endian
    unsigned int bus_width;    // 32 bits ou 64bits
    unsigned int next_segment_start = textStart; // compteur pour designer le debut de la prochaine section

    symtab=new_stab(0);
    stab symtab_libc= new_stab(0); // table des symboles de la libc
    FILE * pf_elf, *pf_libc;


    if ((pf_elf = fopen(name,"r")) == NULL) {
        WARNING_MSG("cannot open file '%s'", name);
        return -1;
    }

    if (!assert_elf_file(pf_elf)) {
        WARNING_MSG("file %s is not an ELF file", name);
        return -1;
    }


    if ((pf_libc = fopen(PATH_TO_LIBC,"r")) == NULL) {
        ERROR_MSG("cannot open file %s", PATH_TO_LIBC);
    }

    if (!assert_elf_file(pf_libc))
        ERROR_MSG("file %s is not an ELF file", PATH_TO_LIBC);

    // recuperation des info de l'architecture
    elf_get_arch_info(pf_elf, &type_machine, &endianness, &bus_width);
    // et des symboles
    elf_load_symtab(pf_elf, bus_width, endianness, &symtab);
    elf_load_symtab(pf_libc, bus_width, endianness, &symtab_libc);
    elf_load_symtab(pf_libc, bus_width, endianness, &libcsymtab);


    nsegments = get_nsegments(symtab,section_names,NB_SECTIONS);
    nsegments += get_nsegments(symtab_libc,section_names,NB_SECTIONS);
    // allouer la memoire virtuelle
    memory=init_mem(nsegments);


    next_segment_start = LIBC_MEM_END;
    //printf("\ndebut : %08x\n",next_segment_start);
    j=0;

    // on alloue libc
    for (i=0; i<NB_SECTIONS; i++) {
        if (is_in_symbols(section_names[i],symtab_libc)) {
            elf_load_section_in_memory(pf_libc,memory, section_names[i],segment_permissions[i],next_segment_start);
            next_segment_start-= ((memory->seg[j].size._32+0x1000)>>12 )<<12; // on arrondit au 1k supp�rieur
            memory->seg[j].start._32 = next_segment_start;
//            print_segment_raw_content(&memory->seg[j]);
            j++;
        }
    }
    if(verbose>0) {
        INFO_MSG("--------------Relocation de %s-------------------\n",PATH_TO_LIBC);
    }
    for (i=0; i<j; i++) {
        reloc_segment(pf_libc, memory->seg[i], memory,endianness,&symtab_libc,NULL,NULL);
    }

    // on change le nom des differents segments de libc
    for (i=0; i<j; i++) {
        char seg_name [256]= {0};
        strcpy(seg_name,"libc");
        strcat(seg_name,memory->seg[i].name);
        free(memory->seg[i].name);
        memory->seg[i].name=strdup(seg_name);
    }


    // On va chercher les sections du fichier
    int k =j;
    next_segment_start = textStart;




    for (i=0; i<NB_SECTIONS; i++) {
        if (is_in_symbols(section_names[i],symtab)) {
            elf_load_section_in_memory(pf_elf,memory, section_names[i],segment_permissions[i],next_segment_start);
            next_segment_start+= ((memory->seg[j].size._32+0x1000)>>12 )<<12; // on arrondit au 1k supperieur
//            print_segment_raw_content(&memory->seg[j]);
            j++;
        }
    }

    // on reloge chaque section du fichier
    if(verbose>0) {
        INFO_MSG("--------------Relocation de %s-------------------\n",name);
    }
    for (i=k; i<j; i++) {
        reloc_segment(pf_elf, memory->seg[i], memory,endianness,&symtab,&symtab_libc,pf_libc);
    }
    //TODO allouer la pile (et donc modifier le nb de segments)

    // printf("\n------ Fichier ELF \"%s\" : sections lues lors du chargement ------\n", name) ;
    // print_mem(memory);
    //stab32_print( symtab);
    //stab32_print( symtab_libc);
    // on fait le menage avant de partir
    //del_mem(memory);
    //del_stab(symtab_libc);
    //del_stab(symtab);

    //Recuperation des bornes des segments rx
    for (k = 0; k < memory->nseg; k++) {
        if(strcmp(memory->seg[k].name,".text")==0) {
            textStart=memory->seg[k].start._32;
            textEnd=memory->seg[k].start._32+memory->seg[k].size._32;
        }
        if(strcmp(memory->seg[k].name,"libc.text")==0) {
            libcTextStart=memory->seg[k].start._32;
            libcTextEnd=memory->seg[k].start._32+memory->seg[k].size._32;
        }
    }
    if(verbose>4)INFO_MSG("Segments R_X : %8.8X-%8.8X et %8.8X-%8.8X",textStart,textEnd,libcTextStart,libcTextEnd );


    //Initialisation de l'emulateur en vu d'un run
    initprog();



    INFO_MSG("File loaded :'%s'",name);
    fclose(pf_elf);
    fclose(pf_libc);
    //puts("");
    return 0;
}


int dispmemPlage(uint32_t start_addr,uint32_t size) {           //Affiche une plage memoire
    if(memory==NULL) {
        WARNING_MSG("No memory loaded");
        return -1;
    }
    uint32_t i=0;
    uint32_t current_addr=start_addr;
    int value;
    while (i<size) {
        if (i%16==0) {
            printf("\n0x%8.8X ",current_addr);
        }


//    printf("j= %d i=%d, current_addr= %d ,start= %d, size = %d, diff=%d \n",j,i,current_addr,memory->seg[j-1].start._32,memory->seg[j-1].size._32,current_addr-memory->seg[j-1].start._32+memory->seg[j-1].size._32);
        if(memRead(current_addr,0,&value)==0) { //on verifie qu'il soit dans une plage memoire valide
            printf("%2.2X ",value);
        }
        else printf("XX ");

        current_addr++;
        i++;
    }
    printf("\n");
    return 0;
}

