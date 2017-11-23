/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

Test1.
Test2 from Engenuics.
Test3.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */


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
  extern u8 G_au8DebugScanfBuffer[]; 
  extern u8 G_u8DebugScanfCharCount;
  DebugPrintf("LED Programming Interface");
  DebugLineFeed();
  DebugPrintf("Press 1 to program LED command sequence");
  DebugLineFeed();
  DebugPrintf("Press 2 to show current USER program");
  DebugLineFeed();

  //u8 au8UserApp1Start1[] = "LED program task started\n\r";
  
  /* Turn off the Debug task command processor and announce the task is ready */
  //DebugSetPassthrough();
  //DebugPrintf(au8UserApp1Start1);
  
    /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
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
/* Wait for input */
static void UserApp1SM_Idle(void)
{
   static u8 au8UserInputBuffer[5];
  static LedCommandType LED;
  static u8 u8String[20];
  static u8 u8index=0,u8index1,u8i,u8j,u8number=1;
  static u32 u32TimeOn,u32TimeOff;
  static bool Menu=TRUE;
  static bool Enter=FALSE;
  static bool List=FALSE;
  
 if(G_u8DebugScanfCharCount>0)
  {
    DebugScanf(au8UserInputBuffer);
/**************************************
              PRESS 1
**************************************/
    if(Menu==TRUE)
    {
      if(au8UserInputBuffer[0]=='1')
      {
        DebugLineFeed();
        DebugLineFeed();
        DebugPrintf("Enter commands as LED-ONTIME-OFFTIME and press Enter");
        DebugLineFeed();
        DebugPrintf("Time is in milliseconds,max 100 commands");
        DebugLineFeed();
        DebugPrintf("LED colors:R, O, Y, G, C, B, P, W");
        DebugLineFeed();
        DebugPrintf("Example:R-100-200 (Red on at 100ms and off at 200ms)");
        DebugLineFeed();
        DebugPrintf("Press Enter on blank line to end");
        DebugLineFeed();
        DebugPrintf("1:");
        Menu=FALSE;
       }
    }
    u8String[u8index]=au8UserInputBuffer[0];
    u8index++;
    if(u8String[u8index-1]==0x0D)
    {
      u8index=1;
      Enter=TRUE;
      u8number++;
      DebugLineFeed();
      DebugPrintNumber(u8number);
      DebugPrintf(":");
    }
/*********************START**************/
    if(Enter==TRUE)
    {
      for(u8i=3;u8i<20;u8i++)
      {
        if(u8String[u8i]==0x2D)
        {
          u8index1=u8i;
          u8i=20;
        }
      }
/*******TIME ON****************/
      if(u8index1-3==3)
      {
        u32TimeOn=(u8String[3]-48)*100+(u8String[4]-48)*10+(u8String[5]-48);
      }
      if(u8index1-3==4)
      {
        u32TimeOn=(u8String[3]-48)*1000+(u8String[4]-48)*100+(u8String[5]-48)*10+(u8String[6]-48);
      }
/********TIME OFF*************/
      if(u8String[u8index1+4]==0x0D)
      {
        u32TimeOff=(u8String[u8index1+1]-48)*100+(u8String[u8index1+2]-48)*10+(u8String[u8index1+3]-48);
      }
      if(u8String[u8index1+5]==0x0D)
      {
        u32TimeOff=(u8String[u8index1+1]-48)*1000+(u8String[u8index1+2]-48)*100+(u8String[u8index1+3]-48)*10+(u8String[u8index1+4]-48);
      }
/*********************************
        BACK TO THE MENU    
*********************************/
      if(u8String[1]=='b'&&u8String[2]=='a'&&u8String[3]=='c'&&u8String[4]=='k')
      {
          DebugLineFeed();
          DebugPrintf("LED Programming Interface");
          DebugLineFeed();
          DebugPrintf("Press 1 to program LED command sequence");
          DebugLineFeed();
          DebugPrintf("Press 2 to show current USER program");
          DebugLineFeed();
          Menu=TRUE;
          for(u8 i=1;i<20;i++)
          {
            u8String[i]='\0';
          }
      }
/******************WHITE****************************/
      if(u8String[1]=='W')
      {
        LED.eLED=WHITE;
        LED.u32Time=u32TimeOn;
        LED.bOn=TRUE;
        LED.u32Time=u32TimeOff;
        LED.bOn=FALSE;
        LedDisplayAddCommand(USER_LIST,&LED);
      }
/**********************PURPLE**************************/
      if(u8String[1]=='P')
      {
        LED.eLED=PURPLE;
        LED.u32Time=u32TimeOn;
        LED.bOn=TRUE;
        LED.u32Time=u32TimeOff;
        LED.bOn=FALSE;
        LedDisplayAddCommand(USER_LIST,&LED);
      }
/*******************BLUE****************************/
      if(u8String[1]=='B')
      {
        LED.eLED=BLUE;
        LED.u32Time=u32TimeOn;
        LED.bOn=TRUE;
        LED.u32Time=u32TimeOff;
        LED.bOn=FALSE;
        LedDisplayAddCommand(USER_LIST,&LED);
      }
/*******************CYAN********************************/
      if(u8String[1]=='C')
      {
        LED.eLED=CYAN;
        LED.u32Time=u32TimeOn;
        LED.bOn=TRUE;
        LED.u32Time=u32TimeOff;
        LED.bOn=FALSE;
        LedDisplayAddCommand(USER_LIST,&LED);
      }
/***********************GREEN************************/
      if(u8String[1]=='G')
      {
        LED.eLED=GREEN;
        LED.u32Time=u32TimeOn;
        LED.bOn=TRUE;
        LED.u32Time=u32TimeOff;
        LED.bOn=FALSE;
        LedDisplayAddCommand(USER_LIST,&LED);
      }
/**********************YELLOW*************************/
      if(u8String[1]=='Y')
      {
        LED.eLED=YELLOW;
        LED.u32Time=u32TimeOn;
        LED.bOn=TRUE;
        LED.u32Time=u32TimeOff;
        LED.bOn=FALSE;
        LedDisplayAddCommand(USER_LIST,&LED);
      }
/**********************ORANGE**************************/
      if(u8String[1]=='O')
      {
        LED.eLED=ORANGE;
        LED.u32Time=u32TimeOn;
        LED.bOn=TRUE;
        LED.u32Time=u32TimeOff;
        LED.bOn=FALSE;
        LedDisplayAddCommand(USER_LIST,&LED);
      }
/**********************RED*****************************/
      if(u8String[1]=='R')
      {
        LED.eLED=RED;
        LED.u32Time=u32TimeOn;
        LED.bOn=TRUE;
        LED.u32Time=u32TimeOff;
        LED.bOn=FALSE;
        LedDisplayAddCommand(USER_LIST,&LED);
      }
     }
/************************************
            PRESS 2
*************************************/
    if(Menu==TRUE)
    {
      if(au8UserInputBuffer[0]=='2')
      { 
        DebugLineFeed();
        DebugLineFeed();
        DebugPrintf("Current USER program:");
        DebugLineFeed();
        DebugLineFeed();
        DebugPrintf("LED  ON TIME  OFF TIME");
        DebugLineFeed();
        DebugPrintf("-----------------------");
        DebugLineFeed();
        Menu=FALSE;
        List=TRUE;
      }
      if(List==TRUE)
      {
        for(u8j=0;u8j<u8number;u8j++)
        {
          LedDisplayPrintListLine(u8j);
        }
      }
    }
  }
} /* end UserApp1SM_Idle() */
                      
            
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
