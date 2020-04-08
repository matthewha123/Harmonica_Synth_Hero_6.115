/*******************************************************************************
* File Name: Button4.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Button4_H) /* Pins Button4_H */
#define CY_PINS_Button4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Button4_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Button4__PORT == 15 && ((Button4__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Button4_Write(uint8 value);
void    Button4_SetDriveMode(uint8 mode);
uint8   Button4_ReadDataReg(void);
uint8   Button4_Read(void);
void    Button4_SetInterruptMode(uint16 position, uint16 mode);
uint8   Button4_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Button4_SetDriveMode() function.
     *  @{
     */
        #define Button4_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Button4_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Button4_DM_RES_UP          PIN_DM_RES_UP
        #define Button4_DM_RES_DWN         PIN_DM_RES_DWN
        #define Button4_DM_OD_LO           PIN_DM_OD_LO
        #define Button4_DM_OD_HI           PIN_DM_OD_HI
        #define Button4_DM_STRONG          PIN_DM_STRONG
        #define Button4_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Button4_MASK               Button4__MASK
#define Button4_SHIFT              Button4__SHIFT
#define Button4_WIDTH              1u

/* Interrupt constants */
#if defined(Button4__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Button4_SetInterruptMode() function.
     *  @{
     */
        #define Button4_INTR_NONE      (uint16)(0x0000u)
        #define Button4_INTR_RISING    (uint16)(0x0001u)
        #define Button4_INTR_FALLING   (uint16)(0x0002u)
        #define Button4_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Button4_INTR_MASK      (0x01u) 
#endif /* (Button4__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Button4_PS                     (* (reg8 *) Button4__PS)
/* Data Register */
#define Button4_DR                     (* (reg8 *) Button4__DR)
/* Port Number */
#define Button4_PRT_NUM                (* (reg8 *) Button4__PRT) 
/* Connect to Analog Globals */                                                  
#define Button4_AG                     (* (reg8 *) Button4__AG)                       
/* Analog MUX bux enable */
#define Button4_AMUX                   (* (reg8 *) Button4__AMUX) 
/* Bidirectional Enable */                                                        
#define Button4_BIE                    (* (reg8 *) Button4__BIE)
/* Bit-mask for Aliased Register Access */
#define Button4_BIT_MASK               (* (reg8 *) Button4__BIT_MASK)
/* Bypass Enable */
#define Button4_BYP                    (* (reg8 *) Button4__BYP)
/* Port wide control signals */                                                   
#define Button4_CTL                    (* (reg8 *) Button4__CTL)
/* Drive Modes */
#define Button4_DM0                    (* (reg8 *) Button4__DM0) 
#define Button4_DM1                    (* (reg8 *) Button4__DM1)
#define Button4_DM2                    (* (reg8 *) Button4__DM2) 
/* Input Buffer Disable Override */
#define Button4_INP_DIS                (* (reg8 *) Button4__INP_DIS)
/* LCD Common or Segment Drive */
#define Button4_LCD_COM_SEG            (* (reg8 *) Button4__LCD_COM_SEG)
/* Enable Segment LCD */
#define Button4_LCD_EN                 (* (reg8 *) Button4__LCD_EN)
/* Slew Rate Control */
#define Button4_SLW                    (* (reg8 *) Button4__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Button4_PRTDSI__CAPS_SEL       (* (reg8 *) Button4__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Button4_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Button4__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Button4_PRTDSI__OE_SEL0        (* (reg8 *) Button4__PRTDSI__OE_SEL0) 
#define Button4_PRTDSI__OE_SEL1        (* (reg8 *) Button4__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Button4_PRTDSI__OUT_SEL0       (* (reg8 *) Button4__PRTDSI__OUT_SEL0) 
#define Button4_PRTDSI__OUT_SEL1       (* (reg8 *) Button4__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Button4_PRTDSI__SYNC_OUT       (* (reg8 *) Button4__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Button4__SIO_CFG)
    #define Button4_SIO_HYST_EN        (* (reg8 *) Button4__SIO_HYST_EN)
    #define Button4_SIO_REG_HIFREQ     (* (reg8 *) Button4__SIO_REG_HIFREQ)
    #define Button4_SIO_CFG            (* (reg8 *) Button4__SIO_CFG)
    #define Button4_SIO_DIFF           (* (reg8 *) Button4__SIO_DIFF)
#endif /* (Button4__SIO_CFG) */

/* Interrupt Registers */
#if defined(Button4__INTSTAT)
    #define Button4_INTSTAT            (* (reg8 *) Button4__INTSTAT)
    #define Button4_SNAP               (* (reg8 *) Button4__SNAP)
    
	#define Button4_0_INTTYPE_REG 		(* (reg8 *) Button4__0__INTTYPE)
#endif /* (Button4__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Button4_H */


/* [] END OF FILE */
