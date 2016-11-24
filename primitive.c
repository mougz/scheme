/**
 * @file primitive.c
 * @author Hugo Jallet
 * @date 16/11/16
 * @brief  Fonctions réalisant les primitives
 */

#include "primitive.h"
#include "list.h"
#include "environnement.h"
#include "eval.h"

/* Fonction d'initialisation des primitives : on crée toutes les paires symboles/primitives et on appelle cette fonction dans repl.c */


void init_primitive()
{
    object add=make_pair(make_symbol("+"),make_primitive(handlerPrimPlus));
    add_binding_to_list_env(add, &list_env);

     object diff=make_pair(make_symbol("-"),make_primitive(handlerPrimMinus));
     add_binding_to_list_env(diff, &list_env);

     object prod=make_pair(make_symbol("*"),make_primitive(handlerPrimMult));
     add_binding_to_list_env(prod, &list_env);

     object div=make_pair(make_symbol("/"),make_primitive(handlerPrimQuot));
     add_binding_to_list_env(div, &list_env);

     object rem =make_pair(make_symbol("remainder"),make_primitive(handlerPrimRemain));
     add_binding_to_list_env(rem,&list_env);

     object sup = make_pair(make_symbol(">"),make_primitive(handlerPrimSup));
     add_binding_to_list_env(sup,&list_env);

     object inf = make_pair(make_symbol("<"),make_primitive(handlerPrimInf));
     add_binding_to_list_env(inf,&list_env);

     object equal =make_pair(make_symbol("="),make_primitive(handlerPrimEq));
     add_binding_to_list_env(equal,&list_env);


    object null = make_pair( make_symbol("null?") , make_primitive(handlerPrimIsNull));
    add_binding_to_list_env(null, &list_env);

    object boolean = make_pair( make_symbol("boolean?") , make_primitive(handlerPrimIsBool));
    add_binding_to_list_env(boolean, &list_env);

    object integer = make_pair( make_symbol("integer?") , make_primitive(handlerPrimIsInt));
    add_binding_to_list_env(integer, &list_env);
   
    object symbol = make_pair( make_symbol("symbol?") , make_primitive(handlerPrimIsSymb));
    add_binding_to_list_env(symbol, &list_env);
    
    object charac = make_pair( make_symbol("char?") , make_primitive(handlerPrimIsChar));
    add_binding_to_list_env(charac, &list_env);

    object string = make_pair( make_symbol("string?") , make_primitive(handlerPrimIsString));
    add_binding_to_list_env(string, &list_env);

    object pair = make_pair( make_symbol("pair?") , make_primitive(handlerPrimIsPair));
    add_binding_to_list_env(pair, &list_env);


    object char2int = make_pair( make_symbol("char->integer") , make_primitive(handlerPrimChar2int));
    add_binding_to_list_env(char2int, &list_env);

    object int2char = make_pair( make_symbol("integer->char") , make_primitive(handlerPrimInt2char));
    add_binding_to_list_env(int2char, &list_env);

    object symb2string = make_pair( make_symbol("symbol->string") , make_primitive(handlerPrimSymb2string));
    add_binding_to_list_env(symb2string, &list_env);
   
    object string2symb = make_pair( make_symbol("string->symbol") , make_primitive(handlerPrimString2symb));
    add_binding_to_list_env(string2symb, &list_env);
    
    object numb2string = make_pair( make_symbol("number->string") , make_primitive(handlerPrimInt2string));
    add_binding_to_list_env(numb2string, &list_env);


    object primCar = make_pair( make_symbol("car") , make_primitive(handlerPrimCar));
    add_binding_to_list_env(primCar, &list_env);

    object primCdr = make_pair( make_symbol("cdr") , make_primitive(handlerPrimCdr));
    add_binding_to_list_env(primCdr, &list_env);

    object cons = make_pair( make_symbol("cons") , make_primitive(handlerPrimCons));
    add_binding_to_list_env(cons, &list_env);
   
    object primList = make_pair( make_symbol("list") , make_primitive(handlerPrimList));
    add_binding_to_list_env(primList, &list_env);
    
    object setcar = make_pair( make_symbol("set-car!") , make_primitive(handlerPrimSet_car));
    add_binding_to_list_env(setcar, &list_env);

    object setcdr = make_pair( make_symbol("set-cdr!") , make_primitive(handlerPrimSet_cdr));
    add_binding_to_list_env(setcdr, &list_env);

    object egpol = make_pair( make_symbol("eq?") , make_primitive(handlerPrimEgPol));
    add_binding_to_list_env(egpol, &list_env);


    

    
    
}

/* ATTENTION : Les fonctions suivantes sont écrites pour être appellées sur le cdr de la liste entrée en argument (ie la liste des arguments de la primitive SANS le symbole la représentant) */

/* ARITHMETIQUE */


object handlerPrimPlus(object o)
{
    object elt;
    int somme=0;
    if (o==nil)
    {
        return make_integer(somme);
    }
    if (o->type!=SFS_PAIR)
    {
        WARNING_MSG("Erreur de syntaxe addition");
        return NULL;
    }

    while (o!=nil)
    {
        elt=sfs_eval(o->this.pair.car);
        if (elt->type!=SFS_INTEGER || elt==NULL)
        {
            WARNING_MSG("Erreur addition : les éléments ne sont pas tous entiers");
            return NULL;
        }
        somme+=elt->this.integer;
        o=o->this.pair.cdr;
    }
    return make_integer(somme);

}


