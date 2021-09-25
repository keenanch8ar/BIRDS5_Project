
int8 MISSION_STATUS = 0;//MISSION STATUS FLAGS
static int16 currenttime = 0;
BYTE command[9];
int8 reset_time_data[11] = {};
BYTE PINO_DATA[39] = {0x00};
int8 trigger_time_data[5];
unsigned int32 address_data[4];
unsigned int32 packet_data[2];
unsigned int32 data[7];
unsigned int32 address;
unsigned int32 packet;
BYTE Down[81];
int8 MULTSPEC1_DATA[81] = {};
int8 MULTSPEC2_DATA[81] = {};
int8 IMGCLS_DATA[81] = {};
BYTE SFWD_DATA[81] = {0x00};
BYTE NEW_PINO_DATA[81] = {};
BYTE dummy[1];
BYTE Finish_sign[1];
int8 command_time_data[5];
int8 j;
int i;
int result = 1;

//--------FAB HK collection--------------------------------------------------//
#define HK_size 124                                                              //HK FORMAT ARRAY SIZE
#define CW_size 5                                                                //CW FORMAT ARRAY SIZE
#define HIGH_SAMP_HK_size 124                                                    //High Sampling HK FORMAT ARRAY SIZE
#define FAB_SENSOR_size 45                                                       //HK FAB Part

static unsigned int8 CW_FORMAT[CW_size] = {};
unsigned int8 in_HK[FAB_SENSOR_size] = {};
unsigned int8 HKDATA[HK_size] ={};
unsigned int8 ACK_for_COM[24] = {0xAA,0,0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0,0,0,
                                 0,0,0,0xBB};
//int8 in_High_HK[HIGH_SAMP_HK_size] = {};
BYTE FAB_DATA = 0;
//int t;
static int8 FAB_MEASUERING_FLAG= 0;
static int8 HIGH_SAMP_FAB_MEASUERING_FLAG = 0;
int32 FAB_FLAG = 0;
int8 CHECK_FAB_RESPONSE = 0;

#define buffer_from_FAB  (in_bffr_main[0]==0x33)

void DELETE_CMD_FROM_PC()
{
   for(int num = 0; num < 9; num++)
   {
      CMD_FROM_PC[num] = 0;
   }
   return;
}

void DELETE_CMD_FROM_COMM()
{
   for(int num = 0; num < 16; num++)
   {
      CMD_FROM_COMM[num] = 0;
   }
   return;
}
void Delete_Buffer()                                                             //delete com command buffer
{
   int num = 0;
   for(num = 0;num < 16; num++)
   {
    in_bffr_main[num] = 0x00;
   }
   COM_DATA = 0;
   return;
}


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
//--------Main or MB Commands------------------------------------------------//

void MAIN_MB_CMD()
{
   if(CMD_FROM_PC[0])
   {
      for(int m = 0; m < 9; m++)
      {
         command[m] = CMD_FROM_PC[m];
      }
   }
   
   if(CMD_FROM_COMM[0] && CMD_FROM_COMM[4] != 0xAB)
   {

      command[0] = CMD_FROM_COMM[4];
      command[1] = CMD_FROM_COMM[5];
      command[2] = CMD_FROM_COMM[6];
      command[3] = CMD_FROM_COMM[7];
      command[4] = CMD_FROM_COMM[8];
      command[5] = CMD_FROM_COMM[9];
      command[6] = CMD_FROM_COMM[10];
      command[7] = CMD_FROM_COMM[11];
      command[8] = 0x00;
   }
      
   switch (command[0])
   {
//!      case 0x00:
//!         fputc(0x61, FAB);
//!         delay_ms(200);
//!         fprintf(PC,"Battery Voltage %x \r\n", in_HK[FAB_DATA]);
//!         fprintf(PC, "\r\n");
//!      break;
      
      case 0x01:
         fprintf(PC,"Battery Data From FAB: ");
         fputc(0x60, FAB);
         delay_ms(300);
         //fprintf(PC,"Battery Voltage %x \r\n", in_HK[0]);
         fprintf(PC, "\r\n");
         FAB_DATA = 0;
      break;
      
      case 0x02:
         fprintf(PC,"Collect HK Data From FAB: ");
         fputc(0x61, FAB);
         delay_ms(300);
         //fprintf(PC,"Battery Voltage %x \r\n", in_HK[0]);
         fprintf(PC, "\r\n");
         FAB_DATA = 0;
      break;
      
      case 0x03:
      
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
         fprintf(PC, "\r\nBPB POWER ON\r\n");
         Turn_ON_MBP();
         break;
         
         case 0x05:
         fprintf(PC, "\r\nBPB POWER OFF\r\n");
         Turn_OFF_MBP();
         break;
      
         
      
   
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
         TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
         fprintf (PC, "Finish 0x12\r\n") ;
      break;
      
      
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
   }

}
//--------BC Function--------------------------------------------------------//

//unsigned int8 BC_temp_data[2] = {};
unsigned int16 BC_temp_data_h = 0;
unsigned int16 BC_temp_data_l = 0;
unsigned int16 BC_TEMP = 0;
float  temp = 0;
float initial_temp = 0;
float MAXTEMP = 0;
int16 UNREG2 = 0;


