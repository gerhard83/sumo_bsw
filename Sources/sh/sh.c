/***************************************************************************************************
 * @brief 	Shell and console interface implementation.
 *
 * @author 	(c) 2014 Erich Styger, erich.styger@hslu.ch, Hochschule Luzern
 * @author 	Gerhard Freudenthaler, gefr@tf.uni-kiel.de, CAU Kiel
 * @author 	Henning Weisbarth, hewe@tf.uni-kiel.de, CAU Kiel
 * @date 	02.01.2017
 *
 * @copyright 	LGPL-2.1, https://opensource.org/licenses/LGPL-2.1
 *
 * This module implements the front to the console/shell functionality.
 *
 * =================================================================================================
 */

#define MASTER_SH_C_

/*======================================= >> #INCLUDES << ========================================*/
#include "sh.h"
#include "CLS1.h"
#include "FRTOS1.h"
#include "appl.h"
#include "Buzzer.h"
#include "Motor.h"
#include "RNET1.h"
#include "LED1.h"
#include "LED2.h"
#include "batt.h"
#include "Q4CLeft.h"
#include "Q4CRight.h"
#include "Tacho.h"
#include "KIN1.h"
#include "Pid.h"
#include "drv.h"
#include "RTT1.h"
#include "id.h"
#include "nvm.h"
#include "state.h"
#include "rnet.h"

<<<<<<< Updated upstream
/*======================================= >> #DEFINES << =========================================*/
#define SH_DASH_LINE         (CLS1_DASH_LINE)
#define SH_STAR_LINE         ("\r\n**************************************************************\r\n")
#define SH_DOUBLE_DASH_LINE  ("\r\n==============================================================\r\n")
#define SH_WELCOME           (    "           >> Welcome to the ACon Sumo Project <<             \r\n")
#define SH_SUMO_INTRO        (    "  This is Sumo robot with ID #%d.\r\n  \
				  Type \"-help\" for usage documentation\r\n")
