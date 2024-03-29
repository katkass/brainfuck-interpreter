#include <stdio.h>
#include <stdlib.h>
#include "mem.h"
#include "stack.h"

enum instructions_e {
    /* Liiguta mälu indeksit paremale. */
    BF_RIGHT      = '>',

    /* Liiguta mälu indeksit vasakule. */
    BF_LEFT       = '<',

    /* Suurenda mälu indeksil olevat väärtust. */
    BF_INCREASE   = '+',

    /* Vähenda mälu indeksil olevat väärtust. */
    BF_DECREASE   = '-',

    /* Loe kasutajalt sisendit ja salvesta see mäluindeksile. */
    BF_READ       = ',',

    /* Trüki välja mälu indeksil olev väärtus char'ina. */
    BF_PRINT      = '.',

    /* Tsükli algus. */
    BF_START_LOOP = '[',

    /* Tsükli lõpp. */
    BF_END_LOOP   = ']',

    /* Trüki silumise informatsioon. */
    BF_DEBUG      = '#'
};

void interpret(char *program) {
    int i = 0;

    while (program[i] != 0 ) {
        switch (program[i]) {
            case BF_START_LOOP: {
                if (mem_get() != 0) {
                    stack_push(i);
                } 
                else { //mem_get() on 0, seega ei lähe sisse
                    int brackets = 1;
                    while (brackets != 0) {
                        i++; //Vaatame programmis edasi
                        if (program[i] == BF_START_LOOP) {
                            brackets++;
                        }
                        else if (program[i] == BF_END_LOOP) {
                            brackets--;
                        }
                        else if (program[i] == 0) {
                            printf("Vigane programm!\n");
                            return;
                        }
                    }
                }
                break;
            }
            case BF_END_LOOP: {
                if (!stack_isEmpty()) {
                    i = stack_pop();
                } else {
                    printf("Vigane programm!\n");
                    return;
                }
                continue;
            }
            case BF_INCREASE:
                mem_inc();
                break;
            case BF_DECREASE:
                mem_dec();
                break;
            case BF_RIGHT:
                mem_right();
                break;
            case BF_LEFT:
                mem_left();
                break;
            case BF_DEBUG:
                mem_printDebug();
                break;
            case BF_READ: {
                /* Loeme märgi standardsisendist (kasutaja sisestab konsooli). */
                int c = getc(stdin);
                if (EOF == c) {
                    /* Sisendi lõpu korral lõpetame interpretaatori töö. */
                    printf("Sisendi lõpp!\n");
                    return;
                }

                /* Lisame mällu loetud väärtuse. */
                mem_set((char) c);
                break;
            }
            case BF_PRINT: {
                char c = mem_get();
                printf("%c", c);
                break;
            }
            default:;
                /* Ignoreerime sümboleid, mida me ei tunne. */
                break;
        }

        i++;
    }
}

int main(int argc, char **argv) {
    /* Kontrollime, et programmile anti täpselt üks parameeter (lisaks programmi nimele endale). */
    if (argc != 2) {
        printf("Programmil peab olema üks parameeter, milleks on BF programm!\n");

        /* Tagastame veakoodi. */
        return 1;
    }

    /* Käivitame programmi, mille kasutaja andis käsurealt. */
    interpret(argv[1]);

    return 0;
}