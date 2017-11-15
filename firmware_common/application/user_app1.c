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
  LCDMessage(LINE1_START_ADDR, "B1-Debug B3-Return");
  LCDMessage(LINE2_START_ADDR, "B2-Name(B0-L B1-R)");
  PWMAudioSetFrequency(BUZZER1, 1000);
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
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
  static u8 u8String[40];
  static u8 u8String1[20];
  static u8 u8count=0;
  static u8 u8count1=0;
  static u8 u8count2=3;
  static u8 u8index=0;
  static u8 u8index1=0;
  static u8 au8Message[]="Zong";
  static bool Name=FALSE;
  static bool Input=FALSE;
  static bool Enter=FALSE;
  
/********************************
          B1-DEBUG
********************************/
  if(Name==FALSE)
  {
    if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      LCDCommand(LCD_CLEAR_CMD);
      Input=TRUE;
    }
  }
  if(Input==TRUE)
  {
    if(G_u8DebugScanfCharCount>0)
    {
      DebugScanf(au8UserInputBuffer);
      if(Enter==FALSE)
      {
        u8String[u8index]=au8UserInputBuffer[0];
        u8index++;
        LCDMessage(LINE1_START_ADDR, u8String);
        LCDMessage(LINE1_START_ADDR+20, u8String);
        if(u8index==40)
        {
          for(u8index=1;u8index<=39;u8index++)
          {
            u8String[u8index-1]=u8String[u8index];
          }
          u8index=39;
        }
      }
/*******************************/
      if(u8String[u8index-1]==0x0D)
      {
        Enter=TRUE;
      }
      if(Enter==TRUE)
      {
        if(au8UserInputBuffer[0]!=0x0D)
        {
          u8String1[u8index1]=au8UserInputBuffer[0];
          u8index1++;
          LCDMessage(LINE2_START_ADDR, u8String1);
          if(u8index1==20)
          {
            for(u8index1=1;u8index1<=19;u8index1++)
            {
              u8String1[u8index1-1]=u8String1[u8index1];
            }
            u8index1=19;
          }
        }
      }
    }
  }
/********************************
          B2-MOVE NAME
********************************/
  if(WasButtonPressed(BUTTON2))
  {
    ButtonAcknowledge(BUTTON2);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, au8Message);
    Name=TRUE;
    u8count=0;
  }
/*******  MOVE LEFT  *********/
  if(Name==TRUE)
  {
    if(WasButtonPressed(BUTTON0))
    {
      ButtonAcknowledge(BUTTON0);
      u8count--;
      u8count2--;
      if(u8count2==0)
      {
        LedOn(WHITE);
        LedOn(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
        PWMAudioSetFrequency(BUZZER2, 1300);
        PWMAudioOn(BUZZER2);
        u8count2=3;
      }
      if(u8count2==1)
      {
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOn(BLUE);
        LedOn(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
        PWMAudioSetFrequency(BUZZER2, 1600);
        PWMAudioOn(BUZZER2);
      }
      if(u8count2==2)
      {
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOn(GREEN);
        LedOn(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
        PWMAudioSetFrequency(BUZZER2, 1900);
        PWMAudioOn(BUZZER2);
      }
      if(u8count<17)
      {
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR+u8count, au8Message);
      }
      if(u8count>=17&&u8count<20)
      {
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOn(ORANGE);
        LedOff(RED);
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR+u8count, au8Message);
        LCDMessage(LINE1_START_ADDR+u8count+20, au8Message);
      }
      if(u8count>=20)
      {
        if(u8count<36)
        {
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE2_START_ADDR+u8count-20, au8Message);
        }
        else
        {
          u8count=35;
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE2_START_ADDR+u8count-20, au8Message);
          PWMAudioOff(BUZZER1);
        }
      }
    }
/*******  MOVE RIGHT  *******/
    if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      u8count++;
      u8count1++;
      if(u8count1==1)
      {
        LedOn(WHITE);
        LedOn(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
        PWMAudioSetFrequency(BUZZER2, 700);
        PWMAudioOn(BUZZER2);
      }
      if(u8count1==2)
      {
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOn(BLUE);
        LedOn(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
        PWMAudioSetFrequency(BUZZER2, 1000);
        PWMAudioOn(BUZZER2);
      }
      if(u8count1==3)
      {
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOn(GREEN);
        LedOn(YELLOW);
        LedOff(ORANGE);
        LedOff(RED);
        PWMAudioSetFrequency(BUZZER2, 1300);
        PWMAudioOn(BUZZER2);
        u8count1=0;
      }
      if(u8count<17)
      {
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR+u8count, au8Message);
      }
      if(u8count>=17&&u8count<20)
      {
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOn(ORANGE);
        LedOff(RED);
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR+u8count, au8Message);
        LCDMessage(LINE1_START_ADDR+u8count+20, au8Message);
      }
      if(u8count>=20&&u8count<37)
      {
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE2_START_ADDR+u8count-20, au8Message);
      }
      if(u8count>=37)
      {
        LCDMessage(LINE2_START_ADDR+16, au8Message);
        PWMAudioOn(BUZZER1);
        LedBlink(RED, LED_8HZ);
        LedOff(WHITE);
        LedOff(PURPLE);
        LedOff(BLUE);
        LedOff(CYAN);
        LedOff(GREEN);
        LedOff(YELLOW);
        LedOff(ORANGE);
      }
    }
  }
/********************************   
            B3-RETURN   
********************************/
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "B1-Debug  B3-Return");
    LCDMessage(LINE2_START_ADDR, "B2-Name(B0-L B1-R)");
    PWMAudioOff(BUZZER1);
    PWMAudioOff(BUZZER2);
    Name=FALSE;
    Input=FALSE;
    u8count=0;
    u8count1=0;
    u8count2=3;
    u8index=0;
    u8index1=0;
    LedOff(WHITE);
    LedOff(PURPLE);
    LedOff(YELLOW);
    LedOff(BLUE);
    LedOff(CYAN);
    LedOff(GREEN);
    LedOff(ORANGE);
    LedOff(RED);
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
