/***********************************************************************************************//**
 * @file		nvm_clshdlr.c
 * @ingroup		nvm
 * @brief 		Implementation of the command line shell handler for the SWC @a NVM
 *
 * This module implements the interface of the SWC @ref nvm which is addressed to
 * the SWC @ref sh. It introduces application specific commands for requests
 * of status information or restoring the NVM from ROM via command line shell (@b CLS).
 *
 * @author 	(c) 2014 Erich Styger, erich.styger@hslu.ch, Hochschule Luzern
 * @author 	G. Freudenthaler, gefr@tf.uni-kiel.de, Chair of Automatic Control, University Kiel
 * @date 	23.02.2017
 *  
 * @copyright @LGPL2_1
 *
 ***************************************************************************************************/

#define MASTER_nvm_clshdlr_C_

/*======================================= >> #INCLUDES << ========================================*/
#include "nvm_clshdlr.h"
#include "nvm_api.h"



/*======================================= >> #DEFINES << =========================================*/



/*=================================== >> TYPE DEFINITIONS << =====================================*/



/*============================= >> LOKAL FUNCTION DECLARATIONS << ================================*/
static uint8_t NVM_PrintHelp(const CLS1_StdIOType *io);
static uint8_t NVM_PrintStatus(const CLS1_StdIOType *io);
static uint8_t NVM_PrintVersion(const CLS1_StdIOType *io_);
static void NVM_PrintRestoreStatus(const CLS1_StdIOType *io_);



/*=================================== >> GLOBAL VARIABLES << =====================================*/



/*============================== >> LOKAL FUNCTION DEFINITIONS << ================================*/
static uint8_t NVM_PrintHelp(const CLS1_StdIOType *io) {
	CLS1_SendHelpStr((unsigned char*)"nvm", (unsigned char*)"Group of NVM commands\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Shows NVM help or status\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)"  version", (unsigned char*)"Shows NVM version\r\n", io->stdOut);
	CLS1_SendHelpStr((unsigned char*)"  restore", (unsigned char*)"restores all NVM parameters from ROM and saves to NVM\r\n", io->stdOut);
	return ERR_OK;
}

static uint8_t NVM_PrintStatus(const CLS1_StdIOType *io) {
	CLS1_SendStatusStr((unsigned char*)"nvm", (unsigned char*)"\r\n", io->stdOut);
	CLS1_SendStatusStr((unsigned char*)"  status", "no status information available\r\n", io->stdOut);
	return ERR_OK;
}


static uint8_t NVM_PrintVersion(const CLS1_StdIOType *io_)
{
	uint8_t strBuf[sizeof("0xFF")]={"0x\0"};
	uint8_t ver = 0u;
	if( ERR_OK == NVM_Read_NvmVerFromNVM(&ver) )
	{
		UTIL1_strcatNum8Hex(strBuf+2, sizeof("FF"), ver);
		CLS1_SendStatusStr((unsigned char*)"  version", "", io_->stdOut);
		CLS1_SendStr(strBuf,io_->stdOut);
		CLS1_SendStr("\r\n", io_->stdOut);
	}
	else
	{
		CLS1_SendStatusStr((unsigned char*)"  version", "** ERROR: parameter address invalid **\r\n", io_->stdOut);
	}

	return ERR_OK;
}


static void NVM_PrintRestoreStatus(const CLS1_StdIOType *io_)
{
	uint8_t strBuf[sizeof("0xFF")]={"0x\0"};
	StdRtn_t res = NVM_Restore_AllFromROM();

	if(ERR_OK == res)
	{
		CLS1_SendStr("Done... NVM restoration successful\r\n\n",io_->stdOut);
	}
	else
	{
		UTIL1_strcatNum8Hex(strBuf+2, sizeof("FF"), res);
		CLS1_SendStr("** ERROR ",io_->stdOut);
		CLS1_SendStr(strBuf,io_->stdOut);
		CLS1_SendStr(": NVM restoration failed **\r\n", io_->stdOut);
	}

	return;
}

/*============================= >> GLOBAL FUNCTION DEFINITIONS << ================================*/
uint8_t NVM_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
 	uint8_t res = ERR_OK;
 	if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, (char*)"nvm help")==0) {
 		*handled = TRUE;
 		return NVM_PrintHelp(io);
 	} else if (UTIL1_strcmp((char*)cmd, (char*)CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, (char*)"nvm status")==0) {
 		*handled = TRUE;
 		return NVM_PrintStatus(io);
 	} else if (UTIL1_strcmp((char*)cmd, (char*)"nvm version")==0) {
 		*handled = TRUE;
 		return NVM_PrintVersion(io);
 	} else if (UTIL1_strcmp((char*)cmd, (char*)"nvm restore")==0) {
		*handled = TRUE;
		NVM_PrintRestoreStatus(io);
		return ERR_OK;
 	}
 	return res;
 }



#ifdef MASTER_nvm_clshdlr_C_
#undef MASTER_nvm_clshdlr_C_
#endif /* !MASTER_nvm_clshdlr_C_ */
