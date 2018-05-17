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

extern u32 G_u32AntApiCurrentMessageTimeStamp;                    /* From ant_api.c */  
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */  
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */  
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                /* From ant_api.c */  

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */  
static AntAssignChannelInfoType UserApp1_sSlaveChannel;
static u8 UserApp1_au8MessageFail[] = "\n\r***ANT channel setup failed***\n\n\r"; 


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
  LCDMessage(LINE1_START_ADDR,"Heart Rate B0:Rate"); 
  LCDMessage(LINE2_START_ADDR,"B1:Battery B3:Menu"); 
  LedOn(WHITE);
  LedOff(GREEN);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  
  /* Slave (Channel 1) */  
  UserApp1_sSlaveChannel.AntChannel = ANT_CHANNEL_0;  
  UserApp1_sSlaveChannel.AntChannelType = CHANNEL_TYPE_SLAVE;  
  UserApp1_sSlaveChannel.AntChannelPeriodHi = ANT_CHANNEL_PERIOD_HI_DEFAULT;  
  UserApp1_sSlaveChannel.AntChannelPeriodLo = ANT_CHANNEL_PERIOD_LO_DEFAULT;   
  UserApp1_sSlaveChannel.AntDeviceIdHi = 0x13;  
  UserApp1_sSlaveChannel.AntDeviceIdLo = 0x22;  
  UserApp1_sSlaveChannel.AntDeviceType = 120;  
  UserApp1_sSlaveChannel.AntTransmissionType = 0;      
  UserApp1_sSlaveChannel.AntFrequency = ANT_FREQUENCY_DEFAULT;  
  UserApp1_sSlaveChannel.AntTxPower = ANT_TX_POWER_DEFAULT;  
  UserApp1_sSlaveChannel.AntNetwork = ANT_NETWORK_DEFAULT;  

  UserApp1_sSlaveChannel.AntNetworkKey[0] = 0xB9;
  UserApp1_sSlaveChannel.AntNetworkKey[1] = 0xA5;
  UserApp1_sSlaveChannel.AntNetworkKey[2] = 0x21;
  UserApp1_sSlaveChannel.AntNetworkKey[3] = 0xFB;
  UserApp1_sSlaveChannel.AntNetworkKey[4] = 0xBD;
  UserApp1_sSlaveChannel.AntNetworkKey[5] = 0x72;
  UserApp1_sSlaveChannel.AntNetworkKey[6] = 0xC3;
  UserApp1_sSlaveChannel.AntNetworkKey[7] = 0x45; 

  AntAssignChannel(&UserApp1_sSlaveChannel);  
  UserApp1_u32Timeout = G_u32SystemTime1ms; 
  UserApp1_StateMachine = UserApp1SM_AntConfigureSlave; 
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
static void UserApp1SM_AntConfigureSlave(void)  
{  
/* Wait for the ANT slave channel to be configured */  
  if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_CONFIGURED)  
  {  
    DebugPrintf("Slave channel configured\n\n\r");  
    UserApp1_u32Timeout = G_u32SystemTime1ms; 
    UserApp1_StateMachine = UserApp1SM_Idle;  
  }   
  /* Check for timeout */  
  if( IsTimeUp(&UserApp1_u32Timeout, 3000) )  
  {  
    LCDCommand(LCD_CLEAR_CMD);  
    LCDMessage(LINE1_START_ADDR, "Slave config failed");  
    UserApp1_StateMachine = UserApp1SM_Error;      
  }  
} /* end UserApp1SM_AntConfigureSlave1() */  



/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  if(WasButtonPressed(BUTTON0))  
   {  
      ButtonAcknowledge(BUTTON0); 
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      DebugLineFeed(); 
      DebugPrintf("0:Record");
      DebugLineFeed(); 
      DebugPrintf("1:Lcd Colour");
      DebugLineFeed(); 
      DebugPrintf("2:Back");
      DebugLineFeed();  
      DebugPrintf("3:Alarm");
      DebugLineFeed(); 
      DebugPrintf("4:Close Alarm");
      DebugLineFeed(); 
      PWMAudioSetFrequency(BUZZER1,1000);
      PWMAudioOn(BUZZER1);
      AntOpenChannelNumber(ANT_CHANNEL_0); 
      LedBlink(GREEN, LED_2HZ);
      LedOff(WHITE);
      UserApp1_StateMachine = UserApp1SM_OpeningChannel; 
   }
} /* end UserApp1SM_Idle() */

