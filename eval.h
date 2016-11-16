
/**
 * @file eval.h
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 22 20:10:38 2012
 * @brief Evaluation stuff for SFS.
 *
 * Evaluation stuff for SFS.
 */

#ifndef _EVAL_H_
#define _EVAL_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "object.h"


object sfs_eval( object input);
object sfs_eval_pair(object input);


int isquote (object o);
int isdefine (object o);
int is_set (object o);
int is_if (object o);
int is_and (object o);
int is_or (object o);

int is_reserved(string word);



#ifdef __cplusplus
}
#endif

#endif /* _EVAL_H_ */
