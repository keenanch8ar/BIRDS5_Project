int8 Mission_check_flag = 0;

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
   }
   else if(FIRST_HSSC_DONE == 0)
   {
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
      STORE_FLAG_INFO();                                                         //store flag info on flash
      WRITE_FLAG_to_EEPROM();                                                    //store flag info on EEPROM
   }
   return;
}


void UPLINK_SUCCESS_MULT_CAM()
{
   if(UPLINK_SUCCESS == 0)
   {
      UPLINK_SUCCESS = 1;
      STORE_FLAG_INFO();                                                         //store flag info on flash
      WRITE_FLAG_to_EEPROM();                                                    //store flag info on EEPROM
   }
   return;
}


void UPLINK_SUCCESS_IMGCLS_CAM()
{
   if(UPLINK_SUCCESS == 0)
   {
      UPLINK_SUCCESS = 1;
      STORE_FLAG_INFO();                                                         //store flag info on flash
      WRITE_FLAG_to_EEPROM();                                                    //store flag info on EEPROM
   }
   return;
}

void UPLINK_SUCCESS_S_FWD()
{
   if(UPLINK_SUCCESS == 0)
   {
      UPLINK_SUCCESS = 1;
      STORE_FLAG_INFO();                                                         //store flag info on flash
      WRITE_FLAG_to_EEPROM();                                                    //store flag info on EEPROM
   }
   return;
}


void FWD_CMD_MBP(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{
   
   fputc(CMD0, DC);
   delay_ms(10);
   
   fputc(0x00, DC);                                                              //RSV time = 0x00
   delay_ms(10);
   
   fputc(CMD2, DC);
   delay_ms(10);
   fputc(CMD3, DC);
   delay_ms(10);
   fputc(CMD4, DC);
   delay_ms(10);
   fputc(CMD5, DC);
   delay_ms(10);
   fputc(CMD6, DC);
   delay_ms(10);
   fputc(CMD7, DC);
   delay_ms(10);
   fputc(CMD8, DC);
   delay_ms(10);
   
   return;
}


void MAIN_COMMAND(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{
   
   switch(CMD0)
   {
   
      case 0x00:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC,"Collect HK Data From FAB: ");
         fputc(0x61, FAB);
         FAB_DATA = 0;
      
      break;
   
      case 0x01:                                                                 //Uplink 3 bytes to SMF at specified address
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "Nothing yet\r\n");                                                    //Nothing for now
         
      break;
       
      case 0x02:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "Nothing yet\r\n");                                                    //Nothing for now
         
      break;
     
      case 0x03:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nRESET SATELLITE\r\n");
         SEND_CMD_FOR_RESET_SATELLITE ();
         
      break;
      
      case 0x04:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nKILL SWITCH\r\n");
         Count_Kill_Flag();                                                      //Enable if Kill_COUNTER > 4
         SAVE_SAT_LOG(CMD0, KILL_COUNTER, CMD2);                                 //save reset data and command
      
      break;
      
      case 0x05:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nDISABLE KILL SWITCH\r\n");
         Disable_Kill();                                                         //Kill Switch OFF
         SAVE_SAT_LOG(CMD0, KILL_COUNTER, CMD2);                                 //save reset data and command
      
      break;
      
      case 0x06:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nBPB POWER ON\r\n");
         Turn_ON_MBP();
         
      break;
            
      case 0x07:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nBPB POWER OFF\r\n");
         Turn_OFF_MBP();
         
      break;
      
      case 0x08:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nTurn On ADCS\r\n");
         Turn_On_ADCS();
         
      break;
      
      case 0x09:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nTurn Off ADCS\r\n");
         Turn_Off_ADCS();
         
      break;
        
      case 0x0A:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nRefresh Reservation Table\r\n");
         table_refresh();
         
      break;
      
      case 0x0B:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nDisplay Reservation Table\r\n");
         Disp_RSV();
         
      break;
      
      case 0x0C:
         
         fprintf(PC, "\r\nGive Access to SCF\r\n");
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         GIVE_ACCESS_SCF_Nsec(CMD2);
      
      break;
      
      case 0x0D:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nNothing Yet\r\n");

      
      break;
      
      case 0x0E:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\nNothing Yet\r\n");

      
      break;
      
      case 0x0F:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         GIVE_ACCESS_SCF_Nsec(CMD8);
         fprintf(PC, "\r\nNothing Yet\r\n");
      
      break;
          
      case 0x10:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\Erase Flash Operation for Main Flash\r\n");
         output_low (PIN_A5);                                                    //MF Main Side
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         switch(CMD6)
         {
            case 0x04:
               SUBSECTOR_4KB_ERASE_OF(address);
               fprintf(PC, "Finish 4kb erase\r\n");
               break;
            case 0x32:
               SUBSECTOR_32KB_ERASE_OF(address);
               fprintf(PC, "Finish 32kb erase\r\n");
               break;
            case 0xFF:
               SECTOR_ERASE_OF(address);
               fprintf(PC, "Finish 64kb erase\r\n");
               break;
            default:
               fprintf(PC, "error\r\n");
               break;
         }
         
      break;
      
      case 0x11:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\Erase Flash Operation for Shared COM Flash\r\n");
         output_low (PIN_C4);                                                      // Main side SCF
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         switch(CMD6)
         {
            case 0x04:
               SUBSECTOR_4KB_ERASE_SCF(address);
               fprintf(PC, "Finish 4kb erase\r\n");
               break;
            case 0x32:
               SUBSECTOR_32KB_ERASE_SCF(address);
               fprintf(PC, "Finish 32kb erase\r\n");
               break;
            case 0xFF:
               SECTOR_ERASE_SCF(address);
               fprintf(PC, "Finish 64kb erase\r\n");
               break;
            default:
               fprintf(PC, "error\r\n");
               break;
         }
         
      break;
      
      case 0x12:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "\r\Erase Flash Operation for Shared Mission Flash\r\n");
         output_low (PIN_A5);                                                    //MF Main Side
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         switch(CMD6)
         {
            case 0x04:
               SUBSECTOR_4KB_ERASE_SMF(address);
               fprintf(PC, "Finish 4kb erase\r\n");
               break;
            case 0x32:
               SUBSECTOR_32KB_ERASE_SMF(address);
               fprintf(PC, "Finish 32kb erase\r\n");
               break;
            case 0xFF:
               SECTOR_ERASE_SMF(address);
               fprintf(PC, "Finish 64kb erase\r\n");
               break;
            default:
               fprintf(PC, "error\r\n");
               break;
         }
         output_high(PIN_A5);
         
      break;
      
      case 0x13:
      
         fprintf(PC, "\r\nERASE ALL FLASH OPERATION\r\n");
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         ERASE_EEPROM_INFO(CMD2,CMD3,CMD4);                                      //erase 512byte(from 0x18000 to 0x181ff)
         MEMORY_ERASE(CMD2,CMD3,CMD4);                                           //executes if CMD2 == 0xAA, CMD3 == 0xBB and CMD4 = 0xCC to prevent unintentional deletion
      
      break;
      
      case 0x14:
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf (PC, "\r\nWrite 3 bytes to SMF\r\n") ;
         output_low (PIN_A5) ;                                                   //Main side
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         SUBSECTOR_4KB_ERASE_SMF(address);
         WRITE_DATA_BYTE_SMF (address, CMD6);
         WRITE_DATA_BYTE_SMF (address + 1, CMD7);
         WRITE_DATA_BYTE_SMF (address + 2, CMD8);
         
      break;
      
      
      case 0x15:                                                                 //Uplink command to write the data on Flash Memory 2
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf(PC, "Nothing yet\r\n");                                                    //Nothing for now

 
      break;
      
      
      case 0x16:                                                                 //Transfer 1 sector from MF to SCF
         
         fprintf(PC, "\r\Transfer 1 sector from MF to SCF\r\n");
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;                                                   //put uplink success flag in HIGH and store flags  
         TRANSFER_SECTOR_OF2SCF(CMD2,CMD3,CMD4,CMD5);                            //transfer 1 sector, also this function contains saving log
         
      break;    
      
            
      case 0x17:                                                                 //Transfer 1 sector from SMF to SCF
         
         fprintf(PC, "\r\Transfer 1 sector from SMF to SCF\r\n");
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;                                                   //put uplink success flag in HIGH and store flags  
         TRANSFER_SECTOR_SMF2SCF(CMD2,CMD3,CMD4,CMD5);                           //transfer 1 sector, also this function contains saving log
       
      break;
      
      
      case 0x18:                                                                 //Transfer 1 sector from SCF to MF
         
         fprintf(PC, "\r\Transfer 1 sector from SMF to MF\r\n");
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;                                                   //put uplink success flag in HIGH and store flags  
         TRANSFER_SECTOR_SMF2OF(CMD2,CMD3,CMD4,CMD5);                            //transfer 1 sector, also this function contains saving log
       
      break;
      
      case 0x19:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf (PC, "\r\nTransfer N Packets from OF to SCF\r\n") ;
         fprintf (PC, "From OF:\r\n");
         
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81;
         
         TRANSFER_DATA_NBYTE_TOPC_OF(address, packet);
         delay_ms(1000);
         fprintf (PC, "From SCF:\r\n");
         output_low (PIN_C4); // Main side SCF
         sector_erase_SCF(address);                                   
         TRANSFER_DATA_NBYTE_OFtoSCF(address, address, packet);
         delay_ms(1000);
         TRANSFER_DATA_NBYTE_TOPC_SCF(address, packet);
         
         output_high(PIN_C4); // COMM side
         

      break;
      
      
      case 0x1A:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf (PC, "\r\nTransfer N Packets from SMF to SCF\r\n") ;
         fprintf (PC, "From SMF:\r\n");
         
         output_low (PIN_A5);                                                    // Main side
         
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
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


      break;
      
      case 0x1B:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf (PC, "\r\nTransfer N Packets from SMF to SCF\r\n") ;
         fprintf (PC, "From SMF:\r\n");
         
         output_low (PIN_A5); // Main side
         
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81;
         
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         delay_ms(1000);
         fprintf (PC, "From OF:\r\n");
         sector_erase_SCF(address);
         TRANSFER_DATA_NBYTE_SMFtoOF(address, address, packet);
         delay_ms(1000);
         TRANSFER_DATA_NBYTE_TOPC_OF(address, packet);
         
         output_high(PIN_C4); // COMM side

      
      break;
      
      case 0x1C:                                                                 //Uplink command to write the data on Flash Memory 2
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf (PC, "\r\nRead N packets from SCF to PC\r\n") ;
         
         output_low (PIN_C4);                                                    // Main side
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81;  
         TRANSFER_DATA_NBYTE_TOPC_SCF(address, packet);
         output_high(PIN_C4); // COMM side
          
         fprintf (PC, "\r\nFinish 0x1C\r\n") ;

      break;
      
      
      case 0x1D:                                                                 //Uplink command to write the data on Flash Memory 2
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_CHECK();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  
         fprintf (PC, "\r\nRead N packets from MF to PC\r\n") ;

         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81; 
         TRANSFER_DATA_NBYTE_TOPC_OF(address, packet);
         
         fprintf (PC, "\r\nFinish 0x1D\r\n") ;

      break;
      
      case 0x1E:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_HSSC();                                                  //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;  

         unsigned int16 duration = (unsigned int16)CMD2*6;                      //CMD2 is operation time(min), maximum number of readings in 2 hours = 720
         if(duration > 720){duration = 720;}                                    // 6 readings in 1 min, every 10 seconds
         HIGHSAMP_SENSOR_COLLECTION(duration);                                  //remove mission status issue as it will prevent Hssc from getting adcs data
      
      break;
      
      case 0x1F:                                                                 //Uplink command to write the data on Flash Memory 2
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_HSSC();                                                  //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         fprintf (PC, "\r\nRead N packets from SMF to PC\r\n") ;
         output_low (PIN_A5);
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81;
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         fprintf (PC, "\r\nFinish 0x1F\r\n") ;

      break;
      
      default:
         
         fprintf (PC, "Command:%x", CMD0);
         fprintf (PC, " does not exist\r\n");
         
      break;
      
   }
   
   return;
}


