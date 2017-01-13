/*******************************************************************************
 * @brief 	Main Application Interface.
 *
 * @author 	Gerhard Freudenthaler, gefr@tf.uni-kiel.de, CAU Kiel
 * @author 	Henning Weisbarth, hewe@tf.uni-kiel.de, CAU Kiel
 * @date		02.01.2017
 *
 * @copyright 	LGPL-2.1, https://opensource.org/licenses/LGPL-2.1
 *
 * Interface to the main application module. From here the application
 * runs and performs all tasks.
 *
 * ==============================================================================
 */

#define MASTER_APPL_C_

#include "Platform.h"
#include "appl.h"
#include "appl_Types.h"
#include "appl_cfg.h"
#include "FRTOS1.h"
#include "WAIT1.h"
#include "Shell.h"
#include "LED1.h"
#include "LED2.h"
#include "Buzzer.h"
#include "KEY1.h"
#include "Motor.h"
#include "RNET1.h"
#include "batt.h"
#include "Tacho.h"
#include "Q4CLeft.h"
#include "Q4CRight.h"
#include "Pid.h"
#include "Drive.h"
#include "nvm_cfg.h"

#define NVMC_VERSION  0x03

typedef enum AppStateType_s{
	APP_STATE_STARTUP,
	APP_STATE_INIT,
	APP_STATE_IDLE
} AppStateType;

static void APPL_taskCreate();
static uint8_t APPL_PrintHelp(const CLS1_StdIOType *io);
static uint8_t APPL_PrintStatus(const CLS1_StdIOType *io);
static unsigned char *AppStateString(AppStateType state);

static AppStateType appState = APP_STATE_STARTUP;
static const NVMC_RobotData *RoboDataPtr;

static unsigned char *AppStateString(AppStateType state) {
	switch(state) {
	case APP_STATE_STARTUP: return (unsigned char*)"STARTUP\r\n";
	case APP_STATE_INIT: return (unsigned char*)"INIT\r\n";
	case APP_STATE_IDLE: return (unsigned char*)"IDLE\r\n";
	default:
		break;
	}
	return (unsigned char*)"unknown?\r\n";
}

static uint8_t APPL_PrintHelp(const CLS1_StdIOType *io) {
	CLS1_SendHelpStr((unsigned char*)"appl", (unsigned char*)"Group of app commands\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows appl help or status\r\n", io->stdOut);
	return ERR_OK;
}

static uint8_t APPL_PrintStatus(const CLS1_StdIOType *io) {
	RoboDataPtr = NVMC_GetRobotData();
	uint8_t buf[24];

	CLS1_SendStatusStr((unsigned char*)"appl", (unsigned char*)"\r\n", io->stdOut);
	CLS1_SendStatusStr((unsigned char*)"  Appl State", AppStateString(appState), io->stdOut);

	UTIL1_Num8uToStr(buf, sizeof(buf), RoboDataPtr->version);
	UTIL1_strcat(buf, sizeof(buf), (unsigned char*)" \r\n");
	CLS1_SendStatusStr((unsigned char*)"  ROBO NVM", buf, io->stdOut);

	return ERR_OK;
}


static void InitNVMCValues(void) {
	const NVMC_RobotData *ptr;
	NVMC_RobotData data;
	uint8_t res;


	ptr = NVMC_GetRobotData();
	if (ptr==NULL || ptr->version != NVMC_VERSION) {
		data.version = NVMC_VERSION;
		res = NVMC_SaveRobotData(&data);
		if (res!=ERR_OK) {
			for(;;); /* error */
		}
	}
}

static void APPL_AdoptToHardware(void) {
	/*Motor direction & Quadrature configuration for CAU_ZUMO */
	(void)Q4CRight_SwapPins(TRUE);
  MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_LEFT), TRUE); /* invert left motor */
  MOT_Invert(MOT_GetMotorHandle(MOT_MOTOR_RIGHT), TRUE); /* invert right motor */

	/* SW1: enable and turn on pull-up resistor for PTA14 (push button) */
	PORT_PDD_SetPinPullSelect(PORTA_BASE_PTR, 14, PORT_PDD_PULL_UP);
	PORT_PDD_SetPinPullEnable(PORTA_BASE_PTR, 14, PORT_PDD_PULL_ENABLE);

	/* pull-ups for Quadrature Encoder Pins */
	PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 10, PORT_PDD_PULL_UP);
	PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 10, PORT_PDD_PULL_ENABLE);
	PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 11, PORT_PDD_PULL_UP);
	PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 11, PORT_PDD_PULL_ENABLE);
	PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 16, PORT_PDD_PULL_UP);
	PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 16, PORT_PDD_PULL_ENABLE);
	PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 17, PORT_PDD_PULL_UP);
	PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 17, PORT_PDD_PULL_ENABLE);
}

