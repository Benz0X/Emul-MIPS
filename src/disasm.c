#include <stdint.h>
#include "disasm.h"
#include "define.h"
#include "fonctions.h"
#include "common/notify.h"

int disasm(uint32_t start_addr,uint32_t size) {
    if(memory==NULL) {
        WARNING_MSG("No memory loaded");
        return -1;
    }
    if (start_addr%4!=0) WARNING_MSG("%X mod 4 != 0, read instruction may not be alligned with real instruction",start_addr);
    uint32_t i=0;
    uint32_t current_addr=start_addr;
    instruction current_instr;
    uint32_t instr_value;
    int text_ident;
    int j,k;


    //get .text scnidx in order to show only .text label
    for (k = 1; k < symtab.size; ++k) {
        if(!strcmp(symtab.sym[k].name,".text")) {
            text_ident=symtab.sym[k].scnidx;
            break;
        }
    }




    while (i<size) {
        j=0;
        while(current_addr>=memory->seg[j].start._32 && j < memory->nseg)
        {
            j++;
        }
        //printf("%d %d\n",i,j );
        if( j>0 && !strcmp(memory->seg[j-1].name,".text") && current_addr < memory->seg[j-1].start._32+memory->seg[j-1].size._32) {

            getInstr(current_addr,&current_instr);
            memcpy(&instr_value,&current_instr,4);
            printf("\n%X :: %8.8X	",current_addr,instr_value);


            int dico_entry=0;
            /*for (dico_entry = 0; dico_entry < 15; ++dico_entry)
            {
            printf("instr value : %X, mask : %X, comb : %X instr : %X\n",instr_value,dico_data[dico_entry].mask,instr_value & dico_data[dico_entry].mask,dico_data[dico_entry].instr );

            }*/
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
                for (k = 1; k < symtab.size; ++k)
                {
                    if(((current_addr-memory->seg[j-1].start._32)==symtab.sym[k].addr._32)&&(symtab.sym[k].type != section) && (symtab.sym[k].scnidx == text_ident)) {
                        //printf("curr-start= %d size : %d %s\n", current_addr-memory->seg[j-1].start._32,symtab.sym[k].size,symtab.sym[k].name);
                        printf("%s: ",symtab.sym[k].name);
                        break;
                    }
                }



                printf("%s",dico_data[dico_entry].name );
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

                        printf(" %s, %s, %s",regname1,regname2,regname3);
                        dico_data[dico_entry].exec(current_instr);

                    }
                    //RT RD SA
                    else if(!strcmp("RT",dico_data[dico_entry].argname[0]) && !strcmp("RD",dico_data[dico_entry].argname[1]) && !strcmp("SA",dico_data[dico_entry].argname[2])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rd,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rt,regname2);
                        char regname3[MAX_NAME_SIZE];
                        parseReg(current_instr.r.sa,regname3);

                        printf(" %s, %s, %s",regname1,regname2,regname3);

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

                        printf(" %s, %s",regname1,regname2);

                    }
                    else if(!strcmp("RD",dico_data[dico_entry].argname[0]) && !strcmp("RS",dico_data[dico_entry].argname[1])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rd,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rs,regname2);

                        printf(" %s, %s",regname1,regname2);

                    }
                    else if(!strcmp("RT",dico_data[dico_entry].argname[0]) && !strcmp("RD",dico_data[dico_entry].argname[1])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rd,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rt,regname2);

                        printf(" %s, %s",regname1,regname2);

                    }
                    else {
                        WARNING_MSG("Unknown arg 1 or 2 for R command");
                        printf("\n");
                        return -1;
                    }

                    break;

                case 1:
                    if(!strcmp("RS",dico_data[dico_entry].argname[0])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rs,regname1);
                        printf(" %s",regname1);
                    }
                    else if(!strcmp("RD",dico_data[dico_entry].argname[0])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.r.rd,regname1);
                        printf(" %s",regname1);
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

                        printf(" %s, %s, %d",regname1,regname2,current_instr.i.immediate);

                    }
                    else if(!strcmp("RS",dico_data[dico_entry].argname[0]) && !strcmp("RT",dico_data[dico_entry].argname[1]) && !strcmp("OFFSET",dico_data[dico_entry].argname[2])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rs,regname1);
                        char regname2[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rt,regname2);

                        printf(" %s, %s, %d",regname1,regname2,4*current_instr.i.immediate);

                        for (k = 1; k < symtab.size; ++k) {
                            if(((current_addr+ 4 + 4*current_instr.i.immediate-memory->seg[j-1].start._32)==symtab.sym[k].addr._32)&&(symtab.sym[k].type != section)&& (symtab.sym[k].scnidx == text_ident)) {
                                printf(" <%s>",symtab.sym[k].name);
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
                        printf(" %s, %d(%s)",regname1,current_instr.i.immediate,regname2);

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

                        printf(" %s, %d",regname1,4*current_instr.i.immediate);

                        for (k = 1; k < symtab.size; ++k) {
                            if(((current_addr+ 4 + 4*current_instr.i.immediate-memory->seg[j-1].start._32)==symtab.sym[k].addr._32)&&(symtab.sym[k].type != section)&& (symtab.sym[k].scnidx == text_ident)) {
                                printf(" <%s>",symtab.sym[k].name);
                                break;
                            }
                        }

                    }
                    else if(!strcmp("RT",dico_data[dico_entry].argname[0]) && !strcmp("IM",dico_data[dico_entry].argname[1])) {
                        char regname1[MAX_NAME_SIZE];
                        parseReg(current_instr.i.rt,regname1);

                        printf(" %s, %d",regname1,current_instr.i.immediate);

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



                    printf(" %d",4*current_instr.j.target);
                    for (k = 1; k < symtab.size; ++k) {
                        if(((4*current_instr.j.target)==symtab.sym[k].addr._32)&&(symtab.sym[k].type != section)&& (symtab.sym[k].scnidx == text_ident)) {
                            printf(" <%s>",symtab.sym[k].name);
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
    printf("\n");
    return 0;
}
