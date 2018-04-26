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
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static AntAssignChannelInfoType UserApp1_sMasterChannel; 
static AntAssignChannelInfoType UserApp1_sSlave1Channel; 
static AntAssignChannelInfoType UserApp1_sSlave0Channel;
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

 /* Start with all LEDs off except red blacklight */ 
   LedOff(RED); 
   LedOff(ORANGE); 
   LedOff(YELLOW); 
   LedOff(GREEN); 
   LedOff(CYAN); 
   LedOff(BLUE); 
   LedOff(PURPLE); 
   LedOff(WHITE); 
   LedOn(LCD_GREEN); 
   LedOn(LCD_BLUE); 
   LedOn(LCD_RED);
   
   LCDCommand(LCD_CLEAR_CMD);
   LCDMessage(LINE1_START_ADDR,"hide and seek");
   LCDMessage(LINE2_START_ADDR,"Press B0 to Start");  

/* Master (Channel 2) */ 
   UserApp1_sMasterChannel.AntChannel = ANT_CHANNEL_2; 
   UserApp1_sMasterChannel.AntChannelType = CHANNEL_TYPE_MASTER; 
   UserApp1_sMasterChannel.AntChannelPeriodHi = ANT_CHANNEL_PERIOD_HI_DEFAULT; 
   UserApp1_sMasterChannel.AntChannelPeriodLo = ANT_CHANNEL_PERIOD_LO_DEFAULT;     
   UserApp1_sMasterChannel.AntDeviceIdHi = 0x13; 
   UserApp1_sMasterChannel.AntDeviceIdLo = 0x22; 
   UserApp1_sMasterChannel.AntDeviceType = 1; 
   UserApp1_sMasterChannel.AntTransmissionType = 1;     
   UserApp1_sMasterChannel.AntFrequency = ANT_FREQUENCY_DEFAULT; 
   UserApp1_sMasterChannel.AntTxPower = ANT_TX_POWER_DEFAULT; 
   UserApp1_sMasterChannel.AntNetwork = ANT_NETWORK_DEFAULT; 
   
/* Slave1 (Channel 1) */ 
   UserApp1_sSlave1Channel.AntChannel = ANT_CHANNEL_1; 
   UserApp1_sSlave1Channel.AntChannelType = CHANNEL_TYPE_SLAVE; 
   UserApp1_sSlave1Channel.AntChannelPeriodHi = ANT_CHANNEL_PERIOD_HI_DEFAULT; 
   UserApp1_sSlave1Channel.AntChannelPeriodLo = ANT_CHANNEL_PERIOD_LO_DEFAULT;  
   UserApp1_sSlave1Channel.AntDeviceIdHi = 0x13; 
   UserApp1_sSlave1Channel.AntDeviceIdLo = 0x22; 
   UserApp1_sSlave1Channel.AntDeviceType = 1; 
   UserApp1_sSlave1Channel.AntTransmissionType = 1;     
   UserApp1_sSlave1Channel.AntFrequency = ANT_FREQUENCY_DEFAULT; 
   UserApp1_sSlave1Channel.AntTxPower = ANT_TX_POWER_DEFAULT; 
   UserApp1_sSlave1Channel.AntNetwork = ANT_NETWORK_DEFAULT; 

