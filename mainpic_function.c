
int8 MISSION_STATUS = 0;                                                         //MISSION STATUS FLAGS
static int16 currenttime = 0;
BYTE command[9];
int8 reset_time_data[11] = {};
BYTE PINO_DATA[39] = {0x00};
int8 trigger_time_data[5];
unsigned int32 address_data[4];
unsigned int32 packet_data[2];
unsigned int32 address;
unsigned int32 packet;
BYTE Down[81];
BYTE dummy[1];
BYTE Finish_sign[1];
int8 command_time_data[5];
int8 j;
int i;
int result = 1;


void Turn_On_CAM()

{
   output_high (PIN_D7);
   return;
}

void Turn_Off_CAM()
{
   output_low (PIN_D7);
   return;
}

void Flash_Memory_Access()
{
   sector_erase_OF (0x00000000);
   WRITE_DATA_BYTE_OF (0x00000000, 0x01) ;
   TRANSFER_DATA_NBYTE_TOPC_OF (0x00000000, 1) ;
   delay_ms (1000);
   
   sector_erase_OF (0x00000000);
   WRITE_DATA_BYTE_OF (0x00000000, 0x02) ;
   TRANSFER_DATA_NBYTE_TOPC_OF (0x00000000, 1) ;
   delay_ms (1000);
   
   sector_erase_OF (0x00000000);
   WRITE_DATA_BYTE_OF (0x00000000, 0x03) ;
   TRANSFER_DATA_NBYTE_TOPC_OF (0x00000000, 1) ;
   delay_ms (1000);
}

void ten_sec_counter(){
   int counter=0;
   int sec=0;


      counter++; //16.384ms for one increment
      if(counter>60)
      {
         counter = 0;
         sec++;
      }
      else if(sec>9){
         break;
      }
}

