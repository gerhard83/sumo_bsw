/*******************************************************************************
 * @brief 	This is the interface entrance layer for students.
 *
 * @author 	Gerhard Freudenthaler, gefr@tf.uni-kiel.de, CAU Kiel
 * @date	 	10.01.2017
 *
 * @copyright 	LGPL-2.1, https://opensource.org/licenses/LGPL-2.1
 *
 *
 * ==============================================================================
 */

#ifndef RTE_H
#define RTE_H

#include "Platform.h"

#ifdef MASTER_RTE_C_
#define EXTERNAL_
#else
#define EXTERNAL_ extern
#endif

EXTERNAL_ StdRetType RTE_Write_LedRiOn();

EXTERNAL_ StdRetType RTE_Write_LedRiOff();

EXTERNAL_ StdRetType RTE_Write_LedRiNeg();

EXTERNAL_ StdRetType RTE_Write_LedRiSt(uint8_t state);

EXTERNAL_ StdRetType RTE_Read_LedRiSt(uint8_t *state);

EXTERNAL_ StdRetType RTE_Write_LedLeOn();

EXTERNAL_ StdRetType RTE_Write_LedLeOff();

EXTERNAL_ StdRetType RTE_Write_LedLeNeg();

EXTERNAL_ StdRetType RTE_Write_LedLeSt(uint8_t state);

EXTERNAL_ StdRetType RTE_Read_LedLeSt(uint8_t *state);


#ifdef EXTERNAL_
#undef EXTERNAL_
#endif

#endif /* RTE_H */
