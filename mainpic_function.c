
int8 MISSION_STATUS = 0;                                                         //MISSION STATUS FLAGS
static int16 currenttime = 0;
BYTE command[1];
int8 reset_time_data[11] = {};
BYTE PINO_DATA[17] = {0x00};
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
            break;
         case 0x91:
            reset_time_data[0] = 0x82;
            fprintf(PC,"Command 2 Recieved\r\n");
            fputc(reset_time_data[0],PC);
            fputc(0x91, reset);
            break;
      }
   }
}

void GET_RESET_DATA()                                                            //funcion que carga el array HKDATA con los datos del Reset PIC
{
   RESET_DATA = 0;
   int8 dayh;
   int8 dayl;
   int8 hr;
   int8 min;
   int8 sec;
   int8 j;
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
      for(int num = 0; num < 5; num++)                                           //carga el array HKDATA con timedata en las posiciones 2 al 6
      {
         reset_time_data[num] = reset_bffr[num + 1];
         //fputc(reset_bffr[num + 1], DC);
         fprintf(PC,"%x,",reset_bffr[num + 1]);
      }
      sec = reset_time_data[0];
      min = reset_time_data[1];
      hr = reset_time_data[2];
      dayl = reset_time_data[3];
      dayh = reset_time_data[4];
      fprintf(PC,"\r\n");
      
      PINO_DATA[0] = 0x81;
      
      PINO_DATA[1] = reset_time_data[0];
      PINO_DATA[2] = reset_time_data[1];
      PINO_DATA[3] = reset_time_data[2];
      PINO_DATA[4] = reset_time_data[3];
      PINO_DATA[5] = reset_time_data[4];
      
      PINO_DATA[6] = 0x01;
      PINO_DATA[7] = 0x01;
      PINO_DATA[8] = 0x01;
      PINO_DATA[9] = 0x01;
      PINO_DATA[10] = 0x01;
      PINO_DATA[11] = 0x01;
      PINO_DATA[12] = 0x01;
      PINO_DATA[13] = 0x01;
      PINO_DATA[14] = 0x01;
      PINO_DATA[15] = 0x01;
      PINO_DATA[16] = 0x01;
          
      for (j = 0; j < 17; j++)
      {
         fprintf(PC,"%x,",PINO_DATA[j]);
         fputc(PINO_DATA[j], DC);
      }
      fprintf(PC,"\r\n");
      

   }else{
      fprintf(PC,"\r\nRESET DATA NO OBTAINED\r\n");
   }
   return;
}
