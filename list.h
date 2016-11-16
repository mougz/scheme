
/**
 * @file list.h
 * @author Hugo Jallet
 * @date 16/11/16
 * @brief  Definition of functions for lists
 */
#ifndef _LIST_H_
#define _LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"
#include "environnement.h"

object cdr(object o);
object car(object o);
void set_car(object o, object p);
object caar (object o);
object cadr (object o);
object cdar (object o);
object cddr (object o);
object caddr (object o);
object cdddr (object o);

#ifdef __cplusplus
}
#endif

#endif /* _LIST_H_ */
