/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"
#include  <stdlib.h>

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */
static int code[30];


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/
u8 whichButtonPressed(void){
  if(WasButtonPressed(BUTTON0)){
    ButtonAcknowledge(BUTTON0);
    return 0;
  }

  if(WasButtonPressed(BUTTON1)){
    ButtonAcknowledge(BUTTON1);
    return 1;
  }

  if(WasButtonPressed(BUTTON2)){
    ButtonAcknowledge(BUTTON2);
    return 2;
  }

  if(WasButtonPressed(BUTTON3))
    ButtonAcknowledge(BUTTON3);
    return 3;
}

bool Array_match(int a[], int b[], int n){
  for (int i=0;i<n;i++)
    if(a[i]!=b[i])
      return FALSE;

  return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  /* If good initialization, set state to Idle */
  if(1)
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
    HEARTBEAT_OFF();
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR, "BUTTON LOCATIONS");
    LcdMessage(LINE2_START_ADDR, "0");
    LcdMessage(LINE2_START_ADDR+6,"1");
    LcdMessage(LINE2_START_ADDR+13,"2");
    LcdMessage(LINE2_END_ADDR,"3");
    
  
    //Fill code randomly with the digits 0-3
    /*
    Code currently not working
    for(int index=0;index<LEVEL-1;index++){
      code[index] = rand()%4;
    }
    */
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();
  code[0] = 1;
  code[1] = 0;
  code[2] = 3;
  
  static level = 1;
  int inputCode[30];
  static bool levelPassed = TRUE;

  int index;
  if(levelPassed){
    
    for(index=0;index<level;index++){
      LedOn(code[index]);
    }

    for (index = 0; index < level; index++)
    {
      inputCode[index] = whichButtonPressed();
    }
    inputCode[level] = 4; //4 is used, so the program know when the array ends
    level++;
  }
  
  else{
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR, "Incorrect");
  }

  
} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{

} 
 /* end UserApp1SM_Idle() */


/*------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
