
int8 MISSION_STATUS = 0;                                                         //MISSION STATUS FLAGS
static int16 currenttime = 0;
BYTE command[9];
int8 reset_time_data[11] = {};
BYTE PINO_DATA[39] = {0x00};
int8 test_data[11] = {0};
unsigned int32 address_data[4];
unsigned int32 address;
BYTE RTUC[8];
BYTE Down[81];
BYTE RTDC[9];
BYTE dummy[1];
BYTE Finish_sign[1];
int i;
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
void PINO_Test()
{
   dummy[0] = 0x01;
   int32 num;
   while (TRUE)
   {
      command[0] = 0x00;
      
      for (num = 0; num < 100; num++)
      {
         if (kbhit (fab))   //Devuelve verdadero si un caracter ha sido recibido
         {
            //y espera en el buffer para ser leido
            for (int i = 0; i < 9; i++)
            {
               fprintf (fab, "Get command\r\n") ;
               command[i] = fgetc (fab);
            }//Lee el caracter del buffer, UART receive from main pic
            break;
         }
      }
      switch (command[0])
      {
         case 0x13:
         fprintf (fab, "Start 0x13\r\n") ;
         output_low (PIN_A5); //Main side
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         TRANSFER_DATA_NBYTE_TOFAB_SMF(address, command[5] + command[6]);
         fprintf (fab, "Finish 0x13\r\n") ;
         
         break;
         
         case 0x14://Main side
         output_low (PIN_A5) ;
         fprintf (fab, "Start 0x14\r\n") ;
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         sector_erase_SMF (address);
         WRITE_DATA_BYTE_SMF (address, command[5]) ;
         WRITE_DATA_BYTE_SMF (address + 1, command[6]) ;
         WRITE_DATA_BYTE_SMF (address + 2, command[7]) ;
         WRITE_DATA_BYTE_SMF (address + 3, command[8]) ;
         fprintf (fab, "Finish 0x14\r\n");
         break;
         
         case 0x16:
         output_low (PIN_A5);
         fprintf(fab, "Start 0x16\r\n");
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
            switch(command[5]){
               case 0x04:
                  SUBSECTOR_4KB_ERASE_SMF(address);
               case 0x32:
                  SUBSECTOR_32KB_ERASE_SMF(address);
            }
         
         /* case 0x91:
         reset_time_data[0] = 0x82;
         //fprintf (PC, "Command 2 Recieved\r\n") ;
         //fputc (reset_time_data[0], PC) ;
         fputc (0x91, reset);
         break; */
         case 0x91://Finish
         fprintf (fab, "Start 0x91\r\n");
         for (i = 0; i < 9; i++)
         {
            PINO_DATA[i] = command[i];
         }
         
         for (i = 0; i<24; i++)
         {
            fputc (PINO_DATA[i], DC);
         }
         fprintf (fab, "Finish 0x91\r\n") ;
         break;
         
         case 0x92:
         fprintf (fab, "Start 0x92\r\n") ;
         for (i = 0; i < 9; i++)
         {
            PINO_data[i] = command[i];
         }
         
         for (i = 0; i<24; i++)
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
               //fprintf (fab, "Finish transmitting\r\n") ;
               for (i = 0; i < 10; i++)
               {
                  fputc (Down[i], fab);
               }
               fprintf (fab, "Finish 0x92\r\n") ;
               break;
            }
         }
         
         case 0x93:
         fprintf(fab, "Start 0x93\r\n");
         output_high (PINO_power);
         output_high (sel);
         fprintf(fab, "Finish 0x93\r\n");
         
         
         case 0x9B:
         fprintf (fab, "Start 0x9B\r\n") ;
         output_high (hvs);
         fprintf (fab, "Finish 0x9B\r\n");
         output_low (PINO_power);
         output_low (sel);
         break;
         
         case 0x9F:
         fprintf (fab, "Start 0x9F\r\n") ;
         GET_RESET_DATA ();
         fprintf (fab, "Finish 0x9F\r\n") ;
         break;
      }
   }
}
void GET_RESET_DATA()                                                            //funcion que carga el array HKDATA con los datos del Reset PIC
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
   if (RESET_bffr[0] == 0x8e)   //si el byte de cabecera es el correcto
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
      fprintf (fab, "\r\n") ;
      
      PINO_DATA[0] = 0x81;
      
      PINO_DATA[1] = reset_time_data[0];
      PINO_DATA[2] = reset_time_data[1];
      PINO_DATA[3] = reset_time_data[2];
      PINO_DATA[4] = reset_time_data[3];
      PINO_DATA[5] = reset_time_data[4];
      
      for(i=6; i<39; i++)
      {
         PINO_DATA[i] = 0x01;
      }
      
      
      
      for (j = 0; j < 39; j++)
      {
         //fprintf (fab, " % x, ", PINO_DATA[j]) ;
         
         fputc (PINO_DATA[j], DC);
      }
      fputc (Finish_sign[0], DC);
      
      //fprintf (fab, "\r\n") ;
      
      }else{
      fprintf (fab, "\r\nRESET DATA NO OBTAINED\r\n") ;
   }
   return;
}

