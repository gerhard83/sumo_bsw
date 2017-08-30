/***********************************************************************************************//**
 * @file		tacho_api.h
 * @ingroup		tacho
 * @brief 		API of the SWC @a Tacho
 *
 * This API provides a BSW-internal interface of the SWC @ref tacho. It is supposed to be
 * available to all other Basic Software Components.
 *
 * @author 	G. Freudenthaler, gefr@tf.uni-kiel.de, Chair of Automatic Control, University Kiel
 * @date 	04.05.2017
 *
 * @note API for BSW-internal use only
 *
 * @copyright @LGPL2_1
 *
 **************************************************************************************************/

#ifndef TACHO_API_H_
#define TACHO_API_H_

/*======================================= >> #INCLUDES << ========================================*/
#include "ACon_Types.h"



#ifdef MASTER_tacho_api_C_
#define EXTERNAL_
#else
#define EXTERNAL_ extern
#endif

/**
 * @addtogroup tacho
 * @{
 */
/*======================================= >> #DEFINES << =========================================*/
/**
 * String identification of the SWC @ref tacho
 */
#define TACHO_SWC_STRING ("tacho")

/**
 *  Speed sample period in ms. Make sure that speed is sampled at the given rate.
 */
#define TACHO_SAMPLE_PERIOD_MS (5)

/**
 *
 */
#define TACHO_FILTER_ID_INVALID  (0xFF)

/**
 *
 */
#define TACHO_SPEED_VALUE_INVALID (0xFFFF)



/*=================================== >> TYPE DEFINITIONS << =====================================*/



/*============================ >> GLOBAL FUNCTION DECLARATIONS << ================================*/
 /**
 * @brief Returns the previously calculated speed of the motor.
 * @param isLeft TRUE for left speed, FALSE for right speed.
 * @return Actual speed value
 */
EXTERNAL_ StdRtn_t TACHO_Read_PosLe(int32_t* pos_);

/**
 *
 * @param pos_
 * @return
 */
EXTERNAL_ StdRtn_t TACHO_Read_PosRi(int32_t* pos_);

/**
 *
 * @param spd_
 * @return
 */
EXTERNAL_ StdRtn_t TACHO_Read_RawSpdLe(int16_t* spd_);

/**
 *
 * @param spd_
 * @return
 */
EXTERNAL_ StdRtn_t TACHO_Read_RawSpdRi(int16_t* spd_);

/**
 *
 * @param spd_
 * @return
 */
EXTERNAL_ StdRtn_t TACHO_Read_SpdLe(int16_t* spd_);

/**
 *
 * @param spd_
 * @return
 */
EXTERNAL_ StdRtn_t TACHO_Read_SpdRi(int16_t* spd_);

/**
 *
 * @param type_
 * @return
 */
EXTERNAL_ StdRtn_t TACHO_Set_FltrReq(uint8_t idx_);

/**
 *
 * @return
 */
uint8_t TACHO_Get_ActFltrIdx(void);


/**
 * @}
 */
#ifdef EXTERNAL_
#undef EXTERNAL_
#endif

#endif /* !TACHO_API_H_ */