/* Slave0 (Channel 0) */ 
   UserApp1_sSlave0Channel.AntChannel = ANT_CHANNEL_0; 
   UserApp1_sSlave0Channel.AntChannelType = CHANNEL_TYPE_SLAVE; 
   UserApp1_sSlave0Channel.AntChannelPeriodHi = ANT_CHANNEL_PERIOD_HI_DEFAULT; 
   UserApp1_sSlave0Channel.AntChannelPeriodLo = ANT_CHANNEL_PERIOD_LO_DEFAULT;  
   UserApp1_sSlave0Channel.AntDeviceIdHi = 0x13; 
   UserApp1_sSlave0Channel.AntDeviceIdLo = 0x22; 
   UserApp1_sSlave0Channel.AntDeviceType = 1; 
   UserApp1_sSlave0Channel.AntTransmissionType = 1;     
   UserApp1_sSlave0Channel.AntFrequency = ANT_FREQUENCY_DEFAULT; 
   UserApp1_sSlave0Channel.AntTxPower = ANT_TX_POWER_DEFAULT; 
   UserApp1_sSlave0Channel.AntNetwork = ANT_NETWORK_DEFAULT; 
   
   
   for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++) 
   { 
     UserApp1_sMasterChannel.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
     UserApp1_sSlave1Channel.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
     UserApp1_sSlave0Channel.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
   } 
  AntAssignChannel(&UserApp1_sMasterChannel); 
  UserApp1_u32Timeout = G_u32SystemTime1ms; 
  if(1)
  {
    UserApp1_StateMachine = UserApp1SM_AntConfigureMaster; 
  }
  else 
  { 
  /* The task isn't properly initialized, so shut it down and don't run */ 
    DebugPrintf(UserApp1_au8MessageFail);    
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
/* Wait for ANT channel assignment */ 
static void UserApp1SM_AntConfigureMaster(void) 
{ 
 if( AntRadioStatusChannel(ANT_CHANNEL_2) == ANT_CONFIGURED) 
 {
   DebugPrintf("Master channel configured\n\n\r");       
   /* Queue configuration of Slave channel */ 
   AntAssignChannel(&UserApp1_sSlave1Channel); 
   UserApp1_u32Timeout = G_u32SystemTime1ms; 
   UserApp1_StateMachine = UserApp1SM_AntConfigureSlave1; 
 } 
  
 /* Watch for time out */ 
 if(IsTimeUp(&UserApp1_u32Timeout, 3000)) 
 { 
   LCDCommand(LCD_CLEAR_CMD); 
   LCDMessage(LINE1_START_ADDR, "Master config failed"); 
   UserApp1_StateMachine = UserApp1SM_Error;     
 }     
} /* end UserApp1SM_AntConfigureMaster */ 

static void UserApp1SM_AntConfigureSlave1(void) 
{ 
 /* Wait for the ANT slave channel to be configured */ 
 if(AntRadioStatusChannel(ANT_CHANNEL_1) == ANT_CONFIGURED) 
 { 
   DebugPrintf("Slave1 channel configured\n\n\r"); 
   AntAssignChannel(&UserApp1_sSlave0Channel);
   UserApp1_u32Timeout = G_u32SystemTime1ms;
   UserApp1_StateMachine = UserApp1SM_AntConfigureSlave0; 
 }  
 /* Check for timeout */ 
 if( IsTimeUp(&UserApp1_u32Timeout, 3000) ) 
 { 
   LCDCommand(LCD_CLEAR_CMD); 
   LCDMessage(LINE1_START_ADDR, "Slave1 config failed"); 
   UserApp1_StateMachine = UserApp1SM_Error;     
 } 
} /* end UserApp1SM_AntConfigureSlave1() */ 

static void UserApp1SM_AntConfigureSlave0(void) 
{ 
 /* Wait for the ANT slave channel to be configured */ 
 if(AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_CONFIGURED) 
 { 
   DebugPrintf("Slave0 channel configured\n\n\r"); 
   UserApp1_u32Timeout = G_u32SystemTime1ms;
   UserApp1_StateMachine = UserApp1SM_Idle; 
 }  
 /* Check for timeout */ 
 if( IsTimeUp(&UserApp1_u32Timeout, 3000) ) 
 { 
   LCDCommand(LCD_CLEAR_CMD); 
   LCDMessage(LINE1_START_ADDR, "Slave0 config failed"); 
   UserApp1_StateMachine = UserApp1SM_Error;     
 } 
} /* end UserApp1SM_AntConfigureSlave1() */ 
/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{ 
  /* Write the one line of code to use the BUTTON API to check if BUTTON0 was pressed */  
   if(WasButtonPressed(BUTTON0)) 
   { 
     ButtonAcknowledge(BUTTON0);   
     /* Queue the Channel Open messages and then go to wait state */ 
     AntOpenChannelNumber(ANT_CHANNEL_0); 
     AntOpenChannelNumber(ANT_CHANNEL_1);      
     UserApp1_u32Timeout = G_u32SystemTime1ms; 
     UserApp1_StateMachine = UserApp1SM_OpeningChannels;  
   }
   if(WasButtonPressed(BUTTON3)) 
   { 
     ButtonAcknowledge(BUTTON3);
     LedOff(WHITE);
     LedOff(PURPLE);
     LedOff(BLUE);
     LedOff(CYAN);
     LedOff(GREEN);
     LedOff(YELLOW);
     LedOff(ORANGE);
     LedOff(RED);
     PWMAudioOff(BUZZER1);
   } 
} /* end UserApp1SM_Idle() */
    
static void UserApp1SM_OpeningChannels(void) 
{ 
   static u16 u16time=0;
   static u16 u16timecounter=0;
   for(u16time=0;u16time<10000;u16time++)
   {
     u16timecounter++;
     LCDMessage(LINE1_START_ADDR,"time"); 
   }
   if(u16time==10000)
   {
     /* Ensure channel0 have opened */ 
     if((AntRadioStatusChannel(ANT_CHANNEL_0) == ANT_OPEN)) 
     { 
       LCDCommand(LCD_CLEAR_CMD); 
       LCDMessage(LINE1_START_ADDR,"Ready or not?"); 
       LCDMessage(LINE2_START_ADDR,"Here i come!");    
       UserApp1_StateMachine = UserApp1SM_RadioActive;     
     } 
     /* Check for timeout */ 
     if( IsTimeUp(&UserApp1_u32Timeout, 3000) ) 
     { 
       LCDCommand(LCD_CLEAR_CMD); 
       LCDMessage(LINE1_START_ADDR, "Channel0 open failed"); 
       UserApp1_StateMachine = UserApp1SM_Error;     
     }
     /* Ensure channel1 have opened */ 
     if((AntRadioStatusChannel(ANT_CHANNEL_1) == ANT_OPEN)) 
     { 
       LCDCommand(LCD_CLEAR_CMD); 
       LCDMessage(LINE1_START_ADDR,"Channel1 open");     
       UserApp1_StateMachine = UserApp1SM_RadioActive;     
     } 
     /* Check for timeout */ 
     if( IsTimeUp(&UserApp1_u32Timeout, 3000) ) 
     { 
       LCDCommand(LCD_CLEAR_CMD); 
       LCDMessage(LINE1_START_ADDR, "Channel1 open failed"); 
       UserApp1_StateMachine = UserApp1SM_Error;     
     }  
   }
} /* end UserApp1SM_OpeningChannels() */ 

static void UserApp1SM_RadioActive(void) 
{
   static s8 s8RssiChannel0 = -99; 
   static s8 s8RssiChannel1 = -99; 
   static u8 u8TestMessage[]={0,0,0,0,0x15,0,0,0};
   static u8 au8LastAntData[ANT_APPLICATION_MESSAGE_BYTES] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   static u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";
   static bool bNewData=FALSE;
   
   if( AntReadAppMessageBuffer() )
   {
     if(G_eAntApiCurrentMessageClass == ANT_DATA)
     {
       for(u8 i = 0; i < ANT_APPLICATION_MESSAGE_BYTES; i++) 
       { 
         if(G_au8AntApiCurrentMessageBytes[i] != au8LastAntData[i]) 
         { 
           bNewData = TRUE; 
           au8LastAntData[i] = G_au8AntApiCurrentMessageBytes[i]; 
           au8DataContent[2 * i]     = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] / 16); 
           au8DataContent[2 * i + 1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16);  
         } 
       } 
       if(bNewData)
       {
         LCDCommand(LCD_CLEAR_CMD);  
         LCDMessage(LINE1_START_ADDR,"Seeker");   
         LCDMessage(LINE2_START_ADDR, au8DataContent);  
       }      
       u8TestMessage[7]++; 
       if(u8TestMessage[7] == 0) 
       { 
         u8TestMessage[6]++; 
         if(u8TestMessage[6] == 0) 
         { 
           u8TestMessage[5]++; 
         } 
       } 
       s8RssiChannel0=G_sAntApiCurrentMessageExtData.s8RSSI;
       s8RssiChannel1=G_sAntApiCurrentMessageExtData.s8RSSI;
       if((s8RssiChannel0>=-99&&s8RssiChannel0<=-95)||(s8RssiChannel1>=-99&&s8RssiChannel1<=-95))
       {
         LedOn(WHITE);
         LedOff(PURPLE);
         LedOff(BLUE);
         LedOff(CYAN);
         LedOff(GREEN);
         LedOff(YELLOW);
         LedOff(ORANGE);
         LedOff(RED);
         PWMAudioOff(BUZZER1);
       }
       if((s8RssiChannel0>=-94&&s8RssiChannel0<=-90)||(s8RssiChannel1>=-94&&s8RssiChannel1<=-90))
       {
         LedOn(WHITE);
         LedOn(PURPLE);
         LedOff(BLUE);
         LedOff(CYAN);
         LedOff(GREEN);
         LedOff(YELLOW);
         LedOff(ORANGE);
         LedOff(RED);
         PWMAudioOff(BUZZER1);
       }
       if((s8RssiChannel0>=-89&&s8RssiChannel0<=-85)||(s8RssiChannel1>=-89&&s8RssiChannel1<=-85))
       {
         LedOn(WHITE);
         LedOn(PURPLE);
         LedOn(BLUE);
         LedOff(CYAN);
         LedOff(GREEN);
         LedOff(YELLOW);
         LedOff(ORANGE);
         LedOff(RED);
         PWMAudioOff(BUZZER1);
       }
       if((s8RssiChannel0>=-84&&s8RssiChannel0<=-80)||(s8RssiChannel1>=-84&&s8RssiChannel1<=-80))
       {
         LedOn(WHITE);
         LedOn(PURPLE);
         LedOn(BLUE);
         LedOn(CYAN);
         LedOff(GREEN);
         LedOff(YELLOW);
         LedOff(ORANGE);
         LedOff(RED);
         PWMAudioOff(BUZZER1);
       }
       if((s8RssiChannel0>=-79&&s8RssiChannel0<=-75)||(s8RssiChannel1>=-79&&s8RssiChannel1<=-75))
       {
         LedOn(WHITE);
         LedOn(PURPLE);
         LedOn(BLUE);
         LedOn(CYAN);
         LedOn(GREEN);
         LedOff(YELLOW);
         LedOff(ORANGE);
         LedOff(RED);
         PWMAudioOff(BUZZER1);
       }
       if((s8RssiChannel0>=-74&&s8RssiChannel0<=-70)||(s8RssiChannel1>=-74&&s8RssiChannel1<=-70))
       {
         LedOn(WHITE);
         LedOn(PURPLE);
         LedOn(BLUE);
         LedOn(CYAN);
         LedOn(GREEN);
         LedOn(YELLOW);
         LedOff(ORANGE);
         LedOff(RED);
         PWMAudioOff(BUZZER1);
       }
       if((s8RssiChannel0>=-69&&s8RssiChannel0<=-65)||(s8RssiChannel1>=-69&&s8RssiChannel1<=-65))
       {
         LedOn(WHITE);
         LedOn(PURPLE);
         LedOn(BLUE);
         LedOn(CYAN);
         LedOn(GREEN);
         LedOn(YELLOW);
         LedOn(ORANGE);
         LedOff(RED);
         PWMAudioOff(BUZZER1);
       }
       if((s8RssiChannel0>=-60&&s8RssiChannel0<=-50)||(s8RssiChannel1>=-60&&s8RssiChannel1<=-50))
       {
         LedOn(WHITE);
         LedOn(PURPLE);
         LedOn(BLUE);
         LedOn(CYAN);
         LedOn(GREEN);
         LedOn(YELLOW);
         LedOn(ORANGE);
         LedOn(RED); 
         PWMAudioSetFrequency(BUZZER1,2000);
         PWMAudioOn(BUZZER1);
         u8TestMessage[0]=0x02;
         AntQueueBroadcastMessage(ANT_CHANNEL_0, u8TestMessage); 
         AntQueueBroadcastMessage(ANT_CHANNEL_1, u8TestMessage);
         AntOpenChannelNumber(ANT_CHANNEL_2); 
         UserApp1_StateMachine = UserApp1SM_OpenMasterChannel;
       }
     }
     else if(G_eAntApiCurrentMessageClass == ANT_TICK)
     {
       
     }
   }
   UserApp1_u32Timeout = G_u32SystemTime1ms;
}


