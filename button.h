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
#ifndef __BUTTON__H_
#define __BUTTON__H_
#include "Button1.h"
#include "Button2.h"
#include "Button3.h"
#include "Button4.h"
// #include "Button5.h"
    
typedef enum {
    HELD,
    UNHELD,
    RISING_EDGE,
    FALLING_EDGE,
} Button_State;

typedef struct {
    uint32 last_edge_time;   // always store the state of the last pin_value, if it changes, store time in last_edge, then, if it doesn't change, check time > 60 ms
    Button_State last_state;
    uint32 num_presses;
    uint8 edge;
} Button;
    
Button_State poll_button(Button* b, uint8 pin_value);
void init_button(Button* b);

#endif // __BUTTON__H_

/* [] END OF FILE */
