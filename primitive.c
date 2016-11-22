/**
 * @file primitive.c
 * @author Hugo Jallet
 * @date 16/11/16
 * @brief  Fonctions réalisant les primitives
 */

#include "primitive.h"


void init_primitiv(object env)
{
make_primitive(handlerPrimPlus);
ajouter_variable(env,+,handlerPrimPlus);
/* On reitere l'opération pour toutes les primitives*/

}


object handlerPrimPlus(object o)
{
    int s = 0;
    object cdr = cdr(o);
    while (cdr != nil)
    {   if (car(cdr)->this.type != SFS_INTEGER)
	{
		WARNING_MSG("All arguments must be integer")
		return NULL;
	}

	s += car(cdr)->this.integer;
	cdr = cdr(cdr);
    }
    
    return s;

}

