/*
 * File: BIRDS5_OBC.c
 * Author: Keenan Chatar and Yukihisa Otani
 * PIC: 18F67J94 w/Int OSC @ 16MHz, 3.3v
 * Program: MAIN PIC Program
 * Compiler: XC8 (v1.35, CCS C Compiler v5.026)
 * Program Version: 1.0
 *                
 * Program Description: This is the main program loop for the BIRDS-5 OBC.
 *                      
 */

#include <BIRDS5_OBC.h>

#int_timer0                                                                      //TMR0 Interrupt
void intval()
{   
   static int32 counter = 0;                                                     //the variable is activated as global, it maintains its value when entering and exiting the function
   counter++;                                                                    //increases every 16.384ms
   if (counter > 60)                                                             //the counters within the if are incremented approx every 1s
   {
      counter = 0;
      FAB_FLAG++;                                                               //time counter in seconds, at 90 seconds it requests data from the FAB
      RESERVE_SEC_FLAG++;                                                       //time counter in seconds (used in reservation table)
      currenttime++;                                                            //time counter in seconds (Reset Function)
      //fprintf(PC,"currenttime:%ld \r\n", currenttime);                        //Check TMR0 operation
      //fprintf(PC,"Done\r\n");
      
      
      if(RESERVE_SEC_FLAG > 59)                                                  //the counters inside the if are incremented every 60s
      {
         RESERVE_SEC_FLAG = 0;
         RESERVE_MIN_FLAG++;                                                     //time counter in minutes (used in reservation table)
         missiontime++;
         FAB_DATA = 0;
      }
   }
}

#INT_rda                                                                         //PC Interrupt, RS232 receive data available in buffer 1
void UART1_RXD(void)
{
   CMD_FROM_PC[PC_DATA] = fgetc(PC);                                             //loads the [] array with the data sent by PC
   PC_DATA = ((PC_DATA + 1) % 9);                                                //when the data is obtained in position 9, PC_DATA = 0
}

#INT_rda2 HIGH                                                                   //COM Interrupt, RS232 receive data available in buffer 2
void UART2_RXD(void)
{
   //SerialDataReceive();  //get 32 byte
   //COM_ONEBYTE_COMMAND = fgetc(COM);
//!   in_bffr_main[COM_DATA] = fgetc(COM);                                          //load the array in_bffr_main [] with the data sent by COM PIC
//!   fprintf(PC,"%x, ", in_bffr_main[COM_DATA]);
//!   COM_DATA = ((COM_DATA + 1) % 16);                                             //when the data is obtained in position 16, COM_DATA = 0
   CMD_FROM_COMM[COM_DATA] = fgetc(COM);   
   fprintf(PC,"%x,", CMD_FROM_COMM[COM_DATA]);
   COM_DATA = ((COM_DATA + 1) % 16);

}

#INT_rda3                                                                        //FAB Interrupt, RS232 receive data available in buffer 3
void UART3_RXD(void)
{

   in_HK[FAB_DATA] = fgetc(FAB);                                                 //load the array in_HK [] with the data sent by the FAB PIC
   fprintf(PC,"%x ",in_HK[FAB_DATA]);
   FAB_DATA = ((FAB_DATA + 1) % FAB_SENSOR_size);                                //when the data is obtained in position 45 FAB_DATA = 0

}

#INT_rda4                                                                        //Reset PIC Interrupt, RS232 receive data available in buffer 4
void UART4_RXD(void)
{
   //fprintf(PC,"%x,",reset_bffr[RESET_DATA]);
   reset_bffr[RESET_DATA] = fgetc(reset);                                        //loads the reset_bffr array with the data sent by the Reset PIC (carga el array reset_bffr con los datos enviados por el Reset PIC)
   RESET_DATA = ((RESET_DATA + 1) % RESET_size);                                 //when the data is obtained in position 11 RESET_DATA = 0 (cuando se obtenga el dato en la posicion 11 RESET_DATA=0)
   if(reset_bffr[0] == 0xaa)                                                     //get ready for reset satellite
   {
      reset_flag = 1;                                                            //raise flag to reset (pone a alto bandera para reseteo)
      RESET_DATA = 0;                                                            //position indicator within reset_data vector (indicador de posicion dentro del vector reset_data)
   }
   RESET_TO_MAIN_FLAG = 1;   
}