void MAIN_COMMAND_PC(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{
   
   switch(CMD0)
   {
   
      case 0x00:
      
         fprintf(PC,"Collect HK Data From FAB: ");
         fputc(0x61, FAB);
         FAB_DATA = 0;
      
      break;
   
      case 0x01:                                                                 //Uplink 3 bytes to SMF at specified address
      
         fprintf(PC, "Nothing yet\r\n");                                                    //Nothing for now
         
      break;
       
      case 0x02:

         fprintf(PC, "Nothing yet\r\n");                                                    //Nothing for now
         
      break;
     
      case 0x03:
      
         fprintf(PC, "\r\nRESET SATELLITE\r\n");
         SEND_CMD_FOR_RESET_SATELLITE ();
         
      break;
      
      case 0x04:

         fprintf(PC, "\r\nKILL SWITCH\r\n");
         Count_Kill_Flag();                                                      //Enable if Kill_COUNTER > 4
         SAVE_SAT_LOG(CMD0, KILL_COUNTER, CMD2);                                 //save reset data and command
      
      break;
      
      case 0x05:

         fprintf(PC, "\r\nDISABLE KILL SWITCH\r\n");
         Disable_Kill();                                                         //Kill Switch OFF
         SAVE_SAT_LOG(CMD0, KILL_COUNTER, CMD2);                                 //save reset data and command
      
      break;
      
      case 0x06:

         fprintf(PC, "\r\nBPB POWER ON\r\n");
         Turn_ON_MBP();
         
      break;
            
      case 0x07:

         fprintf(PC, "\r\nBPB POWER OFF\r\n");
         Turn_OFF_MBP();
         
      break;
      
      case 0x08:

         fprintf(PC, "\r\nTurn On ADCS\r\n");
         Turn_On_ADCS();
         
      break;
      
      case 0x09:

         fprintf(PC, "\r\nTurn Off ADCS\r\n");
         Turn_Off_ADCS();
         
      break;
        
      case 0x0A:

         fprintf(PC, "\r\nRefresh Reservation Table\r\n");
         table_refresh();
         
      break;
      
      case 0x0B:

         fprintf(PC, "\r\nDisplay Reservation Table\r\n");
         Disp_RSV();
         
      break;
      
      case 0x0C:
         
         fprintf(PC, "\r\nGive Access to SCF\r\n");
         GIVE_ACCESS_SCF_Nsec(CMD2);
      
      break;
      
      case 0x0D:
         
         fprintf(PC, "\r\nNothing Yet\r\n");

      
      break;
      
      case 0x0E:

         fprintf(PC, "\r\nNothing Yet\r\n");

      
      break;
      
      case 0x0F:

         fprintf(PC, "\r\nNothing Yet\r\n");
      
      break;
          
      case 0x10:

         fprintf(PC, "\r\Erase Flash Operation for Main Flash\r\n");
         output_low (PIN_A5);                                                    //MF Main Side
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         switch(CMD6)
         {
            case 0x04:
               SUBSECTOR_4KB_ERASE_OF(address);
               fprintf(PC, "Finish 4kb erase\r\n");
               break;
            case 0x32:
               SUBSECTOR_32KB_ERASE_OF(address);
               fprintf(PC, "Finish 32kb erase\r\n");
               break;
            case 0xFF:
               SECTOR_ERASE_OF(address);
               fprintf(PC, "Finish 64kb erase\r\n");
               break;
            default:
               fprintf(PC, "error\r\n");
               break;
         }
         
      break;
      
      case 0x11:

         fprintf(PC, "\r\Erase Flash Operation for Shared COM Flash\r\n");
         output_low (PIN_C4);                                                      // Main side SCF
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         switch(CMD6)
         {
            case 0x04:
               SUBSECTOR_4KB_ERASE_SCF(address);
               fprintf(PC, "Finish 4kb erase\r\n");
               break;
            case 0x32:
               SUBSECTOR_32KB_ERASE_SCF(address);
               fprintf(PC, "Finish 32kb erase\r\n");
               break;
            case 0xFF:
               SECTOR_ERASE_SCF(address);
               fprintf(PC, "Finish 64kb erase\r\n");
               break;
            default:
               fprintf(PC, "error\r\n");
               break;
         }
         
      break;
      
      case 0x12:

         fprintf(PC, "\r\Erase Flash Operation for Shared Mission Flash\r\n");
         output_low (PIN_A5);                                                    //MF Main Side
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         switch(CMD6)
         {
            case 0x04:
               SUBSECTOR_4KB_ERASE_SMF(address);
               fprintf(PC, "Finish 4kb erase\r\n");
               break;
            case 0x32:
               SUBSECTOR_32KB_ERASE_SMF(address);
               fprintf(PC, "Finish 32kb erase\r\n");
               break;
            case 0xFF:
               SECTOR_ERASE_SMF(address);
               fprintf(PC, "Finish 64kb erase\r\n");
               break;
            default:
               fprintf(PC, "error\r\n");
               break;
         }
         
         output_high (PIN_A5);
         
      break;
      
      case 0x13:
      
         fprintf(PC, "\r\nERASE ALL FLASH OPERATION\r\n");
         ERASE_EEPROM_INFO(CMD2,CMD3,CMD4);                                      //erase 512byte(from 0x18000 to 0x181ff)
         MEMORY_ERASE(CMD2,CMD3,CMD4);                                           //executes if CMD2 == 0xAA, CMD3 == 0xBB and CMD4 = 0xCC to prevent unintentional deletion
      
      break;
      
      case 0x14:

         fprintf (PC, "\r\nWrite 3 bytes to SMF\r\n") ;
         output_low (PIN_A5) ;                                                   //Main side
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         SUBSECTOR_4KB_ERASE_SMF(address);
         WRITE_DATA_BYTE_SMF (address, CMD6);
         WRITE_DATA_BYTE_SMF (address + 1, CMD7);
         WRITE_DATA_BYTE_SMF (address + 2, CMD8);
         
      break;
      
      
      case 0x15:                                                                 //Uplink command to write the data on Flash Memory 2

         fprintf(PC, "Nothing yet\r\n");                                                    //Nothing for now

 
      break;
      
      
      case 0x16:                                                                 //Transfer 1 sector from MF to SCF
         
         fprintf(PC, "\r\Transfer 1 sector from MF to SCF\r\n"); 
         TRANSFER_SECTOR_OF2SCF(CMD2,CMD3,CMD4,CMD5);                            //transfer 1 sector, also this function contains saving log
         
      break;    
      
            
      case 0x17:                                                                 //Transfer 1 sector from SMF to SCF
         
         fprintf(PC, "\r\Transfer 1 sector from SMF to SCF\r\n");
         TRANSFER_SECTOR_SMF2SCF(CMD2,CMD3,CMD4,CMD5);                           //transfer 1 sector, also this function contains saving log
       
      break;
      
      
      case 0x18:                                                                 //Transfer 1 sector from SCF to MF
         
         fprintf(PC, "\r\Transfer 1 sector from SMF to MF\r\n"); 
         TRANSFER_SECTOR_SMF2OF(CMD2,CMD3,CMD4,CMD5);                            //transfer 1 sector, also this function contains saving log
       
      break;
      
      case 0x19:

         fprintf (PC, "\r\nTransfer N Packets from OF to SCF\r\n") ;
         fprintf (PC, "From OF:\r\n");
         
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81;
         
         TRANSFER_DATA_NBYTE_TOPC_OF(address, packet);
         delay_ms(1000);
         fprintf (PC, "From SCF:\r\n");
         output_low (PIN_C4); // Main side SCF
         sector_erase_SCF(address);                                  
         TRANSFER_DATA_NBYTE_OFtoSCF(address, address, packet);
         delay_ms(1000);
         TRANSFER_DATA_NBYTE_TOPC_SCF(address, packet);
         
         output_high(PIN_C4); // COMM side
         

      break;
      
      
      case 0x1A:

         fprintf (PC, "\r\nTransfer N Packets from SMF to SCF\r\n") ;
         fprintf (PC, "From SMF:\r\n");
         
         output_low (PIN_A5);                                                    // Main side
         
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81;
         
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         delay_ms(1000);
         fprintf (PC, "From SCF:\r\n");
         output_low (PIN_C4); // Main side SCF
         sector_erase_SCF(address);
//!         if (packet > 40500)                                                      //limits to 500 packets (40500 bytes)
//!         {
//!            fprintf (PC, "LIMITED TO 500 PACKETS\r\n");
//!            packet = 40500;
//!         } 
         TRANSFER_DATA_NBYTE_SMFtoSCF(address, address, packet);
         delay_ms(1000);
         TRANSFER_DATA_NBYTE_TOPC_SCF(address, packet);
         
         output_high(PIN_C4); // COMM side


      break;
      
      case 0x1B:

         fprintf (PC, "\r\nTransfer N Packets from SMF to OF\r\n") ;
         fprintf (PC, "From SMF:\r\n");
         
         output_low (PIN_A5); // Main side
         
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81;
         
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         delay_ms(1000);
         fprintf (PC, "From OF:\r\n");
         sector_erase_SCF(address);
         TRANSFER_DATA_NBYTE_SMFtoOF(address, address, packet);
         delay_ms(1000);
         TRANSFER_DATA_NBYTE_TOPC_OF(address, packet);
         
         output_high(PIN_C4); // COMM side

      
      break;
      
      case 0x1C:                                                                 //Uplink command to write the data on Flash Memory 2

         fprintf (PC, "\r\nRead N packets from SCF to PC\r\n") ;
         
         output_low (PIN_C4);                                                    // Main side
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81; 
         TRANSFER_DATA_NBYTE_TOPC_SCF(address, packet);
         output_high(PIN_C4); // COMM side
          
         fprintf (PC, "\r\nFinish 0x1C\r\n") ;

      break;
      
      
      case 0x1D:                                                                 //Uplink command to write the data on Flash Memory 2

         fprintf (PC, "\r\nRead N packets from MF to PC\r\n") ;

         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81;
         TRANSFER_DATA_NBYTE_TOPC_OF(address, packet);
         
         fprintf (PC, "\r\nFinish 0x1D\r\n") ;

      break;
      
      case 0x1E:
         
         unsigned int16 duration = (unsigned int16)CMD2*6;                      //CMD2 is operation time(min), maximum number of readings in 2 hours = 720
         if(duration > 720){duration = 720;}                                    // 6 readings in 1 min, every 10 seconds
         HIGHSAMP_SENSOR_COLLECTION(duration);                                  //remove mission status issue as it will prevent Hssc from getting adcs data
      
      break;
      
      case 0x1F:                                                                 //Uplink command to write the data on Flash Memory 2
         
         fprintf (PC, "\r\nRead N packets from SMF to PC\r\n") ;
         output_low (PIN_A5);
         address_data[0] = CMD2<<24;
         address_data[1] = CMD3<<16;
         address_data[2] = CMD4<<8;
         address_data[3] = CMD5;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         packet_data[0] = CMD6<<8;
         packet_data[1] = CMD7;
         packet = (packet_data[0] + packet_data[1])*81;
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         fprintf (PC, "\r\nFinish 0x1F\r\n") ;
         

      break;
      
      default:
      
         fprintf (PC, "Command:%x", CMD0);
         fprintf (PC, " does not exist\r\n");
         
      break;
      
   }
   
   return;
}


void MULT_SPEC_COMMAND(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{

   switch(CMD0)
   {
      ////////////////////////////FOR CAM1 RPi on MB1////////////////////////////////
      
      case 0x2E: //Turn on CAM1 RPi for MOSFET on MB1 to power RPI from 5V
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         Turn_On_MBP();
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x2E - Turn ON MULTSPEC CAM1\r\n") ;
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         missiontime = 0;
         output_high(pin_G3); //Turn on DIO for MULTSPEC CAM1
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x2E\r\n"); 
      
      break;
      
      
      case 0x20: //Turn off CAM1 RPi DIO for MOSFET on MB1 to power RPI from 5V      

         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5);                                                      //SFM2 mission side access
         fprintf (PC, "Start 0x20 - Turn OFF MULTSPEC CAM1 (MB1)\r\n") ;
         output_low(pin_G3);                                                        //Turn off DIO for MULTSPEC CAM1
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         missiontime = 0;
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x20\r\n");
         
      break;
      
      
      case 0x21: //Real time uplink command

         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x21 - Real time uplink MULTSPEC CAM1\r\n") ;
         if (MISSION_STATUS == 1)
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         MISSION_OPERATING = 0;
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x21\r\n");

      break;
      
      
      case 0x22: //Real time downlink command
      
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         fprintf (PC, "Start 0x22 - Request MULT-SPEC MB1 Downlink Data\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
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
         output_high(PIN_C4);                                                    //give access back to COMM
         MISSION_OPERATING = 0;
         
      break;
      
      
      case 0x23:
      
         //Capture in mode 1
         //(1 image, captured immediately, saved to specified address) 
         //eg. 23 is command, 00 02 06 08 is the address location, 01 is number of images to capture, 00 00 00 for remaining unused command bytes (command: 23 00 02 06 08 01 00 00 00)
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         fprintf (PC, "Start 0x23\r\n") ;
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
         output_high (PIN_A5); //SFM2 mission side access
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         MISSION_OPERATING = 0;
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x23\r\n");
      break;
      
      
      case 0x24: //Turn on CAM1 RPi trigger at specific time from Reservation time
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         fprintf (PC, "Start 0x24\r\n") ;
         output_low (PIN_A5);
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
         RESERVE_TARGET_FLAG = 3;
//!            if(RESERVE_MIN_FLAG >= RESERVE_TARGET_FLAG) 
//!            {
//!               
//!            }
         MISSION_OPERATING = 0;
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x24\r\n");    
      break;
      
      
      case 0x25: //Copy specific images based on selected thumbnails selected from GS. Forwward all mission command data (8 bytes) to MB, MB will send command data (specific image name) to MB1 RPI
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x25\r\n") ;
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         MISSION_OPERATING = 0;
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x25\r\n");
         
      break;
      
      
////////////////////////////FOR CAM2 RPi on MB2////////////////////////////////
      case 0x3E: // Turn on MULTSPEC CAM2 (MB2)
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         Turn_On_MBP();
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         missiontime = 0;
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x3E - Turn ON MULTSPEC CAM2 (MB2)\r\n") ;
         output_high(pin_F6); //Turn on DIO for MULTSPEC CAM2
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x3E\r\n");
         
      break;
      
     case 0x30: //Turn off CAM2 RPi DIO for MOSFET on MB2 to power RPI from 5V
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         missiontime = 0;
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x30 - Turn OFF MULTSPEC CAM2 (MB2)\r\n") ;

         output_low(pin_F6); //Turn off DIO for MULTSPEC CAM2
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x30\r\n");
         
         
      break;
      
      case 0x31: //Turn off CAM1 RPi for MOSFET on MB1 to power RPI from 5V
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x21 - Real time uplink MULTSPEC CAM1\r\n") ;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x21\r\n");
         
      break;
      
      case 0x32: //Turn on CAM1 RPi trigger
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         fprintf (PC, "Start 0x32 - Request MULT-SPEC MB2 Downlink Data\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
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
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x32\r\n");
      break;
      
      case 0x33: 
         //Capture in mode 1
         //(1 image, captured immediately, saved to specified address) 
         //eg. 23 is command, 00 02 06 08 is the address location, 01 is number of images to capture, 00 00 00 for remaining unused command bytes (command: 23 00 02 06 08 01 00 00 00)
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         fprintf (PC, "Start 0x33\r\n") ;
         
         output_high (PIN_A5); //SFM2 mission side access
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x33\r\n");
      break;
      
      case 0x34: //Turn on CAM1 RPi trigger at specific time from RESET_PIC time information
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
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
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x34\r\n");    
      break;
      
      case 0x35: //Turn on CAM1 and CAM2 RPi trigger
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x35\r\n") ;
         output_high(pin_F7); //Turn on DIO for trigger MB2
         output_high(pin_G2); //Turn on DIO for trigger MB1
         delay_ms(10000);
         output_low(pin_F7); //Turn off DIO for trigger
         output_low(pin_G2); //Turn off DIO for trigger
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x35\r\n");   
         
      break;
      
      case 0x36: //Turn on CAM1 and CAM2 RPi
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_MULT_CAM();                                              //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x36\r\n") ;
         output_high(pin_G3); //Turn on DIO for trigger MB2
         delay_ms(5000);
         output_high(pin_F6); //Turn on DIO for trigger MB1
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x36\r\n");
         
      break;
      
      default:
      
         fprintf (PC, "Command:%x", CMD0);
         fprintf (PC, " does not exist\r\n");
         
      break;
   
   }
   
   return;
   
}


void MULT_SPEC_COMMAND_PC(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{

   switch(CMD0)
   {
      ////////////////////////////FOR CAM1 RPi on MB1////////////////////////////////
      
      case 0x2E: //Turn on CAM1 RPi for MOSFET on MB1 to power RPI from 5V
         
         Turn_On_MBP();
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x2E - Turn ON MULTSPEC CAM1\r\n") ;
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         missiontime = 0;
         output_high(pin_G3); //Turn on DIO for MULTSPEC CAM1
         fprintf (PC, "Finish 0x2E\r\n"); 
      
      break;
      
      
      case 0x20: //Turn off CAM1 RPi DIO for MOSFET on MB1 to power RPI from 5V      
         
         output_low (PIN_A5);                                                      //SFM2 mission side access
         fprintf (PC, "Start 0x20 - Turn OFF MULTSPEC CAM1 (MB1)\r\n") ;
         output_low(pin_G3);                                                        //Turn off DIO for MULTSPEC CAM1
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         missiontime = 0;
         fprintf (PC, "Finish 0x20\r\n");
         
      break;
      
      
      case 0x21:                                                                    //Real time uplink command
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x21 - Real time uplink MULTSPEC CAM1\r\n") ;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         fprintf (PC, "Finish 0x21\r\n");

      break;
      
      
      case 0x22: //Real time downlink command
      
         fprintf (PC, "Start 0x22 - Request MULT-SPEC MB1 Downlink Data\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         MISSION_OPERATING = 1;
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
         
         MISSION_OPERATING = 0;
         
      break;
      
      
      case 0x23: 
         
         fprintf (PC, "Start 0x23\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x23)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
      
         fprintf (PC, "Finish 0x23\r\n");
         
      break;
      
      
      case 0x24: 
         
         fprintf (PC, "Start 0x24\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x24)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
      
         fprintf (PC, "Finish 0x24\r\n");
         
      break;
      
      
      case 0x25:                                                                 //Trigger Command
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x25\r\n");
         output_high(pin_F7); //Turn on DIO for trigger MB2
         output_high(pin_G2); //Turn on DIO for trigger MB1
         delay_ms(10000);
         output_low(pin_F7); //Turn off DIO for trigger
         output_low(pin_G2); //Turn off DIO for trigger
         fprintf (PC, "Finish 0x25\r\n");  
         
      break;
      
      case 0x26: 
         
         fprintf (PC, "Start 0x26\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x26)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
      
         fprintf (PC, "Finish 0x26\r\n");
         
      break;
      
      case 0x27: 
         
         fprintf (PC, "Start 0x27\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x27)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
      
         fprintf (PC, "Finish 0x27\r\n");
         
      break;
      
      case 0x28: 
         
         fprintf (PC, "Start 0x28\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x28)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
      
         fprintf (PC, "Finish 0x28\r\n");
         
      break;
      
      case 0x29: 
         
         fprintf (PC, "Start 0x29\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x29)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
      
         fprintf (PC, "Finish 0x29\r\n");
         
      break;
      
      
      
////////////////////////////FOR CAM2 RPi on MB2////////////////////////////////
      case 0x3E: // Turn on MULTSPEC CAM2 (MB2)
         
         Turn_On_MBP();
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         missiontime = 0;
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x3E - Turn ON MULTSPEC CAM2 (MB2)\r\n") ;
         output_high(pin_F6); //Turn on DIO for MULTSPEC CAM2
       
         fprintf (PC, "Finish 0x3E\r\n");
         
      break;
      
     case 0x30: //Turn off CAM2 RPi DIO for MOSFET on MB2 to power RPI from 5V
         
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         missiontime = 0;
         output_low (PIN_A5);                                                    //SFM2 main side access
         fprintf (PC, "Start 0x30 - Turn OFF MULTSPEC CAM2 (MB2)\r\n") ;
         output_low(pin_F6); //Turn off DIO for MULTSPEC CAM2
         fprintf (PC, "Finish 0x30\r\n");
         
      break;
      
      case 0x31: //Turn off CAM1 RPi for MOSFET on MB1 to power RPI from 5V
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x21 - Real time uplink MULTSPEC CAM1\r\n") ;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         fprintf (PC, "Finish 0x21\r\n");
         
      break;
      
      case 0x32: //Turn on CAM1 RPi trigger
      
         fprintf (PC, "Start 0x32 - Request MULT-SPEC MB2 Downlink Data\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
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
         
         fprintf (PC, "Start 0x33\r\n") ;
         output_high (PIN_A5); //SFM2 mission side access
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x33)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
      
         fprintf (PC, "Finish 0x33\r\n");
         
      break;
      
      case 0x34: //Turn on CAM1 RPi trigger at specific time from RESET_PIC time information

         fprintf (PC, "Start 0x34\r\n") ; 
         output_high (PIN_A5);                                                   //SFM2 mission side access   
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x34)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
         
         fprintf (PC, "Finish 0x34\r\n");    
         
      break;
      
      case 0x35: //Turn on CAM1 and CAM2 RPi trigger
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x35\r\n");
         output_high(pin_F7); //Turn on DIO for trigger MB2
         output_high(pin_G2); //Turn on DIO for trigger MB1
         delay_ms(10000);
         output_low(pin_F7); //Turn off DIO for trigger
         output_low(pin_G2); //Turn off DIO for trigger
         fprintf (PC, "Finish 0x35\r\n");   
         
      break;
      
      case 0x36: //Turn on CAM1 and CAM2 RPi

         fprintf (PC, "Start 0x36\r\n");
         
         output_high (PIN_A5);                                                   //SFM2 mission side access
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x36)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");  
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x36\r\n");
         
      break;
      
      case 0x37:

         fprintf (PC, "Start 0x37\r\n");
         
         output_high (PIN_A5);                                                   //SFM2 mission side access
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x37)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x37\r\n");
           
      break;
      
      case 0x38:

         fprintf (PC, "Start 0x38\r\n");
         
         output_high (PIN_A5);                                                   //SFM2 mission side access
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x38)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x38\r\n");
           
      break;
      
      case 0x39:

         fprintf (PC, "Start 0x39\r\n");
         
         output_high (PIN_A5);                                                   //SFM2 mission side access
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         for(num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x39)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x39\r\n");
           
      break;
      
      default:
      
         fprintf (PC, "Command:%x", CMD0);
         fprintf (PC, " does not exist\r\n");
         
      break;
   
   }
   
   return;
   
}


void IMGCLS_COMMAND(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{  
   switch(CMD0)
   {
      
      case 0x8E:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_IMGCLS_CAM();                                             //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         Turn_On_MBP();
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x8E - Turn ON IMGCLS\r\n") ;
         
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         missiontime = 0;
         fprintf (PC, "Finish 0x8E\r\n");
         

      break;
      
      case 0x80: //Turn off IMGCLS RPi DIO for MOSFET on RAB to power RPI from 5V line
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_IMGCLS_CAM();                                             //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x80 - Turn off IMGCLS\r\n");
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         missiontime = 0;
         fprintf (PC, "Finish 0x80\r\n"); 
         
      break;
      
      case 0x81: //Real time uplink command
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_IMGCLS_CAM();                                            //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x81 - Real time uplink IMGCLS\r\n") ;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         fprintf (PC, "Finish 0x81\r\n"); 
      break;
      
      case 0x82: //Real time downlink IMGCLS
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_IMGCLS_CAM();                                             //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5);
         fprintf (PC, "Start 0x82 - Real time downlink IMGCLS\r\n") ;
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         int8 counter_cls = 0;
         for(int32 num_cls = 0; num_cls < 1500000; num_cls++)
            {
               if(kbhit(DC))
               {
                  IMGCLS_DATA[counter_cls] = fgetc(DC);
                  counter_cls++;
                  if(counter_cls == 81)
                  {
                     break;
                  }
               }
            }
         fprintf(PC,"Data Recieved: ");
         for(int8 c = 0; c < 81; c++)
         {
            fprintf(PC,"%x, ",IMGCLS_DATA[c]);
         }
         fprintf(PC,"\r\n");

         fprintf (PC, "Finish 0x82\r\n"); 
         for(c = 0; c < 81; c++)
         {
            IMGCLS_DATA[c] = 0;
         }

      break;
      
      
      case 0x83:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_IMGCLS_CAM();                                             //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5);
         fprintf (PC, "Start 0x83 - Mission Mode 1 IMGCLS\r\n") ;
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);

         MISSION_OPERATING = 0;
         
         fprintf (PC, "Finish 0x83\r\n"); 
         
      break;
      
      case 0x84:
      
         //Capture in mode 1
         //(1 image, captured immediately, saved to specified address) 
         //eg. 23 is command, 00 02 06 08 is the address location, 01 is number of images to capture, 00 00 00 for remaining unused command bytes (command: 23 00 02 06 08 01 00 00 00)
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_IMGCLS_CAM();                                             //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         fprintf (PC, "Start 0x84\r\n") ;
         
         output_high (PIN_A5); //SFM2 mission side access
         
         
         fprintf (PC, "Finish 0x84\r\n");
      break;
      
      case 0x85:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_IMGCLS_CAM();                                             //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
      
         output_high (PIN_A5); 
         fprintf (PC, "Start 0x85\r\n");
         
         fputc(0x48, DC); //Forward command to MB
         fputc(0x48, PC); 
         
         int8 IMGCLS_ACK = 0;
         for(int32 num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               IMGCLS_ACK = fgetc(DC);
               break;
            }
            
         }
         
         if(IMGCLS_ACK == 0x70)                                                          //acknowledge
         {
            fputc(0x49, DC); 
            fputc(0x49, PC); 
            fprintf(PC,"\r\nIMGCLS ACK received\r\n");
            fprintf(PC,"Recieved ACK: %x\r\n",IMGCLS_ACK);
            for(int l = 1; l < 9; l++)
            {
               fputc(command[l], DC);
               delay_ms(10);
               fprintf(PC,"%x",command[l]);
               delay_ms(10);
            }
            fprintf(PC,"\r\n");

         }
         else
         {
            
            fprintf(PC,"Recieved ACK: %x\r\n",IMGCLS_ACK);
         
         }
         fprintf (PC, "Finish 0x85\r\n");
         break;
   
      default:
      
         fprintf (PC, "Command:%x", CMD0);
         fprintf (PC, " does not exist\r\n");
         
      break;
      
   }

   return;
}


void IMGCLS_COMMAND_PC(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{  
   switch(CMD0)
   {
      
      case 0x8E:
         
         Turn_On_MBP();
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x8E - Turn ON IMGCLS\r\n") ;
         
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         missiontime = 0;
         fprintf (PC, "Finish 0x8E\r\n");
         

      break;
      
      case 0x80: //Turn off IMGCLS RPi DIO for MOSFET on RAB to power RPI from 5V line
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x80 - Turn off IMGCLS\r\n");
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         missiontime = 0;
         fprintf (PC, "Finish 0x80\r\n"); 
         
      break;
      
      case 0x81:                                                                  //Real time uplink command
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x81 - Real time uplink IMGCLS\r\n") ;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         fprintf (PC, "Finish 0x81\r\n"); 
      break;
      
      case 0x82:                                                                  //Real time downlink IMGCLS
         output_high (PIN_A5);
         fprintf (PC, "Start 0x82 - Real time downlink IMGCLS\r\n") ;
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         int8 counter_cls = 0;
         for(int32 num_cls = 0; num_cls < 1500000; num_cls++)
            {
               if(kbhit(DC))
               {
                  IMGCLS_DATA[counter_cls] = fgetc(DC);
                  counter_cls++;
                  if(counter_cls == 81)
                  {
                     break;
                  }
               }
            }
         fprintf(PC,"Data Recieved: ");
         for(int8 c = 0; c < 81; c++)
         {
            fprintf(PC,"%x, ",IMGCLS_DATA[c]);
         }
         fprintf(PC,"\r\n");
         
         MISSION_OPERATING = 0;
         
         fprintf (PC, "Finish 0x82\r\n"); 
         for(c = 0; c < 81; c++)
         {
            IMGCLS_DATA[c] = 0;
         }

      break;
      
      
      case 0x83:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x83\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(int8 num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x83)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x83\r\n");
         
      break;
      
      case 0x84:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x84\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x84)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x84\r\n");
         
      break;
      
      case 0x85:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x85\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x85)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x85\r\n");
         
         break;
         
         case 0x86:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x86\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x86)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x86\r\n");
         
         break;
         
         case 0x87:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x87\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x87)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x87\r\n");
         
         break;
         
         case 0x88:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x88\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x88)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x88\r\n");
         
         break;
         
         case 0x89:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x89\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x89)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x89\r\n");
         
         break;

         case 0x8A:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x8A\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x8A)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x8A\r\n");
         
         break;
         
         case 0x8B:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x8B\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x8B)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x8B\r\n");
         
         break;
         
         case 0x8C:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x8C\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x8C)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x8C\r\n");
         
         break;
         
         case 0x8D:
      
         output_high (PIN_A5);                                                   //Mission Side
         
         fprintf (PC, "Start 0x8D\r\n") ;

         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);            //Forward command to Mission Boss

         for(num = 0; num < 1000000; num++)                                      //wait for MBP ack
         {
            if(kbhit(DC))
            {
               Mission_check_flag = fgetc(DC);
               break;
            }
         }
         
         fprintf(PC,"\r\nMission Check Flag:%lx\r\n",(int8)Mission_check_flag);
         if (Mission_check_flag == 0x8D)
         {
            MISSION_OPERATING = 1;
            fprintf (PC, "Mission operating\r\n");
            
         }
         else
         {
            MISSION_OPERATING = 0;
            fprintf (PC, "Mission NOT operating\r\n");
         }
         
         Mission_check_flag = 0;
         fprintf (PC, "Finish 0x8D\r\n");
         
         break;
   
      default:
      
         fprintf (PC, "Command:%x", CMD0);
         fprintf (PC, " does not exist\r\n");
         
      break;
      
   }

   return;
}


