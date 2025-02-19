
int8 MISSION_STATUS = 0;                                                         //MISSION STATUS FLAGS
static int16 currenttime = 0;
BYTE command[1];
int8 reset_time_data[11] = {};
int8 test_data[11] = {0};


void Turn_On_CAM()
{
   output_high(PIN_D7);
   return;
}


void Turn_Off_CAM()
{
   output_low(PIN_D7);
   return;
}

void Flash_Memory_Access()
{
      sector_erase_OF(0x00000000);
      WRITE_DATA_BYTE_OF(0x00000000,0x01);
      TRANSFER_DATA_NBYTE_TOPC_OF(0x00000000,1);
      delay_ms(1000);
      
      sector_erase_OF(0x00000000);
      WRITE_DATA_BYTE_OF(0x00000000,0x02);
      TRANSFER_DATA_NBYTE_TOPC_OF(0x00000000,1);
      delay_ms(1000);
      
      sector_erase_OF(0x00000000);
      WRITE_DATA_BYTE_OF(0x00000000,0x03);
      TRANSFER_DATA_NBYTE_TOPC_OF(0x00000000,1);
      delay_ms(1000);
}

void PINO_Test()
{
   int i;
   int32 x;
   int32 num;
   while(TRUE)
   {
      command[0] = 0x00;
   
      for(num = 0; num < 100; num++)                                                      
         {                                                                       
           if(kbhit(PC))                                                       //Devuelve verdadero si un caracter ha sido recibido
           {                                                                     //y espera en el buffer para ser leido
               command[0] = fgetc(PC);                                           //Lee el caracter del buffer, UART receive from main pic
               break;
           }                                                                                                                                         
         }
      switch(command[0])
      {
         case 0x9F:
            fprintf(PC,"Command Recieved\r\n");
            GET_RESET_DATA();
            //fputc(reset_time_data[0], PC);
//!            RESET_DATA = 0;
//!            COLLECT_RESET_DATA();                                                         //carga el RESET_bffr[] con los datos del reset PIC
//!            if(RESET_bffr[0] == 0x8e)
//!            {
//!               reset_time_data[0] = 0xda;                                                    //header of the satellite log
//!               reset_time_data[1] = 0xda;                                                    //header of the satellite log
//!
//!               reset_time_data[2] = RESET_bffr[1];
//!               reset_time_data[3] = RESET_bffr[2];
//!               reset_time_data[4] = RESET_bffr[3];
//!               reset_time_data[5] = RESET_bffr[4];
//!               reset_time_data[6] = RESET_bffr[5];
//!               
//!               reset_time_data[7] = 0x00;
//!               reset_time_data[8] = 0x01;
//!               reset_time_data[9] = 0x02;
//!               reset_time_data[10] = 0xef;
//!               fprintf(PC,"Data entered Recieved\r\n");
//!            }
            
//!            for(num = 0; num < 100; num++)                                                      
//!            {                                                                       
//!               if(kbhit(reset))                                                       //Devuelve verdadero si un caracter ha sido recibido
//!               {
//!                  for(i = 0; i < 11; i++)
//!                  {
//!                     reset_time_data[i] = fgetc(reset);
//!                     fprintf(PC,"\r\nRESET DATA test");
//!                  }
//!               }                                                                                                                                         
//!            }
//!            if(fgetc(reset)>0)
//!            {
//!               for(i = 0; i < 11; i++)
//!               {
//!                  reset_time_data[i] = fgetc(reset);
//!                  fprintf(PC, "RESET Data Recieved\r\n");
//!               }
//!            }
//!            x = make32(reset_time_data[0],reset_time_data[1], reset_time_data[2], reset_time_data[3]);
//!            //fprintf(PC,"\r\nRESET DATA:%lx\r\n\r\n",reset_time_data[0]);
//!            fprintf(PC,"\r\nRESET DATA:%lx\r\n\r\n",reset_time_data);
//!            fprintf(PC,"\r\nTEST DATA:%lx\r\n\r\n",x);
            break;
         case 0x91:
            reset_time_data[0] = 0x82;
            fprintf(PC,"Command 2 Recieved\r\n");
            fputc(reset_time_data[0],PC);
            fputc(0x91, reset);
            break;
//!         default:
//!            fprintf(PC, "Not a command:%lx\r\n", command[0]);
//!            break;
      }
   }
}

void GET_RESET_DATA()                                                            //funcion que carga el array HKDATA con los datos del Reset PIC
{
   RESET_DATA = 0;
   for(int i = 0; i < 6; i++)
   {
      COLLECT_RESET_DATA();
      if(reset_bffr[0] == 0x8e)
      {
         break;
      }
   }   
   if(RESET_bffr[0] == 0x8e)                                                     //si el byte de cabecera es el correcto
   {
      fprintf(PC,"\r\nRESET DATA OBTAINED\r\n");
      for(int num = 0; num < 10; num++)                                           //carga el array HKDATA con timedata en las posiciones 2 al 6
      {
         //HKDATA[num + 2] = reset_bffr[num + 1];
         /*
         fputc(HKDATA[num + 2],PC);
         */
         reset_time_data[num] = reset_bffr[num + 1];
         fputc(reset_bffr[num + 1], DC);
         fprintf(PC,"%x,",reset_bffr[num + 1]);
      }
      fprintf(PC,"\r\n");
      //fprintf(PC,"\r\nDATA FOR MB:%lx\r\n", reset_time_data);
   
//!      for(num = 0; num < 5; num++)                                               //carga el array HKDATA con reset sensor data en las posiciones 110 al 114
//!      {
//!         //HKDATA[num + 116] = reset_bffr[num + 6];                                //carga el HKDATA[] con los datos del reset[]
//!         /*
//!         fputc(HKDATA[num + 116],PC);
//!         */
//!         fprintf(PC,"%x,",reset_bffr[num + 6]);
//!      }

      
   }else{
      fprintf(PC,"\r\nRESET DATA NO OBTAINED\r\n");
   }
   //Delete_Reset();
   return;
}