
/**
 * @file object.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:02:57 2012
 * @brief Object definitions for SFS.
 *
 * Object definitions for SFS.
 */

#include "object.h"
#include "mem.h"


object make_object( uint type ) {

    object t = sfs_malloc( sizeof( *t ) );

    t->type = type;

    return t;
}

object make_nil( void ) {

    object t = make_object( SFS_NIL );

    t->this.special = t;

    return t;
}

object make_character(char caractere) {
    object t=make_object(SFS_CHARACTER);
    t->this.character=caractere;
    return t;
}


object make_string(char * str ) {
    object t=make_object(SFS_STRING);
    strcpy(t->this.string, str);
    return t;
}


object make_bool(void) {
    object t=make_object(SFS_BOOLEAN);
    t->this.special = t;
    return t;
}

object make_integer(int number) {
    object t=make_object(SFS_INTEGER);
    t->this.integer=number;
    return t;
}

object make_pair(object car,object cdr) {
    object t=make_object(SFS_PAIR);
    t->this.pair.car=car;
    t->this.pair.cdr=cdr;
    return t;
}

object make_symbol( string str )
{

    object t = make_object(SFS_SYMBOL);

    strcpy(t->this.symbol, str);

    return t;
}

object car(object o) {
    if ( o->type !=SFS_PAIR)
    {
        ERROR_MSG("Not a pair !");
    }
    return o->this.pair.car;
}

object cdr(object o) {
    if ( o->type !=SFS_PAIR)
    {
        ERROR_MSG("Not a pair !");
    }
    return o->this.pair.cdr;
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

object make_inf(void) {
    object t=make_object(SFS_INTEGER);
    t->this.special=t;
    return t;
}

/* FONCTIONS DE GESTION D'ENVIRONNEMENTS */

/* @fn object make_new_env(void)
 @brief : initialise un nouvel environnmenent vide */

object make_new_env(void)
{
    return nil;
}

object make_new_list_env(void)
{
    return nil;
}

/* @fn object init_top_level_env(void)
   @brief initialise l'environnment de top level */


/*object init_top_level_env(void){
	object top=make_new_env();
	top=make_pair(); */

object get_variable_value_env(object env, object symbole) {
    if (symbole->type!=SFS_SYMBOL)
    {
        WARNING_MSG("Not a symbol !");
        return NULL;
    }
    if (env!=nil)
    {
        if (strcmp(symbole->this.symbol,car(car(env))->this.symbol) == 0)
        {
            return car(env);
        }
        else
        {
            return get_variable_value_env(cdr(env),symbole);
        }
    }
    else
    {
        return NULL;
    }
}

object get_variable_value_list_env(object list_env, object symbole) 
{
    
    if (list_env->type != SFS_NIL)
    {
	
        if (get_variable_value_env(car(list_env),symbole) != NULL)
        {
            return get_variable_value_env(car(list_env),symbole);
        }
        else
        {
            return get_variable_value_list_env(cdr(list_env),symbole);
        }
    }
    else
    {
        return NULL;
    }
}
void enlever_env(object* list_env){
    object list_env_temp = cdr(*list_env);
    *list_env = list_env_temp;
    return;
}

void ajouter_env(object env, object* list_env){
    object list_env_temp=make_pair(env,*list_env);
    *list_env = list_env_temp;
    return;
}

void ajouter_variable(object* list_env,object symbole,object valeur) {
        object variable=make_pair(symbole,valeur);
	object env_courant=car(*list_env);
    	object env_temp=make_pair(variable,env_courant);
    	enlever_env(list_env);
    	ajouter_env(env_temp,list_env);
    	return;
}

object retourner_valeur_symbole_env(object symbole,object env) {
    if (get_variable_value_env(env,symbole)==NULL)
    {
        return nil;
    }
    else
    {
        return cdr(get_variable_value_env(env,symbole));
    }
}

object retourner_valeur_symbole_list_env(object symbole,object list_env) {
    if (cdr(list_env)==nil)
    {
        WARNING_MSG("Unbound variable !");
        return nil;
    }
    if (retourner_valeur_symbole_env(symbole,car(list_env))==nil)
    {
        return retourner_valeur_symbole_list_env(symbole,cdr(list_env));
    }
    else
    {
        return retourner_valeur_symbole_env(symbole,car(list_env));
    }
}

object envt_courant (object list_env)
{
    return car ( list_env);
}

