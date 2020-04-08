/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
* This example project demonstrates the basic functionality of the DVDAC
* component. The Delta Sigma ADC component is used to measure the output
* voltage.
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <device.h>
#include <stdio.h>
#include "photo_mux.h"
#include "harmonica_control.h"
#include "common.h"
#include "Num_Notes_Selector_ADC.h"
#include "oled_ssd1331.h"
#include "game.h"

int16 ADC_DelSig_ReadInt16(void);


int main()
{
    char   muxline[16u];
    char   labelline[16u];
    
    /* Start components */
    VDAC8_1_Start();
    ADC_DelSig_Start();
    LCD_Start();
    Timer_1_Start();
    Clock_1_Start();
    Clock_2_Start();
    Clock_3_Start();
    Timer_2_Start();
    ms_counter_Start();
    game_random_mode_generator_Start();
    init_harmonica(&harmonica);
    init_harmonica_control();
    
    audio_isr_Start();  // this interrupt plays c4 currently
    //harmonica_isr_Start();
    CySysTickStart();
    CyGlobalIntEnable; /* enable Global interrupts */ 

    Num_Notes_Selector_ADC_Start();
    spi_done_Start();
    SPI_OLED_Start();
    SPI_OLED_TxEnable();
    init_oled();
    clear_whole_display();
    
    game_init();
    
    for(;;)
    {
        
        // send_cmd(0xAE);
        // 4, 5, 6, 7, 8 channels don't work
        // 0100, 0101, 0110, 0111, 1000
//        for(int channel = 0; channel < 10; channel++) {
//            sprintf(
//            muxline, "%d",
//            (int16) photo_read_mux(channel)
//            );
//            sprintf(
//                labelline, "Photo #%d",
//                channel);
//            
//            LCD_ClearDisplay();
//            LCD_Position(0u, 0u);
//            LCD_PrintString(labelline);
//            LCD_Position(1u, 0u);
//            LCD_PrintString(muxline);
//
//            CyDelay(1000u);
//        }
//            uint8 channel = 4;
//            photo_enable_mux(channel);
//            inhibit_output(0);
//            CyDelayCycles(10);
//            int16 result = read_mux();
//            sprintf(
//            muxline, "%d",
//            (int16) photo_read_mux(channel)
//            );
//            sprintf(
//                labelline, "Photo #%d",
//                channel);
//            
//            LCD_ClearDisplay();
//            LCD_Position(0u, 0u);
//            LCD_PrintString(labelline);
//            LCD_Position(1u, 0u);
//            LCD_PrintString(muxline);
//            CyDelay(1000u);
        
        
//            OLED_CS_Write(1);
//            OLED_DC_Write(1);
//            OLED_SDA_Write(1);
//            OLED_SCK_Write(1);
             
            game_tick(&harmonica);
            
            harmonica_tick(&harmonica);
            if(harmonica.recent_change == TRUE) {
                LCD_ClearDisplay();
                switch(harmonica.current_note_mode) {
                    case BLOWN_NOTES:
                        LCD_Position(0u,0u);
                        LCD_PrintString("Blow");
                    break;
                    case SUCKED_NOTES:
                        LCD_Position(0u,0u);
                        LCD_PrintString("Suck");
                    break;
                    case NO_NOTES:
                        LCD_Position(0u,0u);
                        LCD_PrintString("None");
                    break;
                }
                
                    LCD_Position(1u,0u);
                    sprintf(muxline, "%d", harmonica.active_hole);
                    LCD_PrintString(muxline);
                    
                    sprintf(muxline, "%d", harmonica.raw_harmonica_output.num_notes_played);
                    LCD_Position(1u,15u);
                    LCD_PrintString(muxline);
                    
                if(harmonica.output_mode == RECORDING) {

                    uint8 recordings_idx = harmonica.playback_notes.recordings_idx;
                    LCD_Position(0u, 5u);
                    char rec_string_buffer[20];
                    sprintf(rec_string_buffer, "Rec %d/20", recordings_idx);
                    LCD_PrintString(rec_string_buffer);
                }
                
                if(harmonica.output_mode == ECHO) {
                    LCD_Position(0u, 5u);
                    LCD_PrintString("ECHO");
                }
            }
            
    }
}


/* [] END OF FILE */
