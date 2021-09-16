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
      
      if((RESERVE_SEC_FLAG % 10) == 0)
      {
         //FAB_DATA = 0;
         //fputc(0x61, FAB);
         //fprintf(PC,"SENT FAB \r\n"); 
      }
      
      if(RESERVE_SEC_FLAG > 59)                                                  //the counters inside the if are incremented every 60s
      {
         RESERVE_SEC_FLAG = 0;
         RESERVE_MIN_FLAG++;//time counter in minutes (used in reservation table)
         //FAB_DATA = 0;
         //fputc(0x61, FAB);

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
//!   fprintf(PC,"I receive from COM PIC: %x \r\n", in_bffr_main[COM_DATA]);
//!   COM_DATA = ((COM_DATA + 1) % 16);                                             //when the data is obtained in position 16, COM_DATA = 0
   CMD_FROM_COMM[COM_DATA] = fgetc(COM);   
   fprintf(PC,"%x,", CMD_FROM_COMM[COM_DATA]);
   COM_DATA = ((COM_DATA + 1) % 16);  
}

#INT_rda3//FAB Interrupt, RS232 receive data available in buffer 3
void UART3_RXD(void)
{
   //collect_HK_from_FAB();
   in_HK[FAB_DATA] = fgetc(FAB);//load the array in_HK [] with the data sent by the FAB PIC
   //fprintf(PC,"Battery Voltage %x \r\n", in_HK[FAB_DATA]);
   //fprintf(PC,"%x,",in_HK[FAB_DATA]);
   FAB_DATA = ((FAB_DATA + 1) % FAB_SENSOR_size);                                //when the data is obtained in position 45 FAB_DATA = 0
   //fprintf(PC, "\r\n");
   
}

#INT_rda4                                                                        //Reset PIC Interrupt, RS232 receive data available in buffer 4
void UART4_RXD(void)
{
   reset_bffr[RESET_DATA] = fgetc(reset);                                        //loads the reset_bffr array with the data sent by the Reset PIC (carga el array reset_bffr con los datos enviados por el Reset PIC)
   //fprintf(PC,"%x,",reset_bffr[RESET_DATA]);
   RESET_DATA = ((RESET_DATA + 1) % RESET_size);                                 //when the data is obtained in position 11 RESET_DATA = 0 (cuando se obtenga el dato en la posicion 11 RESET_DATA=0)
   if(reset_bffr[0] == 0xaa)                                                     //get ready for reset satellite
   {
      reset_flag = 1;                                                            //raise flag to reset (pone a alto bandera para reseteo)
      RESET_DATA = 0;                                                            //position indicator within reset_data vector (indicador de posicion dentro del vector reset_data)
   }
   
}

void settings()
{
   set_tris_b(0b11010000);                                                       // Port b configuration
   enable_interrupts(global);  // Enabling global interrupts
   enable_interrupts(int_timer0);                                                //HK data show by 1sec
   enable_interrupts(INT_rda);                                                   // Main to PC interrupt
   enable_interrupts(INT_rda2);                                                  // Main to COM PIC interrupt
   enable_interrupts(INT_rda3);                                                  // Main to FAB PIC interrupt   
   enable_interrupts(INT_rda4);                                                  // Main to RESET PIC interrupt

   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256 | RTCC_8_BIT);                     //Config TMR0 interrupt, internal clock, prescaler 256, clock/counter 8bit
   set_timer0(0);                                                                //this sets timer0 register to 0
   
   READ_WRTITING_ADDRESS_LOCATION();                                             //save in ADD_INFO_ADDRESS with address info location
   RESERVE_CHECK = 0;                                                            //flag that indicates if the reservation table is used, set to 0 to indicate false
   RESET_DATA = 0;                                                               //position indicator within reset_data vector
 
   CHECK_FLAG_INFO();                                                            //prints the state of the flags and separates them in each corresponding variable
   CHECK_ADDRESS_DATA();                                                         //prints the address data and separates it into each corresponding variable
   
   BC_SETUP();                                                                   //configuration of the digital analog converter for temperature sensor reading
  
   output_low(PIN_A4);                                                           //kill switch off
   Turn_OFF_CAM();                                                               //Camera switch OFF, RD7=0
   //Turn_OFF_ADCS();                                                            //ADCS switch OFF, RD6=0  
   //Turn_ON_ADCS();                                                             //ADCS switch ON, RD6=1
   Turn_ON_MBP();                                                                //Mission Boss switch ON, RF5=1
   Turn_OFF_BC();                                                                //Burner Circuit switch OFF, RD5=0

   fprintf(PC,"\r\n");
   CURRENT_FLAG_STATUS();                                                        //Print on screen the status of all flags
   fprintf(PC,"\r\n");
   CURRENT_ADDRESS_OF_FLASH();                                                   //Print current address allocation
   CHANGE_ADDRESS_WRITING_LOCATION();                                            //change writing direction when 95.000 cycles are exceeded

   output_high(PIN_C4);                                                          //MUX: COM side (MAIN-COM)
   output_low(PIN_A5);                                                           //MUX: Main side (MAIN-MISSION)
  
   //Get_RSV();                                                                    //read the reservation table info from flash memory  
   SAVE_SAT_LOG(0x25,0x25,0x25);                                                 //0X25 0X25 0X25 SATELLITE RESET INDICATOR
   
   
   fprintf(PC,"Start Operating\r\n");
   output_low(PIN_A5); //MUX MAIN SIDE
   
   return;                                                                       
  
} 

