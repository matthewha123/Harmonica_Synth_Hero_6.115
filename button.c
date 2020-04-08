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
#include "button.h"
const uint8 DEBOUNCE_TIME = 60;
void init_button(Button* b) {
    b->edge = 0;
    b->last_edge_time = CySysTickGetValue();
    b->last_state = UNHELD;
    b->num_presses = 0;
}

Button_State poll_button(Button* b, uint8 pin_value) {
    
    Button_State current_state = (pin_value == 0) ? HELD : UNHELD;
    
    if(current_state != b->last_state) {
        // edge detected
        b->last_state = current_state;
        b->last_edge_time = ms_counter_ReadCounter();
        b->edge = 1;
    } else {
        
        if(b->edge == 1) {
            b->edge = 0;
            
            if(current_state == UNHELD) {
                b->num_presses++;
            }
            
            return (current_state == UNHELD) ? FALLING_EDGE : RISING_EDGE;
        }
        
    }
    return current_state;
}

/* [] END OF FILE */
