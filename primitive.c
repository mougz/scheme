/**
 * @file primitive.c
 * @author Hugo Jallet
 * @date 16/11/16
 * @brief  Fonctions réalisant les primitives
 */

#include "primitive.h"
#include "list.h"




/* ATTENTION : Ces fonctions sont écrites pour être appellées sur le cdr de la liste entrée en argument (ie les arguments de la fonction SANS le symbole la représentant) */

/* ARITHMETIQUE */


object handlerPrimPlus(object o) 
{
    int s = 0;
    while (o->type != SFS_NIL)
    {   
	object eval_car;
        int type = (sfs_eval(car(o)))->type;
        eval_car = make_object(type);		
	if (eval_car->type != SFS_INTEGER)
        {
            WARNING_MSG("All arguments must be integer");
            return NULL;
        }

        s += eval_car->this.integer;   
        o = cdr(o);
    }

    return make_integer(s);

}

object handlerPrimMinus (object o)

{
    int s = 0;
    int one_arg = TRUE;

    /* On compte le 1er argument positivement */
    
        object eval_car = sfs_eval(car(o));
        s+= eval_car->this.integer;
        o = cdr(o);
                   
    while (o->type != SFS_NIL)
    {   
	one_arg = FALSE;

	object eval_car = sfs_eval(car(o));
	if (eval_car->type != SFS_INTEGER)
        {
            WARNING_MSG("All arguments must be integer");
            return NULL;
        }

        s -= eval_car->this.integer;  
        o = cdr(o);
    }
    if(one_arg)
    {
	s = -s;
    }
    return make_integer(s);

}

object handlerPrimMult (object o)

{
    int p = 1;
    while (o->type != SFS_NIL)
    {   object eval_car = sfs_eval(car(o));
	if (eval_car->type != SFS_INTEGER)
        {
            WARNING_MSG("All arguments must be integer");
            return NULL;
        }

        p *= eval_car->this.integer; 
        o = cdr(o);
    }

    return make_integer(p);

}

object handlerPrimQuot (object o)

{
    int q = 1;

    object eval_car = sfs_eval(car(o));
    q *= eval_car->this.integer;
    o = cdr(o);
   

    while (o->type != SFS_NIL)
{       object eval_car = sfs_eval(car(o));
        
        if (eval_car->type != SFS_INTEGER)
        {
            WARNING_MSG("All arguments must be integer");
            return NULL;
        }
        if (eval_car->this.integer == 0)
        {
            WARNING_MSG("You have divided by zero and open a blackhole. Well done.");
            return NULL;
        }


        q /= eval_car->this.integer;  
        o = cdr(o);
}

    return make_integer(q);

}

object handlerPrimRemain (object o)

{
    int r = 1;
    object eval_car = sfs_eval(car(o));
    r *= eval_car->this.integer;
    o = cdr(o);
       
    
    while (o->type != SFS_NIL)
    {   
        object eval_car = sfs_eval(car(o));

        if (eval_car->type != SFS_INTEGER)
        {
            WARNING_MSG("All arguments must be integer");
            return NULL;
        }
        if (eval_car->this.integer == 0)
        {
            WARNING_MSG("You have divided by zero and open a blackhole. Well done.");
            return NULL;
        }

        r %= eval_car->this.integer;  
        o = cdr(o);
    }

    return make_integer(r);

}


object handlerPrimSup (object o)

{   if (car(o)->type == SFS_NIL || cadr(o)->type == SFS_NIL)
    {
        WARNING_MSG("too few arguments (at least: 2)");
        return NULL;
    }

    while (o->type != SFS_NIL)
    {   object eval_car = sfs_eval(car(o));
	object eval_cadr = sfs_eval(cadr(o));

        if( eval_car <= eval_cadr )  
        {
            return faux;
        }
        o = cdr(o);
    }

    return vrai;
}

object handlerPrimInf (object o)

{   if (car(o)->type == SFS_NIL || cadr(o)->type == SFS_NIL)
    {
        WARNING_MSG("too few arguments (at least: 2)");
        return NULL;
    }

