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
#include "photo_mux.h"

int16 photo_read_mux(int8 channel) {
    photo_enable_mux(channel);
    inhibit_output(0);
    CyDelayCycles(10);
    int16 result = read_mux();
    inhibit_output(1);
    return result;
}

void photo_enable_mux(int8 channel) {    
    int8 channel_masked = channel & (0x0F);
    MUX_A_Write((channel_masked >> 0)&0x01);
    MUX_B_Write((channel_masked >> 1) & 0x01);
    MUX_C_Write((channel_masked >> 2) & 0x01);
    MUX_D_Write((channel_masked >> 3) & 0x01);
}

void inhibit_output(int8 inhibit) {
    if(inhibit == 0) {
        MUX_INHIBIT_Write(0);
    } else {
        MUX_INHIBIT_Write(1);
    }
}

int16 read_mux(void)
{
    int16 result;

    ADC_DelSig_DEC_SR_REG |= ADC_DelSig_DEC_INTR_CLEAR;
    ADC_DelSig_StartConvert();
    (void) ADC_DelSig_IsEndConversion(ADC_DelSig_WAIT_FOR_RESULT);
    ADC_DelSig_StopConvert();
    result = ADC_DelSig_GetResult16();

    return(result);
}
/* [] END OF FILE */
