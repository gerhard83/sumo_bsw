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

#define MASTER_RTE_C_
#include "LED1.h"
#include "LED2.h"
#include "KEY1.h"
#include "Buzzer.h"
#include "rte.h"
#include "tacho.h"
#include "drive.h"

#define USER_SWITCH_MASK (0x01u)

/**
 * Interface implementation for the right LED
 */
StdRtnType RTE_Write_LedRiOn()
{
	LED1_On();
	return RTN_OK;
}

StdRtnType RTE_Write_LedRiOff()
{
	LED1_Off();
	return RTN_OK;
}

StdRtnType RTE_Write_LedRiNeg()
{
	LED1_Neg();
	return RTN_OK;
}

StdRtnType RTE_Write_LedRiSt(uint8_t state)
{
	if(0u==state)
	{
		LED1_Put(FALSE);
	}
	else
	{
		LED1_Put(TRUE);
	}
	return RTN_OK;
}

StdRtnType RTE_Read_LedRiSt(uint8 *state_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL!=state_)
	{
		*state_ = (uint8)LED1_Get();
		retVal = RTN_OK;
	}
	return retVal;
}
/*========================================================*/



/**
 * Interface implementation for the right LED
 */
StdRtnType RTE_Write_LedLeOn()
{
	LED2_On();
	return RTN_OK;
}

StdRtnType RTE_Write_LedLeOff()
{
	LED2_Off();
	return RTN_OK;
}

StdRtnType RTE_Write_LedLeNeg()
{
	LED2_Neg();
	return RTN_OK;
}

StdRtnType RTE_Write_LedLeSt(uint8 state_)
{
	if(0u==state_)
	{
		LED2_Put(FALSE);
	}
	else
	{
		LED2_Put(TRUE);
	}
	return RTN_OK;
}

StdRtnType RTE_Read_LedLeSt(uint8 *state_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL!=state_)
	{
		*state_ = (uint8)LED2_Get();
		retVal = RTN_OK;
	}
	return retVal;
}
/*========================================================*/



/**
 * Interface implementation for the user switch
 */
typedef struct CbFctTab_s{
	EvntCbFct_t *cbFctOnPrsd;
	EvntCbFct_t *cbFctOnLngPrsd;
	EvntCbFct_t *cbFctOnRlsd;
	EvntCbFct_t *cbFctOnLngRlsd;
}CbFctTab_t;

static CbFctTab_t cbFctTab={NULL,NULL,NULL,NULL};

StdRtnType RTE_Read_SwtSt(uint8 *state_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL!=state_)
	{
		*state_ = (uint8)KEY1_GetKeys() & USER_SWITCH_MASK;
		retVal = RTN_OK;
	}
	return retVal;
}

StdRtnType RTE_Write_SwtOnPrsdCbFct(const EvntCbFct_t *cbFct_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL != cbFct_)
	{
		cbFctTab.cbFctOnPrsd = cbFct_;
		retVal = RTN_OK;
	}
	return retVal;
}

StdRtnType RTE_Write_SwtOnLngPrsdCbFct(const EvntCbFct_t *cbFct_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL != cbFct_)
	{
		cbFctTab.cbFctOnLngPrsd = cbFct_;
		retVal = RTN_OK;
	}
	return retVal;
}

StdRtnType RTE_Write_SwtOnRlsdCbFct(const EvntCbFct_t *cbFct_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL != cbFct_)
	{
		cbFctTab.cbFctOnRlsd = cbFct_;
		retVal = RTN_OK;
	}
	return retVal;
}

StdRtnType RTE_Write_SwtOnLngRlsdCbFct(const EvntCbFct_t *cbFct_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL != cbFct_)
	{
		cbFctTab.cbFctOnLngRlsd = cbFct_;
		retVal = RTN_OK;
	}
	return retVal;
}

EvntCbFct_t *RTE_Get_SwtOnPrsdCbFct(void)
{
	return cbFctTab.cbFctOnPrsd;
}

EvntCbFct_t *RTE_Get_SwtOnLngPrsdCbFct(void)
{
	return cbFctTab.cbFctOnLngPrsd;
}

EvntCbFct_t *RTE_Get_SwtOnRlsdCbFct(void)
{
	return cbFctTab.cbFctOnRlsd;
}

EvntCbFct_t *RTE_Get_SwtOnLngRlsdCbFct(void)
{
	return cbFctTab.cbFctOnLngRlsd;
}
/*========================================================*/

/**
 * Interface implementation for the buzzer
 */
static BUZ_Tunes Trsnlte_TuneRTE2BUZ(RTE_BuzTune_t tune_);