    while (o->type != SFS_NIL)
    {   object eval_car = sfs_eval(car(o));
	object eval_cadr = sfs_eval(cadr(o));

        if( eval_car >= eval_cadr )  
        {
            return faux;
        }
        o = cdr(o);
    }

    return vrai;
}

object handlerPrimEq (object o)

{   if (car(o)->type == SFS_NIL || cadr(o)->type == SFS_NIL)
    {
        WARNING_MSG("too few arguments (at least: 2)");
        return NULL;
    }

    while (o->type != SFS_NIL)
    {   object eval_car = sfs_eval(car(o));
	object eval_cadr = sfs_eval(cadr(o));

        if( eval_car != eval_cadr )  
        {
            return faux;
        }
        o = cdr(o);
    }

    return vrai;
}


/* PREDICATS */

object handlerPrimIsNull (object o)
{   if (cdr(o)->type == SFS_NIL)
    {
	WARNING_MSG("too many arguments (at most: 1)");
	return NULL;
    }
    object eval_car = sfs_eval(car(o));

    if (eval_car->type == SFS_NIL)
    {
	return vrai;
    }
    
    return faux;

}


object handlerPrimIsBool (object o)
{   if (cdr(o)->type == SFS_NIL)
    {
	WARNING_MSG("too many arguments (at most: 1)");
	return NULL;
    }
    object eval_car = sfs_eval(car(o));

    if (eval_car->type == SFS_BOOLEAN)
    {
	return vrai;
    }
    
    return faux;

}

object handlerPrimIsInt (object o)
{   if (cdr(o)->type == SFS_NIL)
    {
	WARNING_MSG("too many arguments (at most: 1)");
	return NULL;
    }
    object eval_car = sfs_eval(car(o));

    if (eval_car->type == SFS_INTEGER)
    {
	return vrai;
    }
    
    return faux;

}


object handlerPrimIsChar (object o)
{   if (cdr(o)->type == SFS_NIL)
    {
	WARNING_MSG("too many arguments (at most: 1)");
	return NULL;
    }
    object eval_car = sfs_eval(car(o));

    if (eval_car->type == SFS_CHARACTER)
    {
	return vrai;
    }
    
    return faux;

}


object handlerPrimIsString (object o)
{   if (cdr(o)->type == SFS_NIL)
    {
	WARNING_MSG("too many arguments (at most: 1)");
	return NULL;
    }
    object eval_car = sfs_eval(car(o));

    if (eval_car->type == SFS_STRING)
    {
	return vrai;
    }
    
    return faux;

}

object handlerPrimIsPair (object o)
{   if (cdr(o)->type == SFS_NIL)
    {
	WARNING_MSG("too many arguments (at most: 1)");
	return NULL;
    }
    object eval_car = sfs_eval(car(o));

    if (eval_car->type == SFS_PAIR)
    {
	return vrai;
    }
    
    return faux;

}


/* CONVERSIONS */


object handlerPrimChar2int(object o)
{
    object eval_car = sfs_eval(car(o));
    
    if (eval_car->type == SFS_CHARACTER && cdr(o)->type == SFS_NIL)
    {
        return make_integer((int)eval_car->this.character);
    }
     else if ( cdr(o)->type != SFS_NIL)
    {
        WARNING_MSG("Wrong number of arguments (expected: 1)");
        return NULL;
    }
    else
    {
        WARNING_MSG("Character required");
        return NULL;
    }
}

object handlerPrimInt2char(object o)
{
    object eval_car = sfs_eval(car(o));
    
    if (eval_car->type == SFS_INTEGER && cdr(o)->type == SFS_NIL)
    {
        return make_character((char)eval_car->this.integer);
    }
    else if ( cdr(o)->type != SFS_NIL)
    {
        WARNING_MSG("Wrong number of arguments (expected: 1)");
        return NULL;
    }
    else 
    {
        WARNING_MSG("Integer required");
        return NULL;
    }
}

object handlerPrimSymb2string(object o)
{
    object eval_car = sfs_eval(car(o));
    