void main()
{

   fprintf(PC, "Turn on the satellite\r\n");

   settings();  //Prepare all interrupts, timers, flag information, BC setup etc.
   
   Antenna_Deploy(); //Attempt deploying of antenna. This is the 2nd, 3rd and 4th attempts

   while(TRUE)
   {
      BC_ON_30min(); //Attempt 1st antenna deployment after 30mins
      
      
      if(CMD_FROM_PC[0])
      {
         fprintf(PC,"\r\n");
         fprintf(PC,"COMMAND RECEIVED FROM PC: ");
         for(int m = 0; m < 9; m++)
         {
            fprintf(PC,"%x",CMD_FROM_PC[m]);
         }
         fprintf(PC,"\r\n");
         //0000 0000 to 0001 FFFF is MAIN PIC/MB
         
         BYTE command_ID = CMD_FROM_PC[0];
         command_ID &= 0xF0;
         //fprintf(PC,"%x",command_ID);
         //fprintf(PC,"\r\n");
         
         if(command_ID == 0x00 || command_ID == 0x10)
         {
            fprintf(PC,"Main PIC or MB Command only\r\n");
            MAIN_MB_CMD();
         }
         
         if(command_ID == 0x20 || command_ID == 0x30)
         {
            fprintf(PC,"MULT-SPEC Command\r\n");
            MULT_SPEC_Test();
         }
         
         if(command_ID == 0x40)
         {
            fprintf(PC,"ADCS Command\r\n");
            ADCS_test();
         }
         
         if(command_ID == 0x50)
         {
            fprintf(PC,"S-FWD Command\r\n");
            SFWD_test();
         }
         
         if(command_ID == 0x80)
         {
            fprintf(PC,"IMG-CLS Command\r\n");
            IMGCLS_test();
         }
         
         if(command_ID == 0x90)
         {
            fprintf(PC,"PINO Command\r\n");
            NEW_PINO_test();
         }

         DELETE_CMD_FROM_PC();                                                   //clear CMD_FROM_PC[] array
         Delete_Buffer();                                                        //clear in_bffr_main[] array
         CMD_FROM_PC[1] = 0;
         COM_DATA = 0;                                                           //clear COM correct receiving data flag
         PC_DATA = 0;                                                            //clear PC correct receiving data flag
      }
      
      
      //if((CMD_FROM_COMM[15]==0xBB) && (CMD_FROM_COMM[4] != 0xAB))
      if(CMD_FROM_COMM[0] == 0xAA && CMD_FROM_COMM[15] == 0xBB)
      {
         
         if(CMD_FROM_COMM[4]!= 0xAB)
         {
         
            BYTE command_ID_from_COMM = CMD_FROM_COMM[4];
            command_ID_from_COMM &= 0xF0;
            //fprintf(PC,"%x",command_ID);
            //fprintf(PC,"\r\n");
            
            if(command_ID_from_COMM == 0x00 || command_ID_from_COMM == 0x10)
            {
               fprintf(PC,"Main PIC or MB Comm Command only\r\n");
               MAIN_MB_CMD();
            }
            
            if(command_ID_from_COMM == 0x20 || command_ID_from_COMM == 0x30)
            {
               fprintf(PC,"MULT-SPEC Command\r\n");
               MULT_SPEC_Test();
            }
            
            if(command_ID_from_COMM == 0x40)
            {
               fprintf(PC,"ADCS Command\r\n");
               ADCS_test();
            }
            
            if(command_ID_from_COMM == 0x50)
            {
               fprintf(PC,"S-FWD Command\r\n");
               SFWD_test();
            }
            
            if(command_ID_from_COMM == 0x80)
            {
               fprintf(PC,"IMG-CLS Command\r\n");
               IMGCLS_test();
            }
            
            if(command_ID_from_COMM == 0x90)
            {
               fprintf(PC,"PINO Command\r\n");
               NEW_PINO_test();
            }
            DELETE_CMD_FROM_COMM();
            DELETE_CMD_FROM_PC();                                                   //clear CMD_FROM_PC[] array
            Delete_Buffer();                                                        //clear in_bffr_main[] array
            CMD_FROM_PC[1] = 0;
            COM_DATA = 0;                                                           //clear COM correct receiving data flag
            PC_DATA = 0;//clear PC correct receiving data flag
         }
      }
      
      DELETE_CMD_FROM_COMM();
      
      delay_ms(400); 
   }

}
