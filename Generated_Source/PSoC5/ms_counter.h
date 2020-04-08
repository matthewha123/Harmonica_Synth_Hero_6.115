/*******************************************************************************
* File Name: ms_counter.h  
* Version 3.0
*
*  Description:
*   Contains the function prototypes and constants available to the counter
*   user module.
*
*   Note:
*    None
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
    
#if !defined(CY_COUNTER_ms_counter_H)
#define CY_COUNTER_ms_counter_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 ms_counter_initVar;

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component Counter_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Error message for removed ms_counter_CounterUDB_sCTRLReg_ctrlreg through optimization */
#ifdef ms_counter_CounterUDB_sCTRLReg_ctrlreg__REMOVED
    #error Counter_v3_0 detected with a constant 0 for the enable, a \
                                constant 0 for the count or constant 1 for \
                                the reset. This will prevent the component from\
                                operating.
#endif /* ms_counter_CounterUDB_sCTRLReg_ctrlreg__REMOVED */


/**************************************
*           Parameter Defaults        
**************************************/

#define ms_counter_Resolution            32u
#define ms_counter_UsingFixedFunction    0u
#define ms_counter_ControlRegRemoved     0u
#define ms_counter_COMPARE_MODE_SOFTWARE 0u
#define ms_counter_CAPTURE_MODE_SOFTWARE 0u
#define ms_counter_RunModeUsed           0u


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Mode API Support
 * Backup structure for Sleep Wake up operations
 *************************************************************************/
typedef struct
{
    uint8 CounterEnableState; 
    uint32 CounterUdb;         /* Current Counter Value */

    #if (!ms_counter_ControlRegRemoved)
        uint8 CounterControlRegister;               /* Counter Control Register */
    #endif /* (!ms_counter_ControlRegRemoved) */

}ms_counter_backupStruct;


/**************************************
 *  Function Prototypes
 *************************************/
void    ms_counter_Start(void) ;
void    ms_counter_Stop(void) ;
void    ms_counter_SetInterruptMode(uint8 interruptsMask) ;
uint8   ms_counter_ReadStatusRegister(void) ;
#define ms_counter_GetInterruptSource() ms_counter_ReadStatusRegister()
#if(!ms_counter_ControlRegRemoved)
    uint8   ms_counter_ReadControlRegister(void) ;
    void    ms_counter_WriteControlRegister(uint8 control) \
        ;
#endif /* (!ms_counter_ControlRegRemoved) */
#if (!(ms_counter_UsingFixedFunction && (CY_PSOC5A)))
    void    ms_counter_WriteCounter(uint32 counter) \
            ; 
#endif /* (!(ms_counter_UsingFixedFunction && (CY_PSOC5A))) */
uint32  ms_counter_ReadCounter(void) ;
uint32  ms_counter_ReadCapture(void) ;
void    ms_counter_WritePeriod(uint32 period) \
    ;
uint32  ms_counter_ReadPeriod( void ) ;
#if (!ms_counter_UsingFixedFunction)
    void    ms_counter_WriteCompare(uint32 compare) \
        ;
    uint32  ms_counter_ReadCompare( void ) \
        ;
#endif /* (!ms_counter_UsingFixedFunction) */

#if (ms_counter_COMPARE_MODE_SOFTWARE)
    void    ms_counter_SetCompareMode(uint8 compareMode) ;
#endif /* (ms_counter_COMPARE_MODE_SOFTWARE) */
#if (ms_counter_CAPTURE_MODE_SOFTWARE)
    void    ms_counter_SetCaptureMode(uint8 captureMode) ;
#endif /* (ms_counter_CAPTURE_MODE_SOFTWARE) */
void ms_counter_ClearFIFO(void)     ;
void ms_counter_Init(void)          ;
void ms_counter_Enable(void)        ;
void ms_counter_SaveConfig(void)    ;
void ms_counter_RestoreConfig(void) ;
void ms_counter_Sleep(void)         ;
void ms_counter_Wakeup(void)        ;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Counter__CompareModes, Used in Compare Mode retained for backward compatibility of tests*/
#define ms_counter__B_COUNTER__LESS_THAN 1
#define ms_counter__B_COUNTER__LESS_THAN_OR_EQUAL 2
#define ms_counter__B_COUNTER__EQUAL 0
#define ms_counter__B_COUNTER__GREATER_THAN 3
#define ms_counter__B_COUNTER__GREATER_THAN_OR_EQUAL 4
#define ms_counter__B_COUNTER__SOFTWARE 5

