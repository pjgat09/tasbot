/*******************************************************************************
* File Name: P2_3_PM.c
* Version 2.30
*
* Description:
*  This file provides the API source code for sleep mode support for Shift
*  Register component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "P2_3.h"

static P2_3_BACKUP_STRUCT P2_3_backup =
{
    P2_3_DISABLED,

    ((uint16) P2_3_DEFAULT_A0),
    ((uint16) P2_3_DEFAULT_A1),

    #if(CY_UDB_V0)
        ((uint16) P2_3_INT_SRC),
    #endif /* (CY_UDB_V0) */
};


/*******************************************************************************
* Function Name: P2_3_SaveConfig
********************************************************************************
*
* Summary:
*  Saves Shift Register configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void P2_3_SaveConfig(void) 
{
    /* Store working registers A0 and A1 */
    P2_3_backup.saveSrA0Reg   = CY_GET_REG16(P2_3_SHIFT_REG_LSB_PTR);
    P2_3_backup.saveSrA1Reg   = CY_GET_REG16(P2_3_SHIFT_REG_VALUE_LSB_PTR);

    #if(CY_UDB_V0)
        P2_3_backup.saveSrIntMask = P2_3_SR_STATUS_MASK;
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: P2_3_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores Shift Register configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void P2_3_RestoreConfig(void) 
{
    /* Restore working registers A0 and A1 */
    CY_SET_REG16(P2_3_SHIFT_REG_LSB_PTR, P2_3_backup.saveSrA0Reg);
    CY_SET_REG16(P2_3_SHIFT_REG_VALUE_LSB_PTR, P2_3_backup.saveSrA1Reg);

    #if(CY_UDB_V0)
        P2_3_SR_STATUS_MASK = ((uint8) P2_3_backup.saveSrIntMask);
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: P2_3_Sleep
********************************************************************************
*
* Summary:
*  Prepare the component to enter a Sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void P2_3_Sleep(void) 
{
    P2_3_backup.enableState = ((uint8) P2_3_IS_ENABLED);

    P2_3_Stop();
    P2_3_SaveConfig();
}


/*******************************************************************************
* Function Name: P2_3_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void P2_3_Wakeup(void) 
{
    P2_3_RestoreConfig();

    if(0u != P2_3_backup.enableState)
    {
        P2_3_Enable();
    }
}


/* [] END OF FILE */
