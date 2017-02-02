/*******************************************************************************
 * @brief 	Main state machine of application software layer.
 *
 * @author 	Gerhard Freudenthaler, gefr@tf.uni-kiel.de, CAU Kiel
 * @date 	13.01.2017
 *
 * @copyright 	LGPL-2.1, https://opensource.org/licenses/LGPL-2.1
 *
 * This module implements an (extended) state machine for the Sumo robot. It
 * handles the main states of the application software layer.
 *
 * ==============================================================================
 */

#define MASTER_STATE_C_

#include "Platform.h"
#include "CLS1.h"
#include "RNET1.h"
#include "state.h"
#include "stud.h"
#include "appl_cfg.h"
#include "buzzer.h"
#include "LED1.h"


static uint8_t STATE_PrintHelp(const CLS1_StdIOType *io_);
static StdRtn_t STATE_PrintStatus(const CLS1_StdIOType *io_);
static const uint8 * STATE_ReadStateString(mainState_t state_);
static void STATE_RunStateMachine(void);
static StdRtn_t STATE_msSTARTUP(void);
static StdRtn_t STATE_msINIT(void);
static StdRtn_t STATE_msIDLE(void);
static StdRtn_t STATE_msNORMAL(void);
static StdRtn_t STATE_msDEBUG(void);
static void STATE_msERROR(void);

static mainState_t mainState = MAIN_STATE_STARTUP;

const APPL_TaskCfgItm_t *shTaskCfg = NULL;


static StdRtn_t STATE_SyncStateMachineWithISR()
{
	BaseType_t notfRes = pdFAIL;
	uint32 notfVal = 0u;

	notfRes = FRTOS1_xTaskNotifyWait( pdFALSE,
			                          UINT32_MAX,
						              (uint32_t *)&notfVal,
						              pdMS_TO_TICKS( 0u ) );
	if(( pdPASS == notfRes ) && ( MAIN_STATE_ERROR != mainState ))
	{
		if( (notfVal & KEY_PRESSED_NOTIFICATION_VALUE) != FALSE)
		{
			if( MAIN_STATE_IDLE == mainState )
			{
				mainState = MAIN_STATE_NORMAL;
				BUZ_PlayTune(BUZ_TUNE_BUTTON);
			}
		}

		if( (notfVal & KEY_PRESSED_LONG_NOTIFICATION_VALUE) != FALSE)
		{
			if( MAIN_STATE_NORMAL == mainState )
			{
				mainState = MAIN_STATE_DEBUG;
				FRTOS1_vTaskResume(shTaskCfg->taskHdl);
				BUZ_PlayTune(BUZ_TUNE_ACCEPT);
			}
			else if( MAIN_STATE_DEBUG == mainState )
			{
				mainState = MAIN_STATE_NORMAL;
				FRTOS1_vTaskSuspend(shTaskCfg->taskHdl);
				BUZ_PlayTune(BUZ_TUNE_DECLINE);
			}
			else
			{
				/* nothing to do */
			}
		}
	}
}

static void STATE_RunStateMachine(void) {
	static StdRtn_t retVal = ERR_OK;
	switch (mainState)
	{
		case MAIN_STATE_STARTUP:
		{
			retVal = STATE_msSTARTUP();
			if(ERR_OK == retVal)
			{
				mainState = MAIN_STATE_INIT;
			}
			else
			{
				mainState = MAIN_STATE_ERROR;
			}
			break;
		}
		case MAIN_STATE_INIT:
		{
			retVal = STATE_msINIT();
			if(ERR_OK == retVal)
			{
				mainState = MAIN_STATE_IDLE;
			}
			else
			{
				mainState = MAIN_STATE_ERROR;
			}
			break;
		}
		case MAIN_STATE_IDLE:
		{
			retVal = STATE_msIDLE();
			if(ERR_OK != retVal)
			{
				mainState = MAIN_STATE_ERROR;
			}
			break;
		}
		case MAIN_STATE_NORMAL:
		{
			retVal = STATE_msNORMAL();
			if(ERR_OK != retVal)
			{
				mainState = MAIN_STATE_ERROR;
			}
			break;
		}
		case MAIN_STATE_DEBUG:
		{
			retVal = STATE_msDEBUG();
			if(ERR_OK != retVal)
			{
				mainState = MAIN_STATE_ERROR;
			}
			break;
		}
		default:
		case MAIN_STATE_ERROR:
		{
			STATE_msERROR();
				break;
		}
	}/* switch */

	STATE_SyncStateMachineWithISR();

	return;
}

static StdRtn_t STATE_msSTARTUP(void)
{
	return ERR_OK;
}

static StdRtn_t STATE_msINIT(void)
{
	StdRtn_t retVal = ERR_PARAM_ADDRESS;

	STUD_Init();

	shTaskCfg = Get_APPL_ShTaskCfg();
	if(NULL != shTaskCfg)
	{
		retVal = ERR_OK;
	}
	return retVal;
}

static StdRtn_t STATE_msIDLE(void)
{
	LED1_On();
	return ERR_OK;
}

static StdRtn_t STATE_msNORMAL(void)
{
	STUD_Main();
	LED1_Off();
	return ERR_OK;
}

static StdRtn_t STATE_msDEBUG(void)
{
	return ERR_OK;
}
static void STATE_msERROR(void)
{
	return;
}



static uint8_t STATE_PrintHelp(const CLS1_StdIOType *io_) {
	CLS1_SendHelpStr((unsigned char*)"state", (unsigned char*)"Group of state commands\r\n", io_->stdOut);
	CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows state help or status\r\n", io_->stdOut);
	return ERR_OK;
}

static StdRtn_t STATE_PrintStatus(const CLS1_StdIOType *io_) {
	uint8 retVal = RTN_OK;
	CLS1_SendStatusStr((unsigned char*)"state", (unsigned char*)"\r\n", io_->stdOut);
	CLS1_SendStatusStr((unsigned char*)"  current", STATE_ReadStateString(mainState) , io_->stdOut);
	return retVal;
}

static const uint8 * STATE_ReadStateString(mainState_t state_){
	switch(state_) {
		case MAIN_STATE_STARTUP: return "STARTUP\r\n";
		case MAIN_STATE_INIT:    return "INIT\r\n";
		case MAIN_STATE_IDLE:    return "IDLE\r\n";
		case MAIN_STATE_NORMAL:  return "NORMAL\r\n";
		case MAIN_STATE_DEBUG:  return "DEBUG\r\n";
		case MAIN_STATE_ERROR:   return "ERROR\r\n";
		default: return ">> fatal error - unknown or undefined main state <<\r\n";
	}
}

void STATE_mainFct(void){
	STATE_RunStateMachine();
}


uint8 STATE_ParseCommand(const unsigned char *cmd_, bool *handled, const CLS1_StdIOType *io_) {
	uint8 res = ERR_OK;
	if (UTIL1_strcmp((char*)cmd_, (char*)CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd_, (char*)"state help")==0) {
		*handled = TRUE;
		return STATE_PrintHelp(io_);
	} else if (UTIL1_strcmp((char*)cmd_, (char*)CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd_, (char*)"state status")==0) {
		*handled = TRUE;
		return STATE_PrintStatus(io_);
	}
	return res;
}


#ifdef MASTER_STATE_C_
#undef MASTER_STATE_C_
#endif
