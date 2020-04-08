/*******************************************************************************
* File Name: Num_Notes_Selector_ADC_theACLK.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_Num_Notes_Selector_ADC_theACLK_H)
#define CY_CLOCK_Num_Notes_Selector_ADC_theACLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
* Conditional Compilation Parameters
***************************************/

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component cy_clock_v2_20 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */


/***************************************
*        Function Prototypes
***************************************/

void Num_Notes_Selector_ADC_theACLK_Start(void) ;
void Num_Notes_Selector_ADC_theACLK_Stop(void) ;

#if(CY_PSOC3 || CY_PSOC5LP)
void Num_Notes_Selector_ADC_theACLK_StopBlock(void) ;
#endif /* (CY_PSOC3 || CY_PSOC5LP) */

void Num_Notes_Selector_ADC_theACLK_StandbyPower(uint8 state) ;
void Num_Notes_Selector_ADC_theACLK_SetDividerRegister(uint16 clkDivider, uint8 restart) 
                                ;
uint16 Num_Notes_Selector_ADC_theACLK_GetDividerRegister(void) ;
void Num_Notes_Selector_ADC_theACLK_SetModeRegister(uint8 modeBitMask) ;
void Num_Notes_Selector_ADC_theACLK_ClearModeRegister(uint8 modeBitMask) ;
uint8 Num_Notes_Selector_ADC_theACLK_GetModeRegister(void) ;
void Num_Notes_Selector_ADC_theACLK_SetSourceRegister(uint8 clkSource) ;
uint8 Num_Notes_Selector_ADC_theACLK_GetSourceRegister(void) ;
#if defined(Num_Notes_Selector_ADC_theACLK__CFG3)
void Num_Notes_Selector_ADC_theACLK_SetPhaseRegister(uint8 clkPhase) ;
uint8 Num_Notes_Selector_ADC_theACLK_GetPhaseRegister(void) ;
#endif /* defined(Num_Notes_Selector_ADC_theACLK__CFG3) */

#define Num_Notes_Selector_ADC_theACLK_Enable()                       Num_Notes_Selector_ADC_theACLK_Start()
#define Num_Notes_Selector_ADC_theACLK_Disable()                      Num_Notes_Selector_ADC_theACLK_Stop()
#define Num_Notes_Selector_ADC_theACLK_SetDivider(clkDivider)         Num_Notes_Selector_ADC_theACLK_SetDividerRegister(clkDivider, 1u)
#define Num_Notes_Selector_ADC_theACLK_SetDividerValue(clkDivider)    Num_Notes_Selector_ADC_theACLK_SetDividerRegister((clkDivider) - 1u, 1u)
#define Num_Notes_Selector_ADC_theACLK_SetMode(clkMode)               Num_Notes_Selector_ADC_theACLK_SetModeRegister(clkMode)
#define Num_Notes_Selector_ADC_theACLK_SetSource(clkSource)           Num_Notes_Selector_ADC_theACLK_SetSourceRegister(clkSource)
#if defined(Num_Notes_Selector_ADC_theACLK__CFG3)
#define Num_Notes_Selector_ADC_theACLK_SetPhase(clkPhase)             Num_Notes_Selector_ADC_theACLK_SetPhaseRegister(clkPhase)
#define Num_Notes_Selector_ADC_theACLK_SetPhaseValue(clkPhase)        Num_Notes_Selector_ADC_theACLK_SetPhaseRegister((clkPhase) + 1u)
#endif /* defined(Num_Notes_Selector_ADC_theACLK__CFG3) */


/***************************************
*             Registers
***************************************/

/* Register to enable or disable the clock */
#define Num_Notes_Selector_ADC_theACLK_CLKEN              (* (reg8 *) Num_Notes_Selector_ADC_theACLK__PM_ACT_CFG)
#define Num_Notes_Selector_ADC_theACLK_CLKEN_PTR          ((reg8 *) Num_Notes_Selector_ADC_theACLK__PM_ACT_CFG)

/* Register to enable or disable the clock */
#define Num_Notes_Selector_ADC_theACLK_CLKSTBY            (* (reg8 *) Num_Notes_Selector_ADC_theACLK__PM_STBY_CFG)
#define Num_Notes_Selector_ADC_theACLK_CLKSTBY_PTR        ((reg8 *) Num_Notes_Selector_ADC_theACLK__PM_STBY_CFG)

/* Clock LSB divider configuration register. */
#define Num_Notes_Selector_ADC_theACLK_DIV_LSB            (* (reg8 *) Num_Notes_Selector_ADC_theACLK__CFG0)
#define Num_Notes_Selector_ADC_theACLK_DIV_LSB_PTR        ((reg8 *) Num_Notes_Selector_ADC_theACLK__CFG0)
#define Num_Notes_Selector_ADC_theACLK_DIV_PTR            ((reg16 *) Num_Notes_Selector_ADC_theACLK__CFG0)

/* Clock MSB divider configuration register. */
#define Num_Notes_Selector_ADC_theACLK_DIV_MSB            (* (reg8 *) Num_Notes_Selector_ADC_theACLK__CFG1)
#define Num_Notes_Selector_ADC_theACLK_DIV_MSB_PTR        ((reg8 *) Num_Notes_Selector_ADC_theACLK__CFG1)

/* Mode and source configuration register */
#define Num_Notes_Selector_ADC_theACLK_MOD_SRC            (* (reg8 *) Num_Notes_Selector_ADC_theACLK__CFG2)
#define Num_Notes_Selector_ADC_theACLK_MOD_SRC_PTR        ((reg8 *) Num_Notes_Selector_ADC_theACLK__CFG2)

#if defined(Num_Notes_Selector_ADC_theACLK__CFG3)
/* Analog clock phase configuration register */
#define Num_Notes_Selector_ADC_theACLK_PHASE              (* (reg8 *) Num_Notes_Selector_ADC_theACLK__CFG3)
#define Num_Notes_Selector_ADC_theACLK_PHASE_PTR          ((reg8 *) Num_Notes_Selector_ADC_theACLK__CFG3)
#endif /* defined(Num_Notes_Selector_ADC_theACLK__CFG3) */


/**************************************
*       Register Constants
**************************************/

/* Power manager register masks */
#define Num_Notes_Selector_ADC_theACLK_CLKEN_MASK         Num_Notes_Selector_ADC_theACLK__PM_ACT_MSK
#define Num_Notes_Selector_ADC_theACLK_CLKSTBY_MASK       Num_Notes_Selector_ADC_theACLK__PM_STBY_MSK

/* CFG2 field masks */
#define Num_Notes_Selector_ADC_theACLK_SRC_SEL_MSK        Num_Notes_Selector_ADC_theACLK__CFG2_SRC_SEL_MASK
#define Num_Notes_Selector_ADC_theACLK_MODE_MASK          (~(Num_Notes_Selector_ADC_theACLK_SRC_SEL_MSK))

#if defined(Num_Notes_Selector_ADC_theACLK__CFG3)
/* CFG3 phase mask */
#define Num_Notes_Selector_ADC_theACLK_PHASE_MASK         Num_Notes_Selector_ADC_theACLK__CFG3_PHASE_DLY_MASK
#endif /* defined(Num_Notes_Selector_ADC_theACLK__CFG3) */

#endif /* CY_CLOCK_Num_Notes_Selector_ADC_theACLK_H */


/* [] END OF FILE */
