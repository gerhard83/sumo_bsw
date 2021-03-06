/***********************************************************************************************//**
 * @file		ind.h
 * @ingroup		ind
 * @brief 		Interface of the SWC @a Indication for initialisation call
 *
 * This header file provides the internal interface between the SWC @ref ind and the
 * SWC @ref appl which runs the initialisation within its STARTUP state.
 *
 * @author 	G. Freudenthaler, gefr@tf.uni-kiel.de, Chair of Automatic Control, University Kiel
 * @date 	27.03.2017
 *  
 * @note Interface for BSW-specific use only
 *
 * @copyright @LGPL2_1
 *
 ***************************************************************************************************/

#ifndef IND_H_
#define IND_H_

/*======================================= >> #INCLUDES << ========================================*/



#ifdef MASTER_ind_C_
#define EXTERNAL_
#else
#define EXTERNAL_ extern
#endif

/**
 * @addtogroup ind
 * @{
 */
/*======================================= >> #DEFINES << =========================================*/



/*=================================== >> TYPE DEFINITIONS << =====================================*/



/*============================ >> GLOBAL FUNCTION DECLARATIONS << ================================*/
/**
 * This function initialises the indication software component
 */
EXTERNAL_ void IND_Init(void);



/**
 * @}
 */
#ifdef EXTERNAL_
#undef EXTERNAL_
#endif

#endif /* !IND_H_ */