void PINO_Test()
{
   dummy[0] = 0x01;
   int32 num;
   while (TRUE)
   {
      command[0] = 0x00;
      
      for (num = 0; num < 100; num++)
      {
         if (kbhit (PC))
         {

            for (int i = 0; i < 9; i++)
            {
               //fprintf (fab, "Get command\r\n") ;
               command[i] = fgetc (PC);
            }
            break;
         }
      }
      switch (command[0])
      {
         case 0x12:
         fprintf (PC, "Start 0x12\r\n") ;
         output_low (PIN_A5);
         address_data[0] = command[2]<<24;
         address_data[1] = command[3]<<16;
         address_data[2] = command[4]<<8;
         address_data[3] = command[5];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         packet_data[0] = command[6]<<8;
         packet_data[1] = command[7];
         packet = (packet_data[0] + packet_data[1])*81;

         //fputc(command[5] + command[6], fab);
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         output_high (PIN_A5);
         //TRANSFER_DATA_NBYTE_TOFAB_SMF(address,81);
         
         fprintf (PC, "Finish 0x12\r\n") ;
         break;
         
         
         /*case 0x13://Get the data from Flash Memory 2
         fprintf (fab, "Start 0x13\r\n") ;
         output_low (PIN_A5);
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         TRANSFER_DATA_NBYTE_TOFAB_SMF(address, command[5] + command[6]);
         fprintf (fab, "Finish 0x13\r\n") ;
         
         break;
         */
         
         case 0x14://Uplink command to write the data on Flash Memory 2
         output_low (PIN_A5) ;//Main side
         fprintf (PC, "Start 0x14\r\n") ;
         address_data[0] = command[2]<<24;
         address_data[1] = command[3]<<16;
         address_data[2] = command[4]<<8;
         address_data[3] = command[5];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         //sector_erase_SMF (address);
         WRITE_DATA_BYTE_SMF (address, command[6]) ;
         WRITE_DATA_BYTE_SMF (address + 1, command[7]) ;
         WRITE_DATA_BYTE_SMF (address + 2, command[8]) ;
         fprintf (PC, "Finish 0x14\r\n");
         output_high (PIN_A5);//Mission side
         break;
         
         case 0x16://Erase the data on Flash Memory 2
         output_low (PIN_A5);
         fprintf(PC, "Start 0x16\r\n");
         address_data[0] = command[2]<<24;
         address_data[1] = command[3]<<16;
         address_data[2] = command[4]<<8;
         address_data[3] = command[5];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
            switch(command[6]){
               case 0x04:
                  SUBSECTOR_4KB_ERASE_SMF(address);
                  fprintf(PC, "Finish 0x16\r\n");
                  break;
               case 0x32:
                  SUBSECTOR_32KB_ERASE_SMF(address);
                  fprintf(PC, "Finish 0x16\r\n");
                  break;
               case 0xFF:
                  SECTOR_ERASE_SMF(address);
                  fprintf(PC, "Finish 0x16\r\n");
                  break;
               default:
                  fprintf(PC, "error\r\n");
            }
            output_high (PIN_A5);//Mission side
            break;
         
         /* case 0x91:
         reset_time_data[0] = 0x82;
         //fprintf (PC, "Command 2 Recieved\r\n") ;
         //fputc (reset_time_data[0], PC) ;
         fputc (0x91, reset);
         break; */

         case 0x90://Turn off PINO
         fprintf (PC, "Start 0x90\r\n") ;
         output_high (hvs);
         fprintf (PC, "Finish 0x90\r\n");
         delay_ms(5000);
         output_low (PINO_power);
         output_low (sel);
         output_low(PIN_A5);//Main side
         
         break;
         
         case 0x91://PINO Real Time Uplink Command
         output_high(PIN_A5);//Mission Side
         fprintf (PC, "Start 0x91\r\n");
         PINO_DATA[0] = command[0];
         PINO_DATA[1] = command[2];
         PINO_DATA[2] = command[3];
         PINO_DATA[3] = command[4];
         PINO_DATA[4] = command[5];
         PINO_DATA[5] = command[6];
         PINO_DATA[6] = command[7];
         PINO_DATA[7] = command[8];
         
         for (i = 0; i<39; i++)
         {
            fputc (PINO_DATA[i], DC);
         }
         fprintf (PC, "Finish 0x91\r\n") ;
         output_high(PIN_A5);//Mission Side
         break;
         
         case 0x92://PINO Real Time Downlink Command
         fprintf (PC, "Start 0x92\r\n") ;
         output_high(PIN_A5);
         PINO_DATA[0] = command[0];
         PINO_DATA[1] = command[2];
         PINO_DATA[2] = command[3];
         PINO_DATA[3] = command[4];
         PINO_DATA[4] = command[5];
         PINO_DATA[5] = command[6];
         PINO_DATA[6] = command[7];
         PINO_DATA[7] = command[8];
         
         for (i = 0; i<39; i++)
         {
            fputc (PINO_DATA[i], DC);
         }
         
         while (1)
         {
            if (kbhit (DC))
            {
               for (i = 0; i < 10; i++)
               {
                  Down[i] = fgetc (DC);
               }
               //fprintf (PC, "Finish transmitting\r\n") ;
               for (i = 0; i < 10; i++)
               {
                  fputc (Down[i], PC);
               }
               fprintf (PC, "Finish 0x92\r\n") ;
               output_high(PIN_A5);//Mission side
               break;
            }
         }
         
         case 0x9E: //Turn on PINO
         fprintf(PC, "Start 0x93\r\n");
         output_high (PINO_power);
         output_high (sel);
         output_high (PIN_A5);//Mission side
         fprintf(PC, "Finish 0x93\r\n");
         

         break;
   
         case 0x94:
         output_high(PIN_A5);
         fprintf (PC, "Start 0x94\r\n") ;
         for (i = 0; i < 5; i++)
         {
            GET_RESET_DATA ();
            delay_ms(5000);
         }
         fprintf (PC, "Finish 0x94\r\n") ;
         output_high(PIN_A5);
         break;
         
         
         case 0x9B:
         output_high(PIN_A5);
         fprintf (PC, "Start 0x9B\r\n") ;
         output_high (hvs);
         fprintf (PC, "Finish 0x9B\r\n");
         output_low (PINO_power);
         output_low (sel);
         output_low (PIN_A5);
         break;
         
         case 0x9C://Time and attitude information
         output_high(PIN_A5);
         int a=0;
         fprintf (PC, "Start 0x9C\r\n") ;
         
         for(a=0; a<12; a++){
            GET_RESET_DATA();
            delay_ms(10000);
         }
         
         fprintf (PC, "Finish 0x9F\r\n") ;
         fprintf (PC, "Finish 0x9C\r\n") ;
         output_high(PIN_A5);

         break;
         

      }
   }
}

