
/**
 * @file list.c
 * @author Hugo Jallet
 * @date 16/11/16
 * @brief  Manipulation fonctions for lists
 */

#include "list.h"

object car(object o) {
    if ( o->type !=SFS_PAIR)
    {
        WARNING_MSG("Not a pair !");
    }
    return o->this.pair.car;
}

object cdr(object o) {
    if ( o->type !=SFS_PAIR)
    {
        WARNING_MSG("Not a pair !");
    }
    return o->this.pair.cdr;
}

/* @fn : void set_car(object o, object p)
   @brief : initialise le car de la paire p avec o
   @pre: p doit être une paire   */

void set_car(object o, object p)
{   if ( p->type !=SFS_PAIR)
    {
        WARNING_MSG("Not a pair !");
    }
    p->this.pair.car = o ;
}


/* @fn : object caar (object o)
   @brief : renvoie le car du car de l'object o
   @pre: le car de o doit être une paire    */

object caar (object o)
{
    return car(o)->this.pair.car; /* le prerequis est vérifié par la fonction car */
}


/* @fn : object cadr (object o)
   @brief : renvoie le car du cdr de l'object o
   @pre: le cdr de o doit être une paire    */

object cadr (object o)
{
    return cdr(o)->this.pair.car;
}


/* @fn : object cdar(object o)
   @brief : renvoie le cdr du car de l'object o
   @pre: le car de o doit être une paire    */

object cdar (object o)
{
    return car(o)->this.pair.cdr;
}


/* @fn : object cddr(object o)
   @brief : renvoie le cdr du cdr de l'object o
   @pre: le cdr de o doit être une paire    */

object cddr(object o)
{
    return cdr(o)->this.pair.cdr;
}

/* @fn : object caddr(object o)
   @brief : renvoie le car du cdr du cdr de l'object o
   @pre: le cdr du cdr de o doit être une paire    */

object caddr (object o)
{
    return cddr(o)->this.pair.car;
}

/* @fn : object cdddr(object o)
   @brief : renvoie le cdr du cdr du cdr de l'object o
   @pre: le cdr du cdr de o doit être une paire    */

object cdddr (object o)
{
    return cddr(o)->this.pair.cdr;
}




