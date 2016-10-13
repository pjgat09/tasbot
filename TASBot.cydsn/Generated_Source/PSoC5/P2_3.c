/*******************************************************************************
* File Name: P2_3.c
* Version 2.30
*
* Description:
*  This file provides the API source code for the Shift Register component.
*
* Note: none
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "P2_3.h"

uint8 P2_3_initVar = 0u;


/*******************************************************************************
* Function Name: P2_3_Start
********************************************************************************
*
* Summary:
*  Starts the Shift Register.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  P2_3_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void P2_3_Start(void) 
{
    if(0u == P2_3_initVar)
    {
        P2_3_Init();
        P2_3_initVar = 1u; /* Component initialized */
    }

    P2_3_Enable();
}


/*******************************************************************************
* Function Name: P2_3_Enable
********************************************************************************
*
* Summary:
*  Enables the Shift Register.
*
* Parameters:
*  void.
*
* Return:
*  void.
*
*******************************************************************************/
void P2_3_Enable(void) 
{
    /* Changing address in Datapath Control Store
       from NOP to component state machine commands space */
    P2_3_SR_CONTROL |= P2_3_CLK_EN;

    P2_3_EnableInt();
}


/*******************************************************************************
* Function Name: P2_3_Init
********************************************************************************
*
* Summary:
*  Initializes Tx and/or Rx interrupt sources with initial values.
*
* Parameters:
*  void.
*
* Return:
*  void.
*
*******************************************************************************/
void P2_3_Init(void) 
{
    P2_3_SetIntMode(P2_3_INT_SRC);
}


/*******************************************************************************
* Function Name: P2_3_Stop
********************************************************************************
*
* Summary:
*  Disables the Shift Register
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void P2_3_Stop(void) 
{
    /*changing Datapath Control Store address to NOP space*/
    P2_3_SR_CONTROL &= ((uint8) ~P2_3_CLK_EN);
    P2_3_DisableInt();
}


/*******************************************************************************
* Function Name: P2_3_EnableInt
********************************************************************************
*
* Summary:
*  Enables the Shift Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void P2_3_EnableInt(void) 
{
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();
    P2_3_SR_AUX_CONTROL |= P2_3_INTERRUPTS_ENABLE;
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: P2_3_DisableInt
********************************************************************************
*
* Summary:
*  Disables the Shift Register interrupt.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void P2_3_DisableInt(void) 
{
    uint8 interruptState;

    interruptState = CyEnterCriticalSection();
    P2_3_SR_AUX_CONTROL &= ((uint8) ~P2_3_INTERRUPTS_ENABLE);
    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: P2_3_GetFIFOStatus
********************************************************************************
*
* Summary:
*  Returns current status of input or output FIFO.
*
* Parameters:
*  fifoId.
*
* Return:
*  FIFO status.
*
*******************************************************************************/
uint8 P2_3_GetFIFOStatus(uint8 fifoId) 
{
    uint8 result;

    result = P2_3_RET_FIFO_NOT_DEFINED;

    #if(0u != P2_3_USE_INPUT_FIFO)
        if(P2_3_IN_FIFO == fifoId)
        {
            switch(P2_3_GET_IN_FIFO_STS)
            {
                case P2_3_IN_FIFO_FULL :
                    result = P2_3_RET_FIFO_FULL;
                    break;

                case P2_3_IN_FIFO_EMPTY :
                    result = P2_3_RET_FIFO_EMPTY;
                    break;

                case P2_3_IN_FIFO_PARTIAL:
                    result = P2_3_RET_FIFO_PARTIAL;
                    break;
                    
                default:
                    /* Initial result value, while 
                       IN_FIFO_EMPTY case is false 
                     */
                    result = P2_3_RET_FIFO_EMPTY;
                    break;
            }   
        }
    #endif /* (0u != P2_3_USE_INPUT_FIFO) */

    if(P2_3_OUT_FIFO == fifoId)
    {
        switch(P2_3_GET_OUT_FIFO_STS)
        {
            case P2_3_OUT_FIFO_FULL :
                result = P2_3_RET_FIFO_FULL;
                break;

            case P2_3_OUT_FIFO_EMPTY :
                result = P2_3_RET_FIFO_EMPTY;
                break;

            case P2_3_OUT_FIFO_PARTIAL :
                result = P2_3_RET_FIFO_PARTIAL;
                break;

            default:
                /* Initial result value, while 
                   OUT_FIFO_FULL case is false 
                 */
                result = P2_3_RET_FIFO_FULL;
                break;
        }
    }

    return(result);
}


