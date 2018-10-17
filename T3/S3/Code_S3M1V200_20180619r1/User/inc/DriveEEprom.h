#ifndef _DRIVEEEPROM_H__
#define _DRIVEEEPROM_H__
#include "stm8l15x.h"

#define EEPROM_BASE             0x1000





void eep_init(void);
u8 eep_read8(u8 reg);
u16 eep_read16(u8 reg);
u32 eep_read32(u8 reg);
void eep_write8(u8 reg, u8 val);
void eep_write16(u8 reg, s16 val);
void eep_write32(u8 reg, s32 val);

#endif