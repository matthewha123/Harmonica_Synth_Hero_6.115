/*******************************************************************************
* File Name: Button3.h  
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

#if !defined(CY_PINS_Button3_H) /* Pins Button3_H */
#define CY_PINS_Button3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Button3_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Button3__PORT == 15 && ((Button3__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Button3_Write(uint8 value);
void    Button3_SetDriveMode(uint8 mode);
uint8   Button3_ReadDataReg(void);
uint8   Button3_Read(void);
void    Button3_SetInterruptMode(uint16 position, uint16 mode);
uint8   Button3_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Button3_SetDriveMode() function.
     *  @{
     */
        #define Button3_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Button3_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Button3_DM_RES_UP          PIN_DM_RES_UP
        #define Button3_DM_RES_DWN         PIN_DM_RES_DWN
        #define Button3_DM_OD_LO           PIN_DM_OD_LO
        #define Button3_DM_OD_HI           PIN_DM_OD_HI
        #define Button3_DM_STRONG          PIN_DM_STRONG
        #define Button3_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Button3_MASK               Button3__MASK
#define Button3_SHIFT              Button3__SHIFT
#define Button3_WIDTH              1u

/* Interrupt constants */
#if defined(Button3__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Button3_SetInterruptMode() function.
     *  @{
     */
        #define Button3_INTR_NONE      (uint16)(0x0000u)
        #define Button3_INTR_RISING    (uint16)(0x0001u)
        #define Button3_INTR_FALLING   (uint16)(0x0002u)
        #define Button3_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Button3_INTR_MASK      (0x01u) 
#endif /* (Button3__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Button3_PS                     (* (reg8 *) Button3__PS)
/* Data Register */
#define Button3_DR                     (* (reg8 *) Button3__DR)
/* Port Number */
#define Button3_PRT_NUM                (* (reg8 *) Button3__PRT) 
/* Connect to Analog Globals */                                                  
#define Button3_AG                     (* (reg8 *) Button3__AG)                       
/* Analog MUX bux enable */
#define Button3_AMUX                   (* (reg8 *) Button3__AMUX) 
/* Bidirectional Enable */                                                        
#define Button3_BIE                    (* (reg8 *) Button3__BIE)
/* Bit-mask for Aliased Register Access */
#define Button3_BIT_MASK               (* (reg8 *) Button3__BIT_MASK)
/* Bypass Enable */
#define Button3_BYP                    (* (reg8 *) Button3__BYP)
/* Port wide control signals */                                                   
#define Button3_CTL                    (* (reg8 *) Button3__CTL)
/* Drive Modes */
#define Button3_DM0                    (* (reg8 *) Button3__DM0) 
#define Button3_DM1                    (* (reg8 *) Button3__DM1)
#define Button3_DM2                    (* (reg8 *) Button3__DM2) 
/* Input Buffer Disable Override */
#define Button3_INP_DIS                (* (reg8 *) Button3__INP_DIS)
/* LCD Common or Segment Drive */
#define Button3_LCD_COM_SEG            (* (reg8 *) Button3__LCD_COM_SEG)
/* Enable Segment LCD */
#define Button3_LCD_EN                 (* (reg8 *) Button3__LCD_EN)
/* Slew Rate Control */
#define Button3_SLW                    (* (reg8 *) Button3__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Button3_PRTDSI__CAPS_SEL       (* (reg8 *) Button3__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Button3_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Button3__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Button3_PRTDSI__OE_SEL0        (* (reg8 *) Button3__PRTDSI__OE_SEL0) 
#define Button3_PRTDSI__OE_SEL1        (* (reg8 *) Button3__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Button3_PRTDSI__OUT_SEL0       (* (reg8 *) Button3__PRTDSI__OUT_SEL0) 
#define Button3_PRTDSI__OUT_SEL1       (* (reg8 *) Button3__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Button3_PRTDSI__SYNC_OUT       (* (reg8 *) Button3__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Button3__SIO_CFG)
    #define Button3_SIO_HYST_EN        (* (reg8 *) Button3__SIO_HYST_EN)
    #define Button3_SIO_REG_HIFREQ     (* (reg8 *) Button3__SIO_REG_HIFREQ)
    #define Button3_SIO_CFG            (* (reg8 *) Button3__SIO_CFG)
    #define Button3_SIO_DIFF           (* (reg8 *) Button3__SIO_DIFF)
#endif /* (Button3__SIO_CFG) */

/* Interrupt Registers */
#if defined(Button3__INTSTAT)
    #define Button3_INTSTAT            (* (reg8 *) Button3__INTSTAT)
    #define Button3_SNAP               (* (reg8 *) Button3__SNAP)
    
	#define Button3_0_INTTYPE_REG 		(* (reg8 *) Button3__0__INTTYPE)
#endif /* (Button3__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Button3_H */


/* [] END OF FILE */