=======
<<<<<<< Updated upstream
void SH_SendString(unsigned char *msg) {
  CLS1_SendStr(msg, SH_GetStdio()->stdOut);
  CLS1_SendStr(msg, RTT1_stdio.stdOut);
=======
/*======================================= >> #DEFINES << =========================================*/
#define SH_DASH_LINE         (CLS1_DASH_LINE)
#define SH_STAR_LINE         ("\r\n**************************************************************\r\n")
#define SH_DOUBLE_DASH_LINE  (    "==============================================================\r\n")
#define SH_WELCOME_LINE1     (    "        >> Welcome to the ACon SUMO Robots Project <<         \r\n")
#define SH_WELCOME_LINE2     (    "         -------------------------------------------          \r\n")
#define SH_SUMO_INTRO_LINE1  ("\r\n  Hello, my name is SULLEN SUMO and my ID is #%2d. I am highly\r\n")
#define SH_SUMO_INTRO_LINE2  (    "  trained with basic software version %c.\r\n")
#define SH_SHELL_INTRO_LINE1 ("\r\n  This is a command line shell for debugging the SUMO basic \r\n")
#define SH_SHELL_INTRO_LINE2 (    "  software and firmware. Type \"help\" for usage documentation.\r\n")
#define SH_COPYRIGHT_LINE1   ("\r\n  (c)2017 Gerhard Freudenthaler, gefr@tf.uni-kiel.de, CAU Kiel\r\n")
#define SH_COPYRIGHT_LINE2   (    "          Erich Styger, erich.styger@hslu.ch, HSLU Luzern\r\n")
>>>>>>> Stashed changes
>>>>>>> Stashed changes

/*=================================== >> TYPE DEFINITIONS << =====================================*/
typedef struct SH_IODesc_s{
  unsigned char *buf;
  size_t bufSize;
  CLS1_ConstStdIOType *stdio;
} SH_IODesc;


/*============================= >> LOKAL FUNCTION DECLARATIONS << ================================*/
static uint8_t SH_PrintHelp(const CLS1_StdIOType *io);
static uint8_t SH_PrintStatus(const CLS1_StdIOType *io);
static uint8_t SH_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
static void SH_PrintWelcomeMsg(const CLS1_StdIOType *io_);

/*=================================== >> GLOBAL VARIABLES << =====================================*/
static const CLS1_ParseCommandCallback CmdParserTable[] =
{
  CLS1_ParseCommand,
  SH_ParseCommand,
  FRTOS1_ParseCommand,
  APPL_ParseCommand,
  STATE_ParseCommand,
  NVM_ParseCommand,
  MOT_ParseCommand,
  DRV_ParseCommand,
  TACHO_ParseCommand,
  PID_ParseCommand,
  Q4CLeft_ParseCommand,
  Q4CRight_ParseCommand,
  BUZ_ParseCommand,
  LED1_ParseCommand,
  LED2_ParseCommand,
  RNET1_ParseCommand,
  BATT_ParseCommand,
  KIN1_ParseCommand,
  RNET_ParseCommand,
  NULL /* Sentinel */
};

static const SH_IODesc ios[] =
{
    {CLS1_DefaultShellBuffer, sizeof(CLS1_DefaultShellBuffer), &CLS1_stdio},
    {RTT1_DefaultShellBuffer, sizeof(RTT1_DefaultShellBuffer), &RTT1_stdio},
};


/*============================== >> LOKAL FUNCTION DEFINITIONS << ================================*/
static uint8_t SH_PrintHelp(const CLS1_StdIOType *io) {
  CLS1_SendHelpStr((unsigned char*)"shell", (unsigned char*)"Group of shell commands\r\n", io->stdOut);
  CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows shell help or status\r\n", io->stdOut);
  return ERR_OK;
}

static uint8_t SH_PrintStatus(const CLS1_StdIOType *io) {
  CLS1_SendStatusStr((unsigned char*)"shell", (unsigned char*)"\r\n", io->stdOut);
  CLS1_SendStatusStr((unsigned char*)"  connections", NULL, io->stdOut);
  CLS1_SendStr((unsigned char*)"DEFAULT", io->stdOut);
  CLS1_SendStr((unsigned char*)"   +RTT", io->stdOut);
  CLS1_SendStr((unsigned char*)"\r\n", io->stdOut);
  return ERR_OK;
}

static uint8_t SH_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
  uint8_t res = ERR_OK;

  if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"shell help")==0) {
      *handled = TRUE;
      return SH_PrintHelp(io);
  } else if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"shell status")==0) {
      *handled = TRUE;
      return SH_PrintStatus(io);
  }
  return res;
}