void BC_SETUP()  //Analog read configuration (AN9)
{
   ANCON2= 0x01;                                                                 // PIN RC2 Analog enable
   ADCON1L = 0x00;                                                               // SAMP bit must be cleared by software to start conversion (ends sampling and starts converting)
   ADCHS0L = 0x09;                                                               // Connect AN9 as S/H+ input
                                                                                 // AN9 is the input
   ADCON2H = 0x00;                                                               //A/D control register 2H
   ADRC = 1;                                                                     //RC Clock source
   ADCSS0L = 0;                                                                  //Sample select register
   ADCON3L = 0x02;                                                               // Conversion clock select bits, Tad = 3.2/Fosc
   //setup_adc(ADC_CLOCK_DIV_32);                                                //VSS_VDD
   ADCON2L = 0;                                                                  //A/D control register 2L
   return;
}

void BC_READ_TO_PC()
{
   ADON = 1;
   SAMP = 1;
   delay_ms(100);
   
   SAMP = 0; // start converting
   while (!DONE){};
   delay_ms(100);
   //BC_temp_data[0] = ADCBUF0H;//read_adc();
   //BC_temp_data[1] = ADCBUF0L;
   
   BC_temp_data_h = ADCBUF0H;
   BC_temp_data_l = ADCBUF0L;
   //temp = ((data_h << 8 ) | data_l);
   fprintf(PC,"data_l:%x \r\n", BC_temp_data_l);
   fprintf(PC,"data_h:%x \r\n", BC_temp_data_h);
   BC_TEMP = BC_temp_data_h | BC_temp_data_l;
   fprintf(PC,"temp:%f\r\n",temp);
   temp = BC_TEMP/1024*3.31*100-50;    //VDD: 3.31V(MEASURED)
   fprintf(PC,"%1.1f \r\n", temp);
   return;
}

void CHECK_BC_TEMP()                                                             //lee la temperatura del BC y lo guarda en MAXTEMP
{
   BC_SETUP();                                                                   //Analog read configuration (AN9)
   ADON = 1;
   SAMP = 1;
   delay_ms(10);
   
   SAMP = 0;                                                                     // start converting
   while (!DONE){};
   delay_ms(10);
   BC_temp_data_h = ADCBUF0H;                                                    //read_adc();
   BC_temp_data_l = ADCBUF0L;
   //fprintf(PC,"%x%x\r\n",ADCBUF0H,BC_temp_data_l);
   BC_temp = BC_temp_data_h<<8 | BC_temp_data_l;                                 //convierte en un int16
   //fprintf(PC,"HEX : %x%x\r\n",BC_temp>>8,BC_temp);

   temp = BC_temp;                                                               //convierte a float
   //temp = temp/1024*3.25*100-50;
   temp = (temp*3.3/1023);
   temp = temp*100-50;
   fprintf(PC,"Current Temp: %1.1f\r\n",temp);
   
   if(MAXTEMP < temp)
   {
      MAXTEMP = temp;
   }
   fprintf(PC,"%f\r\n",temp);
   return;
}

void MEASURE_BC_TEMP()
{
   BC_SETUP();                                                                   //configuracion para lectura analogica
   ADON = 1;
   SAMP = 1;
   delay_ms(1);
   
   SAMP = 0;                                                                     // start converting
   while (!DONE){};
   delay_ms(1);
   BC_temp_data_h = ADCBUF0H;                                                    //read_adc();
   BC_temp_data_l = ADCBUF0L;

   return;
}

void Turn_ON_BC()
{
   output_high(PIN_D5);                                                          //BC switch ON, RD5=1
   return;
}

void Turn_OFF_BC()
{
   output_low(PIN_D5);                                                           //BC switch OFF, RD5=0
   return;
}

void BC_OPERATION()                                                              //Turn ON BC and read temperature
{
   CHECK_BC_TEMP();                                                              //read BC temperature and save It in float MAXTEMP
   initial_temp = MAXTEMP;                                                       //save temperature in initial_temp before turn_ON BC
   fprintf(PC,"INITIAL TEMPERATURE is %1.1f\r\n\r\n",initial_temp);
   Turn_ON_BC();                                                                 //BC switch ON, RD5=1
   fprintf(PC,"Turned ON BC\r\n");
   currenttime = 0;
   int8 counter = 0;   
   while(currenttime < 30)                                                       //turn ON BC for 30 sec
   {
      for(int i = 0; i < 10; i++)
      {
         COLLECT_RESET_DATA();                                                   //send command to reset PIC requesting RESET DATA
         if(reset_bffr[0] == 0x8e)
         {
            for(i = 0; i < 10; i++);                                             //shows the received array
            {
               fprintf(PC,"%x,",reset_bffr[i]);
            }
            fprintf(PC,"%x\r\n",reset_bffr[10]);
            break;
         }
      }
      CHECK_BC_TEMP();                                                           //read BC temperature and save it in float MAXTEMP
      UNREG2 = UNREG2 + RESET_bffr[10];                                          //add the unreg2 current every second
      counter++;
      delay_ms(900);
   }
      
   Turn_OFF_BC();                                                                //BC switch OFF, RD5=0
   fprintf(PC,"Turned OFF BC\r\n");
   delay_ms(500);
   CHECK_BC_TEMP();                                                              //reads the temperature of the BC and saves it to float MAXTEMP
   if(MAXTEMP-initial_temp > 5)                                                  //sets the HIGH flag if there was an increase in temperature
   {
      ANT_DEP_STATUS = 1;
   }
   fprintf(PC,"MAXIMUM TEMPERATURE is %1.1f\r\n\r\n",MAXTEMP);
   MAXTEMP = 0;
   initial_temp = 0;
   return;
}

