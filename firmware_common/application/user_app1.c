/**********************************************************************************************************************
File: user_app1.c                                                                

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

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
 LCDCommand(LCD_CLEAR_CMD);
 LedOff(WHITE);
 LedOff(PURPLE);
 LedOff(BLUE);
 LedOff(CYAN);
 LedOff(GREEN);
 LedOff(YELLOW);
 LedOff(ORANGE);
 LedOff(RED);
 LedOff(LCD_RED);
 LedOff(LCD_GREEN);
 LedOff(LCD_BLUE);
 PWMAudioOff(BUZZER1);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  extern u8 G_au8DebugScanfBuffer; 
  extern u8 G_u8DebugScanfCharCount; 

  static u8 au8UserInputBuffer[5];
  static u8 u8String[3];
  static u8 u8index=0;
  static u16 u16time=0;
  static bool ring=FALSE;
  
  if(G_u8DebugScanfCharCount>0) 
  { 
    DebugScanf(au8UserInputBuffer); 
    u8String[u8index]=au8UserInputBuffer[0];
    u8index++;
  }
  /**************  STATE 1  ***************************/
  if((WasButtonPressed(BUTTON1)||(u8String[0]=='1'&&u8String[1]==0x0D)))
  {
    u8index=0;
    for(u8 i=0;i<3;i++)
    {
      u8String[i]='\0';
    }
    ButtonAcknowledge(BUTTON1);
    LCDCommand(LCD_CLEAR_CMD);
    DebugPrintf("------------------------");
    DebugLineFeed();
    DebugPrintf("Entering state 1");
    DebugLineFeed();
    DebugPrintf("------------------------");
    DebugLineFeed();
    LCDMessage(LINE1_START_ADDR, "STATE 1");
    LedOn(WHITE);
    LedOn(PURPLE);
    LedOn(BLUE);
    LedOn(CYAN);
    LedOff(GREEN);
    LedOff(YELLOW);
    LedOff(ORANGE);
    LedOff(RED);
    LedOn(LCD_RED);
    LedOff(LCD_GREEN);
    LedOn(LCD_BLUE);
    PWMAudioOff(BUZZER1);
    ring=FALSE;
  }
  /***************  STATE 2  **********************/
  if((WasButtonPressed(BUTTON2)||(u8String[0]=='2'&&u8String[1]==0x0D)))
  {
    u8index=0;
    for(u8 j=0;j<3;j++)
    {
      u8String[j]='\0';
    }
    ButtonAcknowledge(BUTTON2);
    LCDCommand(LCD_CLEAR_CMD);
    DebugPrintf("------------------------");
    DebugLineFeed();
    DebugPrintf("Entering state 2");
    DebugLineFeed();
    DebugPrintf("------------------------");
    DebugLineFeed();
    LCDMessage(LINE1_START_ADDR, "STATE 2");
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOff(CYAN);
    LedBlink(GREEN, LED_1HZ);
    LedBlink(YELLOW, LED_2HZ);
    LedBlink(ORANGE, LED_4HZ);
    LedBlink(RED, LED_8HZ);
    LedOff(LCD_BLUE);
    LedPWM(LCD_RED, LED_PWM_100);
    LedPWM(LCD_GREEN, LED_PWM_50);
    PWMAudioSetFrequency(BUZZER1, 200);
    ring=TRUE;
  }
  if(ring==TRUE)
  {
    if(u16time<=100)
      {
       PWMAudioOn(BUZZER1);
       u16time++;
      }
    if(u16time>100&&u16time<=1000)
    {
      PWMAudioOff(BUZZER1);
      u16time++;
    }
    if(u16time>1000)
    {
      u16time=0;
    }
  }
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
