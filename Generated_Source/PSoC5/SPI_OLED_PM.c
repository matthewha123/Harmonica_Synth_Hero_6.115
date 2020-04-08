/*******************************************************************************
* File Name: SPI_OLED_PM.c
* Version 2.50
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_OLED_PVT.h"

static SPI_OLED_BACKUP_STRUCT SPI_OLED_backup =
{
    SPI_OLED_DISABLED,
    SPI_OLED_BITCTR_INIT,
};


/*******************************************************************************
* Function Name: SPI_OLED_SaveConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI_OLED_SaveConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_OLED_RestoreConfig
********************************************************************************
*
* Summary:
*  Empty function. Included for consistency with other components.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void SPI_OLED_RestoreConfig(void) 
{

}


/*******************************************************************************
* Function Name: SPI_OLED_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI_OLED_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_OLED_Sleep(void) 
{
    /* Save components enable state */
    SPI_OLED_backup.enableState = ((uint8) SPI_OLED_IS_ENABLED);

    SPI_OLED_Stop();
}


/*******************************************************************************
* Function Name: SPI_OLED_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  SPI_OLED_backup - used when non-retention registers are restored.
*  SPI_OLED_txBufferWrite - modified every function call - resets to
*  zero.
*  SPI_OLED_txBufferRead - modified every function call - resets to
*  zero.
*  SPI_OLED_rxBufferWrite - modified every function call - resets to
*  zero.
*  SPI_OLED_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void SPI_OLED_Wakeup(void) 
{
    #if(SPI_OLED_RX_SOFTWARE_BUF_ENABLED)
        SPI_OLED_rxBufferFull  = 0u;
        SPI_OLED_rxBufferRead  = 0u;
        SPI_OLED_rxBufferWrite = 0u;
    #endif /* (SPI_OLED_RX_SOFTWARE_BUF_ENABLED) */

    #if(SPI_OLED_TX_SOFTWARE_BUF_ENABLED)
        SPI_OLED_txBufferFull  = 0u;
        SPI_OLED_txBufferRead  = 0u;
        SPI_OLED_txBufferWrite = 0u;
    #endif /* (SPI_OLED_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    SPI_OLED_ClearFIFO();

    /* Restore components block enable state */
    if(0u != SPI_OLED_backup.enableState)
    {
        SPI_OLED_Enable();
    }
}


/* [] END OF FILE */