/* Enumerated Type Counter_CompareModes */
#define ms_counter_CMP_MODE_LT 1u
#define ms_counter_CMP_MODE_LTE 2u
#define ms_counter_CMP_MODE_EQ 0u
#define ms_counter_CMP_MODE_GT 3u
#define ms_counter_CMP_MODE_GTE 4u
#define ms_counter_CMP_MODE_SOFTWARE_CONTROLLED 5u

/* Enumerated Type B_Counter__CaptureModes, Used in Capture Mode retained for backward compatibility of tests*/
#define ms_counter__B_COUNTER__NONE 0
#define ms_counter__B_COUNTER__RISING_EDGE 1
#define ms_counter__B_COUNTER__FALLING_EDGE 2
#define ms_counter__B_COUNTER__EITHER_EDGE 3
#define ms_counter__B_COUNTER__SOFTWARE_CONTROL 4

/* Enumerated Type Counter_CompareModes */
#define ms_counter_CAP_MODE_NONE 0u
#define ms_counter_CAP_MODE_RISE 1u
#define ms_counter_CAP_MODE_FALL 2u
#define ms_counter_CAP_MODE_BOTH 3u
#define ms_counter_CAP_MODE_SOFTWARE_CONTROLLED 4u


/***************************************
 *  Initialization Values
 **************************************/
#define ms_counter_CAPTURE_MODE_CONF       0u
#define ms_counter_INIT_PERIOD_VALUE       4294967294u
#define ms_counter_INIT_COUNTER_VALUE      0u
#if (ms_counter_UsingFixedFunction)
#define ms_counter_INIT_INTERRUPTS_MASK    ((uint8)((uint8)0u << ms_counter_STATUS_ZERO_INT_EN_MASK_SHIFT))
#else
#define ms_counter_INIT_COMPARE_VALUE      128u
#define ms_counter_INIT_INTERRUPTS_MASK ((uint8)((uint8)0u << ms_counter_STATUS_ZERO_INT_EN_MASK_SHIFT) | \
        ((uint8)((uint8)0u << ms_counter_STATUS_CAPTURE_INT_EN_MASK_SHIFT)) | \
        ((uint8)((uint8)0u << ms_counter_STATUS_CMP_INT_EN_MASK_SHIFT)) | \
        ((uint8)((uint8)0u << ms_counter_STATUS_OVERFLOW_INT_EN_MASK_SHIFT)) | \
        ((uint8)((uint8)0u << ms_counter_STATUS_UNDERFLOW_INT_EN_MASK_SHIFT)))
#define ms_counter_DEFAULT_COMPARE_MODE    0u

#if( 0 != ms_counter_CAPTURE_MODE_CONF)
    #define ms_counter_DEFAULT_CAPTURE_MODE    ((uint8)((uint8)0u << ms_counter_CTRL_CAPMODE0_SHIFT))
#else    
    #define ms_counter_DEFAULT_CAPTURE_MODE    (0u )
#endif /* ( 0 != ms_counter_CAPTURE_MODE_CONF) */

#endif /* (ms_counter_UsingFixedFunction) */


/**************************************
 *  Registers
 *************************************/