void settings()
{
   set_tris_b(0b11010000);                                                       // Port b configuration
   
   enable_interrupts(global);                                                    // Enabling global interrupts
   enable_interrupts(int_timer0);                                                // HK data show by 1sec
   enable_interrupts(INT_rda);                                                   // Main to PC interrupt
   enable_interrupts(INT_rda2);                                                  // Main to COM PIC interrupt
   enable_interrupts(INT_rda3);                                                  // Main to FAB PIC interrupt   
   enable_interrupts(INT_rda4);                                                  // Main to RESET PIC interrupt

   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256 | RTCC_8_BIT);                     //Config TMR0 interrupt, internal clock, prescaler 256, clock/counter 8bit
   set_timer0(0);                                                                //this sets timer0 register to 0
   
   READ_WRITING_ADDRESS_LOCATION();                                             //save in ADD_INFO_ADDRESS with address info location
   RESERVE_CHECK = 0;                                                            //flag that indicates if the reservation table is used, set to 0 to indicate false
   RESET_DATA = 0;                                                               //position indicator within reset_data vector
 
   CHECK_FLAG_INFO();                                                            //prints the state of the flags and separates them in each corresponding variable
   fprintf(PC,"\r\n");
   CHECK_ADDRESS_DATA();                                                         //prints the address data and separates it into each corresponding variable
   
   BC_SETUP();                                                                   //configuration of the digital analog converter for temperature sensor reading
  
   output_low(PIN_A4);                                                           //kill switch off
   Turn_Off_MBP();                                                                //Mission Boss switch ON, RF5=1
   Turn_Off_ADCS();
   Turn_OFF_BC();                                                                //Burner Circuit switch OFF, RD5=0

   fprintf(PC,"\r\n");
   CURRENT_FLAG_STATUS();                                                        //Print on screen the status of all flags
   fprintf(PC,"\r\n");
   CURRENT_ADDRESS_OF_FLASH();                                                   //Print current address allocation
   CHANGE_ADDRESS_WRITING_LOCATION();                                            //change writing direction when 95.000 cycles are exceeded
   
   output_high(PIN_C4);                                                          //MUX: COM side (MAIN-COM)
   output_low(PIN_A5);                                                           //MUX: Main side (MAIN-MISSION)
  
   Get_RSV();                                                                    //read the reservation table info from flash memory  
   SAVE_SAT_LOG(0x25,0x25,0x25);                                                 //0X25 0X25 0X25 SATELLITE RESET INDICATOR
   delay_ms(500);
   output_low(PIN_A5);                                                           //MUX MAIN SIDE
   
   fprintf(PC,"\r\n*********Start Operating*********\r\n");

   return;                                                                       
  
} 

