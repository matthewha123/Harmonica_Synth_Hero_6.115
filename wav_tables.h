/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef __WAV_TABLES_H_
#define __WAV_TABLES_H_
    
#include "cytypes.h"
  // There are 

typedef struct {
    uint8* table;
    uint16 index;
    uint16 period;
} WAV_TABLE;

extern WAV_TABLE c4_sus;
extern WAV_TABLE d4_sus;
extern WAV_TABLE e4_sus;
extern WAV_TABLE g4_sus;
extern WAV_TABLE g5_sus;
extern WAV_TABLE b4_sus;
extern WAV_TABLE b5_sus;
extern WAV_TABLE c5_sus;
extern WAV_TABLE d5_sus;
extern WAV_TABLE e5_sus;
extern WAV_TABLE f5_sus;
extern WAV_TABLE g6_sus;
extern WAV_TABLE a5_sus;
extern WAV_TABLE c6_sus;
extern WAV_TABLE b6_sus;
extern WAV_TABLE e6_sus;
extern WAV_TABLE d6_sus;
extern WAV_TABLE f6_sus;
extern WAV_TABLE c7_sus;
extern WAV_TABLE a6_sus;

uint8 get_wav_val_and_inc(WAV_TABLE* wav_table);
void reset_wav_tables_counter(void);
#endif // __WAV_TABLES_H_

/* [] END OF FILE */
