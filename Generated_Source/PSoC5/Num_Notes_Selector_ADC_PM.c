/*******************************************************************************
* File Name: Num_Notes_Selector_ADC_PM.c
* Version 3.10
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Num_Notes_Selector_ADC.h"


/***************************************
* Local data allocation
***************************************/

static Num_Notes_Selector_ADC_BACKUP_STRUCT  Num_Notes_Selector_ADC_backup =
{
    Num_Notes_Selector_ADC_DISABLED
};


/*******************************************************************************
* Function Name: Num_Notes_Selector_ADC_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Num_Notes_Selector_ADC_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: Num_Notes_Selector_ADC_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Num_Notes_Selector_ADC_RestoreConfig(void)
{
    /* All congiguration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: Num_Notes_Selector_ADC_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred routine to prepare the component for sleep.
*  The Num_Notes_Selector_ADC_Sleep() routine saves the current component state,
*  then it calls the ADC_Stop() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Num_Notes_Selector_ADC_backup - The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void Num_Notes_Selector_ADC_Sleep(void)
{
    if((Num_Notes_Selector_ADC_PWRMGR_SAR_REG  & Num_Notes_Selector_ADC_ACT_PWR_SAR_EN) != 0u)
    {
        if((Num_Notes_Selector_ADC_SAR_CSR0_REG & Num_Notes_Selector_ADC_SAR_SOF_START_CONV) != 0u)
        {
            Num_Notes_Selector_ADC_backup.enableState = Num_Notes_Selector_ADC_ENABLED | Num_Notes_Selector_ADC_STARTED;
        }
        else
        {
            Num_Notes_Selector_ADC_backup.enableState = Num_Notes_Selector_ADC_ENABLED;
        }
        Num_Notes_Selector_ADC_Stop();
    }
    else
    {
        Num_Notes_Selector_ADC_backup.enableState = Num_Notes_Selector_ADC_DISABLED;
    }
}


/*******************************************************************************
* Function Name: Num_Notes_Selector_ADC_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred routine to restore the component to the state when
*  Num_Notes_Selector_ADC_Sleep() was called. If the component was enabled before the
*  Num_Notes_Selector_ADC_Sleep() function was called, the
*  Num_Notes_Selector_ADC_Wakeup() function also re-enables the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Num_Notes_Selector_ADC_backup - The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void Num_Notes_Selector_ADC_Wakeup(void)
{
    if(Num_Notes_Selector_ADC_backup.enableState != Num_Notes_Selector_ADC_DISABLED)
    {
        Num_Notes_Selector_ADC_Enable();
        #if(Num_Notes_Selector_ADC_DEFAULT_CONV_MODE != Num_Notes_Selector_ADC__HARDWARE_TRIGGER)
            if((Num_Notes_Selector_ADC_backup.enableState & Num_Notes_Selector_ADC_STARTED) != 0u)
            {
                Num_Notes_Selector_ADC_StartConvert();
            }
        #endif /* End Num_Notes_Selector_ADC_DEFAULT_CONV_MODE != Num_Notes_Selector_ADC__HARDWARE_TRIGGER */
    }
}


/* [] END OF FILE */