void SF_COMMAND_PC(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{
   switch(CMD0)
   {
      
      case 0x5E: //Turn on
         
         Turn_On_MBP();
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x5E - Turn ON S&F\r\n") ;
         delay_ms(1000);
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         missiontime = 0;
         fprintf (PC, "Finish 0x5E\r\n");
         

      break;
      
      case 0x50: //Turn off
         
         output_low (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x50 - Turn off S&F\r\n");
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         missiontime = 0;
         fprintf (PC, "Finish 0x50\r\n"); 
         
      break;
      
      case 0x51:                                                                  //Real time uplink command
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x51 - Real time uplink S&F\r\n") ;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         
         fprintf (PC, "Finish 0x51\r\n"); 
      break;
      
      case 0x52:                                                                  //Real time downlink IMGCLS
         output_high (PIN_A5);
         fprintf (PC, "Start 0x52 - Real time downlink S&F\r\n") ;
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         int8 counter_sf = 0;
         for(int32 num_sf = 0; num_sf < 1500000; num_sf++)
            {
               if(kbhit(DC))
               {
                  SFWD_DATA[counter_sf] = fgetc(DC);
                  counter_sf++;
                  if(counter_sf == 81)
                  {
                     break;
                  }
               }
            }
         fprintf(PC,"Data Recieved: ");
         for(int8 sf = 0; sf < 81; sf++)
         {
            fprintf(PC,"%x, ",SFWD_DATA[sf]);
         }
         fprintf(PC,"\r\n");
         
         MISSION_OPERATING = 0;
         
         fprintf (PC, "Finish 0x52\r\n"); 
         for(sf = 0; sf < 81; sf++)
         {
            SFWD_DATA[sf] = 0;
         }

      break;
      
      case 0x53:
         
         output_high (PIN_A5);
         
         fprintf (PC, "Start 0x53 - Transfer S&F Data\r\n") ;
         MISSION_OPERATING = 1;
     
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         address_data[0] = 0x03<<24;
         address_data[1] = 0x90<<16;
         address_data[2] = 0x00<<8;
         address_data[3] = 0x00;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         int8 PcktNo = CMD8;
         int16 SFWD_DATAS = 0;
         counter_sf = 0;
         output_low(PIN_C4);
         delay_ms(10);
         sector_erase_SCF(address);
         delay_ms(20);
         
         for(num_sf = 0; num_sf < 20000000; num_sf++)
         {
            if (kbhit(DC))
            {
               SFWD_DATAS = fgetc(DC);
               fprintf(PC, "%x ", SFWD_DATAS);
               WRITE_DATA_BYTE_SCF(address, SFWD_DATAS);
               address++;
               counter_sf++;
               if (counter_sf == 81*PcktNo)
               {
                  break;
               } 
            }
         }
         
//!         address = address-(81*PcktNo);
//!         delay_ms(500);
//!         fprintf (PC, "\r\nReading Data from SCFM\r\n") ;
//!         
//!         for(int32 w = 0; w < (81*pcktNo); w++)
//!         {
//!             int8 z = READ_DATA_BYTE_SCF(address);
//!             fprintf(PC, "%x ", z);
//!             address++;
//!         }
//!         fprintf (PC, "\r\nFinished  PC Display\r\n");
//!         
//!         delay_ms(1000);
//!         address = address-(81*PcktNo);
//!         fprintf (PC, "\r\n") ;
         MISSION_OPERATING = 0;
         fprintf (PC, "Finish 0x53 - Transfer S&F Data\r\n") ;
      break;
      
      case 0x54:
      
         output_high (PIN_A5);
         fprintf (PC, "Start 0x54 - Downlink SF Address Pointer S&F\r\n") ;
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         counter_sf = 0;
         for(num_sf = 0; num_sf < 1500000; num_sf++)
            {
               if(kbhit(DC))
               {
                  SFWD_DATA[counter_sf] = fgetc(DC);
                  counter_sf++;
                  if(counter_sf == 81)
                  {
                     break;
                  }
               }
            }
         fprintf(PC,"Data Recieved: ");
         for(sf = 0; sf < 81; sf++)
         {
            fprintf(PC,"%x, ",SFWD_DATA[sf]);
         }
         fprintf(PC,"\r\n");
         
         MISSION_OPERATING = 0;
         
         fprintf (PC, "Finish 0x54\r\n"); 
         for(sf = 0; sf < 81; sf++)
         {
            SFWD_DATA[sf] = 0;
         }
      
      break;
      
      case 0x55: //Turn on
         
         Turn_On_MBP();
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x55 - Erase S&F Flash\r\n") ;
         delay_ms(1000);
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         missiontime = 0;
         fprintf (PC, "Finish 0x55\r\n");
         

      break;
      
      
   }
}


void SF_COMMAND(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{
   switch(CMD0)
   {
      
      case 0x5E: //Turn on
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_S_FWD();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         Turn_On_MBP();
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x5E - Turn ON S&F\r\n") ;
         delay_ms(1000);
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         missiontime = 0;
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x5E\r\n");
         

      break;
      
      case 0x50: //Turn off
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_S_FWD();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_low (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x50 - Turn off S&F\r\n");
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         missiontime = 0;
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x50\r\n"); 
         
      break;
      
      case 0x51:                                                                  //Real time uplink command

         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_S_FWD();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x51 - Real time uplink S&F\r\n") ;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x51\r\n"); 
         
      break;
      
      case 0x52:                                                                 //Real time downlink SF
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_S_FWD();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         output_high (PIN_A5);
         fprintf (PC, "Start 0x52 - Real time downlink S&F\r\n") ;
         MISSION_OPERATING = 1;
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         int8 counter_sf = 0;
         for(int32 num_sf = 0; num_sf < 1500000; num_sf++)
            {
               if(kbhit(DC))
               {
                  SFWD_DATA[counter_sf] = fgetc(DC);
                  counter_sf++;
                  if(counter_sf == 81)
                  {
                     break;
                  }
               }
            }
         fprintf(PC,"Data Recieved: ");
         for(int8 sf = 0; sf < 81; sf++)
         {
            fprintf(PC,"%x, ",SFWD_DATA[sf]);
         }
         fprintf(PC,"\r\n");
         
         MISSION_OPERATING = 0;
         
         fprintf (PC, "Finish 0x52\r\n"); 
         for(sf = 0; sf < 81; sf++)
         {
            SFWD_DATA[sf] = 0;
         }
         output_high(PIN_C4);                                                    //give access back to COMM
      break;
      
      case 0x53:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_S_FWD();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         output_high (PIN_A5);
         
         fprintf (PC, "Start 0x53 - Transfer S&F Data\r\n") ;
         MISSION_OPERATING = 1;
     
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         address_data[0] = 0x03<<24;
         address_data[1] = 0x90<<16;
         address_data[2] = 0x00<<8;
         address_data[3] = 0x00;
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         
         int8 PcktNo = CMD8;
         int16 SFWD_DATAS = 0;
         counter_sf = 0;
         output_low(PIN_C4);
         delay_ms(10);
         sector_erase_SCF(address);
         delay_ms(20);
         
         for(num_sf = 0; num_sf < 20000000; num_sf++)
         {
            if (kbhit(DC))
            {
               SFWD_DATAS = fgetc(DC);
               fprintf(PC, "%x ", SFWD_DATAS);
               WRITE_DATA_BYTE_SCF(address, SFWD_DATAS);
               address++;
               counter_sf++;
               if (counter_sf == 81*PcktNo)
               {
                  break;
               } 
            }
         }
         
//!         address = address-(81*PcktNo);
//!         delay_ms(500);
//!         fprintf (PC, "\r\nReading Data from SCFM\r\n") ;
//!         
//!         for(int32 w = 0; w < (81*pcktNo); w++)
//!         {
//!             int8 z = READ_DATA_BYTE_SCF(address);
//!             fprintf(PC, "%x ", z);
//!             address++;
//!         }
//!         fprintf (PC, "\r\nFinished  PC Display\r\n");
//!         
//!         delay_ms(1000);
//!         address = address-(81*PcktNo);
//!         fprintf (PC, "\r\n") ;
         MISSION_OPERATING = 0;
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x53 - Transfer S&F Data\r\n") ;
      break;
      
      
      case 0x54:
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_S_FWD();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         output_high (PIN_A5);
         fprintf (PC, "Start 0x54 - Downlink SF Address Pointer S&F\r\n") ;
         
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         counter_sf = 0;
         for(num_sf = 0; num_sf < 1500000; num_sf++)
            {
               if(kbhit(DC))
               {
                  SFWD_DATA[counter_sf] = fgetc(DC);
                  counter_sf++;
                  if(counter_sf == 81)
                  {
                     break;
                  }
               }
            }
         fprintf(PC,"Data Recieved: ");
         for(sf = 0; sf < 81; sf++)
         {
            fprintf(PC,"%x, ",SFWD_DATA[sf]);
         }
         fprintf(PC,"\r\n");
         
         MISSION_OPERATING = 0;
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x54\r\n"); 
         for(sf = 0; sf < 81; sf++)
         {
            SFWD_DATA[sf] = 0;
         }
      
      break;
      
      case 0x55: //Turn on
         
         REPLY_TO_COM(0x66,0);
         SAVE_SAT_LOG(CMD0, 0, CMD2);                                            //save reset data         
         UPLINK_SUCCESS_S_FWD();                                                 //put uplink succes flag in high and store flags
         ACK_for_COM[14] = 0x00;
         Turn_On_MBP();
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x55 - Erase S&F Flash\r\n") ;
         
         delay_ms(1000);
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         missiontime = 0;
         output_high(PIN_C4);                                                    //give access back to COMM
         fprintf (PC, "Finish 0x55\r\n");
         

      break;
      
   }
}


void NEW_PINO_test_PC(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{
   fprintf(PC, "Start NEW_PINO_test_PC\r\n");
   switch (CMD0)
   {
      
      case 0x90: //Turn off SFWD MCU

         fprintf (PC, "Start 0x90 - Turn OFF PINO\r\n") ;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         output_high (hvs);
         delay_ms(15000);
         output_low (sel);
         output_low (PIN_A5);
         delay_ms(10);
         output_low (hvs);
         //fputc(0xAB, reset);
         output_low (PINO_power);
         fprintf (PC, "Finish 0x90\r\n");
         
      break;
      
      case 0x9e: //Turn on PINO MCU
   
         fprintf (PC, "Start 0x9E - Turn ON PINO\r\n") ;
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         delay_ms(100);
         output_high (PIN_A5);
         delay_ms(50);
         output_high (PINO_power);
         //fputc(0xBC, reset);
         
         output_high (sel);
         output_low (hvs);
         fprintf (PC, "Finish 0x9E\r\n");
         
      break;
      
      case 0x91:
         fprintf (PC, "Start 0x91 - Real Time Uplink Command");
         output_high(PIN_A5);
         delay_ms(10);
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         fprintf (PC, "Finish 0x91\r\n");
         break;
         
         case 0x9C://Time and attitude information
         //output_high(PIN_A5);
         int a=0;
         fprintf (PC, "Start 0x9C\r\n") ;
         
         for(a=0; a<12; a++){
            //fputc(0x9C,DC);
            //delay_ms(50);
            //GET_RESET_DATA_for_PINO();
            FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
            BYTE pp[1] = 0x00;
            for(i=0; i<39; i++)
            {
               fputc(pp[0], DC);
               fputc(pp[0], PC);
               pp[0]++;
               delay_ms(50);
            }
            
            delay_ms(10000);
         }
         
         fprintf (PC, "Finish 0x9C\r\n") ;
         

         break;
         
      
      case 0x92:
         fprintf (PC, "Start 0x92 - Request PINO Downlink Data\r\n") ;
         output_high(PIN_A5);
         delay_ms(10);
         FWD_CMD_MBP(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
         int8 pino_counter = 0;
         for(int32 num = 0; num < 1500000; num++)
            {
             
               if(kbhit(DC))
               {
                  NEW_PINO_DATA[pino_counter] = fgetc(DC);
                  pino_counter++;
               }
               if(pino_counter == 81)
               {
                  break;
               }
            }
         fprintf(PC,"Data Recieved: ");
         for(int l = 0; l < 81; l++)
         {
            fprintf(PC,"%x",NEW_PINO_DATA[l]);
         }
         fprintf(PC,"\r\n");
         fprintf (PC, "Finish 0x92\r\n");
         for(l = 0; l < 81; l++)
         {
            NEW_PINO_DATA[l] = 0;
         }
   
         break;
         
         case 0x95:
            output_high(hvs);
            break;
            
         case 0x96:
            output_low(hvs);
            break;
 
   }
   
   for(int m = 0; m < 9; m++)
   {
      command[m] = 0;
   }
}


void EXECUTE_COMMAND_from_PC(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{
   
   BYTE command_ID = CMD0;
   command_ID &= 0xF0;
   
   if(command_ID == 0x00 || command_ID == 0x10)
   {
      fprintf(PC,"Main PIC or Mission Boss Command\r\n");
      MAIN_COMMAND_PC(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
   }
   
   if(command_ID == 0x20 || command_ID == 0x30)
   {
      fprintf(PC,"MULT-SPEC Command\r\n");
      missiontime = 0;
      MULT_SPEC_COMMAND_PC(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
   }
   
   if(command_ID == 0x40)
   {
      fprintf(PC,"ADCS Command\r\n");
      ADCS_test();
   }
   
   if(command_ID == 0x50)
   {
      fprintf(PC,"S-FWD Command\r\n");
      SF_COMMAND_PC(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
   }
   
   if(command_ID == 0x80)
   {
      fprintf(PC,"IMG-CLS Command\r\n");
      missiontime = 0;
      IMGCLS_COMMAND_PC(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
      
   }
   
   if(command_ID == 0x90)
   {
      fprintf(PC,"PINO Command\r\n");
      NEW_PINO_test_PC(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
   }
   
   return;
}


void EXECUTE_COMMAND_from_COMM(int8 CMD0,int8 CMD2,int8 CMD3,int8 CMD4,int8 CMD5,int8 CMD6, int8 CMD7, int8 CMD8)
{
   
   BYTE command_ID = CMD0;
   command_ID &= 0xF0;
   
   if(command_ID == 0x00 || command_ID == 0x10)
   {
      fprintf(PC,"Main PIC or Mission Boss Command\r\n");
      MAIN_COMMAND(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
   }
   
   if(command_ID == 0x20 || command_ID == 0x30)
   {

      fprintf(PC,"MULT-SPEC Command\r\n");
      missiontime = 0;
      MULT_SPEC_COMMAND(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);
   }
   
   if(command_ID == 0x40)
   {
      fprintf(PC,"ADCS Command\r\n");
      //ADCS_test();
   }
   
   if(command_ID == 0x50)
   {
      fprintf(PC,"S-FWD Command\r\n");
      SF_COMMAND(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8); 
   }
   
   if(command_ID == 0x60)
   {
      fprintf(PC,"APRS Command\r\n");
      SF_COMMAND(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8); 
   }
   
   if(command_ID == 0x80)
   {  
      fprintf(PC,"IMG-CLS Command\r\n");
      missiontime = 0;
      IMGCLS_COMMAND(CMD0, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8);    
   }
   
   if(command_ID == 0x90)
   {
      fprintf(PC,"PINO Command\r\n");
      //NEW_PINO_test();
   }
   
   return;
}


void check_rsv(int8 rsvt)
{
   BYTE reservation_time = rsvt;
   if(reservation_time != 0x00)
   {
      
   }
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