void PINO_Test_for_PINO()
{
   dummy[0] = 0x01;
   int32 num;
   while (TRUE)
   {
      command[0] = 0x00;
      
      for (num = 0; num < 100; num++)
      {
         if (kbhit (fab))
         {

            for (int i = 0; i < 9; i++)
            {
               //fprintf (fab, "Get command\r\n") ;
               command[i] = fgetc (fab);
            }
            fprintf(fab, "Get the command\r\n");
            break;
         }
      }
      switch (command[0])
      {
         case 0x05:
         fprintf(fab, "Analyse the command\r\n");
         break;
         
         case 0x12:
         fprintf (fab, "Start 0x12\r\n") ;
         output_low (PIN_A5);
         address_data[0] = command[2]<<24;
         address_data[1] = command[3]<<16;
         address_data[2] = command[4]<<8;
         address_data[3] = command[5];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         packet_data[0] = command[6]<<8;
         packet_data[1] = command[7];
         packet = (packet_data[0] + packet_data[1])*81;

         //fputc(command[5] + command[6], fab);
         TRANSFER_DATA_NBYTE_TOFAB_SMF(address, packet);
         output_high (PIN_A5);
         //TRANSFER_DATA_NBYTE_TOFAB_SMF(address,81);
         
         fprintf (fab, "Finish 0x12\r\n") ;
         break;
         
         
         /*case 0x13://Get the data from Flash Memory 2
         fprintf (fab, "Start 0x13\r\n") ;
         output_low (PIN_A5);
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         TRANSFER_DATA_NBYTE_TOFAB_SMF(address, command[5] + command[6]);
         fprintf (fab, "Finish 0x13\r\n") ;
         
         break;
         */
         
         case 0x14://Uplink command to write the data on Flash Memory 2
         output_low (PIN_A5) ;//Main side
         fprintf (fab, "Start 0x14\r\n") ;
         address_data[0] = command[2]<<24;
         address_data[1] = command[3]<<16;
         address_data[2] = command[4]<<8;
         address_data[3] = command[5];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         //sector_erase_SMF (address);
         WRITE_DATA_BYTE_SMF (address, command[6]) ;
         WRITE_DATA_BYTE_SMF (address + 1, command[7]) ;
         WRITE_DATA_BYTE_SMF (address + 2, command[8]) ;
         fprintf (fab, "Finish 0x14\r\n");
         output_high (PIN_A5);//Mission side
         break;
         
         case 0x16://Erase the data on Flash Memory 2
         output_low (PIN_A5);
         fprintf(fab, "Start 0x16\r\n");
         address_data[0] = command[2]<<24;
         address_data[1] = command[3]<<16;
         address_data[2] = command[4]<<8;
         address_data[3] = command[5];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
            switch(command[6]){
               case 0x04:
                  SUBSECTOR_4KB_ERASE_SMF(address);
                  fprintf(fab, "Finish 0x16\r\n");
                  break;
               case 0x32:
                  SUBSECTOR_32KB_ERASE_SMF(address);
                  fprintf(fab, "Finish 0x16\r\n");
                  break;
               case 0xFF:
                  SECTOR_ERASE_SMF(address);
                  fprintf(fab, "Finish 0x16\r\n");
                  break;
               default:
                  fprintf(fab, "error\r\n");
            }
            output_high (PIN_A5);//Mission side
            break;
         
         /* case 0x91:
         reset_time_data[0] = 0x82;
         //fprintf (PC, "Command 2 Recieved\r\n") ;
         //fputc (reset_time_data[0], PC) ;
         fputc (0x91, reset);
         break; */

         case 0x90://Turn off PINO
         fprintf (fab, "Start 0x90\r\n") ;
         output_high (hvs);
         fprintf (fab, "Finish 0x90\r\n");
         delay_ms(5000);
         output_low (PINO_power);
         output_low (sel);
         output_low(PIN_A5);//Main side
         
         break;
         
         case 0x91://PINO Real Time Uplink Command
         output_high(PIN_A5);//Mission Side
         fprintf (fab, "Start 0x91\r\n");
         PINO_DATA[0] = command[0];
         PINO_DATA[1] = command[2];
         PINO_DATA[2] = command[3];
         PINO_DATA[3] = command[4];
         PINO_DATA[4] = command[5];
         PINO_DATA[5] = command[6];
         PINO_DATA[6] = command[7];
         PINO_DATA[7] = command[8];
         
         for (i = 0; i<8; i++)
         {
            fputc (PINO_DATA[i], DC);
         }
         fprintf (fab, "Finish 0x91\r\n") ;
         output_high(PIN_A5);//Mission Side
         break;
         
         case 0x92://PINO Real Time Downlink Command
         fprintf (fab, "Start 0x92\r\n") ;
         output_high(PIN_A5);
         PINO_DATA[0] = command[0];
         PINO_DATA[1] = command[2];
         PINO_DATA[2] = command[3];
         PINO_DATA[3] = command[4];
         PINO_DATA[4] = command[5];
         PINO_DATA[5] = command[6];
         PINO_DATA[6] = command[7];
         PINO_DATA[7] = command[8];
         
         for (i = 0; i<8; i++)
         {
            fputc (PINO_DATA[i], DC);
         }
         
         while (1)
         {
            if (kbhit (DC))
            {
               for (i = 0; i < 81; i++)
               {
                  Down[i] = fgetc (DC);
               }
               //fprintf (PC, "Finish transmitting\r\n") ;
               for (i = 0; i < 81; i++)
               {
                  fputc (Down[i], fab);
               }
               fprintf (fab, "Finish 0x92\r\n") ;
               output_high(PIN_A5);//Mission side
               break;
            }
            break;
         }
         
         case 0x9E: //Turn on PINO
         fprintf(fab, "Start 0x93\r\n");
         output_high (PINO_power);
         output_high (sel);
         output_high (PIN_A5);//Mission side
         fprintf(fab, "Finish 0x93\r\n");
         

         break;
   
         case 0x94:
         output_high(PIN_A5);
         fprintf (fab, "Start 0x94\r\n") ;
         for (i = 0; i < 5; i++)
         {
            GET_RESET_DATA ();
            delay_ms(5000);
         }
         fprintf (fab, "Finish 0x94\r\n") ;
         output_high(PIN_A5);
         break;
         
         
         case 0x9B:
         output_high(PIN_A5);
         fprintf (fab, "Start 0x9B\r\n") ;
         output_high (hvs);
         fprintf (fab, "Finish 0x9B\r\n");
         output_low (PINO_power);
         output_low (sel);
         output_low (PIN_A5);
         break;
         
         case 0x9C://Time and attitude information
         output_high(PIN_A5);
         int a=0;
         fprintf (fab, "Start 0x9C\r\n") ;
         
         for(a=0; a<12; a++){
            GET_RESET_DATA();
            delay_ms(10000);
         }
         
         fprintf (fab, "Finish 0x9F\r\n") ;
         fprintf (fab, "Finish 0x9C\r\n") ;
         output_high(PIN_A5);

         break;
         

      }
   }
}