void CLEAR_BC_FLAG()
{
   BC_ATTEMPT_FLAG = 0;
   fprintf(PC,"\r\nBC Attempt Flag clear done\r\n");
   return;
}

void MAKE_BC_FLAG_1()
{
   BC_ATTEMPT_FLAG = 1;
   fprintf(PC,"\r\nBC Attempt Flag:1\r\n");
   return;
}

void MAKE_BC_FLAG_2()
{
   BC_ATTEMPT_FLAG = 2;
   fprintf(PC,"\r\nBC Attempt Flag:2\r\n");
   return;
}

void MAKE_BC_FLAG_3()
{
   BC_ATTEMPT_FLAG = 3;
   fprintf(PC,"\r\nBC Attempt Flag:3\r\n");
   return;
}

void MAKE_BC_FLAG_4()
{
   BC_ATTEMPT_FLAG = 4;
   fprintf(PC,"\r\nBC Attempt Flag:4\r\n");
   return;
}
void BC_ON_30min()
{
   if(currenttime > 1805 && BC_ATTEMPT_FLAG == 0)                                //if first attempt and 30 min(1800sec) passed // changed to 1 min
   {
      fprintf(PC,"sending BC command to RESET PIC\r\n");
      for(int16 num = 0; num < 200; num++)
      {
         fputc(0xBC,reset);
         delay_ms(100);
         if(reset_bffr[0] == 0xCB)                                               //condicion para que deje de enviar al recibir el ACK
         {
         break;
         }
      }
      delay_ms(1000);                                                            
      if(reset_bffr[0] == 0xCB)
      {
         BC_OPERATION();
         BC_ATTEMPT_FLAG++;
         WRITE_FLAG_to_EEPROM();
         STORE_FLAG_INFO();
         STORE_ADRESS_DATA_TO_FLASH();
         reset_bffr[0] = 0;
         RESET_DATA = 0;
         delay_ms(1000);
         delay_ms(20000);                                                        //wait until RESET goes back to nomal loop
         SAVE_SAT_LOG(0xBC,0x30,0x30);                                           //first 30 min antenna deployment
      }
   }
   return;
}

void Antenna_Deploy()
{
   fprintf(PC,"Ant Dep Attempt No: %x\r\n",BC_ATTEMPT_FLAG);
   
   if(BC_ATTEMPT_FLAG < 4 && BC_ATTEMPT_FLAG != 0)                                                      //IT WILL BE REPEATED 3 MORE TIMES AFTER THE FIRST DEPLOYMENT, SUCCESSFUL OR NOT
   {
   
      fprintf(PC,"BC command sent to RESET PIC\r\n");
      for(int num = 0; num < 50; num++)
      {
         fputc(0xBC,reset);
         delay_ms(10);
      }
      delay_ms(1000);
      if(reset_bffr[0] == 0xCB)                                                  //check the header of reset respond
      {
         RESET_DATA = 0;
         BC_OPERATION();                                                         //Turn ON BC for 30s and read temperature
         BC_ATTEMPT_FLAG++;                                                      //increase attempt Flag
         STORE_FLAG_INFO();                                                      //save flag data to flash memory
         WRITE_FLAG_to_EEPROM();                                                 //guarda los flags en la EEPROM a partir de la direccion 0x18000 (75%)
         STORE_ADRESS_DATA_TO_FLASH();                                           //guarda los datos de direcciones, en un nuevo sector si se cumple el ciclo de R/W
         reset_bffr[0] = 0;
         CMD_FROM_PC[1] = 0;
         RESET_DATA = 0;
         delay_ms(1000);
         delay_ms(20000);                                                        //wait until RESET goes back to normal loop
         SAVE_SAT_LOG(0xBC,BC_ATTEMPT_FLAG,BC_ATTEMPT_FLAG);
      }
   }
   return;
}

//--------Mission Boss PIC Data Collection-----------------------------------//

int8 DC_ACK = 0;
int8 MBP_DATA[9] = {};

void Turn_On_MBP()
{
   output_high(PIN_F5);
   return;
}

void Turn_Off_MBP()
{
   output_low(PIN_F5);
   return;
}

void DEL_MBP_DATA()
{
   for(int n = 0; n < 9; n++)
   {
      MBP_DATA[n] = 0;  
   }
   return;
}



