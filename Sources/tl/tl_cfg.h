/***********************************************************************************************//**
 * @file		tacho_cfg.h
 * @ingroup		tacho
 * @brief 		<This is a brief description.>
 *
 * <This is a detailed description.>
 *
 * @author 	G. Freudenthaler, gefr@tf.uni-kiel.de, Chair of Automatic Control, University Kiel
 * @date 	13.06.2017
 *
 * @copyright @LGPL2_1
 *
 ***************************************************************************************************/

#ifndef TACHO_CFG_H_
#define TACHO_CFG_H_

/*======================================= >> #INCLUDES << ========================================*/
#include "pid_api.h"


#ifdef MASTER_tacho_cfg_C_
#define EXTERNAL_
#else
#define EXTERNAL_ extern
#endif

/**
 * @addtogroup tacho
 * @{
 */
/*======================================= >> #DEFINES << =========================================*/



/*=================================== >> TYPE DEFINITIONS << =====================================*/
/**
 *
 * @param
 * @return
 */
typedef StdRtn_t TL_readFct_t(int32_t*);

/**
 *
 */
typedef struct TL_Cfg_s
{
	const uchar_t   *pItmName;
	PID_Gain_t gain;
	TL_readFct_t *measValFct;
}TL_Cfg_t;

/**
 *
 */
typedef struct TL_Data_s{
	int32_t fltrdVal;
	int16_t dfltrdValdt;
	PID_Data_t pid;
} TL_Data_t;

/**
 *
 */
typedef struct TL_Itm_s{
	TL_Cfg_t cfg;
	TL_Data_t data;
}TL_Itm_t;


/**
 *
 */
typedef struct TL_ItmTbl_t
{
	TL_Itm_t *aTls;
	uint8_t numTls;
} TL_ItmTbl_t;


/*============================ >> GLOBAL FUNCTION DECLARATIONS << ================================*/
EXTERNAL_ TL_ItmTbl_t *Get_pTlItmTbl(void);



#ifdef EXTERNAL_
#undef EXTERNAL_
#endif

#endif /* !TACHO_CFG_H_ */
