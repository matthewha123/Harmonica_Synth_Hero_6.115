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
#ifndef __PHOTO_MUX_H_
#define __PHOTO_MUX_H_

#include "ADC_DelSig.h"
#include "MUX_A.h"
#include "MUX_B.h"
#include "MUX_C.h"
#include "MUX_D.h"
#include "MUX_INHIBIT.h"
    
#define MAX_CHANNELS 10
    
int16 photo_read_mux(int8 channel);
void photo_enable_mux(int8 channel);
void inhibit_output(int8 inhibit);
int16 read_mux(void);

#endif // __PHOTO_MUX_H_

/* [] END OF FILE */