#if (ms_counter_UsingFixedFunction)
    #define ms_counter_STATICCOUNT_LSB     (*(reg16 *) ms_counter_CounterHW__CAP0 )
    #define ms_counter_STATICCOUNT_LSB_PTR ( (reg16 *) ms_counter_CounterHW__CAP0 )
    #define ms_counter_PERIOD_LSB          (*(reg16 *) ms_counter_CounterHW__PER0 )
    #define ms_counter_PERIOD_LSB_PTR      ( (reg16 *) ms_counter_CounterHW__PER0 )
    /* MODE must be set to 1 to set the compare value */
    #define ms_counter_COMPARE_LSB         (*(reg16 *) ms_counter_CounterHW__CNT_CMP0 )
    #define ms_counter_COMPARE_LSB_PTR     ( (reg16 *) ms_counter_CounterHW__CNT_CMP0 )
    /* MODE must be set to 0 to get the count */
    #define ms_counter_COUNTER_LSB         (*(reg16 *) ms_counter_CounterHW__CNT_CMP0 )
    #define ms_counter_COUNTER_LSB_PTR     ( (reg16 *) ms_counter_CounterHW__CNT_CMP0 )
    #define ms_counter_RT1                 (*(reg8 *) ms_counter_CounterHW__RT1)
    #define ms_counter_RT1_PTR             ( (reg8 *) ms_counter_CounterHW__RT1)