/*******************************************************************************
* Function Name: P2_3_SetIntMode
********************************************************************************
*
* Summary:
*  Sets the Interrupt Source for the Shift Register interrupt. Multiple
*  sources may be ORed together
*
* Parameters:
*  interruptSource: Byte containing the constant for the selected interrupt
*  source/s.
*
* Return:
*  None.
*
*******************************************************************************/
void P2_3_SetIntMode(uint8 interruptSource) 
{
    P2_3_SR_STATUS_MASK &= ((uint8) ~P2_3_INTS_EN_MASK);          /* Clear existing int */
    P2_3_SR_STATUS_MASK |= (interruptSource & P2_3_INTS_EN_MASK); /* Set int */
}


/*******************************************************************************
* Function Name: P2_3_GetIntStatus
********************************************************************************
*
* Summary:
*  Gets the Shift Register Interrupt status.
*
* Parameters:
*  None.
*
* Return:
*  Byte containing the constant for the selected interrupt source/s.
*
*******************************************************************************/
uint8 P2_3_GetIntStatus(void) 
{
    return(P2_3_SR_STATUS & P2_3_INTS_EN_MASK);
}


/*******************************************************************************
* Function Name: P2_3_WriteRegValue
********************************************************************************
*
* Summary:
*  Send state directly to shift register
*
* Parameters:
*  shiftData: containing shift register state.
*
* Return:
*  None.
*
*******************************************************************************/
void P2_3_WriteRegValue(uint16 shiftData)
                                                                     
{
    CY_SET_REG16(P2_3_SHIFT_REG_LSB_PTR, shiftData);
}


#if(0u != P2_3_USE_INPUT_FIFO)
    /*******************************************************************************
    * Function Name: P2_3_WriteData
    ********************************************************************************
    *
    * Summary:
    *  Send state to FIFO for later transfer to shift register based on the Load
    *  input
    *
    * Parameters:
    *  shiftData: containing shift register state.
    *
    * Return:
    *  Indicates: successful execution of function
    *  when FIFO is empty; and error when FIFO is full.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    cystatus P2_3_WriteData(uint16 shiftData)
                                                                         
    {
        cystatus result;

        result = CYRET_INVALID_STATE;

        /* Writes data into the input FIFO if it is not FULL */
        if(P2_3_RET_FIFO_FULL != (P2_3_GetFIFOStatus(P2_3_IN_FIFO)))
        {
            CY_SET_REG16(P2_3_IN_FIFO_VAL_LSB_PTR, shiftData);
            result = CYRET_SUCCESS;
        }

        return(result);
    }
#endif /* (0u != P2_3_USE_INPUT_FIFO) */


#if(0u != P2_3_USE_OUTPUT_FIFO)
    /*******************************************************************************
    * Function Name: P2_3_ReadData
    ********************************************************************************
    *
    * Summary:
    *  Returns state in FIFO due to Store input.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Shift Register state
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 P2_3_ReadData(void) 
    {
        return(CY_GET_REG16(P2_3_OUT_FIFO_VAL_LSB_PTR));
    }
#endif /* (0u != P2_3_USE_OUTPUT_FIFO) */


/*******************************************************************************
* Function Name: P2_3_ReadRegValue
********************************************************************************
*
* Summary:
*  Directly returns current state in shift register, not data in FIFO due
*  to Store input.
*
* Parameters:
*  None.
*
* Return:
*  Shift Register state. Clears output FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint16 P2_3_ReadRegValue(void) 
{
    uint16 result;

    /* Clear FIFO before software capture */
    while(P2_3_RET_FIFO_EMPTY != P2_3_GetFIFOStatus(P2_3_OUT_FIFO))
    {
        (void) CY_GET_REG16(P2_3_OUT_FIFO_VAL_LSB_PTR);
    }

    /* Read of 8 bits from A1 causes capture to output FIFO */
    (void) CY_GET_REG8(P2_3_SHIFT_REG_CAPTURE_PTR);

    /* Read output FIFO */
    result  = CY_GET_REG16(P2_3_OUT_FIFO_VAL_LSB_PTR);
    
    #if(0u != (P2_3_SR_SIZE % 8u))
        result &= ((uint16) P2_3_SR_MASK);
    #endif /* (0u != (P2_3_SR_SIZE % 8u)) */
    
    return(result);
}


/* [] END OF FILE */