void Forward_CMD_MBP()
{
      int count = 0;
      for(int8 n = 0; n < 9; n++)
      {
         fputc(command[n], DC);
         delay_ms(10);
      }
      
//!      for(int32 num = 0; num < 1000000; num++)
//!         {
//!            if(kbhit(DC))
//!            {
//!               MBP_DATA[count] = fgetc(DC);
//!               count++;
//!            }
//!            if(count == 9)
//!               break;
//!         }
//!         
//!      fprintf(PC,"Data Recieved from MBP:");
//!      
//!      for(n = 0; n < 9; n++)
//!      {
//!         
//!         fprintf(PC,"%x,",MBP_DATA[n]);
//!      }
//!      
//!      fprintf(PC,"\r\n");
//!      DEL_MBP_DATA();
//!        
//!         
//! 
   return;
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
         fputc(0xAB, reset);
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
         output_low(hvs);
         output_high (PINO_power);
         output_high (sel);
         output_high (PIN_A5);//Mission side
         fputc(0xBC, reset);
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
         Forward_CMD_MBP();
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

   if(CMD_FROM_COMM[0] && CMD_FROM_COMM[4] != 0xAB)
   {

      command[0] = CMD_FROM_COMM[4];
      command[1] = CMD_FROM_COMM[5];
      command[2] = CMD_FROM_COMM[6];
      command[3] = CMD_FROM_COMM[7];
      command[4] = CMD_FROM_COMM[8];
      command[5] = CMD_FROM_COMM[9];
      command[6] = CMD_FROM_COMM[10];
      command[7] = CMD_FROM_COMM[11];
      command[8] = 0x00;
   }
   
   if(CMD_FROM_PC[0])
   {
      for(int m = 0; m < 9; m++)
         {
            command[m] = CMD_FROM_PC[m];
         }
   }
      
   switch (command[0])
   {
   
////////////////////////////FOR CAM1 RPi on MB1////////////////////////////////
      
      case 0x20: //Turn off CAM1 RPi DIO for MOSFET on MB1 to power RPI from 5V      
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x20 - Turn OFF MULTSPEC CAM1 (MB1)\r\n") ;
         output_low(pin_G3); //Turn off DIO for MULTSPEC CAM1
         //Forward_CMD_MBP();
         for(int16 num_reset = 0; num_reset < 200; num_reset++)
         {
            fputc(0xCD,reset);
            delay_ms(100);
            if(reset_bffr[0] == 0xCD)
               {
                  fprintf (PC, "RESET Turned off 5V\r\n");
                  break;
               }
         }
         //fputc(0xCD, reset);
         
         fprintf (PC, "Finish 0x20\r\n");
         
      break;
      
      case 0x21: //Real time uplink command
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x21 - Real time uplink MULTSPEC CAM1\r\n") ;
         Forward_CMD_MBP();
         fprintf (PC, "Finish 0x21\r\n");

      break;
      
      case 0x22: //Real time downlink command
      
//!         output_high (PIN_A5); //SFM2 mission side access
//!         fprintf (PC, "Start 0x22\r\n") ;
//!         output_high(pin_G2); //Turn on DIO for trigger
//!         delay_ms(10000);
//!         output_low(pin_G2); //Turn off DIO for trigger
//!         fprintf (PC, "Finish 0x22\r\n");
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
//!            if(RESERVE_MIN_FLAG >= RESERVE_TARGET_FLAG) 
//!            {
//!               
//!            }
//!
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
      
      case 0x26: //Transfer N packets of data from SFM2 to PC at the specified address locations
      
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

 
      case 0x27://Uplink command to write the data on Flash Memory 2
      
         output_low (PIN_A5) ;//Main side
         fprintf (PC, "Start 0x27\r\n") ;
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
         fprintf (PC, "Finish 0x27\r\n");
      
      break;
      
      case 0x28://Erase the data on SFM2 at the given address. Specify how much data to erase eg. 16 00 01 02 03 FF 00 00 will erase the entire 64kB sector at address 00010203
      
         output_low (PIN_A5);
         fprintf(PC, "Start 0x28\r\n");
         address_data[0] = command[1]<<24;
         address_data[1] = command[2]<<16;
         address_data[2] = command[3]<<8;
         address_data[3] = command[4];
         address = address_data[0] + address_data[1] + address_data[2] + address_data[3];
         packet_data[0] = command[5]<<8;
         packet_data[1] = command[6];
         packet = (packet_data[0] + packet_data[1])*81;
         switch(command[7])
         {
            case 0x04:
               SUBSECTOR_4KB_ERASE_SMF(address);
               fprintf(PC, "Finish 0x28 4KB Erase\r\n");
               TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
               break;
            case 0x32:
               SUBSECTOR_32KB_ERASE_SMF(address);
               TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
               fprintf(PC, "Finish 0x28 32KB Erase\r\n");
               break;
            case 0xFF:
               SECTOR_ERASE_SMF(address);
               TRANSFER_DATA_NBYTE_TOPC_SMF(address, packet);
               fprintf(PC, "Finish 0x28 Sector Erase\r\n");
               break;
         }
         
      break;
      
      case 0x2E: //Turn on CAM1 RPi for MOSFET on MB1 to power RPI from 5V
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start - Turn ON MULTSPEC CAM1 0x2E\r\n") ;
         //fputc(0xEC, reset);
         output_high(pin_G3); //Turn on DIO for MULTSPEC CAM1
         //Forward_CMD_MBP();
         
         for(num_reset = 0; num_reset < 200; num_reset++)
         {
            fputc(0xEC,reset);
            delay_ms(100);
            if(reset_bffr[0] == 0xEC)
               {
                  fprintf (PC, "RESET Turned on 5V\r\n");
                  break;
               }
         }
         fprintf (PC, "Finish 0x2E\r\n"); 
      
      break;
      
////////////////////////////FOR CAM2 RPi on MB2////////////////////////////////
      
     case 0x30: //Turn off CAM2 RPi DIO for MOSFET on MB2 to power RPI from 5V
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x30 - Turn OFF MULTSPEC CAM2 (MB2)\r\n") ;
         //fputc(0xCD, reset);
         output_low(pin_F6); //Turn off DIO for MULTSPEC CAM2
         //Forward_CMD_MBP();
         
         for(num_reset = 0; num_reset < 200; num_reset++)
         {
            fputc(0xCD,reset);
            delay_ms(100);
            if(reset_bffr[0] == 0xCD)
               {
                  fprintf (PC, "RESET Turned off 5V\r\n");
                  break;
               }
         }
         fprintf (PC, "Finish 0x30\r\n");
         
         
      break;
      
      case 0x31: //Turn off CAM1 RPi for MOSFET on MB1 to power RPI from 5V
      

         
      break;
      
      case 0x32: //Turn on CAM1 RPi trigger
      
//!         output_high (PIN_A5); //SFM2 mission side access
//!         fprintf (PC, "Start 0x32\r\n") ;
//!         output_high(pin_F7); //Turn on DIO for trigger
//!         delay_ms(10000);
//!         output_low(pin_F7); //Turn off DIO for trigger
//!         fprintf (PC, "Finish 0x32\r\n");

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
      
      case 0x3E: // Turn on MULTSPEC CAM2 (MB2)
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x3E - Turn ON MULTSPEC CAM2 (MB2)\r\n") ;
         output_high(pin_F6); //Turn on DIO for MULTSPEC CAM2
         //fputc(0xEC, reset);
         
         for(num_reset = 0; num_reset < 200; num_reset++)
         {
            fputc(0xEC,reset);
            delay_ms(100);
            if(reset_bffr[0] == 0xEC)
               {
                  fprintf (PC, "RESET Turned on 5V\r\n");
                  break;
               }
         }
         
         fprintf (PC, "Finish 0x3E\r\n");
         
      break;
      
      default:
         fprintf (PC, "Command:%x", command[0]);
         fprintf (PC, " does not exist\r\n");
         break;
   }
   
   for(int m = 0; m < 9; m++)
   {
      command[m] = 0;
   }
}