#else
    
    #if (ms_counter_Resolution <= 8u) /* 8-bit Counter */
    
        #define ms_counter_STATICCOUNT_LSB     (*(reg8 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__F0_REG )
        #define ms_counter_STATICCOUNT_LSB_PTR ( (reg8 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__F0_REG )
        #define ms_counter_PERIOD_LSB          (*(reg8 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__D0_REG )
        #define ms_counter_PERIOD_LSB_PTR      ( (reg8 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__D0_REG )
        #define ms_counter_COMPARE_LSB         (*(reg8 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__D1_REG )
        #define ms_counter_COMPARE_LSB_PTR     ( (reg8 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__D1_REG )
        #define ms_counter_COUNTER_LSB         (*(reg8 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__A0_REG )  
        #define ms_counter_COUNTER_LSB_PTR     ( (reg8 *)\
            ms_counter_CounterUDB_sC32_counterdp_u0__A0_REG )
    
    #elif(ms_counter_Resolution <= 16u) /* 16-bit Counter */
        #if(CY_PSOC3) /* 8-bit address space */ 
            #define ms_counter_STATICCOUNT_LSB     (*(reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__F0_REG )
            #define ms_counter_STATICCOUNT_LSB_PTR ( (reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__F0_REG )
            #define ms_counter_PERIOD_LSB          (*(reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__D0_REG )
            #define ms_counter_PERIOD_LSB_PTR      ( (reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__D0_REG )
            #define ms_counter_COMPARE_LSB         (*(reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__D1_REG )
            #define ms_counter_COMPARE_LSB_PTR     ( (reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__D1_REG )
            #define ms_counter_COUNTER_LSB         (*(reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__A0_REG )  
            #define ms_counter_COUNTER_LSB_PTR     ( (reg16 *)\
                ms_counter_CounterUDB_sC32_counterdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define ms_counter_STATICCOUNT_LSB     (*(reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__16BIT_F0_REG )
            #define ms_counter_STATICCOUNT_LSB_PTR ( (reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__16BIT_F0_REG )
            #define ms_counter_PERIOD_LSB          (*(reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__16BIT_D0_REG )
            #define ms_counter_PERIOD_LSB_PTR      ( (reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__16BIT_D0_REG )
            #define ms_counter_COMPARE_LSB         (*(reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__16BIT_D1_REG )
            #define ms_counter_COMPARE_LSB_PTR     ( (reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__16BIT_D1_REG )
            #define ms_counter_COUNTER_LSB         (*(reg16 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__16BIT_A0_REG )  
            #define ms_counter_COUNTER_LSB_PTR     ( (reg16 *)\
                ms_counter_CounterUDB_sC32_counterdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */   
    #elif(ms_counter_Resolution <= 24u) /* 24-bit Counter */
        
        #define ms_counter_STATICCOUNT_LSB     (*(reg32 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__F0_REG )
        #define ms_counter_STATICCOUNT_LSB_PTR ( (reg32 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__F0_REG )
        #define ms_counter_PERIOD_LSB          (*(reg32 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__D0_REG )
        #define ms_counter_PERIOD_LSB_PTR      ( (reg32 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__D0_REG )
        #define ms_counter_COMPARE_LSB         (*(reg32 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__D1_REG )
        #define ms_counter_COMPARE_LSB_PTR     ( (reg32 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__D1_REG )
        #define ms_counter_COUNTER_LSB         (*(reg32 *) \
            ms_counter_CounterUDB_sC32_counterdp_u0__A0_REG )  
        #define ms_counter_COUNTER_LSB_PTR     ( (reg32 *)\
            ms_counter_CounterUDB_sC32_counterdp_u0__A0_REG )
    
    #else /* 32-bit Counter */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define ms_counter_STATICCOUNT_LSB     (*(reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__F0_REG )
            #define ms_counter_STATICCOUNT_LSB_PTR ( (reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__F0_REG )
            #define ms_counter_PERIOD_LSB          (*(reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__D0_REG )
            #define ms_counter_PERIOD_LSB_PTR      ( (reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__D0_REG )
            #define ms_counter_COMPARE_LSB         (*(reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__D1_REG )
            #define ms_counter_COMPARE_LSB_PTR     ( (reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__D1_REG )
            #define ms_counter_COUNTER_LSB         (*(reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__A0_REG )  
            #define ms_counter_COUNTER_LSB_PTR     ( (reg32 *)\
                ms_counter_CounterUDB_sC32_counterdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define ms_counter_STATICCOUNT_LSB     (*(reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__32BIT_F0_REG )
            #define ms_counter_STATICCOUNT_LSB_PTR ( (reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__32BIT_F0_REG )
            #define ms_counter_PERIOD_LSB          (*(reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__32BIT_D0_REG )
            #define ms_counter_PERIOD_LSB_PTR      ( (reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__32BIT_D0_REG )
            #define ms_counter_COMPARE_LSB         (*(reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__32BIT_D1_REG )
            #define ms_counter_COMPARE_LSB_PTR     ( (reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__32BIT_D1_REG )
            #define ms_counter_COUNTER_LSB         (*(reg32 *) \
                ms_counter_CounterUDB_sC32_counterdp_u0__32BIT_A0_REG )  
            #define ms_counter_COUNTER_LSB_PTR     ( (reg32 *)\
                ms_counter_CounterUDB_sC32_counterdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */   
    #endif

	#define ms_counter_COUNTER_LSB_PTR_8BIT     ( (reg8 *)\
                ms_counter_CounterUDB_sC32_counterdp_u0__A0_REG )
				
    #define ms_counter_AUX_CONTROLDP0 \
        (*(reg8 *) ms_counter_CounterUDB_sC32_counterdp_u0__DP_AUX_CTL_REG)
    
    #define ms_counter_AUX_CONTROLDP0_PTR \
        ( (reg8 *) ms_counter_CounterUDB_sC32_counterdp_u0__DP_AUX_CTL_REG)
    
    #if (ms_counter_Resolution == 16 || ms_counter_Resolution == 24 || ms_counter_Resolution == 32)
       #define ms_counter_AUX_CONTROLDP1 \
           (*(reg8 *) ms_counter_CounterUDB_sC32_counterdp_u1__DP_AUX_CTL_REG)
       #define ms_counter_AUX_CONTROLDP1_PTR \
           ( (reg8 *) ms_counter_CounterUDB_sC32_counterdp_u1__DP_AUX_CTL_REG)
    #endif /* (ms_counter_Resolution == 16 || ms_counter_Resolution == 24 || ms_counter_Resolution == 32) */
    
    #if (ms_counter_Resolution == 24 || ms_counter_Resolution == 32)
       #define ms_counter_AUX_CONTROLDP2 \
           (*(reg8 *) ms_counter_CounterUDB_sC32_counterdp_u2__DP_AUX_CTL_REG)
       #define ms_counter_AUX_CONTROLDP2_PTR \
           ( (reg8 *) ms_counter_CounterUDB_sC32_counterdp_u2__DP_AUX_CTL_REG)
    #endif /* (ms_counter_Resolution == 24 || ms_counter_Resolution == 32) */
    
    #if (ms_counter_Resolution == 32)
       #define ms_counter_AUX_CONTROLDP3 \
           (*(reg8 *) ms_counter_CounterUDB_sC32_counterdp_u3__DP_AUX_CTL_REG)
       #define ms_counter_AUX_CONTROLDP3_PTR \
           ( (reg8 *) ms_counter_CounterUDB_sC32_counterdp_u3__DP_AUX_CTL_REG)
    #endif /* (ms_counter_Resolution == 32) */

