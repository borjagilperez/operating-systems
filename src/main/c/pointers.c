/**
 * @brief Description...
 *
 * Long description...
 * @file pointers.c
 * @author borjagilperez at github.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"
#include "pointers.h"

int f1(char c) {
    char *pc = NULL;

    fprintf(stdout, "--- f1 ---\n");
    fprintf(stdout, "BEFORE\t\tc =\t%c\t&c = %p\n", c, &c);
    pc = &c;
    fprintf(stdout, "BEFORE\t\t*pc =\t%c\tpc = %p\t&pc = %p\n", *pc, pc, &pc);
    *pc += 2;
    fprintf(stdout, "AFTER\t\tc =\t%c\t&c = %p\n", c, &c);
    fprintf(stdout, "AFTER\t\t*pc =\t%c\tpc = %p\t&pc = %p\n", *pc, pc, &pc);

    return OK;
}

int f2(char *pc) {

    fprintf(stdout, "--- f2 ---\n");
    fprintf(stdout, "BEFORE\t\t*pc =\t%c\tpc = %p\t&pc = %p\n", *pc, pc, &pc);
    *pc += 2;
    fprintf(stdout, "AFTER\t\t*pc =\t%c\tpc = %p\t&pc = %p\n", *pc, pc, &pc);

    return OK;
}

int f3(char *ac) {
    const char ac_f[] = "Cognitive Behavioral Psychology";

    fprintf(stdout, "--- f3 ---\n");
    fprintf(stdout, "BEFORE\t\tac = %p\t%s\n", ac, ac);
    ac = (char *)malloc((strlen(ac_f) + 1) * sizeof(char));
    strcpy(ac, ac_f);
    fprintf(stdout, "AFTER\t\tac = %p\t%s\n", ac, ac);

    /* Run with Valgrind, commenting and uncommenting the following instruction
     * helping you with the next commands:
     * $ make
     * $ make valgrind ARGS="./programming"
     */
    free(ac);

    return OK;
}

int f4(char *ac) {

    fprintf(stdout, "--- f4 ---\n");
    fprintf(stdout, "BEFORE\t\tac = %p\t%s\n", ac, ac);
    strcpy(ac, "Cognitive Behavioral Psychology");
    fprintf(stdout, "AFTER\t\tac = %p\t%s\n", ac, ac);

    return OK;
}

int f5(int *ai, char *ac) {

    fprintf(stdout, "--- f5 ---\n");
    fprintf(stdout, "\t\tac = %p\n\t\tai = %p\n", ac, ai);
    fprintf(stdout, "\t\tai[5] = %d\t\t\tis equivalent to\t*(ai + 5) = %d\n", ai[5], *(ai+5));
    fprintf(stdout, "\t\tac + 5 =\t\t%s\n", ac + 5);
    fprintf(stdout, "\t\tac[18] = %c\t\t\tis equivalent to\t*(ac + 18) = %c\n", ac[18], *(ac + 18));
    fprintf(stdout, "\t\t&(ac[18]) = %p\tis equivalent to\tac + 18 = %p\n", &(ac[18]), (ac+18));

    return OK;
}

int f6(char *ac) {

    fprintf(stdout, "--- f6 ---\n");
    fprintf(stdout, "\t\tac = %p\t%s\n", ac, ac);
    fprintf(stdout, "\t\tac = %p\tsizeof(ac) = %ld Byte(s)\n", ac, sizeof(ac));
    fprintf(stdout, "\t\tstrlen(ac) =\t\t%ld characters\n", strlen(ac));
    fprintf(stdout, "\t\tsizeof(ac[0]) =\t\t%ld Byte(s)\n", sizeof(ac[0]));
    fprintf(stdout, "\t\tPrint character '\\0' =\t<%c> but does not print anything!\n", '\0');
    fprintf(stdout, "\t\tsizeof((char)'\\0') =\t%ld Byte(s)\n", sizeof((char)'\0'));
    fprintf(stdout, "\t\tsizeof(char) =\t\t%ld Byte(s)\n", sizeof(char));
    fprintf(stdout, "\t\tstrlen(ac) * sizeof(char) = %ld Byte(s)\n", strlen(ac) * sizeof(char));
    fprintf(stdout, "\tIt is necessary to add 1 when memory is reserved with malloc to make space for '\\0', for example running\n\t(char *)malloc((strlen(ac) + 1) * sizeof(char))\n");

    return OK;
}