static BUZ_Tunes Trsnlte_TuneRTE2BUZ(RTE_BuzTune_t mode_)
{
	switch(mode_)
	{
		case RTE_BUZ_TUNE_WELCOME:     return BUZ_TUNE_WELCOME;
		case RTE_BUZ_TUNE_BUTTON:      return BUZ_TUNE_BUTTON;
		case RTE_BUZ_TUNE_BUTTON_LONG: return BUZ_TUNE_BUTTON_LONG;
		default:
		case RTE_BUZ_TUNE_NOF_TUNES:   return BUZ_TUNE_NOF_TUNES;
	}
	return BUZ_TUNE_NOF_TUNES;
}

StdRtnType RTE_Write_BuzPlayTune(RTE_BuzTune_t tune_)
{
	StdRtnType retVal = RTN_INVALID;
	if(BUZ_TUNE_NOF_TUNES > tune_)
	{
		retVal &= (StdRtnType)BUZ_PlayTune(Trsnlte_TuneRTE2BUZ(tune_));
	}
	return retVal;
}


StdRtnType RTE_Play_BuzBeep(uint16 freqHz_, uint16 durMs_)
{
	return (StdRtnType)BUZ_Beep(freqHz_, durMs_);
}
/*========================================================*/

/**
 * Interface implementation for the speedometer
 */
#define LEFT   (TRUE)
#define RIGHT  (FALSE)

StdRtnType RTE_Read_SpdoVelLe(uint16 *vel_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL != vel_)
	{
		*vel_ = TACHO_GetSpeed(TRUE);
		retVal = RTN_INVALID;
	}
	return retVal;
}


StdRtnType RTE_Read_SpdoVelRi(uint16 *vel_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL != vel_)
	{
		*vel_ = TACHO_GetSpeed(FALSE);
		retVal = RTN_INVALID;
	}
	return retVal;
}
/*========================================================*/

/**
 * Interface implementation for the drive component
 */
static DRV_Mode Trsnlte_ModeRTE2DRV(RTE_DrvMode_t mode_);
static RTE_DrvMode_t Trsnlte_ModeDRV2RTE(DRV_Mode mode_);

static DRV_Mode Trsnlte_ModeRTE2DRV(RTE_DrvMode_t mode_)
{
	switch(mode_)
	{
		default:
		case RTE_DRV_MODE_NONE:  return DRV_MODE_NONE;
		case RTE_DRV_MODE_STOP:  return DRV_MODE_STOP;
		case RTE_DRV_MODE_SPEED: return DRV_MODE_SPEED;
		case RTE_DRV_MODE_POS:   return DRV_MODE_POS;
	}
	return DRV_MODE_NONE;
}

static RTE_DrvMode_t Trsnlte_ModeDRV2RTE(DRV_Mode mode_)
{
	switch(mode_)
	{
		case DRV_MODE_NONE:  return RTE_DRV_MODE_NONE;
		case DRV_MODE_STOP:  return RTE_DRV_MODE_STOP;
		case DRV_MODE_SPEED: return RTE_DRV_MODE_SPEED;
		case DRV_MODE_POS:   return RTE_DRV_MODE_POS;
		default:             return RTE_DRV_MODE_INVALID;
	}
	return RTE_DRV_MODE_INVALID;
}

StdRtnType RTE_Write_DrvVel(int32 velLe_, int32 velRi_)
{
	return (StdRtnType)DRV_SetSpeed(velLe_, velRi_);
}

StdRtnType RTE_Write_DrvPos(int32 posLe_, int32 posRi_)
{
	return (StdRtnType)DRV_SetSpeed(posLe_, posRi_);
}

StdRtnType RTE_Write_DrvMode(RTE_DrvMode_t mode_)
{
	StdRtnType retVal = RTN_INVALID;
	if((RTE_DRV_MODE_INVALID > mode_) && (RTE_DRV_MODE_NONE <= mode_))
	{
		retVal &= (StdRtnType)DRV_SetMode(Trsnlte_ModeRTE2DRV(mode_));
	}
	return retVal;
}

StdRtnType RTE_Read_DrvMode(RTE_DrvMode_t *mode_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL != mode_)
	{
		*mode_ = Trsnlte_ModeDRV2RTE(DRV_GetMode());
		retVal = RTN_OK;
	}
	return retVal;
}

StdRtnType RTE_Read_DrvIsDrvgBkwd(uint8 *isDrvgBkwd_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL != isDrvgBkwd_)
	{
		*isDrvgBkwd_ = DRV_IsDrivingBackward();
		retVal = RTN_OK;
	}
	return retVal;
}

StdRtnType RTE_Read_DrvHasStpd(uint8 *hasStpd_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL != hasStpd_)
	{
		*hasStpd_ =  DRV_IsStopped();
		retVal = RTN_OK;
	}
	return retVal;
}

StdRtnType RTE_Read_DrvHasRvsd(uint8 *hasRvsd_)
{
	StdRtnType retVal = RTN_INVALID;
	if(NULL != hasRvsd_)
	{
		*hasRvsd_ =  DRV_HasTurned();
		retVal = RTN_OK;
	}
	return retVal;
}
/*========================================================*/

#ifdef MASTER_RTE_C_
#undef MASTER_RTE_C_
#endif