static void UserApp1SM_OpenMasterChannel(void) 
{    
     /* Ensure channel2 have opened */ 
     if((AntRadioStatusChannel(ANT_CHANNEL_2) == ANT_OPEN)) 
     { 
       LCDCommand(LCD_CLEAR_CMD); 
       LCDMessage(LINE1_START_ADDR,"hide");     
       UserApp1_StateMachine = UserApp1SM_MasterActive;     
     } 
     /* Check for timeout */ 
     if( IsTimeUp(&UserApp1_u32Timeout, 3000) ) 
     { 
       LCDCommand(LCD_CLEAR_CMD); 
       LCDMessage(LINE1_START_ADDR, "Channel2 open failed"); 
       UserApp1_StateMachine = UserApp1SM_Error;     
     }   
}
 /* end UserApp1SM_ClosingChannels() */  
 
static void UserApp1SM_MasterActive(void)
{
  static u8 u8MasterMessage[]={0,0,0,0,0xA5,0,0,0};
  u8 u8MasterDataContent[]="xxxxxxxxxxxxxxxx";
  LedOff(RED); 
  LedOff(ORANGE); 
  LedOff(YELLOW); 
  LedOff(GREEN); 
  LedOff(CYAN); 
  LedOff(BLUE); 
  LedOff(PURPLE); 
  LedOff(WHITE); 
  PWMAudioOff(BUZZER1); 
  
  
  if( AntReadAppMessageBuffer() ) 
   { 
      /* New message from ANT task: check what it is */ 
     if(G_eAntApiCurrentMessageClass == ANT_DATA) 
     { 
       /* We got some data: parse it into au8DataContent[] */ 
       for(u8 i = 0; i < ANT_DATA_BYTES; i++) 
       { 
         u8MasterDataContent[2 * i]     = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] / 16); 
         u8MasterDataContent[2 * i + 1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16); 
       } 
       LCDMessage(LINE2_START_ADDR, u8MasterDataContent); 
       if(u8MasterDataContent[0]==0x02)
       {
          LedOn(RED); 
          UserApp1_StateMachine = UserApp1SM_Idle;  
       } 
     } 
     else if(G_eAntApiCurrentMessageClass == ANT_TICK) 
     { 
      /* Update and queue the new message data */ 
       u8MasterMessage[7]++; 
       if(u8MasterMessage[7] == 0) 
       { 
         u8MasterMessage[6]++; 
        if(u8MasterMessage[6] == 0) 
         { 
           u8MasterMessage[5]++; 
         } 
       }
       AntQueueAcknowledgedMessage(ANT_CHANNEL_2, u8MasterMessage);
     } 
   } /* end AntReadData() */ 
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
