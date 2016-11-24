
/**
 * @file print.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:12:17 2012
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#include "print.h"
#include "list.h"
#include "environnement.h"

#include <stdio.h>

void sfs_print_atom( object o ) {
    if (o->type==SFS_NIL)
    {
        printf("()");
    }

    if (o->type==SFS_INTEGER_PINF)
    {
        printf("+inf");
    }
    if (o->type==SFS_INTEGER_MINF)
    {
        printf("-inf");
    }

    if ( o->type == SFS_SYMBOL)
    {
        printf( "%s", o->this.symbol );
    }

    if (o->type == SFS_INTEGER)
    {
        printf("%d",o->this.integer);
    }
 

    if (o->type == SFS_CHARACTER)
    {
        switch (o->this.character)
        {

        case '\n':
            printf("#\\newline");
            break;

        case ' ':
            printf("#\\space");
            break;

        default :
            printf("#\\%c",o->this.character);
            break;
        }
    }

    if (o->type == SFS_STRING)
    {
        printf("\"%s\"",o->this.string);
    }

    if (o->type == SFS_BOOLEAN)
    {
        if (o->this.special==vrai)
        {
            printf("#t");
        }
        if (o->this.special==faux)
        {
            printf("#f");
        }
    }
    return;
}

void sfs_print_pair( object o ) {
    printf("(");
    sfs_print_interieur(car(o),cdr(o));
    printf(")");
}


void sfs_print( object o ) {

    if ( SFS_PAIR == o->type ) {
        sfs_print_pair( o );
    }
    else {
        sfs_print_atom( o );
    }

}

void sfs_print_interieur(object car,object cdr){
	sfs_print(car);
	if (cdr->type==SFS_NIL)
	{
		return;
	}
	printf(" ");
	if (cdr->this.pair.cdr->type==SFS_NIL)
	{
		sfs_print(cdr->this.pair.car);
	}
	else
	{
		sfs_print_interieur(cdr->this.pair.car,cdr->this.pair.cdr);
	}
}

