
/**
 * @file object.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 17:49:46 2012
 * @brief Object definition for SFS.
 *
 * Object definition for SFS.
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "number.h"
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

typedef struct object_t {

    uint type;

    union {

        int              integer;
        char             character;
        string           string;
        string           symbol;

        struct pair_t {
            struct object_t *car;
            struct object_t *cdr;
        }                pair;

        struct object_t *special;

    } this;

} *object;


object make_object( uint type );
object make_nil( void );
object make_character(char caractere);
object make_string(string str);
object make_integer(int number);
object make_bool(void);
object make_pair(object car,object cdr);
object make_symbol( string str );
object make_inf(void);

object cdr(object o);
object car(object o);
void set_car(object o, object p);

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

#define SFS_INTEGER      0x00
#define SFS_CHARACTER    0x01
#define SFS_STRING       0x02
#define SFS_PAIR         0x03
#define SFS_NIL          0x04
#define SFS_BOOLEAN      0x05
#define SFS_SYMBOL       0x06


extern object nil;
extern object vrai;
extern object faux;
extern int inf;
extern object list_env;
extern object top_level_env;


#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
