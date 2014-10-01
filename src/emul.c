//Base de la machine virtuelle et chargement d'un fichier ELF




#include<stdio.h>
#include<stdlib.h>
#include "common/bits.h"
#include "common/notify.h"
#include "elf/elf.h"
#include "elf/syms.h"
#include "mem.h"
#include <stdarg.h>


//init des registres RAJOUTER HI LOW PC
int reg_mips[35];



//init de la mémoire
mem memory;
stab symtab;




// On fixe ici une adresse basse dans la mémoire virtuelle. Le premier segment
// ira se loger à cette adresse.
// nombre max de sections que l'on extraira du fichier ELF
#define NB_SECTIONS 4

// nom de chaque section
#define TEXT_SECTION_STR ".text"
#define RODATA_SECTION_STR ".rodata"
#define DATA_SECTION_STR ".data"
#define BSS_SECTION_STR ".bss"



int is_in_symbols(char* name, stab symtab) {
    int i;
    for (i=0; i<symtab.size; i++) {
        if (!strcmp(symtab.sym[i].name,name)) return 1;
    }
    return 0;
}

// Cette fonction calcule le nombre de segments à prevoir
// Elle cherche dans les symboles si les sections predefinies
// s'y trouve
// parametres :
//  symtab : la table des symboles
//
// retourne le nombre de sections trouvées

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


void print_segment_raw_content(segment* seg) {
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


int loadELF (char* name,...){
	va_list ap;
	uint32_t start_adress;
		va_start(ap, name);
		start_adress = va_arg(ap, uint32_t);
		

		va_end(ap);

	char* section_names[NB_SECTIONS]= {TEXT_SECTION_STR,RODATA_SECTION_STR,DATA_SECTION_STR,BSS_SECTION_STR};
	unsigned int segment_permissions[NB_SECTIONS]= {R_X,R__,RW_,RW_};
	unsigned int nsegments;
	int i=0,j=0;
	unsigned int type_machine;
	unsigned int endianness;   //little ou big endian
	unsigned int bus_width;    // 32 bits ou 64bits
	unsigned int next_segment_start = start_adress; // compteur pour designer le début de la prochaine section

	symtab=new_stab(0);

	FILE * pf_elf;


    if ((pf_elf = fopen(name,"r")) == NULL) {
        ERROR_MSG("cannot open file %s", name);
    }

    if (!assert_elf_file(pf_elf))
        ERROR_MSG("file %s is not an ELF file", name);


    // recuperation des info de l'architecture
    elf_get_arch_info(pf_elf, &type_machine, &endianness, &bus_width);
    // et des symboles
    elf_load_symtab(pf_elf, bus_width, endianness, &symtab);


    nsegments = get_nsegments(symtab,section_names,NB_SECTIONS);

    // allouer la memoire virtuelle
    memory=init_mem(nsegments);

    // Ne pas oublier d'allouer les differentes sections
    j=0;
    for (i=0; i<NB_SECTIONS; i++) {
        if (is_in_symbols(section_names[i],symtab)) {
            elf_load_section_in_memory(pf_elf,memory, section_names[i],segment_permissions[i],next_segment_start);
            next_segment_start+= ((memory->seg[j].size._32+0x1000)>>12 )<<12; // on arrondit au 1k suppérieur
//            print_segment_raw_content(&memory->seg[j]);
            j++;
        }
    }

    //TODO allouer la pile (et donc modifier le nb de segments)

   // printf("\n------ Fichier ELF \"%s\" : sections lues lors du chargement ------\n", name) ;
   // print_mem(memory);
    //stab32_print( symtab);

    // on fait le ménage avant de partir
    //del_mem(memory);
    //del_stab(symtab);
    fclose(pf_elf);
    //puts("");
    return 0;
}