/***********************************************************************************************//**
 * @file		tl_cfg.c
 * @ingroup		tl
 * @brief 		Filter component to estimate system state using a PI tracking loop
 *
 *This component can estimate states from a system of the form
 *	d/dt (X1) = X2
 *with given measurements of the state X1 (see diagram below). The estimate X1_hat is given by integrating
 *the estimate X2_hat. Afterward X1_hat is fed back to a PI controller that is driven by the error
 *between X1 (measurement) and X1_hat. By regulating the error to zero, X1_hat approaches X1 and thus X2_hat
 *must approach X2.
 *The open loop is approximated in the Laplace domain by
 *	l(s) = g_PI(s) * 1/s = KP/s + KI/s^2
 *and thus
 *	t_{X1, X1_hat}(s) = X1_hat(s)/X1(s) = l(s)/(1+l(s)) = (KP/KI * s + 1) / (1/KI * s^2 + KP/KI * s + 1),
 *which is a 2nd order low pass filter and thus has no offset to neither a step nor to a ramp
 *input signal.
 *
 *   X1 						   X2_hat				  X1_hat
 * ----->(+)---->[PI-Controller]----------->[Integrator]----------
 * 		  ^(-)												     '
 * 	      '			  										     '
 *		  '------------------------------------------------------'
 *
 *Note that 'KP' and 'KI' can be evaluated using e.g. MATLABs control system toolbox. KI must be adjusted
 *by multiplying the sampling time 'Ta' on to it to work in the expected (simulated) manner.
 *
 * @author 	G. Freudenthaler, gefr@tf.uni-kiel.de, Chair of Automatic Control, University Kiel
 * @author 	S. Helling, stu112498@tf.uni-kiel.de, Chair of Automatic Control, University Kiel
 * @date 	17.08.2017
 *
 * @copyright @LGPL2_1
 *
 ***************************************************************************************************/

#define MASTER_tl_cfg_C_

//TODO
// - decouple sample time from calling task period and runtime calls
// - Move strings to parent component
// - Connect PID gain setting to NVM
/*======================================= >> #INCLUDES << ========================================*/
#include "tl_cfg.h"

/** Application APIs */
#include "tacho_api.h"



/*======================================= >> #DEFINES << =========================================*/
#define TL_TACHO_LEFT_STRING		("TACHO Left")

#define TL_TACHO_RIGHT_STRING		("TACHO Right")

#define TL_DFLT_PID_SATURATION_VALUE	(0xFFFFu)

#define TL_DFLT_DATA_INIT  				{0}

#define TL_DFLT_D_GAIN					(0x0u)



/*=================================== >> TYPE DEFINITIONS << =====================================*/



/*============================= >> LOKAL FUNCTION DECLARATIONS << ================================*/



/*=================================== >> GLOBAL VARIABLES << =====================================*/
static TL_Itm_t items[] =
{
		{	{TL_TACHO_LEFT_STRING,  TACHO_SAMPLE_PERIOD_MS, {3500u, 25u, TL_DFLT_D_GAIN, 100u, TL_DFLT_PID_SATURATION_VALUE},
			TACHO_Read_PosLe},  TL_DFLT_DATA_INIT
		},
		{ 	{TL_TACHO_RIGHT_STRING, TACHO_SAMPLE_PERIOD_MS, {3500u, 25u, TL_DFLT_D_GAIN, 100u, TL_DFLT_PID_SATURATION_VALUE},
			TACHO_Read_PosRi}, TL_DFLT_DATA_INIT
		},
};

static TL_ItmTbl_t itemTable =
{
		items,
		sizeof(items)/(sizeof(items[0])),
};

/*============================== >> LOKAL FUNCTION DEFINITIONS << ================================*/



/*============================= >> GLOBAL FUNCTION DEFINITIONS << ================================*/
TL_ItmTbl_t *Get_pTlItmTbl(void) {return &itemTable;}



#ifdef MASTER_tl_cfg_C_
#undef MASTER_tl_cfg_C_
#endif /* !MASTER_tl_cfg_C_ */