void MULT_SPEC_Test()
{
   dummy[0] = 0x01;

   int32 num;
   
   while (TRUE)
   {
      command[0] = 0x00;
      
      for (num = 0; num < 100; num++)
      {
         if (kbhit (PC))
         {

            for (int i = 0; i < 9; i++)
            {
               command[i] = fgetc (PC);
               fprintf (PC, "Command Recieved:%x\r\n", command[i]) ;
            }
            break;
         }
      }
      switch (command[0])
      {
      
         case 0x12: //Transfer N packets of data from SFM2 to PC at the specified address locations
         fprintf (PC, "Start 0x12\r\n") ;
         output_low (PIN_A5);
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         packet_data[0] = command[5]<<8;
         packet_data[1] = command[6];
         packet = (packet_data[0] + packet_data[1])*81;
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         fprintf (PC, "Finish 0x12\r\n") ;
         break;
         
         case 0x13://Uplink command to write the data on Flash Memory 2
         output_high (PIN_A5) ;//Mission side
         fprintf (PC, "Start 0x14\r\n") ;
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         //sector_erase_SMF (address);
         WRITE_DATA_BYTE_SMF (address, command[5]) ;
         WRITE_DATA_BYTE_SMF (address + 1, command[6]) ;
         WRITE_DATA_BYTE_SMF (address + 2, command[7]) ;
         WRITE_DATA_BYTE_SMF (address + 3, command[8]) ;
         fprintf (PC, "Finish 0x14\r\n");
         break;
         
         case 0x14://Uplink command to write the data on Flash Memory 2
         output_low (PIN_A5) ;//Main side
         fprintf (PC, "Start 0x14\r\n") ;
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         //sector_erase_SMF (address);
         WRITE_DATA_BYTE_SMF (address, command[5]) ;
         WRITE_DATA_BYTE_SMF (address + 1, command[6]) ;
         WRITE_DATA_BYTE_SMF (address + 2, command[7]) ;
         WRITE_DATA_BYTE_SMF (address + 3, command[8]) ;
         fprintf (PC, "Finish 0x14\r\n");
         break;
         
         case 0x16://Erase the data on SFM2 at the given address. Specify how much data to erase eg. 16 00 01 02 03 FF 00 00 will erase the entire 64kB sector at address 00010203
         output_low (PIN_A5);
         fprintf(PC, "Start 0x16\r\n");
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         packet_data[0] = command[5]<<8;
         packet_data[1] = command[6];
         packet = (packet_data[0] + packet_data[1])*81;
            switch(command[7]){
               case 0x04:
                  SUBSECTOR_4KB_ERASE_SMF(address);
                  fprintf(PC, "Finish 0x16 4KB Erase\r\n");
                  TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
                  break;
               case 0x32:
                  SUBSECTOR_32KB_ERASE_SMF(address);
                  TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
                  fprintf(PC, "Finish 0x16 32KB Erase\r\n");
                  break;
               case 0xFF:
                  SECTOR_ERASE_SMF(address);
                  TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
                  fprintf(PC, "Finish 0x16 Sector Erase\r\n");
                  break;
            }
         break;
         
         /////////////////////FOR CAM1 RPi on MB1////////////////////////////////
         
         case 0x20: //Turn on CAM1 RPi DIO for MOSFET on MB1 to power RPI from 5V
         
            output_high (PIN_A5); //SFM2 mission side access
            fprintf (PC, "Start 0x20\r\n") ;
            output_high(pin_G3); //Turn on DIO for MULTSPEC CAM1
            fprintf (PC, "Finish 0x20\r\n"); 
         break;
         
         case 0x21: //Turn off CAM1 RPi for MOSFET on MB1 to power RPI from 5V
         
            output_high (PIN_A5); //SFM2 mission side access
            fprintf (PC, "Start 0x21\r\n") ;
            output_low(pin_G3); //Turn off DIO for MULTSPEC CAM1
            fprintf (PC, "Finish 0x21\r\n");
         break;
         
         case 0x22: //Turn on CAM1 RPi trigger
         
            output_high (PIN_A5); //SFM2 mission side access
            fprintf (PC, "Start 0x22\r\n") ;
            output_high(pin_G2); //Turn on DIO for trigger
            delay_ms(10000);
            output_low(pin_G2); //Turn off DIO for trigger
            fprintf (PC, "Finish 0x22\r\n"); 
         break;
         
         case 0x23:
         
            //Capture in mode 1
            //(1 image, captured immediately, saved to specified address) 
            //eg. 23 is command, 00 02 06 08 is the address location, 01 is number of images to capture, 00 00 00 for remaining unused command bytes (command: 23 00 02 06 08 01 00 00 00)
            fprintf (PC, "Start 0x23\r\n") ;
            output_high (PIN_A5); //SFM2 mission side access
            
            for (i = 1; i < 9; i++)
            {
               fputc(command[i], DC);
               delay_ms(20);
               fputc(command[i], PC);
            }
            
            fprintf (PC, "Finish 0x23\r\n");
         break;
         
         case 0x24: //Turn on CAM1 RPi trigger at specific time from RESET_PIC time information

            fprintf (PC, "Start 0x24\r\n") ;
            output_low (PIN_A5);
            
            //read command
            command_time_data[0] = command[1];
            command_time_data[1] = command[2];
            command_time_data[2] = command[3];
            command_time_data[3] = command[4];
            command_time_data[4] = command[5];
            
            //read time from reset_pic and compare command time and reset pic time every ten seconds
            
            for (j = 0; j<9; j++)
            {
               GET_TIME();
               delay_ms(20);
               result = trigger_time_data % command_time_data;
            
               if (result = 0)
               
                  {
                     fprintf(PC, "Trigger time occurred\r\n");
                     //output_high(); Turn on DIO for MULTSPEC CAM1
                     //delay_ms(10000);
                     //output_low() Turn off DIO for MULTSPEC CAM1
                  }
                  
               else
  
                  {
                     fprintf(PC, "No trigger\r\n");
                     delay_ms(5000);
                  }
            }
            result = 1;
            fprintf (PC, "Finish 0x24\r\n");    
         break;
         
         /////////////////////FOR CAM2 RPi on MB2////////////////////////////////
         
        case 0x30: //Turn on CAM2 RPi DIO for MOSFET on MB2 to power RPI from 5V
         
            output_high (PIN_A5); //SFM2 mission side access
            fprintf (PC, "Start 0x30\r\n") ;
            output_high(pin_F6); //Turn on DIO for MULTSPEC CAM1
            fprintf (PC, "Finish 0x30\r\n"); 
         break;
         
         case 0x31: //Turn off CAM1 RPi for MOSFET on MB1 to power RPI from 5V
         
            output_high (PIN_A5); //SFM2 mission side access
            fprintf (PC, "Start 0x31\r\n") ;
            output_low(pin_F6); //Turn off DIO for MULTSPEC CAM1
            fprintf (PC, "Finish 0x31\r\n");
            
         break;
         
         case 0x32: //Turn on CAM1 RPi trigger
         
            output_high (PIN_A5); //SFM2 mission side access
            fprintf (PC, "Start 0x32\r\n") ;
            output_high(pin_F7); //Turn on DIO for trigger
            delay_ms(10000);
            output_low(pin_F7); //Turn off DIO for trigger
            fprintf (PC, "Finish 0x32\r\n");   
         break;
         
         case 0x33: 
            //Capture in mode 1
            //(1 image, captured immediately, saved to specified address) 
            //eg. 23 is command, 00 02 06 08 is the address location, 01 is number of images to capture, 00 00 00 for remaining unused command bytes (command: 23 00 02 06 08 01 00 00 00)
            
            fprintf (PC, "Start 0x33\r\n") ;
            
            output_high (PIN_A5); //SFM2 mission side access
            
            for (i = 1; i < 9; i++)
            {
               fputc(command[i], DC);
               delay_ms(20);
               fputc(command[i], PC);
            }
            
            fprintf (PC, "Finish 0x33\r\n");
         break;
         
         case 0x34: //Turn on CAM1 RPi trigger at specific time from RESET_PIC time information

            fprintf (PC, "Start 0x34\r\n") ;
            output_low (PIN_A5);
            
            //read command
            command_time_data[0] = command[1];
            command_time_data[1] = command[2];
            command_time_data[2] = command[3];
            command_time_data[3] = command[4];
            command_time_data[4] = command[5];
            
            //read time from reset_pic and compare command time and reset pic time every ten seconds
            
            for (j = 0; j<9; j++)
            {
               GET_TIME();
               delay_ms(20);
               result = trigger_time_data % command_time_data;
            
               if (result = 0)            
                  {
                     fprintf(PC, "Trigger time occurred\r\n");
                     //output_high(); Turn on DIO for MULTSPEC CAM1
                     //delay_ms(10000);
                     //output_low() Turn off DIO for MULTSPEC CAM1
                  }
                  
               else
  
                  {
                     fprintf(PC, "No trigger\r\n");
                     delay_ms(5000);
                  }
            }
            result = 1;
            fprintf (PC, "Finish 0x34\r\n");    
         break;
         
         case 0x35: //Turn on CAM1 and CAM2 RPi trigger
         
            output_high (PIN_A5); //SFM2 mission side access
            fprintf (PC, "Start 0x35\r\n") ;
            output_high(pin_F7); //Turn on DIO for trigger MB2
            output_high(pin_G2); //Turn on DIO for trigger MB1
            delay_ms(10000);
            output_low(pin_F7); //Turn off DIO for trigger
            output_low(pin_G2); //Turn off DIO for trigger
            fprintf (PC, "Finish 0x35\r\n");   
            
         break;
         
         case 0x36: //Turn on CAM1 and CAM2 RPi
         
            output_high (PIN_A5); //SFM2 mission side access
            fprintf (PC, "Start 0x36\r\n") ;
            output_high(pin_G3); //Turn on DIO for trigger MB2
            delay_ms(5000);
            output_high(pin_F6); //Turn on DIO for trigger MB1
            fprintf (PC, "Finish 0x36\r\n");   
            
         break;
      }
   }
}