void IMGCLS_Test()
{
   if(CMD_FROM_COMM[0] && CMD_FROM_COMM[4] != 0xAB)
   {

      command[0] = CMD_FROM_COMM[4];
      command[1] = CMD_FROM_COMM[5];
      command[2] = CMD_FROM_COMM[6];
      command[3] = CMD_FROM_COMM[7];
      command[4] = CMD_FROM_COMM[8];
      command[5] = CMD_FROM_COMM[9];
      command[6] = CMD_FROM_COMM[10];
      command[7] = CMD_FROM_COMM[11];
      command[8] = 0x00;
   }
   
   if(CMD_FROM_PC[0])
   {
      for(int m = 0; m < 9; m++)
         {
            command[m] = CMD_FROM_PC[m];
         }
   }
   switch (command[0])
   {
      
      case 0x80: //Turn off IMGCLS RPi DIO for MOSFET on RAB to power RPI from 5V line
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x80 - Turn off IMGCLS\r\n") ;
         Forward_CMD_MBP();
         
         for(int16 num_reset_ic = 0; num_reset_ic < 200; num_reset_ic++)
         {
            fputc(0xCD,reset);
            delay_ms(100);
            if(reset_bffr[0] == 0xCD)
               {
                  fprintf (PC, "RESET Turned off 5V\r\n");
                  break;
               }
         }
         fprintf (PC, "Finish 0x80\r\n"); 
         //fputc(0xCD, reset);
         
      break;
      
      case 0x81: //Real time uplink command
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x81 - Real time uplink IMGCLS\r\n") ;
         Forward_CMD_MBP();
         fprintf (PC, "Finish 0x81\r\n"); 
      break;
      
      case 0x82: //Real time downlink IMGCLS
         output_high (PIN_A5);
         fprintf (PC, "Start 0x82 - Real time downlink IMGCLS\r\n") ;
         Forward_CMD_MBP();
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
      
      
      case 0x83://Erase the data on SFM2 at the given address. Specify how much data to erase eg. 16 00 01 02 03 FF will erase the entire 64kB sector at address 00010203
         output_high (PIN_A5);
         fprintf (PC, "Start 0x83 - Mission Mode 1 IMGCLS\r\n") ;
         Forward_CMD_MBP();
         fprintf (PC, "Finish 0x83\r\n"); 
         
      break;
      
      case 0x84:
      
         //Capture in mode 1
         //(1 image, captured immediately, saved to specified address) 
         //eg. 23 is command, 00 02 06 08 is the address location, 01 is number of images to capture, 00 00 00 for remaining unused command bytes (command: 23 00 02 06 08 01 00 00 00)
         
         fprintf (PC, "Start 0x84\r\n") ;
         
         output_high (PIN_A5); //SFM2 mission side access
         
         for (i = 1; i < 9; i++)
         {
            fputc(command[i], DC);
            delay_ms(20);
            fputc(command[i], PC);
         }
         
         fprintf (PC, "Finish 0x84\r\n");
      break;
      
      case 0x85:
      
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
         
         case 0x8E:
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x8E - Turn ON IMGCLS\r\n") ;
         //fputc(0xEC, reset);
         Forward_CMD_MBP();
         
         for(num_reset_ic = 0; num_reset_ic < 200; num_reset_ic++)
         {
            fputc(0xEC,reset);
            delay_ms(100);
            if(reset_bffr[0] == 0xEC)
               {
                  fprintf (PC, "RESET Turned on 5V\r\n");
                  break;
               }
         }
         fprintf (PC, "Finish 0x8E\r\n");
         
         
         break;
     
   }
   for(int m = 0; m < 9; m++)
   {
      command[m] = 0;
   }
}

