/*******************************************************************************
* File Name: game_random_mode_generator.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "game_random_mode_generator.h"

uint8 game_random_mode_generator_initVar = 0u;


/*******************************************************************************
* Function Name: game_random_mode_generator_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void game_random_mode_generator_Init(void) 
{
        #if (!game_random_mode_generator_UsingFixedFunction && !game_random_mode_generator_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!game_random_mode_generator_UsingFixedFunction && !game_random_mode_generator_ControlRegRemoved) */
        
        #if(!game_random_mode_generator_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 game_random_mode_generator_interruptState;
        #endif /* (!game_random_mode_generator_UsingFixedFunction) */
        
        #if (game_random_mode_generator_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            game_random_mode_generator_CONTROL &= game_random_mode_generator_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                game_random_mode_generator_CONTROL2 &= ((uint8)(~game_random_mode_generator_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                game_random_mode_generator_CONTROL3 &= ((uint8)(~game_random_mode_generator_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (game_random_mode_generator_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                game_random_mode_generator_CONTROL |= game_random_mode_generator_ONESHOT;
            #endif /* (game_random_mode_generator_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            game_random_mode_generator_CONTROL2 |= game_random_mode_generator_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            game_random_mode_generator_RT1 &= ((uint8)(~game_random_mode_generator_RT1_MASK));
            game_random_mode_generator_RT1 |= game_random_mode_generator_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            game_random_mode_generator_RT1 &= ((uint8)(~game_random_mode_generator_SYNCDSI_MASK));
            game_random_mode_generator_RT1 |= game_random_mode_generator_SYNCDSI_EN;

        #else
            #if(!game_random_mode_generator_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = game_random_mode_generator_CONTROL & ((uint8)(~game_random_mode_generator_CTRL_CMPMODE_MASK));
            game_random_mode_generator_CONTROL = ctrl | game_random_mode_generator_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = game_random_mode_generator_CONTROL & ((uint8)(~game_random_mode_generator_CTRL_CAPMODE_MASK));
            
            #if( 0 != game_random_mode_generator_CAPTURE_MODE_CONF)
                game_random_mode_generator_CONTROL = ctrl | game_random_mode_generator_DEFAULT_CAPTURE_MODE;
            #else
                game_random_mode_generator_CONTROL = ctrl;
            #endif /* 0 != game_random_mode_generator_CAPTURE_MODE */ 
            
            #endif /* (!game_random_mode_generator_ControlRegRemoved) */
        #endif /* (game_random_mode_generator_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!game_random_mode_generator_UsingFixedFunction)
            game_random_mode_generator_ClearFIFO();
        #endif /* (!game_random_mode_generator_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        game_random_mode_generator_WritePeriod(game_random_mode_generator_INIT_PERIOD_VALUE);
        #if (!(game_random_mode_generator_UsingFixedFunction && (CY_PSOC5A)))
            game_random_mode_generator_WriteCounter(game_random_mode_generator_INIT_COUNTER_VALUE);
        #endif /* (!(game_random_mode_generator_UsingFixedFunction && (CY_PSOC5A))) */
        game_random_mode_generator_SetInterruptMode(game_random_mode_generator_INIT_INTERRUPTS_MASK);
        
        #if (!game_random_mode_generator_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)game_random_mode_generator_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            game_random_mode_generator_WriteCompare(game_random_mode_generator_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            game_random_mode_generator_interruptState = CyEnterCriticalSection();
            
            game_random_mode_generator_STATUS_AUX_CTRL |= game_random_mode_generator_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(game_random_mode_generator_interruptState);
            
        #endif /* (!game_random_mode_generator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: game_random_mode_generator_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void game_random_mode_generator_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (game_random_mode_generator_UsingFixedFunction)
        game_random_mode_generator_GLOBAL_ENABLE |= game_random_mode_generator_BLOCK_EN_MASK;
        game_random_mode_generator_GLOBAL_STBY_ENABLE |= game_random_mode_generator_BLOCK_STBY_EN_MASK;
    #endif /* (game_random_mode_generator_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!game_random_mode_generator_ControlRegRemoved || game_random_mode_generator_UsingFixedFunction)
        game_random_mode_generator_CONTROL |= game_random_mode_generator_CTRL_ENABLE;                
    #endif /* (!game_random_mode_generator_ControlRegRemoved || game_random_mode_generator_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: game_random_mode_generator_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  game_random_mode_generator_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void game_random_mode_generator_Start(void) 
{
    if(game_random_mode_generator_initVar == 0u)
    {
        game_random_mode_generator_Init();
        
        game_random_mode_generator_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    game_random_mode_generator_Enable();        
}


/*******************************************************************************
* Function Name: game_random_mode_generator_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void game_random_mode_generator_Stop(void) 
{
    /* Disable Counter */
    #if(!game_random_mode_generator_ControlRegRemoved || game_random_mode_generator_UsingFixedFunction)
        game_random_mode_generator_CONTROL &= ((uint8)(~game_random_mode_generator_CTRL_ENABLE));        
    #endif /* (!game_random_mode_generator_ControlRegRemoved || game_random_mode_generator_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (game_random_mode_generator_UsingFixedFunction)
        game_random_mode_generator_GLOBAL_ENABLE &= ((uint8)(~game_random_mode_generator_BLOCK_EN_MASK));
        game_random_mode_generator_GLOBAL_STBY_ENABLE &= ((uint8)(~game_random_mode_generator_BLOCK_STBY_EN_MASK));
    #endif /* (game_random_mode_generator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: game_random_mode_generator_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void game_random_mode_generator_SetInterruptMode(uint8 interruptsMask) 
{
    game_random_mode_generator_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: game_random_mode_generator_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   game_random_mode_generator_ReadStatusRegister(void) 
{
    return game_random_mode_generator_STATUS;
}


#if(!game_random_mode_generator_ControlRegRemoved)
/*******************************************************************************
* Function Name: game_random_mode_generator_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   game_random_mode_generator_ReadControlRegister(void) 
{
    return game_random_mode_generator_CONTROL;
}


/*******************************************************************************
* Function Name: game_random_mode_generator_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    game_random_mode_generator_WriteControlRegister(uint8 control) 
{
    game_random_mode_generator_CONTROL = control;
}

#endif  /* (!game_random_mode_generator_ControlRegRemoved) */


#if (!(game_random_mode_generator_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: game_random_mode_generator_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void game_random_mode_generator_WriteCounter(uint16 counter) \
                                   
{
    #if(game_random_mode_generator_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (game_random_mode_generator_GLOBAL_ENABLE & game_random_mode_generator_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        game_random_mode_generator_GLOBAL_ENABLE |= game_random_mode_generator_BLOCK_EN_MASK;
        CY_SET_REG16(game_random_mode_generator_COUNTER_LSB_PTR, (uint16)counter);
        game_random_mode_generator_GLOBAL_ENABLE &= ((uint8)(~game_random_mode_generator_BLOCK_EN_MASK));
    #else
        CY_SET_REG16(game_random_mode_generator_COUNTER_LSB_PTR, counter);
    #endif /* (game_random_mode_generator_UsingFixedFunction) */
}
#endif /* (!(game_random_mode_generator_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: game_random_mode_generator_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) The present value of the counter.
*
*******************************************************************************/
uint16 game_random_mode_generator_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(game_random_mode_generator_UsingFixedFunction)
		(void)CY_GET_REG16(game_random_mode_generator_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(game_random_mode_generator_COUNTER_LSB_PTR_8BIT);
	#endif/* (game_random_mode_generator_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(game_random_mode_generator_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(game_random_mode_generator_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(game_random_mode_generator_STATICCOUNT_LSB_PTR));
    #endif /* (game_random_mode_generator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: game_random_mode_generator_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint16) Present Capture value.
*
*******************************************************************************/
uint16 game_random_mode_generator_ReadCapture(void) 
{
    #if(game_random_mode_generator_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(game_random_mode_generator_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG16(game_random_mode_generator_STATICCOUNT_LSB_PTR));
    #endif /* (game_random_mode_generator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: game_random_mode_generator_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint16) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void game_random_mode_generator_WritePeriod(uint16 period) 
{
    #if(game_random_mode_generator_UsingFixedFunction)
        CY_SET_REG16(game_random_mode_generator_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG16(game_random_mode_generator_PERIOD_LSB_PTR, period);
    #endif /* (game_random_mode_generator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: game_random_mode_generator_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint16) Present period value.
*
*******************************************************************************/
uint16 game_random_mode_generator_ReadPeriod(void) 
{
    #if(game_random_mode_generator_UsingFixedFunction)
        return ((uint16)CY_GET_REG16(game_random_mode_generator_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG16(game_random_mode_generator_PERIOD_LSB_PTR));
    #endif /* (game_random_mode_generator_UsingFixedFunction) */
}


#if (!game_random_mode_generator_UsingFixedFunction)
/*******************************************************************************
* Function Name: game_random_mode_generator_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void game_random_mode_generator_WriteCompare(uint16 compare) \
                                   
{
    #if(game_random_mode_generator_UsingFixedFunction)
        CY_SET_REG16(game_random_mode_generator_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG16(game_random_mode_generator_COMPARE_LSB_PTR, compare);
    #endif /* (game_random_mode_generator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: game_random_mode_generator_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint16) Present compare value.
*
*******************************************************************************/
uint16 game_random_mode_generator_ReadCompare(void) 
{
    return (CY_GET_REG16(game_random_mode_generator_COMPARE_LSB_PTR));
}


#if (game_random_mode_generator_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: game_random_mode_generator_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void game_random_mode_generator_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    game_random_mode_generator_CONTROL &= ((uint8)(~game_random_mode_generator_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    game_random_mode_generator_CONTROL |= compareMode;
}
#endif  /* (game_random_mode_generator_COMPARE_MODE_SOFTWARE) */


#if (game_random_mode_generator_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: game_random_mode_generator_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void game_random_mode_generator_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    game_random_mode_generator_CONTROL &= ((uint8)(~game_random_mode_generator_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    game_random_mode_generator_CONTROL |= ((uint8)((uint8)captureMode << game_random_mode_generator_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (game_random_mode_generator_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: game_random_mode_generator_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void game_random_mode_generator_ClearFIFO(void) 
{

    while(0u != (game_random_mode_generator_ReadStatusRegister() & game_random_mode_generator_STATUS_FIFONEMP))
    {
        (void)game_random_mode_generator_ReadCapture();
    }

}
#endif  /* (!game_random_mode_generator_UsingFixedFunction) */


/* [] END OF FILE */

