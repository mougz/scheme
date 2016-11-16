
/**
 * @file eval.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:11:30 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#include "eval.h"
#include "environnement.h"
#include "list.h"

object sfs_eval( object input )
{  
    /* OBJET AUTOEVALUANT */
    if ( input-> type != SFS_PAIR && input-> type != SFS_SYMBOL)
    {   
        return input;
    }

    /* SYMBOLE */
    if (input->type == SFS_SYMBOL)
    {
        if(get_variable_value_list_env(list_env, input) == NULL)
        {
            return input;
        }


        return retourner_valeur_symbole_env(input,envt_courant(list_env));
	

    }

    /* LISTE */
    if(input->type == SFS_PAIR)
    {
	return sfs_eval_pair(input);
    }
} 
  


object sfs_eval_pair(object input)
{

/* quote */
	
        if ( isquote(car(input)) == 0 )
        {
            if ( cddr(input) != nil)
            {
               WARNING_MSG("Quote takes 1 argument !");
            }
            return cadr(input);
        }
        /* define */
        if ( isdefine(car(input)) == 0 )
        {
            if (cdr(input)->type==SFS_NIL || cddr(input)->type==SFS_NIL || cdddr(input) != nil)
            {
               WARNING_MSG("define takes 2 arguments!");
            }
	    
	    if (is_reserved(cadr(input)->this.symbol)==1)
            {
            	WARNING_MSG("Symbol used is reserved to scheme");
            	return NULL;
            }


            object symbole = cadr(input);
            object val = sfs_eval(caddr(input));

            ajouter_variable(&list_env,symbole,val);
            return symbole ;
        }

        /* set! */
        if ( is_set(car(input)) == 0 )
        {
            if (cdr(input)->type==SFS_NIL || cddr(input)->type==SFS_NIL || cdddr(input) != nil)
            {
                WARNING_MSG("Set takes 2 arguments !");
		return NULL;
            }

            if (get_variable_value_list_env(list_env, cadr(input)) == NULL)
            {
                WARNING_MSG (" Undefined variable !");
		return nil;
            }

	    if (is_reserved(cadr(input)->this.symbol)==1)
            {
            	WARNING_MSG("Symbol used is reserved to scheme");
            	return NULL;
            }

            object symbole = cadr(input);
            object new_val = caddr(input);
	    object couple_actuel = get_variable_value_list_env(list_env, cadr(input));

            object old_val = cdr(couple_actuel);
            memcpy(old_val,cdr(couple_actuel),sizeof(*old_val));
            
            memcpy(cdr(couple_actuel),new_val,sizeof(*new_val));
           
            return symbole;


            
            return symbole ;
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
	object res_car=sfs_eval(car(input));
	if (cdr(input)!=nil)
	{
		return make_pair(res_car,sfs_eval(cdr(input)));
	}
	else
	{
		return make_pair(res_car,sfs_eval(cadr(input)));
	}
}



/* @fn : int isquote (object o)
   @brief : verifie que o est "quote" et retourne 0 si c'est le cas, 1 sinon.
   @pre: o doit être un symbole  */
int isquote (object o)
{  /* if ( o->type != SFS_SYMBOL )
    {
        WARNING_MSG("Not a symbol !");
    }*/
    if ( o->type == SFS_NIL )
    {
        ERROR_MSG("Nil !");
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
{      
   /* if ( o->type != SFS_SYMBOL )
    {
        WARNING_MSG("Not a symbol !");
    }*/
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
{  /* if ( o->type != SFS_SYMBOL )
    {
        WARNING_MSG("Not a symbol !");
    }*/
    if (strcmp(o->this.symbol,"set!") == 0)
    {
        return 0;
    }

    return 1;
}

/* @fn : int is_if (object o)
   @brief : verifie que o est "if" et retourne 0 si c'est le cas, 1 sinon.
   @pre: o doit être un symbole  */

int is_if (object o)
{  /* if ( o->type != SFS_SYMBOL )
    {
        WARNING_MSG("Not a symbol !");
    }*/
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
{  /* if ( o->type != SFS_SYMBOL )
    {
        WARNING_MSG("Not a symbol !");
    }*/
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

 /*   if ( o->type != SFS_SYMBOL )
    {
        WARNING_MSG("Not a symbol !");
    }*/
    if (strcmp(o->this.symbol,"or") == 0)
    {
        return 0;
    }
    return 1;
}


/*
 @fn int is_reserved(string word)
 
 @brief renvoie 1 si word est un mot reservé du scheme(forme, primitives,..), 0 sinon.

A completer dans l'increment 3
 */


int is_reserved(string word)
{
    if (strcmp(word,"define")==0)
        return 1;
    if (strcmp(word,"if")==0)
        return 1;
    if (strcmp(word,"and")==0)
        return 1;
    if (strcmp(word,"or")==0)
        return 1;
    if (strcmp(word,"car")==0)
        return 1;
    if (strcmp(word,"cdr")==0)
        return 1;
    if (strcmp(word,"+")==0)
        return 1;
    if (strcmp(word,"-")==0)
        return 1;
    if (strcmp(word,"*")==0)
        return 1;
    if (strcmp(word,"/")==0)
        return 1;
    else
        return 0;
}































