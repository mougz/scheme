/**
 * @file primitive.h
 * @author Hugo Jallet
 * @date 16/11/16
 * @brief  Definition of functions for primitives
 */

#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"
#include "environnement.h"

void init_primitiv();

object handlerPrimPlus(object o);
object handlerPrimMinus(object o);
object handlerPrimMult(object o);
object handlerPrimQuot(object o);
object handlerPrimRemain(object o);
object handlerPrimSup (object o);
object handlerPrimInf (object o);
object handlerPrimEq (object o);
object handlerPrimIsNull(object o);
object handlerPrimIsBool (object o);
object handlerPrimIsInt (object o);
object handlerPrimIsSymb (object o);
object handlerPrimIsChar (object o);
object handlerPrimIsString (object o);
object handlerPrimIsPair (object o);
object handlerPrimChar2int(object o);
object handlerPrimInt2char(object o);
object handlerPrimSymb2string(object o);
object handlerPrimString2symb(object o);
object handlerPrimInt2string(object o);
object handlerPrimCar(object o);
object handlerPrimCdr(object o);
object handlerPrimCons(object o);
object handlerPrimList(object o);
object handlerPrimSet_car(object o);
object handlerPrimSet_cdr(object o);
object handlerPrimEgPol (object o);


#ifdef __cplusplus
}
#endif

#endif /* _PRIMITIVE_H_ */