#endif  /* (ms_counter_UsingFixedFunction) */

#if (ms_counter_UsingFixedFunction)
    #define ms_counter_STATUS         (*(reg8 *) ms_counter_CounterHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define ms_counter_STATUS_MASK             (*(reg8 *) ms_counter_CounterHW__SR0 )
    #define ms_counter_STATUS_MASK_PTR         ( (reg8 *) ms_counter_CounterHW__SR0 )
    #define ms_counter_CONTROL                 (*(reg8 *) ms_counter_CounterHW__CFG0)
    #define ms_counter_CONTROL_PTR             ( (reg8 *) ms_counter_CounterHW__CFG0)
    #define ms_counter_CONTROL2                (*(reg8 *) ms_counter_CounterHW__CFG1)
    #define ms_counter_CONTROL2_PTR            ( (reg8 *) ms_counter_CounterHW__CFG1)
    #if (CY_PSOC3 || CY_PSOC5LP)
        #define ms_counter_CONTROL3       (*(reg8 *) ms_counter_CounterHW__CFG2)
        #define ms_counter_CONTROL3_PTR   ( (reg8 *) ms_counter_CounterHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define ms_counter_GLOBAL_ENABLE           (*(reg8 *) ms_counter_CounterHW__PM_ACT_CFG)
    #define ms_counter_GLOBAL_ENABLE_PTR       ( (reg8 *) ms_counter_CounterHW__PM_ACT_CFG)
    #define ms_counter_GLOBAL_STBY_ENABLE      (*(reg8 *) ms_counter_CounterHW__PM_STBY_CFG)
    #define ms_counter_GLOBAL_STBY_ENABLE_PTR  ( (reg8 *) ms_counter_CounterHW__PM_STBY_CFG)
    

    /********************************
    *    Constants
    ********************************/

    /* Fixed Function Block Chosen */
    #define ms_counter_BLOCK_EN_MASK          ms_counter_CounterHW__PM_ACT_MSK
    #define ms_counter_BLOCK_STBY_EN_MASK     ms_counter_CounterHW__PM_STBY_MSK 
    
    /* Control Register Bit Locations */    
    /* As defined in Register Map, part of TMRX_CFG0 register */
    #define ms_counter_CTRL_ENABLE_SHIFT      0x00u
    #define ms_counter_ONESHOT_SHIFT          0x02u
    /* Control Register Bit Masks */
    #define ms_counter_CTRL_ENABLE            ((uint8)((uint8)0x01u << ms_counter_CTRL_ENABLE_SHIFT))         
    #define ms_counter_ONESHOT                ((uint8)((uint8)0x01u << ms_counter_ONESHOT_SHIFT))

    /* Control2 Register Bit Masks */
    /* Set the mask for run mode */
    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        #define ms_counter_CTRL_MODE_SHIFT        0x01u    
        #define ms_counter_CTRL_MODE_MASK         ((uint8)((uint8)0x07u << ms_counter_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Use CFG2 Mode bits to set run mode */
        #define ms_counter_CTRL_MODE_SHIFT        0x00u    
        #define ms_counter_CTRL_MODE_MASK         ((uint8)((uint8)0x03u << ms_counter_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    /* Set the mask for interrupt (raw/status register) */
    #define ms_counter_CTRL2_IRQ_SEL_SHIFT     0x00u
    #define ms_counter_CTRL2_IRQ_SEL          ((uint8)((uint8)0x01u << ms_counter_CTRL2_IRQ_SEL_SHIFT))     
    
    /* Status Register Bit Locations */
    #define ms_counter_STATUS_ZERO_SHIFT      0x07u  /* As defined in Register Map, part of TMRX_SR0 register */ 

    /* Status Register Interrupt Enable Bit Locations */
    #define ms_counter_STATUS_ZERO_INT_EN_MASK_SHIFT      (ms_counter_STATUS_ZERO_SHIFT - 0x04u)

    /* Status Register Bit Masks */                           
    #define ms_counter_STATUS_ZERO            ((uint8)((uint8)0x01u << ms_counter_STATUS_ZERO_SHIFT))

    /* Status Register Interrupt Bit Masks*/
    #define ms_counter_STATUS_ZERO_INT_EN_MASK       ((uint8)((uint8)ms_counter_STATUS_ZERO >> 0x04u))
    
    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define ms_counter_RT1_SHIFT            0x04u
    #define ms_counter_RT1_MASK             ((uint8)((uint8)0x03u << ms_counter_RT1_SHIFT))  /* Sync TC and CMP bit masks */
    #define ms_counter_SYNC                 ((uint8)((uint8)0x03u << ms_counter_RT1_SHIFT))
    #define ms_counter_SYNCDSI_SHIFT        0x00u
    #define ms_counter_SYNCDSI_MASK         ((uint8)((uint8)0x0Fu << ms_counter_SYNCDSI_SHIFT)) /* Sync all DSI inputs */
    #define ms_counter_SYNCDSI_EN           ((uint8)((uint8)0x0Fu << ms_counter_SYNCDSI_SHIFT)) /* Sync all DSI inputs */
    
