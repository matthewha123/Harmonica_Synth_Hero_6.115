/*******************************************************************************
* File Name: game_random_mode_generator_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
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

static game_random_mode_generator_backupStruct game_random_mode_generator_backup;


/*******************************************************************************
* Function Name: game_random_mode_generator_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  game_random_mode_generator_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void game_random_mode_generator_SaveConfig(void) 
{
    #if (!game_random_mode_generator_UsingFixedFunction)

        game_random_mode_generator_backup.CounterUdb = game_random_mode_generator_ReadCounter();

        #if(!game_random_mode_generator_ControlRegRemoved)
            game_random_mode_generator_backup.CounterControlRegister = game_random_mode_generator_ReadControlRegister();
        #endif /* (!game_random_mode_generator_ControlRegRemoved) */

    #endif /* (!game_random_mode_generator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: game_random_mode_generator_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  game_random_mode_generator_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void game_random_mode_generator_RestoreConfig(void) 
{      
    #if (!game_random_mode_generator_UsingFixedFunction)

       game_random_mode_generator_WriteCounter(game_random_mode_generator_backup.CounterUdb);

        #if(!game_random_mode_generator_ControlRegRemoved)
            game_random_mode_generator_WriteControlRegister(game_random_mode_generator_backup.CounterControlRegister);
        #endif /* (!game_random_mode_generator_ControlRegRemoved) */

    #endif /* (!game_random_mode_generator_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: game_random_mode_generator_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  game_random_mode_generator_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void game_random_mode_generator_Sleep(void) 
{
    #if(!game_random_mode_generator_ControlRegRemoved)
        /* Save Counter's enable state */
        if(game_random_mode_generator_CTRL_ENABLE == (game_random_mode_generator_CONTROL & game_random_mode_generator_CTRL_ENABLE))
        {
            /* Counter is enabled */
            game_random_mode_generator_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            game_random_mode_generator_backup.CounterEnableState = 0u;
        }
    #else
        game_random_mode_generator_backup.CounterEnableState = 1u;
        if(game_random_mode_generator_backup.CounterEnableState != 0u)
        {
            game_random_mode_generator_backup.CounterEnableState = 0u;
        }
    #endif /* (!game_random_mode_generator_ControlRegRemoved) */
    
    game_random_mode_generator_Stop();
    game_random_mode_generator_SaveConfig();
}


/*******************************************************************************
* Function Name: game_random_mode_generator_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  game_random_mode_generator_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void game_random_mode_generator_Wakeup(void) 
{
    game_random_mode_generator_RestoreConfig();
    #if(!game_random_mode_generator_ControlRegRemoved)
        if(game_random_mode_generator_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            game_random_mode_generator_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!game_random_mode_generator_ControlRegRemoved) */
    
}


/* [] END OF FILE */
