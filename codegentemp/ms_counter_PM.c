/*******************************************************************************
* File Name: ms_counter_PM.c  
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

#include "ms_counter.h"

static ms_counter_backupStruct ms_counter_backup;


/*******************************************************************************
* Function Name: ms_counter_SaveConfig
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
*  ms_counter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void ms_counter_SaveConfig(void) 
{
    #if (!ms_counter_UsingFixedFunction)

        ms_counter_backup.CounterUdb = ms_counter_ReadCounter();

        #if(!ms_counter_ControlRegRemoved)
            ms_counter_backup.CounterControlRegister = ms_counter_ReadControlRegister();
        #endif /* (!ms_counter_ControlRegRemoved) */

    #endif /* (!ms_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: ms_counter_RestoreConfig
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
*  ms_counter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void ms_counter_RestoreConfig(void) 
{      
    #if (!ms_counter_UsingFixedFunction)

       ms_counter_WriteCounter(ms_counter_backup.CounterUdb);

        #if(!ms_counter_ControlRegRemoved)
            ms_counter_WriteControlRegister(ms_counter_backup.CounterControlRegister);
        #endif /* (!ms_counter_ControlRegRemoved) */

    #endif /* (!ms_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: ms_counter_Sleep
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
*  ms_counter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void ms_counter_Sleep(void) 
{
    #if(!ms_counter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(ms_counter_CTRL_ENABLE == (ms_counter_CONTROL & ms_counter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            ms_counter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            ms_counter_backup.CounterEnableState = 0u;
        }
    #else
        ms_counter_backup.CounterEnableState = 1u;
        if(ms_counter_backup.CounterEnableState != 0u)
        {
            ms_counter_backup.CounterEnableState = 0u;
        }
    #endif /* (!ms_counter_ControlRegRemoved) */
    
    ms_counter_Stop();
    ms_counter_SaveConfig();
}


/*******************************************************************************
* Function Name: ms_counter_Wakeup
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
*  ms_counter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void ms_counter_Wakeup(void) 
{
    ms_counter_RestoreConfig();
    #if(!ms_counter_ControlRegRemoved)
        if(ms_counter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            ms_counter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!ms_counter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
