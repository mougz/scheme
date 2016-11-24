
/**
 * @file environnement.c
 * @author Hugo Jallet
 * @date 16/11/16
 * @brief  Fonctions de gestion pour les environnements
 */


#include "environnement.h"



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
void enlever_env(object* list_env) {
    object list_env_temp = cdr(*list_env);
    *list_env = list_env_temp;
    return;
}

void ajouter_env(object env, object* list_env) {
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

void add_binding_to_list_env(object binding, object* list_env)
{
    object current_env=car(*list_env);
    object env_temp = make_pair(binding, current_env);
    enlever_env(list_env);
    ajouter_env(env_temp,list_env);
    return;
}
