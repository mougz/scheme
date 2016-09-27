
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
object make_symbol( string chaine );

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

#ifdef __cplusplus
}
#endif

#endif /* _OBJECT_H_ */
