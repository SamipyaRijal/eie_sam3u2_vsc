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
#include <stdlib.h>

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags; /*!< @brief Global state flags */
static int code[30] = {0, 1, 2, 3, 2, 2, 3, 1, 2};

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;    /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;     /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;      /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags; /*!< @brief From main.c */

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine; /*!< @brief The state machine function pointer */
// static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

void start_message(int index){
  LcdCommand(LCD_CLEAR_CMD);
  char message[] = "Press any button to begin playing";
  if (index==strlen(message))
    index%=strlen(message);
  LcdMessage(LINE1_START_ADDR,message+index);

}

void displaying_code(u8 index)
{
  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR, "Displaying code");
}

void display_button_loc(u8 index){
  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR, "BUTTON LOCATIONS");
  LcdMessage(LINE2_START_ADDR, "0");
  LcdMessage(LINE2_START_ADDR + 6, "1");
  LcdMessage(LINE2_START_ADDR + 13, "2");
  LcdMessage(LINE2_END_ADDR, "3");
}

void display_passed_level(u8 index, u8 level){
  char message_1[] = "Congrats you guess correctly";
  char message_2[] = "Press any button to continue";

  if (index>=strlen(message_1))
    index%=strlen(message_1);

  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR, message_1+index);
  LcdMessage(LINE2_START_ADDR, message_2+index);
}

void display_incorrect(u8 index){
  char end_message[] = "Press any button to play again";

  if (index>=strlen(end_message))
    index%=strlen(end_message);

  LcdCommand(LCD_CLEAR_CMD);
  LcdMessage(LINE1_START_ADDR, "Incorrect");
  LcdMessage(LINE2_START_ADDR, end_message+index);
  //Add what level the user reached
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
  if (1)
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
    HEARTBEAT_OFF();
    // Fill code randomly with the digits 0-3

    // Code currently not working
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
  static int inputcode[30];
  static u16 user_inputs = 0;
  static u16 counter_period = U16_COUNTER_PERIOD_MS;
  static u8 display_index = 0;
  static u8 code_index = 0;
  static u8 level = 1;
  static u8 game_stage = 0;

  static bool guessed_correct = TRUE;

  if (counter_period == 500)
  {
    for (u8 i = 0; i < 7; i++)
    {
      LedOff(i);
    }
  }

  if (counter_period == 0)
  {

    counter_period = U16_COUNTER_PERIOD_MS;

    if(game_stage==0){
      start_message(display_index);
      if(WasButtonPressed(BUTTON0)){
        ButtonAcknowledge(BUTTON0);
        game_stage++;
      }

      else if(WasButtonPressed(BUTTON1)){
        ButtonAcknowledge(BUTTON1);
        game_stage++;
      }

      else if(WasButtonPressed(BUTTON2)){
        ButtonAcknowledge(BUTTON2);
        game_stage++;
      }

      else if(WasButtonPressed(BUTTON3)){
        ButtonAcknowledge(BUTTON3);
        game_stage++;
      }

      display_index++;
    }

    if (game_stage == 1){ // Display code to user
      displaying_code(display_index);
      LedOn(code[code_index]);
      LedOff(code[code_index - 1]);
      code_index++;
      if (code_index == level)
        game_stage++;
      display_index = 0;
      user_inputs = 0;

      ButtonAcknowledge(BUTTON0);
      ButtonAcknowledge(BUTTON1);
      ButtonAcknowledge(BUTTON2);
      ButtonAcknowledge(BUTTON3);
    }

    if (game_stage == 2){          //User guesses the code
      display_button_loc(display_index);
      if (WasButtonPressed(BUTTON0))
      {
        ButtonAcknowledge(BUTTON0);
        inputcode[user_inputs] = 0;
        LedOn(0);
        user_inputs++;
      }

      else if (WasButtonPressed(BUTTON1))
      {
        ButtonAcknowledge(BUTTON1);
        inputcode[user_inputs] = 1;
        LedOn(1);
        user_inputs++;
      }

      else if (WasButtonPressed(BUTTON2))
      {
        ButtonAcknowledge(BUTTON2);
        inputcode[user_inputs] = 2;
        LedOn(2);
        user_inputs++;
      }

      else if (WasButtonPressed(BUTTON3))
      {
        ButtonAcknowledge(BUTTON3);
        inputcode[user_inputs] = 3;
        LedOn(3);
        user_inputs++;
      }

      if (user_inputs == level){
        game_stage++;
        display_index = 0;
      }
    }

    if (game_stage == 3)
    {
      for (int index = 0; index <= level; index++){
        if (code[index] != inputcode[index]){
          game_stage = 5;
          display_index = 0;
          guessed_correct = FALSE;
        }
      }
      // User passed, set game for next level
      game_stage = 4;
      level++;
      code_index=0;
    }
    
    if(game_stage==4){
      display_passed_level(display_index, level);
      display_index++;

      if(WasButtonPressed(BUTTON0)){
        ButtonAcknowledge(BUTTON0);
        game_stage=1;
      }

      else if(WasButtonPressed(BUTTON1)){
        ButtonAcknowledge(BUTTON1);
        game_stage=1;
      }

      else if(WasButtonPressed(BUTTON2)){
        ButtonAcknowledge(BUTTON2);
        game_stage=1;
      }

      else if(WasButtonPressed(BUTTON3)){
        ButtonAcknowledge(BUTTON3);
        game_stage++;
        display_index=0;
      }

    }

    if(game_stage==5){
      display_incorrect(display_index);
      display_index++;
    }
  }
  counter_period--;
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
