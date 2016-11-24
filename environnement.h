/**
 * @file environnement.h
 * @author Hugo Jallet
 * @date 16/11/16
 * @brief  Definition of functions for environments
 */

#ifndef _ENVIRONNEMENT_H_
#define _ENVIRONNEMENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"
#include "list.h"

object make_new_env(void);
object make_new_list_env(void);
object get_variable_value_env(object env, object symbole);
object get_variable_value_list_env(object list_env, object symbole);

void ajouter_env(object env, object* list_env);
void enlever_env(object* list_env);
void ajouter_variable(object* env,object symbole,object valeur);
object retourner_valeur_symbole_env(object symbole,object env);
object retourner_valeur_symbole_list_env(object symbole,object list_env);
object envt_courant (object list_env);
void add_binding_to_list_env(object binding, object* list_env);


#ifdef __cplusplus
}
#endif

#endif /* _ENVIRONNEMENT_H_ */