<<<<<<< Updated upstream
static void SH_PrintWelcomeMsg(const CLS1_StdIOType *io_)
{
  CLS1_SendStr((unsigned char*)SH_STAR_LINE, io_->stdOut);
  CLS1_SendStr((unsigned char*)SH_WELCOME, io_->stdOut);
  CLS1_SendStr((unsigned char*)SH_DASH_LINE, io_->stdOut);
  CLS1_SendStr((unsigned char*)"\r\n", io_->stdOut);
  CLS1_SendStr((unsigned char*)SH_SUMO_INTRO, io_->stdOut);
=======
<<<<<<< Updated upstream
=======
static void SH_PrintWelcomeMsg(const CLS1_StdIOType *io_)
{
  uint8 sumoId;

  sumoId = ID_WhichSumo();
  CLS1_SendStr((unsigned char*)SH_STAR_LINE, io_->stdOut);
  CLS1_SendStr((unsigned char*)SH_WELCOME_LINE1, io_->stdOut);
  CLS1_SendStr((unsigned char*)SH_WELCOME_LINE2, io_->stdOut);
  CLS1_printfIO(io_, (unsigned char*)SH_SUMO_INTRO_LINE1, (int8)sumoId);
  CLS1_printfIO(io_, (unsigned char*)SH_SUMO_INTRO_LINE2, SW_VERSION_STRING);
  CLS1_SendStr((unsigned char*)SH_SHELL_INTRO_LINE1, io_->stdOut);
  CLS1_SendStr((unsigned char*)SH_SHELL_INTRO_LINE2, io_->stdOut);
  CLS1_SendStr((unsigned char*)SH_COPYRIGHT_LINE1, io_->stdOut);
  CLS1_SendStr((unsigned char*)SH_COPYRIGHT_LINE2, io_->stdOut);
>>>>>>> Stashed changes
  CLS1_SendStr((unsigned char*)SH_DOUBLE_DASH_LINE, io_->stdOut);
  CLS1_SendStr((unsigned char*)"\r\n", io_->stdOut);
  CLS1_PrintPrompt(io_);

  return;
}
<<<<<<< Updated upstream
=======
>>>>>>> Stashed changes
>>>>>>> Stashed changes

/*============================= >> GLOBAL FUNCTION DEFINITIONS << ================================*/
CLS1_ConstStdIOType *SH_GetStdio(void)
{
  return CLS1_GetStdio();
}


void SH_SendStr(unsigned char *msg_)
{
  CLS1_SendStr(msg_, CLS1_GetStdio()->stdOut);
  CLS1_SendStr(msg_, RTT1_GetStdio()->stdOut);
}

void SH_SendErrStr(unsigned char *msg_)
{
  CLS1_SendStr(msg_, CLS1_GetStdio()->stdErr);
  CLS1_SendStr(msg_, RTT1_GetStdio()->stdErr);
}


void SH_Init(void)
{
<<<<<<< Updated upstream
  uint8 i = 0u;
  uint8 buf[32];
  uint8 sumoId;

=======
<<<<<<< Updated upstream
     uint8 i = 0u;
	 uint8 buf[32];
	 uint8 sumoId;

	/* initialize buffers */
	for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++) {
		ios[i].buf[0] = '\0';
	}

	SH_SendString("Shell task started!\r\n");

	/* print ID information about current sumo to the shell welcome dialog*/
	sumoId = ID_WhichSumo();
	if (sumoId == ERR_PARAM_ADDRESS)
	{
		UTIL1_strcpy(buf, sizeof(buf), "Sorry! Your sumo is unknown.");
    }
	else
	{
		UTIL1_strcpy(buf, sizeof(buf), "Welcome to Sumo #");
	  	UTIL1_strcatNum8u(buf, sizeof(buf), ID_WhichSumo());
	  	UTIL1_strcat(buf, sizeof(buf), " \r\n");
	}
	SH_SendString(buf);
=======
  uint8 i = 0u;
  uint8 buf[32];
>>>>>>> Stashed changes
  /* initialize buffers */

  for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++)
  {
      /* Eat previous lines */
      while(TRUE == CLS1_ReadLine(ios[i].buf, ios[i].buf, ios[i].bufSize, ios[i].stdio));
      ios[i].buf[0] = '\0';
      SH_PrintWelcomeMsg(ios[i].stdio);
  }
<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
>>>>>>> Stashed changes


  /* print ID information about current sumo to the shell welcome dialog*/
  sumoId = ID_WhichSumo();
  if (sumoId == ERR_PARAM_ADDRESS)
  {
      UTIL1_strcpy(buf, sizeof(buf), "Sorry! Your sumo is unknown.");
  }
  else
  {
      UTIL1_strcpy(buf, sizeof(buf), "Welcome to Sumo #");
      UTIL1_strcatNum8u(buf, sizeof(buf), ID_WhichSumo());
      UTIL1_strcat(buf, sizeof(buf), " \r\n");
  }

  //SH_SendStr(buf);

}


void SH_Deinit(void)
{
  CLS1_Deinit();
}

void SH_MainFct(void)
{
  uint8 i = 0u;
  /* process all I/Os */
  for(i=0;i<sizeof(ios)/sizeof(ios[0]);i++)
  {
      (void)CLS1_ReadAndParseWithCommandTable(ios[i].buf, ios[i].bufSize, ios[i].stdio, CmdParserTable);
  }
}

#ifdef MASTER_SH_C_
#undef MASTER_SH_C_
#endif