object handlerPrimMinus (object o)

{
    object elt;
    int diff=0;
    if (o==nil)
    {
        return make_integer(diff);
    }
    if (o->type!=SFS_PAIR)
    {
        WARNING_MSG("Erreur de syntaxe soustraction");
        return NULL;
    }
    if(o->this.pair.cdr->type == SFS_PAIR && o->this.pair.cdr->this.pair.car->type == SFS_INTEGER)
    {
        elt=sfs_eval(o->this.pair.car);
        diff+=elt->this.integer;
        o=o->this.pair.cdr;
    }

    while (o!=nil)
    {
        elt=sfs_eval(o->this.pair.car);
        if (elt->type!=SFS_INTEGER || elt==NULL)
        {
            WARNING_MSG("Erreur soustraction : les éléments ne sont pas tous entiers");
            return NULL;
        }
        diff-=elt->this.integer;
        o=o->this.pair.cdr;
    }
    return make_integer(diff);

}

object handlerPrimMult(object o)
{
    object elt;
    int prod=1;
    if (o==nil)
    {
        return make_integer(prod);
    }
    if (o->type!=SFS_PAIR)
    {
        WARNING_MSG("Erreur de syntaxe produit");
        return NULL;
    }

    while (o!=nil)
    {
        elt=sfs_eval(o->this.pair.car);
        if (elt->type!=SFS_INTEGER || elt==NULL)
        {
            WARNING_MSG("Erreur produit : les éléments ne sont pas tous entiers");
            return NULL;
        }
        prod*=elt->this.integer;
        o=o->this.pair.cdr;
    }
    return make_integer(prod);

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

    while (o->type != SFS_NIL && cdr(o) != nil)
    {   object eval_car = sfs_eval(car(o));
	object eval_cadr = sfs_eval(cadr(o));

        if( eval_car->this.integer <= eval_cadr->this.integer )
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

    while (o->type != SFS_NIL && cdr(o) != nil)
    {   object eval_car = sfs_eval(car(o));
	object eval_cadr = sfs_eval(cadr(o));

        if( eval_car->this.integer >= eval_cadr->this.integer )
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

    while (o->type != SFS_NIL && cdr(o) != nil)
    {   object eval_car = sfs_eval(car(o));
	object eval_cadr = sfs_eval(cadr(o));

        if( eval_car->this.integer != eval_cadr->this.integer )
        {
            return faux;
        }
        o = cdr(o);
    }

    return vrai;
}



/* PREDICATS */



object handlerPrimIsNull (object o)
{   if (cdr(o)->type != SFS_NIL)
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
{   if (cdr(o)->type != SFS_NIL)
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
{   if (cdr(o)->type != SFS_NIL)
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
{   if (cdr(o)->type != SFS_NIL)
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
{   if (cdr(o)->type != SFS_NIL)
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

object handlerPrimIsSymb (object o)
{   if (cdr(o)->type != SFS_NIL)
    {
	WARNING_MSG("too many arguments (at most: 1)");
	return NULL;
    }
    object eval_car = sfs_eval(car(o));

    if (eval_car->type == SFS_SYMBOL)
    {
	return vrai;
    }

    return faux;

}


object handlerPrimIsPair (object o)
{   if (cdr(o)->type != SFS_NIL)
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

object handlerPrimInt2string(object o)
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





/* PRIMITIVES DE GESTION DE LISTES */


object handlerPrimCar(object o)
{   
    if (car(o)->type == SFS_PAIR)
    {
	return car(sfs_eval(car(o)));
        
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
        return make_pair(sfs_eval(car(o)),handlerPrimList(cdr(o)));
    }
}



object handlerPrimSet_car(object o)
{

   if (cdr(o)->type == SFS_NIL || cddr(o)->type != SFS_NIL || o->type == SFS_NIL)
   {
        WARNING_MSG("Wrong number of arguments (have 2)");
        return NULL;
    }
    else
    {
        object eval_car = sfs_eval(car(o));
        object eval_cadr = sfs_eval(cadr(o));

        if (eval_car->type == SFS_PAIR)
        {
            set_car(eval_cadr, eval_car) = ;
            return NULL;
        }
        else
        {
            WARNING_MSG("First argument must be a pair");
            return NULL;
        }
    }
}


object handlerPrimSet_cdr(object o)
{

    if (cdr(o)->type == SFS_NIL || cddr(o)->type != SFS_NIL || o->type == SFS_NIL)
    {
        WARNING_MSG("Wrong number of arguments (have 2)");
        return NULL;
    }
    else
    {
        object eval_car = sfs_eval(car(o));
        object eval_cadr = sfs_eval(cadr(o));

        if (eval_car->type == SFS_PAIR)
        {
            eval_car->this.pair.cdr = make_pair(eval_cadr,nil);
            return NULL;
        }
        else
        {
            WARNING_MSG("First argument must be a pair");
            return NULL;
        }
    }
}


/*egalité polymorphique */

object handlerPrimEgPol (object o)
{   if (cdr(o)->type == SFS_NIL || cddr(o)->type!=SFS_NIL || o->type == SFS_NIL)
    {
        WARNING_MSG("Wrong number of arguments (have 2)");
        return NULL;
    }
    else
    {
        object eval_car = sfs_eval(car(o));
        object eval_cadr = sfs_eval(cadr(o));
        if (eval_car->type == eval_cadr->type)
        {
            return vrai;
        }
    }
}








