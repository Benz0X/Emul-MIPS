/*
 * \author (m) Fran�ois Portet 2013 <francois.portet@imag.fr>
 * \brief test main for loading ELF file in memory.
 *--------------------------------------------------------------------------
 * Ce programme est un exemple de comment charger un fichier ELF
 * dans une m�moire virtuelle en utilisant l'API elf.
 *
 * ATTENTION, il s'agit juste d'un exemple et ne peut pas �tre utilis� tel quel
 * dans votre code!
 * A vous de vous en inspirer
 *
 */


#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include "common/bits.h"
#include "common/notify.h"
#include "elf/elf.h"
#include "elf/syms.h"
#include "elf/relocator.h"
#include "mem.h"

// On fixe ici une adresse basse dans la m�moire virtuelle. Le premier segment
// ira se loger � cette adresse.
#define START_MEM 0x3000

#define LIBC_MEM_END 0xff7ff000u
// nombre max de sections que l'on extraira du fichier ELF
#define NB_SECTIONS 4

// nom de chaque section
#define TEXT_SECTION_STR ".text"
#define RODATA_SECTION_STR ".rodata"
#define DATA_SECTION_STR ".data"
#define BSS_SECTION_STR ".bss"

//nom du prefix � appliquer pour la section
#define RELOC_PREFIX_STR ".rel"

#define PATH_TO_LIBC "./libc/libc.so"

// Fonction permettant de verifier si une chaine de caracteres
// est bien dans la liste des symboles du fichier ELF
// parametres :
//      name : le nom de la chaine recherch�e
//  symtab : la table des symboles
//
// retourne 1 si present, 0 sinon
int is_in_symbols(char* name, stab symtab) {
    int i;
    for (i=0; i<symtab.size; i++) {
        if (!strcmp(symtab.sym[i].name,name)) return 1;
    }
    return 0;
}

// Cette fonction calcule le nombre de segments � prevoir
// Elle cherche dans les symboles si les sections predefinies
// s'y trouve
// parametres :
//  symtab : la table des symboles
//
// retourne le nombre de sections trouv�es

unsigned int get_nsegments(stab symtab,char* section_names[],int nb_sections) {
    unsigned int n=0;
    int i;
    for (i=0; i<nb_sections; i++) {
        if (is_in_symbols(section_names[i],symtab)) n++;
    }
    return n;
}


// fonction permettant d'extraire une section du fichier ELF et de la charg�e dans le segment du m�me nom
// parametres :
//   fp         : le pointeur du fichier ELF
//   memory     : la structure de m�moire virtuelle
//   scn        : le nom de la section � charger
//   permission : l'entier repr�sentant les droits de lecture/ecriture/execution
//   add_start  : l'addresse virtuelle � laquelle la section doit �tre charg�e
//
// retourne 0 en cas de succes, une valeur non nulle sinon
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
 * @param seg le segment � reloger
 * @param mem l'ensemble des segments
 * @param endianness le boutisme du programme
 * @param symtab la table des symbole du programme 
 * @param symtab_libc la table des symbole de la libc (NULL si inutile)
 * @param fp_libc le fichier elf de la libc (NULL si inutile)
 *
 * @brief Cette fonction effectue la relocation du segment pass� en parametres
 * @brief l'ensemble des segments doit d�j� avoir �t� charg� en memoire.
 *
 * VOUS DEVEZ COMPLETER CETTE FONCTION POUR METTRE EN OEUVRE LA RELOCATION !!
 */
void reloc_segment(FILE* fp, segment seg, mem memory,unsigned int endianness,stab* symtab,stab* symtab_libc,FILE* fp_libc) {
    byte *ehdr    = __elf_get_ehdr( fp );
    uint32_t  scnsz  = 0;
    Elf32_Rel *rel = NULL;
    char* reloc_name = malloc(strlen(seg.name)+strlen(RELOC_PREFIX_STR)+1);
    scntab section_tab;
    scntab section_tab_libc;

    // on recompose le nom de la section
    memcpy(reloc_name,RELOC_PREFIX_STR,strlen(RELOC_PREFIX_STR)+1);
    strcat(reloc_name,seg.name);

    // on r�cupere le tableau de relocation et la table des sections
    rel = (Elf32_Rel *)elf_extract_scn_by_name( ehdr, fp, reloc_name, &scnsz, NULL );
    elf_load_scntab(fp ,32, &section_tab);

    if (symtab_libc!=NULL && fp_libc!=NULL)
        elf_load_scntab(fp_libc ,32, &section_tab_libc);

    if (rel != NULL &&seg.content!=NULL && seg.size._32!=0) {

        INFO_MSG("--------------Relocation de %s-------------------\n",seg.name) ;
        INFO_MSG("Nombre de symboles a reloger: %ld\n",scnsz/sizeof(*rel)) ;


    //TODO : faire la relocation ICI!
        


    }
    del_scntab(section_tab);
    free( rel );
    free( reloc_name );
    free( ehdr );

}



