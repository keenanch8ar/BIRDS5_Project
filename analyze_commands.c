void UPLINK_SUCCESS_CHECK()
{
   if(UPLINK_SUCCESS == 0)
   {
      UPLINK_SUCCESS = 1;
      STORE_FLAG_INFO();                                                         //save flag data to flash memory
      WRITE_FLAG_to_EEPROM();                                                    //save flags on EEPROM from address 0x18000 (75%)
   }
   return;
}


void UPLINK_SUCCESS_HSSC()
{
   if(UPLINK_SUCCESS == 0)
   {
      UPLINK_SUCCESS = 1;
      FIRST_HSSC_DONE = 1;
      STORE_FLAG_INFO();
      WRITE_FLAG_to_EEPROM();
   }else if(FIRST_HSSC_DONE == 0){
      FIRST_HSSC_DONE = 1;
      STORE_FLAG_INFO();
      WRITE_FLAG_to_EEPROM();
   }
   return;
}


void UPLINK_SUCCESS_MBP()
{
   if(UPLINK_SUCCESS == 0)
   {
      UPLINK_SUCCESS = 1;
      AUTO_MBP_DONE = 1;
      STORE_FLAG_INFO();                                                         //store flag info on flash
      WRITE_FLAG_to_EEPROM();                                                    //store flag info on EEPROM
   }else if(AUTO_MBP_DONE == 0){
      AUTO_MBP_DONE = 1;
      STORE_FLAG_INFO();                                                         //store flag info on flash
      WRITE_FLAG_to_EEPROM();                                                    //store flag info on EEPROM
   }
   return;
}


void UPLINK_SUCCESS_MULT_CAM()
{
   if(UPLINK_SUCCESS == 0)
   {
      AUTO_CAM_DONE = 1;
      UPLINK_SUCCESS = 1;
      STORE_FLAG_INFO();                                                         //store flag info on flash
      WRITE_FLAG_to_EEPROM();                                                    //store flag info on EEPROM
   }else if(AUTO_CAM_DONE == 0){
      AUTO_CAM_DONE = 1;
      STORE_FLAG_INFO();                                                         //store flag info on flash
      WRITE_FLAG_to_EEPROM();                                                    //store flag info on EEPROM
   }
   return;
}


