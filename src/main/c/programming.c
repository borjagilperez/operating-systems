/**
 * @brief Description...
 *
 * Long description...
 * @file programming.c
 * @author borjagilperez at github.com
 */

#include <stdio.h>

#include "const.h"
#include "pointers.h"

#define AINT_LEN 10

int main(int argc, char *argv[]) {
    int i, ai[AINT_LEN] = {0,1,2,3,4,5,6,7,8,9};
    char c = 'A', ac[] = "Information and Communications Technology";

    /* f1 */
    fprintf(stdout, "--- main ---\n\t\tc =\t%c\t&c = %p\n", c, &c);
    if(f1(c) != OK) {
        return ERR;
    }
    fprintf(stdout, "--- main ---\n\t\tc =\t%c\t&c = %p\n\n", c, &c);

    /* f2 */
    fprintf(stdout, "--- main ---\n\t\tc =\t%c\t&c = %p\n", c, &c);
    if(f2(&c) != OK) {
        return ERR;
    }
    fprintf(stdout, "--- main ---\n\t\tc =\t%c\t&c = %p\n\n", c, &c);

    /* f3 */
    fprintf(stdout, "--- main ---\n\t\tac = %p\t%s\n", ac, ac);
    if(f3(ac) != OK) {
        return ERR;
    }
    fprintf(stdout, "--- main ---\n\t\tac = %p\t%s\n\n", ac, ac);

    /* f4 */
    fprintf(stdout, "--- main ---\n\t\tac = %p\t%s\n", ac, ac);
    if(f4(ac) != OK) {
        return ERR;
    }
    fprintf(stdout, "--- main ---\n\t\tac = %p\t%s\n\n", ac, ac);

    /* f5 */
    fprintf(stdout, "--- main ---\n\t\tac = %p\t%s\n\t\tai = %p\t", ac, ac, ai);
    for(i=0; i < AINT_LEN; i++) {
        fprintf(stdout, "%d ", ai[i]);
    }
    fprintf(stdout, "\n");
    if(f5(ai, ac) != OK) {
        return ERR;
    }
    fprintf(stdout, "\n");

    /* f6 */
    fprintf(stdout, "--- main ---\n\t\tac = %p\t%s\n", ac, ac);
    if(f6(ac) != OK) {
        return ERR;
    }
    fprintf(stdout, "\n");

    fprintf(stdout, "Salida correcta\n");
    return OK;
}