// fonction affichant les octets d'un segment sur la sortie standard
// parametres :
//   seg        : le segment de la m�moire virtuelle � afficher

void print_segment_raw_content(segment* seg) {
    int k;
    int word =0;
    if (seg!=NULL && seg->size._32>0) {
        for(k=0; k<seg->size._32; k+=4) {
            if(k%16==0) printf("\n  0x%08x ",k);
            word = *((unsigned int *) (seg->content+k));
            FLIP_ENDIANNESS(word);
            printf("%08x ", word);
        }
    }
}


// le main charge un fichier elf en entr�e en utilisant
// les arguments du prototype de la fonction main (cf. fiches infos)
//
int main (int argc, char *argv[]) {


    char* section_names[NB_SECTIONS]= {TEXT_SECTION_STR,RODATA_SECTION_STR,DATA_SECTION_STR,BSS_SECTION_STR};
    unsigned int segment_permissions[NB_SECTIONS]= {R_X,R__,RW_,RW_};

    unsigned int nsegments;
    int i=0,j=0;
    unsigned int type_machine;
    unsigned int endianness;   //little ou big endian
    unsigned int bus_width;    // 32 bits ou 64bits
    unsigned int next_segment_start = START_MEM; // compteur pour designer le d�but de la prochaine section

    mem memory;  // memoire virtuelle, c'est elle qui contiendra toute les donn�es du programme
    stab symtab= new_stab(0); // table des symboles
    stab symtab_libc= new_stab(0); // table des symboles de la libc
    FILE * pf_elf, *pf_libc;

    if ((argc < 2) || (argc > 2)) {
        printf("Usage: %s <fichier elf> \n", argv[0]);
        exit(1);
    }

    if ((pf_elf = fopen(argv[1],"r")) == NULL) {
        ERROR_MSG("cannot open file %s", argv[1]);
    }

    if (!assert_elf_file(pf_elf))
        ERROR_MSG("file %s is not an ELF file", argv[1]);


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


    nsegments = get_nsegments(symtab,section_names,NB_SECTIONS);
    nsegments += get_nsegments(symtab_libc,section_names,NB_SECTIONS);

    // allouer la memoire virtuelle
    memory=init_mem(nsegments);


    next_segment_start = LIBC_MEM_END;
    printf("\ndebut : %08x\n",next_segment_start);
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

    // on reloge libc
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
    next_segment_start = START_MEM;
    for (i=0; i<NB_SECTIONS; i++) {
        if (is_in_symbols(section_names[i],symtab)) {
            elf_load_section_in_memory(pf_elf,memory, section_names[i],segment_permissions[i],next_segment_start);
            next_segment_start+= ((memory->seg[j].size._32+0x1000)>>12 )<<12; // on arrondit au 1k supp�rieur
            print_segment_raw_content(&memory->seg[j]);
            j++;
        }
    }

    // on reloge chaque section du fichier
    for (i=k; i<j; i++) {
        reloc_segment(pf_elf, memory->seg[i], memory,endianness,&symtab,&symtab_libc,pf_libc);

    }

    //TODO allouer la pile (et donc modifier le nb de segments)

    printf("\n------ Fichier ELF \"%s\" : sections lues lors du chargement ------\n", argv[1]) ;
    print_mem(memory);
    stab32_print( symtab);
    stab32_print( symtab_libc);

    // on fait le m�nage avant de partir
    del_mem(memory);
    del_stab(symtab_libc);
    del_stab(symtab);
    fclose(pf_elf);
    fclose(pf_libc);
    puts("");
    return 0;
}