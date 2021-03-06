/***********************************************************************************************//**
 * @file		sh_cfg.c
 * @ingroup		sh
 * @brief 		Implementation of the configuration of the SWC @a Shell
 *
 * This file implements the configuration of callback functions, which parse CLS commands, for the
 * component SWC @ref sh and its internal interface.
 *
 * @author 	(c) 2014 Erich Styger, erich.styger@hslu.ch, Hochschule Luzern
 * @author 	G. Freudenthaler, gefr@tf.uni-kiel.de, Chair of Automatic Control, University Kiel
 * @date 	03.02.2017
 *  
 * @copyright @LGPL2_1
 *
 **************************************************************************************************/

#define MASTER_sh_cfg_C_

/*======================================= >> #INCLUDES << ========================================*/
#include "sh_cfg.h"
/* BSW components */
#include "task_clshdlr.h"
#include "appl_clshdlr.h"
#include "sh_clshdlr.h"
#include "rnet_clshdlr.h"
#include "nvm_clshdlr.h"
#include "pid_clshdlr.h"
#include "tl_clshdlr.h"
#include "mot_clshdlr.h"
#include "tacho_clshdlr.h"
#include "drv_clshdlr.h"
#include "batt_clshdlr.h"
#include "buz_clshdlr.h"
#include "refl_clshdlr.h"
/* Firmware components */
#include "RTT1.h"
#include "RNET1.h"
#include "LED1.h"
#include "LED2.h"
#include "Q4CLeft.h"
#include "Q4CRight.h"
#include "KIN1.h"
#include "FRTOS1.h"
/* include Bluetooth PEx component if FreeMaster is not present */
#include "freemaster_cfg.h"
#if !FMSTR_USE_SCI
#include "BT1.h"
#endif


/*======================================= >> #DEFINES << =========================================*/
#define NUM_OF_IOS 	(sizeof(ios)/sizeof(ios[0]))

/*=================================== >> TYPE DEFINITIONS << =====================================*/



/*============================= >> LOKAL FUNCTION DECLARATIONS << ================================*/


/*=================================== >> GLOBAL VARIABLES << =====================================*/
static const CLS1_ParseCommandCallback CmdParserTable[] =
{
  CLS1_ParseCommand,
  SH_ParseCommand,
  FRTOS1_ParseCommand,
  TASK_ParseCommand,
  APPL_ParseCommand,
  NVM_ParseCommand,
  MOT_ParseCommand,
  DRV_ParseCommand,
  TACHO_ParseCommand,
  PID_ParseCommand,
  TL_ParseCommand,
  Q4CLeft_ParseCommand,
  Q4CRight_ParseCommand,
  BUZ_ParseCommand,
  LED1_ParseCommand,
  LED2_ParseCommand,
  RNET1_ParseCommand,
  BATT_ParseCommand,
  KIN1_ParseCommand,
  RNET_ParseCommand,
#if !FMSTR_USE_SCI
  BT1_ParseCommand,
#endif
  REFL_ParseCommand,
  NULL /* Sentinel */
};

static const SH_IODesc_t ios[] =
{
    {CLS1_DefaultShellBuffer, sizeof(CLS1_DefaultShellBuffer), &CLS1_stdio},
    {RTT1_DefaultShellBuffer, sizeof(RTT1_DefaultShellBuffer), &RTT1_stdio},
};

static const SH_IOCfg_t ioCfg =
{
		ios,
		NUM_OF_IOS,
};

/*============================== >> LOKAL FUNCTION DEFINITIONS << ================================*/
const CLS1_ParseCommandCallback *Get_CmdParserTbl() { return CmdParserTable; }

const SH_IOCfg_t *Get_ShIOCfg() { return &ioCfg; }

/*============================= >> GLOBAL FUNCTION DEFINITIONS << ================================*/



#ifdef MASTER_sh_cfg_C_
#undef MASTER_sh_cfg_C_

#endif /* !MASTER_sh_cfg_C_ */