void EXECUTE_COMMAND_from_PC(int8 CMD1,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6,int8 CMD7, int8 CMD8, int8 CMD9)
{
   switch(CMD1)
   {
   
      case 0x01:
         UPLINK_SUCCESS_CHECK();
         fprintf(PC,"Collect HK Data From FAB: ");
         fputc(0x61, FAB);
         FAB_DATA = 0;
         
      break;
      
      
      case 0x02:
         UPLINK_SUCCESS_CHECK();
         fprintf(PC,"Collect ADCS Data From FAB: ");
         fputc(0x69, FAB);
         FAB_DATA = 0;
         
      break;
      
      
      case 0x03:
         UPLINK_SUCCESS_CHECK();
         fprintf(PC,"Collect Data from RESET: \r\n");
         RESET_DATA = 0;
         int8 dayh;
         int8 dayl;
         int8 hr;
         int8 min;
         int8 sec;
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
            //fprintf (PC, "\r\nRESET DATA OBTAINED\r\n") ;
            for (int num = 0; num < 5; num++)
            {
               reset_time_data[num] = reset_bffr[num + 1];
               //fputc (reset_bffr[num + 1], DC);
               //fprintf (PC, " %x, ", reset_bffr[num + 1]) ;
            }
            sec = reset_time_data[0];
            min = reset_time_data[1];
            hr = reset_time_data[2];
            dayl = reset_time_data[3];
            dayh = reset_time_data[4];
            //fprintf (PC, "\r\n") ;           
         }
         
         else
         {
            fprintf (PC, "\r\nRESET DATA NOT OBTAINED\r\n") ;
            return;
         }

      break;
      
      
      case 0x04:
         UPLINK_SUCCESS_CHECK();
         fprintf(PC, "\r\nBPB POWER ON\r\n");
         Turn_ON_MBP();
         
      break;
         
      
      case 0x05:
         UPLINK_SUCCESS_CHECK();
         fprintf(PC, "\r\nBPB POWER OFF\r\n");
         Turn_OFF_MBP();
         
      break;
      
      case 0x06:
         UPLINK_SUCCESS_CHECK();
         fprintf(PC, "\r\nRefresh Reservation Table\r\n");
         table_refresh();
         
      break;
      
      case 0x07:
         UPLINK_SUCCESS_CHECK();
         fprintf(PC, "\r\nDisplay Reservation Table\r\n");
         Disp_RSV();
         
      break;
      
      case 0x08:
         UPLINK_SUCCESS_CHECK();
         fprintf(PC, "\r\nRESET SATELLITE\r\n");
         SEND_CMD_FOR_RESET_SATELLITE ();
         
      break;
      
      
      case 0x12:
         UPLINK_SUCCESS_CHECK();
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
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         fprintf (PC, "Finish 0x12\r\n") ;
         
      break;
      
      
      case 0x14://Uplink command to write the data on Flash Memory 2
         UPLINK_SUCCESS_CHECK();
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
         
      break;
      
      
      case 0x16://Erase the data on Flash Memory 2
         UPLINK_SUCCESS_CHECK();
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
            
      break;    
            
      case 0x17:
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x17\r\n") ;
         for (i = 1; i < 9; i++)
         {
            fputc(command[i], DC);
            delay_ms(20);
            fputc(command[i], PC);
         }
         fprintf (PC, "\r\n");
         fprintf (PC, "From SMF:\r\n");
         //wait for MB to say MB1 RPI finished copying the image to SF2
        
         // Transfer MULTSPEC data from SF2 to PC and to SCF
         output_low (PIN_A5); // Main side
         
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = command[5]<<8;
         packet_data[1] = command[6];
         packet = (packet_data[0] + packet_data[1])*81;
         
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         delay_ms(1000);
         fprintf (PC, "From SCF:\r\n");
         output_low (PIN_C4); // Main side SCF
         sector_erase_SCF(address);
         TRANSFER_DATA_NBYTE_SMFtoSCF(address, address, packet);
         delay_ms(1000);
         TRANSFER_DATA_NBYTE_TOPC_SCF(address, packet);
         
         output_high(PIN_C4); // COMM side
         
         fprintf (PC, "\r\n");
         fprintf (PC, "Finish 0x17\r\n");
         
      break;
      
      case 0x18:
      
         ERASE_EEPROM_INFO();                                                    //erase 512byte(from 0x18000 to 0x181ff)
         MEMORY_ERASE(); 
         
      break;
      
      case 0x19:
      
         unsigned int16 duration = (unsigned int16)CMD3*6;                //CMD2 is operation time(min), maximum number of readings in 2 hours = 720
         if(duration > 720){duration = 720;}                                   // 6 readings in 1 min, every 10 seconds
         MISSION_STATUS = 1;
         HIGHSAMP_SENSOR_COLLECTION(duration);
         MISSION_STATUS = 0;

      break;
      
      ////////////////////////////FOR CAM1 RPi on MB1////////////////////////////////
      
      case 0x20:                                                                 //Turn off CAM1 RPi DIO for MOSFET on MB1 to power RPI from 5V      
         
         output_high (PIN_A5);                                                   //SFM2 mission side access
         fprintf (PC, "Start 0x20 - Turn OFF MULTSPEC CAM1 (MB1)\r\n") ;
         output_low(pin_G3);                                                     //Turn off DIO for MULTSPEC CAM1
         fprintf (PC, "Finish 0x20\r\n");
         
      break;
      
      case 0x21: //Real time uplink command
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x21 - Real time uplink MULTSPEC CAM1\r\n") ;
         Forward_CMD_MBP();
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x21\r\n");

      break;
      
      case 0x22: //Real time downlink command
      
         fprintf (PC, "Start 0x22 - Request MULT-SPEC MB1 Downlink Data\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         Forward_CMD_MBP();
         int8 counter = 0;
         for(int32 num = 0; num < 1500000; num++)
            {
               if(kbhit(DC))
               {
                  MULTSPEC1_DATA[counter] = fgetc(DC);
                  counter++;
                  if(counter == 81)
                  {
                     break;
                  }
               }
            }
         fprintf(PC,"Data Recieved: ");
         for(int l = 0; l < 81; l++)
         {
            fprintf(PC,"%x",MULTSPEC1_DATA[l]);
         }
         fprintf(PC,"\r\n");
         fprintf (PC, "Finish 0x22\r\n");
         for(l = 0; l < 81; l++)
         {
            MULTSPEC1_DATA[l] = 0;
         }

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
      
      case 0x24: //Turn on CAM1 RPi trigger at specific time from Reservation time

         fprintf (PC, "Start 0x24\r\n") ;
         output_low (PIN_A5);
         RESERVE_TARGET_FLAG = 3;
         fprintf (PC, "Finish 0x24\r\n");    
      break;
      
      case 0x25: //Copy specific images based on selected thumbnails selected from GS. Forwward all mission command data (8 bytes) to MB, MB will send command data (specific image name) to MB1 RPI
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x25\r\n") ;
         for (i = 1; i < 9; i++)
         {
            fputc(command[i], DC);
            delay_ms(20);
            fputc(command[i], PC);
         }
         fprintf (PC, "\r\n");
         fprintf (PC, "From SMF:\r\n");
         //wait for MB to say MB1 RPI finished copying the image to SF2
        
         // Transfer MULTSPEC data from SF2 to PC and to SCF
         output_low (PIN_A5); // Main side
         
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = command[5]<<8;
         packet_data[1] = command[6];
         packet = (packet_data[0] + packet_data[1])*81;
         
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         delay_ms(1000);
         fprintf (PC, "From SCF:\r\n");
         output_low (PIN_C4); // Main side SCF
         sector_erase_SCF(address);
         TRANSFER_DATA_NBYTE_SMFtoSCF(address, address, packet);
         delay_ms(1000);
         TRANSFER_DATA_NBYTE_TOPC_SCF(address, packet);
         
         fprintf (PC, "\r\n");
         fprintf (PC, "Finish 0x25\r\n");
         
      break;
      
      case 0x2E: //Turn on CAM1 RPi for MOSFET on MB1 to power RPI from 5V
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start - Turn ON MULTSPEC CAM1 0x2E\r\n") ;
         output_high(pin_G3); //Turn on DIO for MULTSPEC CAM1
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x2E\r\n"); 
      
      break;
      
////////////////////////////FOR CAM2 RPi on MB2////////////////////////////////
      
     case 0x30: //Turn off CAM2 RPi DIO for MOSFET on MB2 to power RPI from 5V
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x30 - Turn OFF MULTSPEC CAM2 (MB2)\r\n") ;

         output_low(pin_F6); //Turn off DIO for MULTSPEC CAM2
         //Forward_CMD_MBP();

         fprintf (PC, "Finish 0x30\r\n");
         
         
      break;
      
      case 0x31: //Turn off CAM1 RPi for MOSFET on MB1 to power RPI from 5V
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x21 - Real time uplink MULTSPEC CAM1\r\n") ;
         Forward_CMD_MBP();
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x21\r\n");
         
      break;
      
      case 0x32: //Turn on CAM1 RPi trigger
      
         fprintf (PC, "Start 0x32 - Request MULT-SPEC MB2 Downlink Data\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         Forward_CMD_MBP();
         counter = 0;
         for(num = 0; num < 1500000; num++)
            {
               if(kbhit(DC))
               {
                  MULTSPEC2_DATA[counter] = fgetc(DC);
                  counter++;
                  if(counter == 81)
                  {
                     break;
                  }
               }
            }
         fprintf(PC,"Data Recieved: ");
         for(l = 0; l < 81; l++)
         {
            fprintf(PC,"%x",MULTSPEC2_DATA[l]);
         }

         for(l = 0; l < 81; l++)
         {
            MULTSPEC2_DATA[l] = 0;
         }
         fprintf(PC,"\r\n");
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
            //GET_TIME();
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
      
      case 0x3E: // Turn on MULTSPEC CAM2 (MB2)
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x3E - Turn ON MULTSPEC CAM2 (MB2)\r\n") ;
         output_high(pin_F6); //Turn on DIO for MULTSPEC CAM2
       
         fprintf (PC, "Finish 0x3E\r\n");
         
      break;
      
      default:
      
         fprintf (PC, "Command:%x", CMD3);
         fprintf (PC, " does not exist\r\n");
         
      break;
       
   
   }
   return;
}

int8 CHECK_MEMORY_FUNCTION(int8 data)                                            //evita operacion de memoria como comandos reservados
{
   if((data == 0xe0)||(data == 0xe1)||(data == 0xe2))
   {
      data = 0;
   }
   if((data == 0x12)||(data == 0x32)||(data == 0x21))
   {
      data = 0;
   }
   return data;
}