    if (eval_car->type == SFS_SYMBOL && cdr(o)->type == SFS_NIL)
    {
        return make_string(eval_car->this.symbol);
    }
    else if ( cdr(o)->type != SFS_NIL)
    {
        WARNING_MSG("Wrong number of arguments (expected: 1)");
        return NULL;
    }
    else
    {
        WARNING_MSG("Symbol required");
        return NULL;
    }
}

object handlerPrimString2symb(object o)
{
    object eval_car = sfs_eval(car(o));
    
    if (eval_car->type == SFS_STRING && cdr(o)->type == SFS_NIL)
    {
        return make_symbol(eval_car->this.string);
    }
    else if ( cdr(o)->type != SFS_NIL)
    {
        WARNING_MSG("Wrong number of arguments (expected: 1)");
        return NULL;
    }
    else
    {
        WARNING_MSG("String required");
        return NULL;
    }
}

object handlerPrimInteger2string(object o)
{
    object eval_car = sfs_eval(car(o));
    
    if (eval_car->type == SFS_INTEGER && cdr(o)->type == SFS_NIL)
    {
        string str="";
        
        if (sprintf(str, "%d", eval_car->this.integer)<0)
        {
            WARNING_MSG("Error in conversion");
            return NULL;
        }
        else
        {
            return make_string(str);
        }
    }
    else if ( cdr(o)->type != SFS_NIL)
    {
        WARNING_MSG("Wrong number of arguments (expected: 1)");
        return NULL;
    }
    else
    {
        WARNING_MSG("Integer required");
        return NULL;
    }
}

/*object handlerPrimString2integer(object o) 
{

}*/



/* PRIMITIVES DE GESTION DE LISTES */


object handlerPrimCar(object o)
{   object Car;
    if (car(o)->type == SFS_PAIR)
    {
	Car = car(sfs_eval(car(o)));
        return Car;
    }
    else
    {
        WARNING_MSG("Argument of car must be a pair");
        return nil;
    }
}

object handlerPrimCdr(object o)
{
    if (o->type == SFS_PAIR)
    {
        return cdr(sfs_eval(car(o)));
    }
    else
    {
        WARNING_MSG("Argument of cdr must be a pair");
        return nil;
    }
}

object handlerPrimCons(object o)
{   if ( cddr(o)->type != SFS_NIL)
    {
	WARNING_MSG("Cons takes two arguments");
	return nil;
    }

    return make_pair(sfs_eval(car(o)),sfs_eval(cadr(o)));
}

object handlerPrimList(object o)
{
    if (cdr(o)->type == SFS_NIL)
    {
        return make_pair(sfs_eval(car(o)),nil);
    }
    else
    {
        return make_pair(sfs_eval(car(o)),handlerPrimCons(cdr(o)));
    }
}

/*****


object set_car(object o)
{
    /* erreur si trop ou pas assez d'arguments */
   /* if (cdr(o)->type==SFS_NIL || cddr(o)->type!=SFS_NIL || o->type == SFS_NIL)
   {
        WARNING_MSG("set-car! takes 2 arguments");
        return NULL;
    }
    else
    {
        object eval_car = sfs_eval(car(o));
        object eval_cadr = sfs_eval(cadr(o));/*nouveau car*/
        
        /*if (eval_car->type == SFS_PAIR)
        {
            eval_car->this.pair.car = eval_cadr;
            return eval_car;
        }
        else
        {
            WARNING_MSG("set-car! takes a pair in first argument");
            return NULL;
        }
    }
}
*/

/*object set_cdr(object o)
{
    /* erreur si trop ou pas assez d'arguments */
    /*if (cdr(o)->type==SFS_NIL || cddr(o)->type!=SFS_NIL || o->type == SFS_NIL)
    {
        WARNING_MSG("set-cdr! takes 2 arguments");
        return NULL;
    }
    else
    {
        object eval_car = sfs_eval(car(o));
        object eval_cadr = sfs_eval(cadr(o));/*nouveau cdr*/
        
      /*  if (eval_car->type == SFS_PAIR)
        {
            eval_car->this.pair.cdr = cons(eval_cadr,nil);
            return eval_car;
        }
        else
        {
            WARNING_MSG("set-cdr! takes a pair in first argument");
            return NULL;
        }
    }
}
*/






