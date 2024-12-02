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

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */
static u8 UserApp1Name[] = "Button Location";
static u16 correctColours[MAX_ROUND] = {1,2,3,2,1};                             // This will be the correct order of colours for the user to guess

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

void BeginningScreen(int cycles){
  u8 askMessage[] = "Press any button to play";

  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR, askMessage+(cycles%(strlen(askMessage))));
}

void PassedScreen(void){
  u8 PassedMessage[] = "Level Passed, you move on to the next round";
  
  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR,PassedMessage);
}

void FailedScreen(void){
  u8 FailedMessage[] = "You Failed this round";

  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR, FailedMessage);
}

int WasAnyButtonPressed(void){
  if(WasButtonPressed(BUTTON0)){
    ButtonAcknowledge(BUTTON0);
    return 1;
  }

  if(WasButtonPressed(BUTTON1)){
    ButtonAcknowledge(BUTTON1);
    return 1;
  }

  if(WasButtonPressed(BUTTON2)){
    ButtonAcknowledge(BUTTON2);
    return 1;
  }

  if(WasButtonPressed(BUTTON3)){
    ButtonAcknowledge(BUTTON3);
    return 1;
  }

}

void ShowColour(u16 arr[],u16 round, u16 cycle){
    if(cycle>=round)
      return;
    
    switch (arr[round])
    {
    case 0:
      LedOn(BLUE);
      break;
    
    case 1:
      LedOn(RED);
      break;
    
    case 2:
      LedOn(GREEN);
      break;

    case 3:
      LedOn(YELLOW);
      break;
    }
}

void CheckInput(u16 arr[],u16 *InputNum){

  if(WasButtonPressed(BUTTON0)){
    ButtonAcknowledge(BUTTON0);
    arr[*InputNum]=0;
    *InputNum++;
    return;
  }

  if(WasButtonPressed(BUTTON1)){
    ButtonAcknowledge(BUTTON1);
    arr[*InputNum]=1;
    *InputNum++;
    return;
  }

  if(WasButtonPressed(BUTTON2)){
    ButtonAcknowledge(BUTTON2);
    arr[*InputNum]=2;
    *InputNum++;
    return;
  }

  if(WasButtonPressed(BUTTON3)){
    ButtonAcknowledge(BUTTON3);
    arr[*InputNum]=3;
    *InputNum++;
    return;  
  }
}

bool CheckArrayMatched(u16 arr1[], u16 arr2[], int numCheck){
  for(int i; i<numCheck;i++){
    if(arr1[i]!=arr2[i])
      return FALSE;
  
  return TRUE;
  }
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
    LedOff(BLUE);
    LedOff(GREEN);
    LedOff(RED);
    LedOff(YELLOW);
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
  
  static u16 cycle=0, round=0, numOfInputs = 0;
  static bool Beginning = TRUE, RoundPassed = TRUE, RoundFinished = FALSE, 
  InGame = FALSE, END = FALSE, NewRound = TRUE, ColourShown=FALSE;      
  static u16 GuessedColours[MAX_ROUND];
  static u16 counter = U16_COUNTER_PERIOD_MS;

  if (counter==0){

    counter=U16_COUNTER_PERIOD_MS;

    if(Beginning){                          //Beginning screen 
      BeginningScreen(cycle);
      cycle++;
    

      if(WasAnyButtonPressed()){          //User wants to play move to the main game
        Beginning = FALSE;
        InGame = TRUE;
        LcdCommand(LCD_CLEAR_CMD);
      }
    }

    if (InGame){
      //code for what will happen in game
      if(NewRound){
        NewRound=FALSE;
        numOfInputs=0;
        cycle = 0;
      }

      if(RoundFinished){
        //Code for the user has finished a round
        RoundPassed = CheckArrayMatched(correctColours,GuessedColours, round);

        if(RoundPassed){
          NewRound = TRUE;
          PassedScreen();
          round++;
        }

        else{
          FailedScreen();
        }
        RoundFinished=FALSE;
      }

      else{
        //Code for what happens when the user is still in the round
        if(!ColourShown){
          CheckInput(&GuessedColours,&numOfInputs);
          if(numOfInputs==round)
            RoundFinished=TRUE;
        }

        else{
          ShowColour(correctColours, round, cycle);
          if(cycle==round)
            ColourShown=TRUE;
          cycle++;
        }
      }

    }
    

  }
  counter--;
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
