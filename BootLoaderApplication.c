/*******************************************************************/
/* Author  : Nourhan Mansour - Eman Ashraf                         */
/* version : v01                                                   */
/* Date    : 31\12\2020                                            */ 
/*******************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "FPEC_interface.h"
#include "CRC_interface.h"
#include "UART_interface.h"
#include "IWDG_interface.h"

#include "BootLoaderApplication.h"

void BL_voidProcessPayLoadMain(){}

u8 BL_u8CheckBranchingCondition(){
    u16 BC_memo;
    u8 BC_return;  
    BC_memo = FPEC_u16ReadHalfWord(CONDITION_PAGE , 0);
    switch (BC_memo)
    {
    case 0x1111:
      BC_return = 'A';
      break;
    case 0x2222:
      BC_return = 'B';
      break;
    default: 
      /* Error Shouldn't be here */ 
      BC_return = 'F';  // MEMO NOT WRITTEN 
      break;
    }
    return BC_return;
}

void BL_voidWriteBranchingCondition(u16 cpyBC)
{
    // 'A' : App
    // 'B' : BootLoader
    u16 CRCValLOW  = FPEC_u16ReadHalfWord(CONDITION_PAGE , CRC_OFFSET);
    u16 CRCValHigh = FPEC_u16ReadHalfWord(CONDITION_PAGE , CRC_OFFSET+0x02); 
    u16 DataLen = FPEC_u16ReadHalfWord(CONDITION_PAGE , DATA_LEN_OFFSET);
		volatile u16 Data;
    FPEC_voidFlashPageErase(CONDITION_PAGE);

    switch (cpyBC)
    {
    case 'A':
				Data = 0x1111;
        break;
    case 'B':
        Data = 0x2222;
        break;
    default:
        Data = 0xFFFF; 
        break;
    }
		
		FPEC_voidFlashWrite(&Data       , CONDITION_PAGE , 1 , BC_OFFSET );
    FPEC_voidFlashWrite(&DataLen    , CONDITION_PAGE , 1 , DATA_LEN_OFFSET );
    FPEC_voidFlashWrite(&CRCValLOW  , CONDITION_PAGE , 1 , CRC_OFFSET );
    FPEC_voidFlashWrite(&CRCValHigh , CONDITION_PAGE , 1 , CRC_OFFSET+0x02);
}

void BL_voidSoftReset()
{
  IWDG_voidReset();
}
/***
 * TO DO
 ***/
void BL_voidValidateCRCFromFlash(u16 cpyDataLength , u32 cpyCRCValue){}


/********** Private functions **************/

u16 BL_u16ReciveDataLength(){}
void BL_voidRecievePageOfData(u8 * cpyDataStream){}  // 1024 bytes 
u32 BL_u32ReciveCRC(){}