void IMGCLS_Test()
{
   dummy[0] = 0x01;

   int32 num;
   
   while (TRUE)
   {
      command[0] = 0x00;
      
      for (num = 0; num < 100; num++)
      {
         if (kbhit (PC))
         {

            for (int i = 0; i < 9; i++)
            {
               command[i] = fgetc (PC);
            }
            break;
         }
      }
      switch (command[0])
      {
      
         case 0x12: //Transfer N packets of data from SFM2 to PC at the specified address locations
         fprintf (PC, "Start 0x12\r\n") ;
         output_low (PIN_A5);
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         packet_data[0] = command[5]<<8;
         packet_data[1] = command[6];
         packet = (packet_data[0] + packet_data[1])*81;
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         fprintf (PC, "Finish 0x12\r\n") ;
         break;
         
         
         case 0x16://Erase the data on SFM2 at the given address. Specify how much data to erase eg. 16 00 01 02 03 FF will erase the entire 64kB sector at address 00010203
         output_low (PIN_A5);
         fprintf(PC, "Start 0x16\r\n");
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
            switch(command[5]){
               case 0x04:
                  SUBSECTOR_4KB_ERASE_SMF(address);
                  fprintf(PC, "Finish 0x16\r\n");
                  break;
               case 0x32:
                  SUBSECTOR_32KB_ERASE_SMF(address);
                  fprintf(PC, "Finish 0x16\r\n");
                  break;
               case 0xFF:
                  SECTOR_ERASE_SMF(address);
                  fprintf(PC, "Finish 0x16\r\n");
                  break;
            }
         break;
         
         
         case 0x80: //Turn on IMGCLS RPi DIO for MOSFET on RAB to power RPI from 5V line
         
            output_high (PIN_A5); //SFM2 mission side access
            fprintf (PC, "Start 0x20\r\n") ;
            output_high (PIN_A5); //SFM2 mission side access
            
            for (i = 1; i < 9; i++)
            {
               fputc(command[i], DC);
               delay_ms(20);
               fputc(command[i], PC);
            }
            
            fprintf (PC, "Finish 0x20\r\n"); 
         break;
         
         case 0x81: //Turn off IMGCLS RPi for MOSFET on RAB to power RPI from 5V
         
            output_high (PIN_A5); //SFM2 mission side access
            fprintf (PC, "Start 0x21\r\n") ;
            output_high (PIN_A5); //SFM2 mission side access
            
            for (i = 1; i < 9; i++)
            {
               fputc(command[i], DC);
               delay_ms(20);
               fputc(command[i], PC);
            }
            
            fprintf (PC, "Finish 0x21\r\n");
            
         break;
         
         case 0x82:
         
            //Capture in mode 1
            //(1 image, captured immediately, saved to specified address) 
            //eg. 23 is command, 00 02 06 08 is the address location, 01 is number of images to capture, 00 00 00 for remaining unused command bytes (command: 23 00 02 06 08 01 00 00 00)
            
            fprintf (PC, "Start 0x23\r\n") ;
            
            output_high (PIN_A5); //SFM2 mission side access
            
            for (i = 1; i < 9; i++)
            {
               fputc(command[i], DC);
               delay_ms(20);
               fputc(command[i], PC);
            }
            
            fprintf (PC, "Finish 0x23\r\n");
         break;
        
      }
   }
}

