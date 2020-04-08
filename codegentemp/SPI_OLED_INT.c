/*******************************************************************************
* File Name: SPI_OLED_INT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service Routine (ISR) for the SPI Master
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SPI_OLED_PVT.h"
#include "cyapicallbacks.h"

/* User code required at start of ISR */
/* `#START SPI_OLED_ISR_START_DEF` */
/* `#END` */


/*******************************************************************************
* Function Name: SPI_OLED_TX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for TX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPI_OLED_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  SPI_OLED_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified when exist data to
*  sending and FIFO Not Full.
*  SPI_OLED_txBuffer[SPI_OLED_TX_BUFFER_SIZE] - used to store
*  data to sending.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SPI_OLED_TX_ISR)
{
    #if(SPI_OLED_TX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
    #endif /* (SPI_OLED_TX_SOFTWARE_BUF_ENABLED) */

    #ifdef SPI_OLED_TX_ISR_ENTRY_CALLBACK
        SPI_OLED_TX_ISR_EntryCallback();
    #endif /* SPI_OLED_TX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START SPI_OLED_TX_ISR_START` */
    
    /* `#END` */
    
    #if(SPI_OLED_TX_SOFTWARE_BUF_ENABLED)
        /* Check if TX data buffer is not empty and there is space in TX FIFO */
        while(SPI_OLED_txBufferRead != SPI_OLED_txBufferWrite)
        {
            tmpStatus = SPI_OLED_GET_STATUS_TX(SPI_OLED_swStatusTx);
            SPI_OLED_swStatusTx = tmpStatus;

            if(0u != (SPI_OLED_swStatusTx & SPI_OLED_STS_TX_FIFO_NOT_FULL))
            {
                if(0u == SPI_OLED_txBufferFull)
                {
                   SPI_OLED_txBufferRead++;

                    if(SPI_OLED_txBufferRead >= SPI_OLED_TX_BUFFER_SIZE)
                    {
                        SPI_OLED_txBufferRead = 0u;
                    }
                }
                else
                {
                    SPI_OLED_txBufferFull = 0u;
                }

                /* Put data element into the TX FIFO */
                CY_SET_REG8(SPI_OLED_TXDATA_PTR, 
                                             SPI_OLED_txBuffer[SPI_OLED_txBufferRead]);
            }
            else
            {
                break;
            }
        }

        if(SPI_OLED_txBufferRead == SPI_OLED_txBufferWrite)
        {
            /* TX Buffer is EMPTY: disable interrupt on TX NOT FULL */
            SPI_OLED_TX_STATUS_MASK_REG &= ((uint8) ~SPI_OLED_STS_TX_FIFO_NOT_FULL);
        }

    #endif /* (SPI_OLED_TX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SPI_OLED_TX_ISR_END` */

    /* `#END` */
    
    #ifdef SPI_OLED_TX_ISR_EXIT_CALLBACK
        SPI_OLED_TX_ISR_ExitCallback();
    #endif /* SPI_OLED_TX_ISR_EXIT_CALLBACK */
}


/*******************************************************************************
* Function Name: SPI_OLED_RX_ISR
********************************************************************************
*
* Summary:
*  Interrupt Service Routine for RX portion of the SPI Master.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  SPI_OLED_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer modified when FIFO contains
*  new data.
*  SPI_OLED_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified when overflow occurred.
*  SPI_OLED_rxBuffer[SPI_OLED_RX_BUFFER_SIZE] - used to store
*  received data, modified when FIFO contains new data.
*  All described above Global variables are used when Software Buffer is used.
*
*******************************************************************************/
CY_ISR(SPI_OLED_RX_ISR)
{
    #if(SPI_OLED_RX_SOFTWARE_BUF_ENABLED)
        uint8 tmpStatus;
        uint8 rxData;
    #endif /* (SPI_OLED_RX_SOFTWARE_BUF_ENABLED) */

    #ifdef SPI_OLED_RX_ISR_ENTRY_CALLBACK
        SPI_OLED_RX_ISR_EntryCallback();
    #endif /* SPI_OLED_RX_ISR_ENTRY_CALLBACK */

    /* User code required at start of ISR */
    /* `#START SPI_OLED_RX_ISR_START` */

    /* `#END` */
    
    #if(SPI_OLED_RX_SOFTWARE_BUF_ENABLED)

        tmpStatus = SPI_OLED_GET_STATUS_RX(SPI_OLED_swStatusRx);
        SPI_OLED_swStatusRx = tmpStatus;

        /* Check if RX data FIFO has some data to be moved into the RX Buffer */
        while(0u != (SPI_OLED_swStatusRx & SPI_OLED_STS_RX_FIFO_NOT_EMPTY))
        {
            rxData = CY_GET_REG8(SPI_OLED_RXDATA_PTR);

            /* Set next pointer. */
            SPI_OLED_rxBufferWrite++;
            if(SPI_OLED_rxBufferWrite >= SPI_OLED_RX_BUFFER_SIZE)
            {
                SPI_OLED_rxBufferWrite = 0u;
            }

            if(SPI_OLED_rxBufferWrite == SPI_OLED_rxBufferRead)
            {
                SPI_OLED_rxBufferRead++;
                if(SPI_OLED_rxBufferRead >= SPI_OLED_RX_BUFFER_SIZE)
                {
                    SPI_OLED_rxBufferRead = 0u;
                }

                SPI_OLED_rxBufferFull = 1u;
            }

            /* Move data from the FIFO to the Buffer */
            SPI_OLED_rxBuffer[SPI_OLED_rxBufferWrite] = rxData;

            tmpStatus = SPI_OLED_GET_STATUS_RX(SPI_OLED_swStatusRx);
            SPI_OLED_swStatusRx = tmpStatus;
        }

    #endif /* (SPI_OLED_RX_SOFTWARE_BUF_ENABLED) */

    /* User code required at end of ISR (Optional) */
    /* `#START SPI_OLED_RX_ISR_END` */

    /* `#END` */
    
    #ifdef SPI_OLED_RX_ISR_EXIT_CALLBACK
        SPI_OLED_RX_ISR_ExitCallback();
    #endif /* SPI_OLED_RX_ISR_EXIT_CALLBACK */
}

/* [] END OF FILE */