#else /* !ms_counter_UsingFixedFunction */
    #define ms_counter_STATUS               (* (reg8 *) ms_counter_CounterUDB_sSTSReg_stsreg__STATUS_REG )
    #define ms_counter_STATUS_PTR           (  (reg8 *) ms_counter_CounterUDB_sSTSReg_stsreg__STATUS_REG )
    #define ms_counter_STATUS_MASK          (* (reg8 *) ms_counter_CounterUDB_sSTSReg_stsreg__MASK_REG )
    #define ms_counter_STATUS_MASK_PTR      (  (reg8 *) ms_counter_CounterUDB_sSTSReg_stsreg__MASK_REG )
    #define ms_counter_STATUS_AUX_CTRL      (*(reg8 *) ms_counter_CounterUDB_sSTSReg_stsreg__STATUS_AUX_CTL_REG)
    #define ms_counter_STATUS_AUX_CTRL_PTR  ( (reg8 *) ms_counter_CounterUDB_sSTSReg_stsreg__STATUS_AUX_CTL_REG)
    #define ms_counter_CONTROL              (* (reg8 *) ms_counter_CounterUDB_sCTRLReg_ctrlreg__CONTROL_REG )
    #define ms_counter_CONTROL_PTR          (  (reg8 *) ms_counter_CounterUDB_sCTRLReg_ctrlreg__CONTROL_REG )


    /********************************
    *    Constants
    ********************************/
    /* Control Register Bit Locations */
    #define ms_counter_CTRL_CAPMODE0_SHIFT    0x03u       /* As defined by Verilog Implementation */
    #define ms_counter_CTRL_RESET_SHIFT       0x06u       /* As defined by Verilog Implementation */
    #define ms_counter_CTRL_ENABLE_SHIFT      0x07u       /* As defined by Verilog Implementation */
    /* Control Register Bit Masks */
    #define ms_counter_CTRL_CMPMODE_MASK      0x07u 
    #define ms_counter_CTRL_CAPMODE_MASK      0x03u  
    #define ms_counter_CTRL_RESET             ((uint8)((uint8)0x01u << ms_counter_CTRL_RESET_SHIFT))  
    #define ms_counter_CTRL_ENABLE            ((uint8)((uint8)0x01u << ms_counter_CTRL_ENABLE_SHIFT)) 

    /* Status Register Bit Locations */
    #define ms_counter_STATUS_CMP_SHIFT       0x00u       /* As defined by Verilog Implementation */
    #define ms_counter_STATUS_ZERO_SHIFT      0x01u       /* As defined by Verilog Implementation */
    #define ms_counter_STATUS_OVERFLOW_SHIFT  0x02u       /* As defined by Verilog Implementation */
    #define ms_counter_STATUS_UNDERFLOW_SHIFT 0x03u       /* As defined by Verilog Implementation */
    #define ms_counter_STATUS_CAPTURE_SHIFT   0x04u       /* As defined by Verilog Implementation */
    #define ms_counter_STATUS_FIFOFULL_SHIFT  0x05u       /* As defined by Verilog Implementation */
    #define ms_counter_STATUS_FIFONEMP_SHIFT  0x06u       /* As defined by Verilog Implementation */
    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define ms_counter_STATUS_CMP_INT_EN_MASK_SHIFT       ms_counter_STATUS_CMP_SHIFT       
    #define ms_counter_STATUS_ZERO_INT_EN_MASK_SHIFT      ms_counter_STATUS_ZERO_SHIFT      
    #define ms_counter_STATUS_OVERFLOW_INT_EN_MASK_SHIFT  ms_counter_STATUS_OVERFLOW_SHIFT  
    #define ms_counter_STATUS_UNDERFLOW_INT_EN_MASK_SHIFT ms_counter_STATUS_UNDERFLOW_SHIFT 
    #define ms_counter_STATUS_CAPTURE_INT_EN_MASK_SHIFT   ms_counter_STATUS_CAPTURE_SHIFT   
    #define ms_counter_STATUS_FIFOFULL_INT_EN_MASK_SHIFT  ms_counter_STATUS_FIFOFULL_SHIFT  
    #define ms_counter_STATUS_FIFONEMP_INT_EN_MASK_SHIFT  ms_counter_STATUS_FIFONEMP_SHIFT  
    /* Status Register Bit Masks */                
    #define ms_counter_STATUS_CMP             ((uint8)((uint8)0x01u << ms_counter_STATUS_CMP_SHIFT))  
    #define ms_counter_STATUS_ZERO            ((uint8)((uint8)0x01u << ms_counter_STATUS_ZERO_SHIFT)) 
    #define ms_counter_STATUS_OVERFLOW        ((uint8)((uint8)0x01u << ms_counter_STATUS_OVERFLOW_SHIFT)) 
    #define ms_counter_STATUS_UNDERFLOW       ((uint8)((uint8)0x01u << ms_counter_STATUS_UNDERFLOW_SHIFT)) 
    #define ms_counter_STATUS_CAPTURE         ((uint8)((uint8)0x01u << ms_counter_STATUS_CAPTURE_SHIFT)) 
    #define ms_counter_STATUS_FIFOFULL        ((uint8)((uint8)0x01u << ms_counter_STATUS_FIFOFULL_SHIFT))
    #define ms_counter_STATUS_FIFONEMP        ((uint8)((uint8)0x01u << ms_counter_STATUS_FIFONEMP_SHIFT))
    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define ms_counter_STATUS_CMP_INT_EN_MASK            ms_counter_STATUS_CMP                    
    #define ms_counter_STATUS_ZERO_INT_EN_MASK           ms_counter_STATUS_ZERO            
    #define ms_counter_STATUS_OVERFLOW_INT_EN_MASK       ms_counter_STATUS_OVERFLOW        
    #define ms_counter_STATUS_UNDERFLOW_INT_EN_MASK      ms_counter_STATUS_UNDERFLOW       
    #define ms_counter_STATUS_CAPTURE_INT_EN_MASK        ms_counter_STATUS_CAPTURE         
    #define ms_counter_STATUS_FIFOFULL_INT_EN_MASK       ms_counter_STATUS_FIFOFULL        
    #define ms_counter_STATUS_FIFONEMP_INT_EN_MASK       ms_counter_STATUS_FIFONEMP         
    

    /* StatusI Interrupt Enable bit Location in the Auxilliary Control Register */
    #define ms_counter_STATUS_ACTL_INT_EN     0x10u /* As defined for the ACTL Register */
    
    /* Datapath Auxillary Control Register definitions */
    #define ms_counter_AUX_CTRL_FIFO0_CLR         0x01u   /* As defined by Register map */
    #define ms_counter_AUX_CTRL_FIFO1_CLR         0x02u   /* As defined by Register map */
    #define ms_counter_AUX_CTRL_FIFO0_LVL         0x04u   /* As defined by Register map */
    #define ms_counter_AUX_CTRL_FIFO1_LVL         0x08u   /* As defined by Register map */
    #define ms_counter_STATUS_ACTL_INT_EN_MASK    0x10u   /* As defined for the ACTL Register */
    
#endif /* ms_counter_UsingFixedFunction */

#endif  /* CY_COUNTER_ms_counter_H */


/* [] END OF FILE */