void DELETE_ADCS_SENSOR()
{
   for(int a = 0; a < ADCS_SENSOR_SIZE; a++)
   {
      ADCS_SENSOR_DATA[a] = 0;
   }
   return;
}

void ADCS_TEST()

{
   if(CMD_FROM_COMM[0] && CMD_FROM_COMM[4] != 0xAB)
   {

      command[0] = CMD_FROM_COMM[4];
      command[1] = CMD_FROM_COMM[5];
      command[2] = CMD_FROM_COMM[6];
      command[3] = CMD_FROM_COMM[7];
      command[4] = CMD_FROM_COMM[8];
      command[5] = CMD_FROM_COMM[9];
      command[6] = CMD_FROM_COMM[10];
      command[7] = CMD_FROM_COMM[11];
      command[8] = 0x00;
   }
   
   if(CMD_FROM_PC[0])
   {
      for(int m = 0; m < 9; m++)
         {
            command[m] = CMD_FROM_PC[m];
         }
   }
      
   switch (command[0])
   {
      
      case 0x40: //Turn off ADCS MCU
   
         fprintf (PC, "Start 0x40 - Turn OFF ADCS\r\n") ;
         Forward_CMD_MBP();
         fprintf (PC, "Finish 0x40\r\n");
         
      break;
      
      case 0x41: //Turn off ADCS MCU
   
//!         output_high (PIN_A5); 
//!         fprintf (PC, "Start 0x41\r\n") ;
//!         fputc(command[0],DC); //Forward command to MB which will turn on ADCS MCU
//!         output_low(pin_G3); //turns on DIO for testing with ADCS
//!         fprintf (PC, "Finish 0x41\r\n");
         fprintf (PC, "Start 0x41 - Real time Uplink ADCS\r\n") ;
         Forward_CMD_MBP();
         fprintf (PC, "Finish 0x41\r\n");
         
      break;
      
      case 0x42:
         fprintf (PC, "Start 0x42 - Real time Downlink ADCS\r\n") ;
         Forward_CMD_MBP();
         
         int8 counter = 0;
         for(int32 adcs = 0; adcs < 1000000; adcs++)
         {
            if(kbhit(DC))
            {
               int8 header = fgetc(DC);
               if (header == 0x55)
               {
                  ADCS_SENSOR_DATA[counter] = header;
                  counter++;
                  for(int32 num = 0; num < 1000000; num++)
                  {
                     if(kbhit(DC))
                     {
                        ADCS_SENSOR_DATA[counter] = fgetc(DC);
                        counter++;
                        if(counter == 14)
                        {
                           break;
                        }
                     }
                  }
                  break;
               }
            }
         
         }
//!         for(int32 num = 0; num < 1000000; num++)
//!            {
//!               if(kbhit(DC))
//!               {
//!                  ADCS_SENSOR_DATA[counter] = fgetc(DC);
//!                  counter++;
//!                  if(counter == 14)
//!                  {
//!                     break;
//!                  }
//!               }
//!            }
         fprintf(PC,"Data Recieved: ");
         for(int l = 0; l < 14; l++)
         {
            fprintf(PC,"%x",ADCS_SENSOR_DATA[l]);
         }
         fprintf(PC,"\r\n");
         fprintf (PC, "Finish 0x42\r\n");
         for(l = 0; l < 14; l++)
         {
            ADCS_SENSOR_DATA[l] = 0;
         }
      
      break;
      
      case 0x43: //Request ADCS Data from ADCS MCU
   
         output_high (PIN_A5); 
         fprintf (PC, "Start 0x43\r\n") ;
         fputc(0xAA, DC); //Forward command to MB which will turn on ADCS MCU
         fputc(0xAA, PC); //Forward command to MB which will turn on ADCS MCU
         
         ADCS_ACK = 0;
         for(int32 num = 0; num < 1000000; num++)
         {
            if(kbhit(DC))
            {
               ADCS_ACK = fgetc(DC);
               break;
            }
            
         }
         
         counter = 0;
         if(ADCS_ACK == 0x55)                                                          //acknowledge
         {
            fprintf(PC,"\r\nADCS ACK received\r\n");
            fprintf(PC,"Recieved ACK: %x\r\n",ADCS_ACK);
            for(int32 num = 0; num < 1000000; num++)
            {
               if(kbhit(DC))
               {
                  ADCS_SENSOR_DATA[counter] = fgetc(DC);
                  counter++;
                  if(counter == 13)
                  {
                     break;
                  }
               }
            }
         }
         else
         {
            
            fprintf(PC,"Recieved ACK: %x\r\n",ADCS_ACK);
         
         }
         //fprintf(PC,"Recieved Data: %x\r\n",ADCS_SENSOR_DATA);
         fprintf(PC,"Data Recieved: ");
         for(l = 0; l < 13; l++)
         {
            fprintf(PC,"%x",ADCS_SENSOR_DATA[l]);
         }
         fprintf(PC,"\r\n");
         fprintf (PC, "Finish 0x43\r\n");
         
      break;
      
      case 0x44: //Turn off ADCS MCU
   
         output_high (PIN_A5); 
         fprintf (PC, "Start 0x44\r\n") ;
         
         currenttime = 0;
         int8 counter3 = 0;   
         while(currenttime < 30)                                                       //turn ON BC for 30 sec
         {
         
            fputc(0xAA, DC); //Forward command to MB which will turn on ADCS MCU
            fputc(0xAA, PC); //Forward command to MB which will turn on ADCS MCU
            
            ADCS_ACK = 0;
            for(int32 num = 0; num < 100000; num++)
            {
               if(kbhit(DC))
               {
                  ADCS_ACK = fgetc(DC);
                  break;
               }
               
            }
            
            int8 counter2 = 0;
            if(ADCS_ACK == 0x55)                                                          //acknowledge
            {
               fprintf(PC,"Recieved adcs ACK: %x\r\n",ADCS_ACK);
               for(int32 num = 0; num < 1000000; num++)
               {
                  if(kbhit(DC))
                  {
                     ADCS_SENSOR_DATA[counter] = fgetc(DC);
                     counter2++;
                     if(counter2 == 13)
                     {
                        break;
                     }
                  }
               }
            }
            else
            {
               
               fprintf(PC,"\r\nADCS ACK not received\r\n");
               fprintf(PC,"Recieved ACK: %x\r\n",ADCS_ACK);
            
            }
            //fprintf(PC,"Recieved Data: %x\r\n",ADCS_SENSOR_DATA);
            fprintf(PC,"Data Recieved: ");
            for(int l = 0; l < 20; l++)
            {
               fprintf(PC,"%x",ADCS_SENSOR_DATA[l]);
            }
            fprintf(PC,"\r\n");
            fprintf (PC, "Finish 0x43\r\n");
            counter3++;
            delay_ms(900);
         }

         fprintf (PC, "Finish 0x44\r\n");
         
      break;
      
      case 0x4E:
      
         fprintf (PC, "Start 0x4E - Turn ON ADCS\r\n") ;
         Forward_CMD_MBP();
         fprintf (PC, "Finish 0x4E\r\n");
      
      break;
      
   
   }
   
   for(int m = 0; m < 9; m++)
   {
      command[m] = 0;
   }
   
}