static void UserApp1SM_OpeningChannel(void)  
{
  /* Ensure channel0 have opened */  
  if((AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_OPEN))  
  { 
    LCDCommand(LCD_CLEAR_CMD);  
    LCDMessage(LINE1_START_ADDR,"Channel opened");
    UserApp1_StateMachine = UserApp1SM_RadioActive;   
  }  
  /* Check for timeout */  
  if( IsTimeUp(&UserApp1_u32Timeout, 3000) )  
  {  
    LCDCommand(LCD_CLEAR_CMD);  
    LCDMessage(LINE1_START_ADDR, "Channel0 open failed");  
    UserApp1_StateMachine = UserApp1SM_Error;      
  } 
}
static void UserApp1SM_RadioActive(void) 
{
  extern u8 G_au8DebugScanfBuffer; 
  extern u8 G_u8DebugScanfCharCount;
  
  static u8 au8LastAntData[ANT_APPLICATION_MESSAGE_BYTES] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 
  static u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";
  static u8 au8Data[]={0x46,0xff,0xff,0xff,0xff,0x80,0x07,0x01};
  static u8 u8Rate[3],u8Beat[3],u8Battery[2];
  static u8 u8RateRecord[3],u8BeatRecord[3],u8LCDBattery[4];
  static u8 u8rate,u8beat,u8battery;
  static u8 au8UserInputBuffer[5]; 
  static u8 u8String[10];
  static u8 u8index=0;
  static u8 u8time=0;

  static bool bNewData=FALSE;
  static bool bBattery=FALSE;
  static bool bDebugrate=FALSE;
  static bool bDebugalarm=FALSE;
  
  PWMAudioOff(BUZZER1);
  LedBlink(PURPLE, LED_1HZ);
  LedBlink(BLUE, LED_1HZ);
  if( AntReadAppMessageBuffer() ) 
  { 
    if(G_eAntApiCurrentMessageClass == ANT_DATA) 
    { 
      for(u8 i = 0; i < ANT_APPLICATION_MESSAGE_BYTES; i++)  
      {  
        if(G_au8AntApiCurrentMessageBytes[i] != au8LastAntData[i])  
        { 
          bNewData=TRUE;
          au8LastAntData[i] = G_au8AntApiCurrentMessageBytes[i];  
          au8DataContent[2 * i]     = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] / 16);  
          au8DataContent[2 * i + 1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16);   
        }  
        u8rate=au8DataContent[14]*0x10+au8DataContent[15];
        u8beat=au8DataContent[12]*0x10+au8DataContent[13];
        u8Rate[0]=(u8rate/100)%10+0x30;
        u8Rate[1]=(u8rate/10)%10+0x30;
        u8Rate[2]=(u8rate/1)%10+0x30;
        u8Beat[0]=(u8beat/100)%10+0x30;
        u8Beat[1]=(u8beat/10)%10+0x30;
        u8Beat[2]=(u8beat/1)%10+0x30;
      } 
      if(bNewData)
      {
        bNewData=FALSE;
        LedOff(GREEN);
        LedOff(CYAN);
        LCDCommand(LCD_CLEAR_CMD);   
        LCDMessage(LINE1_START_ADDR,"Rate: ");
        LCDMessage(LINE1_START_ADDR + 6,u8Rate);
        LCDMessage(LINE2_START_ADDR,"Beat: "); 
        LCDMessage(LINE2_START_ADDR + 6,u8Beat); 
       } 
        AntQueueAcknowledgedMessage(ANT_CHANNEL_0,au8Data);
        if(WasButtonPressed(BUTTON1))  
        {  
          ButtonAcknowledge(BUTTON1);
          bBattery=TRUE;
        }
     }
  }
  if(bBattery)
  {
    LedOff(PURPLE);
    LedOff(BLUE);
    LedOn(CYAN);  
    if(G_au8AntApiCurrentMessageBytes[0]==0x07)
    {
      u8Battery[1] = G_au8AntApiCurrentMessageBytes[1]%0x10;
      u8Battery[0] = G_au8AntApiCurrentMessageBytes[1]/0x10;
      u8battery = u8Battery[0]*16+u8Battery[1];
      u8LCDBattery[3] ='%';
      u8LCDBattery[2] = u8battery%10+0x30;
      u8LCDBattery[1] = (u8battery/10)%10+0x30;
      u8LCDBattery[0] = u8battery/100+0x30;
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR,"Battery:");
      LCDMessage(LINE1_START_ADDR+16,u8LCDBattery); 
    }  
   }  
  if(G_u8DebugScanfCharCount>0) 
  { 
    DebugScanf(au8UserInputBuffer);
    u8String[u8index]=au8UserInputBuffer[0];
    u8index++;
    if(u8String[u8index]==0x08)
    {
      u8String[u8index-1]='\0';
      u8index--;
    }
    if(u8String[0]=='0')
    {
      u8index=0;
      for(u8 i=0;i<10;i++)
      {
        u8String[i]='\0';
      }
      for(u8 i=0;i<3;i++)
      {
        u8RateRecord[i]=u8Rate[i];
        u8BeatRecord[i]=u8Beat[i];
      }
      DebugLineFeed(); 
      DebugPrintf("Rate: ");
      DebugPrintf(u8RateRecord); 
      DebugLineFeed(); 
      DebugPrintf("Beat: ");
      DebugPrintf(u8BeatRecord);
      DebugLineFeed(); 
    }
    if(u8String[0]=='1')
    {
      u8index=0;
    u8time++;
      for(u8 i=0;i<10;i++)
      {
        u8String[i]='\0';
      }
      if(u8time==1)
      {
        LedOn(LCD_RED);
        LedOff(LCD_GREEN);
        LedOff(LCD_BLUE);
      }
      if(u8time==2)
      {
        LedOff(LCD_RED);
        LedOn(LCD_GREEN);
        LedOff(LCD_BLUE);
      }
      if(u8time==3)
      {
        LedOff(LCD_RED);
        LedOff(LCD_GREEN);
        LedOn(LCD_BLUE);
      }
      if(u8time==4)
      {
        LedOn(LCD_RED);
        LedOn(LCD_GREEN);
        LedOff(LCD_BLUE);
      }
      if(u8time==5)
      {
        LedOff(LCD_RED);
        LedOn(LCD_GREEN);
        LedOn(LCD_BLUE);
      }
      if(u8time==6)
      {
        LedOn(LCD_RED);
        LedOff(LCD_GREEN);
        LedOn(LCD_BLUE);
        u8time=0;
      }
    }
    if(u8String[0]=='2')
    {
      LedOn(LCD_RED);
      LedOn(LCD_GREEN);
      LedOn(LCD_BLUE);
      u8index=0;
      for(u8 i=0;i<10;i++)
      {
        u8String[i]='\0';
      }
      AntCloseChannelNumber(ANT_CHANNEL_0);
      UserApp1_StateMachine = UserApp1Initialize; 
    }
    if(u8String[0]=='3')
    {
      for(u8 i=0;i<10;i++)
      {
        u8String[i]='\0';
      }
      u8index=0;
      PWMAudioSetFrequency(BUZZER2,100);
      bDebugalarm=TRUE;
      DebugLineFeed(); 
    }
    if(bDebugalarm)
    {
      if(u8rate>=110)
      {
        PWMAudioOn(BUZZER2);
        LedBlink(RED, LED_8HZ);
      }
    }
    if(u8String[0]=='4')
    {
      for(u8 i=0;i<10;i++)
      {
        u8String[i]='\0';
      }
      u8index=0;
      PWMAudioOff(BUZZER2);
      LedOff(RED);
      bDebugalarm=FALSE;
    }
  }
  if(WasButtonPressed(BUTTON3))  
  {  
    ButtonAcknowledge(BUTTON3); 
    bBattery=FALSE;
    AntCloseChannelNumber(ANT_CHANNEL_0);
    UserApp1_StateMachine = UserApp1Initialize; 
  } 
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  LedBlink(RED, LED_4HZ);
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