static void APPL_taskCreate()
{
	uint8 i = 0u;
	const APPL_TaskCfg_t *taskCfg = NULL;

	taskCfg = Get_APPL_TaskCfg();
	if(NULL != taskCfg)
	{
		for(i = 0u; i < taskCfg->numTasks; i++)
		{
			if(NULL != taskCfg->tasks)
			{
				if(pdPASS != FRTOS1_xTaskCreate(taskCfg->tasks[i].taskFct,
												taskCfg->tasks[i].taskName,
												taskCfg->tasks[i].stackDepth,
												taskCfg->tasks[i].pvParameters,
												taskCfg->tasks[i].taskPriority,
												taskCfg->tasks[i].taskHdl))
				{
					/* The task could not be created because there was not enough
					FreeRTOS heap memory available for the task data structures and
					stack to be allocated. */
				}
			}
		}
		FRTOS1_vTaskStartScheduler();
	}
	else
	{
		for(;;){};
	}
}


static void APPL_Init(void){
	APPL_taskCreate();
}


void APPL_DebugPrint(unsigned char *str) {
	SHELL_SendString(str);
}

uint8_t APPL_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
	uint8_t res = ERR_OK;
	if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"app help")==0) {
		*handled = TRUE;
		return APPL_PrintHelp(io);
	} else if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"app status")==0) {
		*handled = TRUE;
		return APPL_PrintStatus(io);
	}
	return res;
}

void APPL_Run(void) {
	SHELL_Init();
	BUZ_Init();
	MOT_Init();
	RNET1_Init();
	BATT_Init();
	TACHO_Init();
	PID_Init();
	DRV_Init(); /* Comment DRV_Init() to manual MOTOR duty commands possible  */

	APPL_AdoptToHardware();
	InitNVMCValues();

	APPL_Init();
}


/* Define a task that performs an action every x milliseconds. */
void APPL_cycTaskFct( void * pvParameters_)
{
	uint8 i = 0u;
	APPL_cycTaskFctPar_t *pvPar = NULL;
	TickType_t LastWakeTime;

	pvPar = (APPL_cycTaskFctPar_t *)pvParameters_;

	/* The xLastWakeTime variable needs to be initialized with the current tick
	count.  Note that this is the only time the variable is written to explicitly.
	After this assignment, xLastWakeTime is update d automatically internally within
	vTaskDelayUntil(). */
	LastWakeTime = FRTOS1_xTaskGetTickCount();
	/* Enter the loop that defines the task behavior. */
	FRTOS1_vTaskDelay( pdMS_TO_TICKS( 100u ));
	for( ;; )
	{
		/* This task should execute every 50 milliseconds.  Time is measured
		in ticks. The pdMS_TO_TICKS macro is used to convert milliseconds
		into ticks. xLastWakeTime is automatically updated within vTaskDelayUntil()
		so is not explicitly updated by the task. */
		FRTOS1_vTaskDelayUntil( &LastWakeTime, pdMS_TO_TICKS( pvPar->taskPeriod ) );

		/* Perform the periodic actions here. */
		if(NULL != pvPar->mainFcts)
		{
			for(i = 0u; i < pvPar->numMainFcts; i++)
			{
				if(NULL != &(pvPar->mainFcts[i]))
				{
					pvPar->mainFcts[i]();
				}
			}
		}
	}
}

void APPL_nonCycTaskFct(void *pvParameters_)
{
	uint8 i = 0u;
	APPL_nonCycTaskFctPar_t *pvPar = NULL;

	pvPar = (APPL_nonCycTaskFctPar_t *)pvParameters_;

	FRTOS1_vTaskDelay( pdMS_TO_TICKS( 100u ));
	for(;;) {

		FRTOS1_vTaskDelay( pdMS_TO_TICKS( pvPar->taskDelay) );

		/* Perform the periodic actions here. */
		if(NULL != pvPar->mainFcts)
		{
			for(i = 0u; i < pvPar->numMainFcts; i++)
			{
				if(NULL != &(pvPar->mainFcts[i]))
				{
					pvPar->mainFcts[i]();
				}
			}
		}
	}
}


#ifdef MASTER_APPL_C_
#undef MASTER_APPL_C_
#endif