void SFWD_Test()

{
   if(CMD_FROM_COMM[0] && CMD_FROM_COMM[4] != 0xAB)
   {

      command[0] = CMD_FROM_COMM[4];
      command[1] = CMD_FROM_COMM[5];
      command[2] = CMD_FROM_COMM[6];
      command[3] = CMD_FROM_COMM[7];
      command[4] = CMD_FROM_COMM[8];
      command[5] = CMD_FROM_COMM[9];
      command[6] = CMD_FROM_COMM[10];
      command[7] = CMD_FROM_COMM[11];
      command[8] = 0x00;
   }

   if(CMD_FROM_PC[0])
   {
      for(int m = 0; m < 9; m++)
         {
            command[m] = CMD_FROM_PC[m];
         }
   }
      
   switch (command[0])
   {
      
      case 0x50: //Turn off SFWD MCU

         fprintf (PC, "Start 0x50 - Turn OFF SFWD\r\n") ;
         //fputc(0xCD, reset);
         Forward_CMD_MBP();
         for(int16 num_reset_s = 0; num_reset_s < 200; num_reset_s++)
         {
            fputc(0xCD,reset);
            delay_ms(100);
            if(reset_bffr[0] == 0xCD)
               {
                  fprintf (PC, "RESET Turned off 5V\r\n");
                  break;
               }
         }
         fprintf (PC, "Finish 0x50\r\n");
         
      break;
      
      case 0x5e: //Turn on SFWD MCU
   
         fprintf (PC, "Start 0x5e - Turn ON SWFWD\r\n") ;
         //fputc(0xEC, reset);
         Forward_CMD_MBP();
         for(num_reset_s = 0; num_reset_s < 200; num_reset_s++)
         {
            fputc(0xEC,reset);
            delay_ms(100);
            if(reset_bffr[0] == 0xEC)
               {
                  fprintf (PC, "RESET Turned on 5V\r\n");
                  break;
               }
         }
         
         fprintf (PC, "Finish 0x5e\r\n");
         
      break;
      
      case 0x52:
         fprintf (PC, "Start 0x52 - Request SFWD MB2 Downlink Data\r\n") ;
         for(int y = 0; y < 81; y++)
         {
            SFWD_DATA[y] = 0x00;
         }
         Forward_CMD_MBP();
         int counter_sfd = 0;
         for(int32 num = 0; num < 1000000; num++)
            {
               if(kbhit(DC))
               {
                  SFWD_DATA[counter_sfd] = fgetc(DC);
                  counter_sfd++;
                  if(counter_sfd == 81)
                  {
                     break;
                  }
               }
            }
         fprintf(PC,"Data Recieved from SFWD: ");
         for(y = 0; y < 81; y++)
         {
            fprintf(PC,"%x, ",SFWD_DATA[y]);
            //fputc(SFWD_DATA[l],PC);
         }
         
         fprintf(PC,"\r\n");
         for(y = 0; y < 81; y++)
         {
            //fprintf(PC,"%x, ",SFWD_DATA[y]);
            fputc(SFWD_DATA[y],PC);
         }
         
         fprintf(PC,"\r\n");
         fprintf (PC, "Finish 0x52\r\n");
         for(y = 0; y < 81; y++)
         {
            SFWD_DATA[y] = 0x00;
         }
         int8 dummy[20];
         counter_sfd = 0;
         for(num = 0; num < 100000; num++)
            {
               if(kbhit(DC))
               {
                  dummy[counter_sfd] = fgetc(DC);
                  counter_sfd++;
                  if(counter_sfd == 20)
                  {
                     break;
                  }
               }
            }
         for(y = 0; y < 81; y++)
         {
            SFWD_DATA[y] = 0x00;
         }
         
         
         break;
 
   }
   
   for(int m = 0; m < 9; m++)
   {
      command[m] = 0;
   }

}



