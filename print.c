
/**
 * @file print.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:12:17 2012
 * @brief Printing stuff for SFS.
 *
 * Printing stuff for SFS.
 */

#include "print.h"

#include <stdio.h>

void sfs_print_atom( object o ) {
    if (o->type == SFS_INTEGER)
    {
    	printf("%d",o->this.integer);
    }
    if (o->type == SFS_CHARACTER)
    {	
    	printf("%c",o->this.character);
    }
    if (o->type == SFS_STRING)
    {
    	printf("%s",o->this.string);
    }
	
    return;
}

void sfs_print_pair( object o ) {
      printf("(");
     if ( (o->this.pair.car->type == SFS_PAIR && o->this.pair.cdr->type == SFS_NIL) || (o->this.pair.cdr->type == SFS_PAIR && o->this.pair.car->type == SFS_PAIR) )
    {
	if (o->this.pair.cdr->type == SFS_PAIR && o->this.pair.car->type == SFS_PAIR )
        {
            printf("(");
            sfs_print(o->this.pair.car);
            printf(" ");
            sfs_print(o->this.pair.cdr);
        }
        else
        {
            printf("(");
            sfs_print(o->this.pair.car);
            printf(")");
        }
    }
     else
    {
        if 	(o->this.pair.cdr->type != SFS_PAIR)
        {
            sfs_print(o->this.pair.car);
            printf(")");
        }
        else
        {
            sfs_print(o->this.pair.car);
            printf(" ");
            sfs_print(o->this.pair.cdr);
        }
    }
    

    return;
}

void sfs_print( object o ) {

    if ( SFS_PAIR == o->type ) {
        sfs_print_pair( o );
    }
    else {
        sfs_print_atom( o );
    }

}
