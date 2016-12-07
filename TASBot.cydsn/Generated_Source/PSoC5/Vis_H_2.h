/*******************************************************************************
* File Name: Vis_H_2.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_Vis_H_2_H) /* CY_CONTROL_REG_Vis_H_2_H */
#define CY_CONTROL_REG_Vis_H_2_H

#include "cytypes.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} Vis_H_2_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    Vis_H_2_Write(uint8 control) ;
uint8   Vis_H_2_Read(void) ;

void Vis_H_2_SaveConfig(void) ;
void Vis_H_2_RestoreConfig(void) ;
void Vis_H_2_Sleep(void) ; 
void Vis_H_2_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define Vis_H_2_Control        (* (reg8 *) Vis_H_2_Sync_ctrl_reg__CONTROL_REG )
#define Vis_H_2_Control_PTR    (  (reg8 *) Vis_H_2_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_Vis_H_2_H */


/* [] END OF FILE */