void GET_RESET_DATA()
{
   dummy[0] = 0x11;
   Finish_sign[0] = 0xFF;
   RESET_DATA = 0;
   int8 dayh;
   int8 dayl;
   int8 hr;
   int8 min;
   int8 sec;
   int8 j;
   for (int i = 0; i < 6; i++)
   {
      COLLECT_RESET_DATA ();
      if (reset_bffr[0] == 0x8e)
      {
         break;
      }
   }
   if (RESET_bffr[0] == 0x8e)
   {
      // fprintf (PC, "\r\nRESET DATA OBTAINED\r\n") ;
      for (int num = 0; num < 5; num++)   //carga el array HKDATA con timedata en las posiciones 2 al 6
      {
         reset_time_data[num] = reset_bffr[num + 1];
         //fputc (reset_bffr[num + 1], DC);
         //fprintf (PC, " % x, ", reset_bffr[num + 1]) ;
      }
      //fputc (dummy[0], DC);
      sec = reset_time_data[0];
      min = reset_time_data[1];
      hr = reset_time_data[2];
      dayl = reset_time_data[3];
      dayh = reset_time_data[4];
      fprintf (PC, "\r\n") ;
      
      PINO_DATA[0] = 0xFC; //This the header for the Time attitude data.
      
      PINO_DATA[1] = reset_time_data[0];
      PINO_DATA[2] = reset_time_data[1];
      PINO_DATA[3] = reset_time_data[2];
      PINO_DATA[4] = reset_time_data[3];
      PINO_DATA[5] = reset_time_data[4];
      
      for(i=6; i<39; i++)
      {
         PINO_DATA[i] = 0x01;

      }
      fprintf(PC,"Send Data\r\n");

      for (j = 0; j < 39; j++)
      {
         //fprintf (fab, " % x, ", PINO_DATA[j]) ;
         
         fputc (PINO_DATA[j], DC);
         
      }
      //fputc (Finish_sign[0], DC);
      
      //fprintf (fab, "\r\n") ;
      
      }else{
      fprintf (PC, "\r\nRESET DATA NOT OBTAINED\r\n") ;
   }
   return;
}


void GET_TIME()
{
   dummy[0] = 0x11;

   for (int i = 0; i < 6; i++)
   {
      COLLECT_RESET_DATA ();
      if (reset_bffr[0] == 0x8e)
      {
         break;
      }
   }
   
   if (RESET_bffr[0] == 0x8e)
   {
   
      fprintf (PC, "\r\nRESET DATA OBTAINED\r\n") ;
      
      for (int num = 0; num < 5; num++)
      {
         reset_time_data[num] = reset_bffr[num + 1];
         fputc (reset_bffr[num + 1], DC);
         fputc (reset_bffr[num + 1], PC) ;
      }
      
      fputc (dummy[0], DC);
      trigger_time_data[0] = reset_time_data[0];
      trigger_time_data[1] = reset_time_data[1];
      trigger_time_data[2] = reset_time_data[2];
      trigger_time_data[3] = reset_time_data[3];
      trigger_time_data[4] = reset_time_data[4];
      fprintf (PC, "\r\n") ;
      
      
   }
   
   else
   
   {
      fprintf (PC, "\r\nRESET DATA NOT OBTAINED\r\n") ;
      return;
   }
   
   return;
}