void main()
{

   fprintf(PC, "*********Turn on the satellite*********\r\n");

   settings();                                                                   //Prepare all interrupts, timers, flag information, BC setup etc.
   
   //Antenna_Deploy();                                                           //Attempt deploying of antenna. This is the 2nd, 3rd and 4th attempts
   
   FAB_TEST_OPERATION();
   fprintf(PC, "\r\n********Finish FAB TEST OPERATION*******\r\n");   

   delay_ms(5000);
   

   while(TRUE)                                                                     //Main Operation Loop
   {
      //BC_ON_30min();                                                             //Attempt 1st antenna deployment after 30mins
      
      delay_ms(100); 
      
      if((RESERVE_MIN_FLAG >= RESERVE_TARGET_FLAG) && RESERVE_CHECK == 1)        //check the reservation command, if time came, execute
      {
         Turn_On_MBP();
         delay_ms(1000);
         fprintf(PC,"Execute reserved command\r\n");
         MISSION_CONTENTS = CHECK_MEMORY_FUNCTION(MISSION_CONTENTS);             //avoid erase or transfer sectors from memory as reserved commands
         EXECUTE_COMMAND_from_COMM(MISSION_CONTENTS,RESERVE_DATA_0,RESERVE_DATA_1,RESERVE_DATA_2,RESERVE_DATA_3,RESERVE_DATA_4,RESERVE_DATA_5, RESERVE_DATA_6);    //execute command TODO: fix rsv table to add last byteto command
         Remove_1_Reservation();                                                 //remove the finished command and sort again and save updated command table
         if(reserve_table[90] != 0x00)                                           //if next reservation is registered, wait until time will be come
         {
            Reserve_next();                                                      //take the next CMD from the table
         }
         else
         {                                                                  //if all reservation finished, reset flag about reservation
            RESERVE_TARGET_FLAG = 0;                                             //reset flag
            RESERVE_CHECK = 0;
            MISSION_CONTENTS = 0;
            //MISSION_DETAIL = 0;
         }
      }

      
      if(CMD_FROM_PC[0])
      {
         
         fprintf(PC,"\r\n");
         fprintf(PC,"COMMAND RECEIVED FROM PC: ");
         
         for(int m = 0; m < 9; m++)
         {
            fprintf(PC,"%x",CMD_FROM_PC[m]);
         }
         fprintf(PC,"\r\n");
         
         if(CMD_FROM_PC[1] == 0)
         {
            Turn_On_MBP();
            delay_ms(1000);
            EXECUTE_COMMAND_from_PC(CMD_FROM_PC[0], CMD_FROM_PC[2], CMD_FROM_PC[3], CMD_FROM_PC[4], CMD_FROM_PC[5], CMD_FROM_PC[6], CMD_FROM_PC[7], CMD_FROM_PC[8]);
            //output_high(PIN_A5);
         }
         else
         {
            SAVE_SAT_LOG(0xDD,CMD_FROM_PC[0],CMD_FROM_PC[1]);          //reservation command log
            Reserve_command_PC(); 
         }

         DELETE_CMD_FROM_PC();                                                   //clear CMD_FROM_PC[] array
         DELETE_CMD_FROM_COMM();
         Delete_Buffer();                                                        //clear in_bffr_main[] array
         CMD_FROM_PC[1] = 0;
         COM_DATA = 0;                                                           //clear COM correct receiving data flag
         PC_DATA = 0;                                                            //clear PC correct receiving data flag
        
      }
      //output_high(PIN_A5);
      
      
      
      if(CMD_FROM_COMM[0] == 0xAA && CMD_FROM_COMM[15] == 0xBB)
      {
         COM_TO_MAIN_FLAG = 1;
         
         CHECK_50_and_CW_RESPOND(); 
         
         if(CMD_FROM_COMM[4] == 0x00)
         {
         
            //EXECUTE_COMMAND_from_COMM(CMD_FROM_COMM[4],CMD_FROM_COMM[6], CMD_FROM_COMM[7], CMD_FROM_COMM[8], CMD_FROM_COMM[9], CMD_FROM_COMM[10], CMD_FROM_COMM[11], 0x00);
            Turn_On_MBP();
            delay_ms(1000);
            EXECUTE_COMMAND_from_COMM(CMD_FROM_COMM[3],CMD_FROM_COMM[5], CMD_FROM_COMM[6], CMD_FROM_COMM[7], CMD_FROM_COMM[8], CMD_FROM_COMM[9], CMD_FROM_COMM[10], CMD_FROM_COMM[11]);
         }
         else
         {
            SAVE_SAT_LOG(0xDD,CMD_FROM_COMM[3],CMD_FROM_COMM[4]);                //reservation command log
            Reserve_command_COM();
         }
         DELETE_CMD_FROM_COMM();
         DELETE_CMD_FROM_PC();                                                   //clear CMD_FROM_PC[] array
         Delete_Buffer();                                                        //clear in_bffr_main[] array
         CMD_FROM_PC[1] = 0;
         COM_DATA = 0;                                                           //clear COM correct receiving data flag
         PC_DATA = 0;   
      }


      
      if(MISSION_STATUS)
      {

         if(missiontime > 15)
         {
            SAVE_SAT_LOG(0x99,0x99,0x99);          //mission time elapsed and resetted missions to off stage
            fprintf (PC, "Mission Time: %x \r\n", missiontime);
            fprintf (PC, "Start 0x20 - Turn OFF MULTSPEC CAM1 (MB1)\r\n") ;
            output_low(pin_G3);
            
            fprintf (PC, "Start 0x30 - Turn OFF MULTSPEC CAM2 (MB2)\r\n") ;
            output_low(pin_F6); //Turn off DIO for MULTSPEC CAM2
            
            fprintf (PC, "Start 0x80 - Turn OFF IMGCLS (RAB)\r\n") ;
            FWD_CMD_MBP(0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);         //Turn off IMGCLS Pi command
            
            fprintf (PC, "Start 0x50 - Turn OFF SFWD (RAB)\r\n") ;            
            FWD_CMD_MBP(0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);         //Turn off IMGCLS Pi command
            Turn_Off_MBP();
            missiontime = 0;
            MISSION_STATUS = 0;
            MISSION_OPERATING = 0;
            STORE_FLAG_INFO();                                                         //store flag info on flash
            WRITE_FLAG_to_EEPROM();                                                    //store flag info on EEPROM
         }  
      }
      
      if(FAB_FLAG > 89)                                                            //every 90 sec, OBC gathers sensor data and updates CW format 
      {
         FAB_FLAG = 0;
         fprintf(PC,"\r\n***NORMAL SAMPLING HOUSEKEEPING DATA COLLECTION***\r\n");
         FAB_TEST_OPERATION();
         
         STORE_ADRESS_DATA_TO_FLASH();                                           //for store the address info

         fprintf(PC,"CW:");
         for(int i = 0; i < 4; i++)                                              //show CW format
         {
            fprintf(PC,"%x,",CW_FORMAT[i]);
         }
         fprintf(PC,"%x\r\n",CW_FORMAT[4]);
         DELETE_CMD_FROM_PC();                                                   //delete PC command
         PC_DATA = 0;                                                            //reset interrupt data for safety
         COM_DATA = 0;                                                           //reset interrupt data for safety
         RESET_DATA = 0;                                                         //reset interrupt data for safety
         fprintf(PC,"\r\n");
         //OITA_Test();
      }
      
      
      if(COM_DATA != 0 || PC_DATA != 0)                                          //COM_DATA AND PC_DATA WILL BE ZERO IF THE CORRECT NUMBER OF CHARACTERS ARE RECEIVED
      {
         delay_ms(500);
         if(PC_DATA != 0)                                                        //If the correct number of characters was not received, reset the CMD_FROM_PC array
         {
            CMD_FROM_PC[0] = 0;
            CMD_FROM_PC[1] = 0;
            CMD_FROM_PC[2] = 0;
            CMD_FROM_PC[3] = 0;
            CMD_FROM_PC[4] = 0;
            CMD_FROM_PC[5] = 0;
            CMD_FROM_PC[6] = 0;
            CMD_FROM_PC[7] = 0;
            COM_DATA = 0;
            PC_DATA = 0;
         }
      }
   }

}
