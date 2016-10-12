
/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"

object sfs_eval( object input )
{
    /* OBJET AUTOEVALUANT */
    if ( input-> type != SFS_PAIR && input-> type != SFS_SYMBOL)
    {
        return input;
    }

    /* SYMBOLE */
    if ( input->type == SFS_SYMBOL)
    {
        return input; /* A MODIFIER */
    }

    /* LISTE */
    if ( input->type == SFS_PAIR)
    {   /* quote */
        if ( isquote(car(input)) == 0 )
        {
            if ( cddr(input) != nil)
            {
                ERROR_MSG("Not a valid quote !");
            }
            return cadr(input);
        }

        /* define */
        if ( isdefine(car(input)) == 0 )
        {
            return input ;  /* A MODIFIER */
        }

        /* set */
        if ( is_set(car(input)) == 0 )
        {
            return input ;  /* A MODIFIER */
        }

        /* and */
        if ( is_and(car(input)) == 0 )
        {   input = cdr(input) ;
            while ( input != nil)
            {   if ( input->type != SFS_PAIR)
                {
                    return NULL;
                    WARNING_MSG("Not a pair !");
                }
                if ( sfs_eval(car(input))== faux)
                {
                    return faux;
                }
                input = cdr(input);
            }

            return vrai;
        }

        /* or */
        if ( is_or(car(input)) == 0 )
        {   input = cdr(input) ;
            while ( input != nil)
            {   if ( input->type != SFS_PAIR)
                {
                    return NULL;
                    WARNING_MSG("Not a pair !");
                }
                if ( sfs_eval(car(input))!= faux)
                {
                    return vrai;
                }
                input = cdr(input);
            }

            return faux;

        }

        /* if */
        if ( is_if(car(input)) == 0 )
        {   if ( sfs_eval(cadr(input)) == vrai)
            {
                return sfs_eval(caddr(input));
            }
            else
            {
                input = car(cdddr(input));
                if (input != nil)
                {
                    return sfs_eval(input);
                }
                else
                {
                    return faux;
                }
            }


        }

    }
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

/* @fn : void set_car(object o, object p)
   @brief : initialise le car de la paire p avec o
   @pre: p doit être une paire   */

void set_car(object o, object p)
{   if ( p->type !=SFS_PAIR)
    {
        ERROR_MSG("Not a pair !");
    }
    p->this.pair.car = o ;
}

/* @fn : int isquote (object o)
   @brief : verifie que o est "quote" et retourne 0 si c'est le cas, 1 sinon.
   @pre: o doit être un symbole  */
int isquote (object o)
{   if ( o->type != SFS_SYMBOL )
    {
        ERROR_MSG("Not a symbol !");
    }
    if (strcmp(o->this.symbol,"quote") == 0)
    {
        return 0;
    }

    return 1;

}

/* @fn : int isdefine (object o)
   @brief : verifie que o est "define" et retourne 0 si c'est le cas, 1 sinon.
   @pre: o doit être un symbole  */

int isdefine (object o)
{   if ( o->type != SFS_SYMBOL )
    {
        ERROR_MSG("Not a symbol !");
    }
    if (strcmp(o->this.symbol,"define") == 0)
    {
        return 0;
    }

    return 1;

}

/* @fn : int is_set (object o)
   @brief : verifie que o est "set" et retourne 0 si c'est le cas, 1 sinon.
   @pre: o doit être un symbole  */

int is_set (object o)
{   if ( o->type != SFS_SYMBOL )
    {
        ERROR_MSG("Not a symbol !");
    }
    if (strcmp(o->this.symbol,"set") == 0)
    {
        return 0;
    }

    return 1;
}

/* @fn : int is_if (object o)
   @brief : verifie que o est "if" et retourne 0 si c'est le cas, 1 sinon.
   @pre: o doit être un symbole  */

int is_if (object o)
{   if ( o->type != SFS_SYMBOL )
    {
        ERROR_MSG("Not a symbol !");
    }
    if (strcmp(o->this.symbol,"if") == 0)
    {
        return 0;
    }
    return 1;
}

/* @fn : int is_and (object o)
   @brief : verifie que o est "and" et retourne 0 si c'est le cas, 1 sinon.
   @pre: o doit être un symbole  */

int is_and (object o)
{   if ( o->type != SFS_SYMBOL )
    {
        ERROR_MSG("Not a symbol !");
    }
    if (strcmp(o->this.symbol,"and") == 0)
    {
        return 0;
    }
    return 1;
}

/* @fn : int is_or (object o)
   @brief : verifie que o est "or" et retourne 0 si c'est le cas, 1 sinon.
   @pre: o doit être un symbole  */

int is_or (object o)
{
    if ( o->type != SFS_SYMBOL )
    {
        ERROR_MSG("Not a symbol !");
    }
    if (strcmp(o->this.symbol,"or") == 0)
    {
        return 0;
    }
    return 1;
}

































