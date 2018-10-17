#include "DriveEEprom.h"




/*******************************************************
* Summary:    eeprom��ʼ��
* Parameters:
*             void
*       
* Return :    void
*******************************************************/
void eep_init(void)
{
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    //FLASH_Unlock(FLASH_MemType_Data); 
}


/*******************************************************/
//
//Summary:	 eeprom��һ���ֽ�
//Parameter:     void
//Return:        �ֽڱ����ֵ
//
/*******************************************************/
u8 eep_read8(u8 reg)
{
    u8 rt = 0;

    rt = FLASH_ReadByte(EEPROM_BASE + reg);

    return rt;
}

/*******************************************************/
//
//Summary:	 eeprom�������ֽ�
//Parameter:     void
//Return:        �ֽڱ����ֵ
//
/*******************************************************/
u16 eep_read16(u8 reg)
{
    u16 rt = 0;

    rt = eep_read8(reg);
    rt <<= 8;
    rt += eep_read8(reg + 1);

    return rt;
}

/*******************************************************/
//
//Summary:	 eeprom���ĸ��ֽ�
//Parameter:     void
//Return:        �ֽڱ����ֵ
//
/*******************************************************/
u32 eep_read32(u8 reg)
{
    u32 rt = 0;

    rt = eep_read16(reg);
    rt <<= 16;
    rt += eep_read16(reg + 2);

    return rt;
}


/*******************************************************/
//
//Summary:	 дһ���ֽڵ�eeprom
//Parameter:     reg ��ַ  val ֵ
//Return:        �ֽڱ����ֵ
//
/*******************************************************/
void eep_write8(u8 reg, u8 val)
{
    while(FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
    {
        FLASH_Unlock(FLASH_MemType_Data); 
    }

    
    FLASH_ProgramByte(EEPROM_BASE + reg, val);
    while(FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET);

    while(FLASH_GetFlagStatus(FLASH_FLAG_DUL) == SET)
    {
        FLASH_Lock(FLASH_MemType_Data); 
    }
}


void eep_write16(u8 reg, s16 val)
{
    eep_write8(reg, (u8)(val >> 8));
    eep_write8(reg + 1, (u8)(val & 0xff));
}


void eep_write32(u8 reg, s32 val)
{
    eep_write16(reg, (s16)(val >> 16));
    eep_write16(reg + 2, (s16)(val & 0xffff));
}