void NEW_PINO_Test()

{

   if(CMD_FROM_COMM[0] && CMD_FROM_COMM[4] != 0xAB)
   {

      command[0] = CMD_FROM_COMM[4];
      command[1] = CMD_FROM_COMM[5];
      command[2] = CMD_FROM_COMM[6];
      command[3] = CMD_FROM_COMM[7];
      command[4] = CMD_FROM_COMM[8];
      command[5] = CMD_FROM_COMM[9];
      command[6] = CMD_FROM_COMM[10];
      command[7] = CMD_FROM_COMM[11];
      command[8] = 0x00;
   }

   if(CMD_FROM_PC[0])
   {
      for(int m = 0; m < 9; m++)
         {
            command[m] = CMD_FROM_PC[m];
         }
   }
      
     
   switch (command[0])
   {
      
      case 0x90: //Turn off SFWD MCU

         fprintf (PC, "Start 0x90 - Turn OFF PINO\r\n") ;
         Forward_CMD_MBP();
         output_high (hvs);
         delay_ms(15000);
         output_low (sel);
         output_high (PIN_A5);
         output_low (hvs);
         fputc(0xAB, reset);
         output_low (PINO_power);
         fprintf (PC, "Finish 0x90\r\n");
         
      break;
      
      case 0x9e: //Turn on PINO MCU
   
         fprintf (PC, "Start 0x9E - Turn ON PINO\r\n") ;
         Forward_CMD_MBP();
         output_high (PINO_power);
         fputc(0xBC, reset);
         output_low (PIN_A5);
         output_high (sel);
         output_low (hvs);
         fprintf (PC, "Finish 0x9E\r\n");
         
      break;
      
      case 0x91:
         fprintf (PC, "Start 0x91 - Real Time Uplink Command");
         Forward_CMD_MBP();
         fprintf (PC, "Finish 0x91\r\n");
         break;
         
         case 0x9C://Time and attitude information
         //output_high(PIN_A5);
         int a=0;
         fprintf (PC, "Start 0x9C\r\n") ;
         
         for(a=0; a<12; a++){
            //fputc(0x9C,DC);
            //delay_ms(50);
            //GET_RESET_DATA();
            Forward_CMD_MBP();
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
         fprintf (PC, "Start 0x92 - Request SFWD MB2 Downlink Data\r\n") ;
         Forward_CMD_MBP();
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
void GET_RESET_DATA()
{
   //dummy[0] = 0x11;
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
         fputc (PINO_DATA[j], PC);
         
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
      
      //fputc (dummy[0], DC);
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

