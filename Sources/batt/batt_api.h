/***********************************************************************************************//**
 * @file		batt_api.h
 * @ingroup		batt
 * @brief 		API of the SWC @a Battery
 *
 * This API provides a BSW-internal interface of the SWC @ref batt. It is supposed to be available
 * to all other Basic Software Components.
 *
 * @author 	G. Freudenthaler, gefr@tf.uni-kiel.de, Chair of Automatic Control, University Kiel
 * @author 	(c) 2014 Erich Styger, erich.styger@hslu.ch, Hochschule Luzern
 * @date 	10.04.2017
 *
 * @note API for BSW-internal use only
 *
 * @copyright @LGPL2_1
 *
 ***************************************************************************************************/

#ifndef BATT_API_H_
#define BATT_API_H_

/*======================================= >> #INCLUDES << ========================================*/
#include "ACon_Types.h"


#ifdef MASTER_batt_C_
#define EXTERNAL_
#else
#define EXTERNAL_ extern
#endif

/**
 * @addtogroup batt
 * @{
 */
/*======================================= >> #DEFINES << =========================================*/



/*=================================== >> TYPE DEFINITIONS << =====================================*/



/*============================ >> GLOBAL FUNCTION DECLARATIONS << ================================*/
/**
 * @brief Does a measurement of the battery voltage
 * @param cvP Pointer to variable where to store the voltage in centi-voltage units (330 is 3.3V)
 * @return Error code, ERR_OK if everything was fine
 */
EXTERNAL_ StdRtn_t BATT_MeasureBatteryVoltage(uint16_t *cvP);



/**
 * @}
 */
#ifdef EXTERNAL_
#undef EXTERNAL_
#endif

#endif /* !BATT_API_H_ */
