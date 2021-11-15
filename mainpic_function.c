//--------Mission variables--------------------------------------------------//


static int16 currenttime = 0;
static int16 missiontime = 0;
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
int8 DLP_DATA[81] = {};
BYTE SFWD_DATA[81] = {0x00};
BYTE NEW_PINO_DATA[81] = {};
BYTE dummy[1];
BYTE Finish_sign[1];
int8 command_time_data[5];
int8 j;
int i;
int result = 1;
int8 test[9] = {};
int OitaTestArray[14] = {};
float df;
float dg;
float dk;
int8 CWtest[1] = {};

void GIVE_ACCESS_SCF_Nsec(int8 min)
{
   fprintf(PC,"\r\nGiving access to COM for %d min\r\n", min);
   fputc(0x35,COM);
   output_high(PIN_C4);
   if(min > 5)                                                                   //for accidentaly send the long time delay, maximum should be 5 min
   {
      min = 5;
   }
   for(int16 i = 0; i < min * 60; i++)
   {
      delay_ms(1000);                                                            //wait 1 sec
      if(in_bffr_main[4] == 0xab || CMD_FROM_PC[0] == 0xab)
      {
         CMD_FROM_PC[0] = 0;
         CMD_FROM_PC[1] = 0;
         PC_DATA = 0;
         break;
      }
   }
   fprintf(PC,"\r\nBack to Normal Operation\r\n");
   return;
}

//--------FAB HK collection--------------------------------------------------//
#define HK_size 76                                                               //HK FORMAT ARRAY SIZE
#define CW_size 5                                                                //CW FORMAT ARRAY SIZE
#define HIGH_SAMP_HK_size 76                                                     //High Sampling HK FORMAT ARRAY SIZE
#define FAB_SENSOR_size 45                                                       //HK FAB Part

static unsigned int8 CW_FORMAT[CW_size] = {};
unsigned int8 in_HK[FAB_SENSOR_size] = {};
unsigned int8 HKDATA[HK_size] = {};
unsigned int8 ACK_for_COM[24] = {0xAA,0,0,0,0,0,0,0,0,0,
                                 0,0,0,0,0,0,0,0,0,0,
                                 0,0,0,0xBB};

BYTE FAB_DATA = 0;
static int8 FAB_MEASURING_FLAG= 0;
static int8 HIGH_SAMP_FAB_MEASURING_FLAG = 0;
int32 FAB_FLAG = 0;
int8 CHECK_FAB_RESPONSE = 0;

#define buffer_from_FAB  (in_bffr_main[0]==0x33)


//--------BC Function--------------------------------------------------------//

//unsigned int8 BC_temp_data[2] = {};
unsigned int16 BC_temp_data_h = 0;
unsigned int16 BC_temp_data_l = 0;
unsigned int16 BC_TEMP = 0;
float  temp = 0;
float initial_temp = 0;
float MAXTEMP = 0;
int16 UNREG2 = 0;

//--------Mission Boss PIC Data Collection-----------------------------------//

int8 DC_ACK = 0;
int8 MBP_DATA[9] = {};

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


void DELETE_CMD_ARRAY_DATA()
{
   for(int num = 0; num < 9; num++)
   {
      command[num] = 0;
   }
   return;
}


void Delete_Buffer()                                                            
{
   int num = 0;
   for(num = 0;num < 16; num++)
   {
    in_bffr_main[num] = 0x00;
   }
   COM_DATA = 0;
   return;
}

//-------Kill Switch-----------------------------------------------------------
Static int8 KILL_COUNTER = 0;
void Count_Kill_Flag()
{
   KILL_COUNTER++;
   if(Kill_COUNTER > 4)
   {
      Kill_FLAG_MAIN = 1;
      output_high(PIN_A4);                                                       //Kill Switch ON
      fputc(0x17,FAB);
   }
   fprintf(PC,"Kill counter: %x\r\n",KILL_COUNTER);
   return;
}

void Disable_Kill()
{
   KILL_COUNTER = 0;
   
   Kill_FLAG_MAIN = 0;
   output_low(PIN_A4);                                                           //Kill Switch OFF
   fputc(0x18,FAB);
   
   fprintf(PC,"Kill counter: %x\r\n",KILL_COUNTER);
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
      
         fprintf(PC,"Collect HK Data From FAB: ");
         fputc(0x61, FAB);
         FAB_DATA = 0;
         DELETE_CMD_ARRAY_DATA();
         
      break;
      
      case 0x02:
      
         fprintf(PC,"Collect ADCS Data From FAB: ");
         fputc(0x69, FAB);
         // delay_ms(300);
         //fprintf(PC,"Battery Voltage %x \r\n", in_HK[0]);
         FAB_DATA = 0;
         DELETE_CMD_ARRAY_DATA();
         
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
         DELETE_CMD_ARRAY_DATA();

      break;
      
      
      case 0x04:
      
         fprintf(PC, "\r\nBPB POWER ON\r\n");
         Turn_ON_MBP();
         DELETE_CMD_ARRAY_DATA();
      break;
         
      
      case 0x05:
         fprintf(PC, "\r\nBPB POWER OFF\r\n");
         Turn_OFF_MBP();
      break;
      
      
      case 0x06:
         fprintf(PC, "\r\nRefresh Reservation Table\r\n");
         table_refresh();
         
      break;
      
      case 0x07:
         fprintf(PC, "\r\nDisplay Reservation Table\r\n");
         Disp_RSV();
         
      break;
      
      case 0x08:
         fprintf(PC, "\r\nRESET SATELLITE\r\n");
         SEND_CMD_FOR_RESET_SATELLITE ();
         
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
         ERASE_EEPROM_INFO(command[2], command[3], command[4]);                                                    //erase 512byte(from 0x18000 to 0x181ff)
         MEMORY_ERASE(command[2], command[3], command[4]); 
      break;
      
      case 0x19:
         unsigned int16 duration = (unsigned int16)command[2]*6;                //CMD2 is operation time(min), maximum number of readings in 2 hours = 720
         if(duration > 720){duration = 720;}                                   // 6 readings in 1 min, every 10 seconds
         HIGHSAMP_SENSOR_COLLECTION(duration);
      break;
      
      case 0x1F:
         MISSION_STATUS = 1;
         //RESET SATELLITE
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
      break;
   }

}

//--------BC Function--------------------------------------------------------//

void BC_SETUP()                                                                  //Analog read configuration (AN9)
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


void CHECK_BC_TEMP()                                                             //reads the temperature of the BC and saves it in MAXTEMP
{
   BC_SETUP();                                                                   //Analog read configuration (AN9)
   ADON = 1;
   SAMP = 1;
   delay_ms(10);
   
   SAMP = 0;                                                                     //start converting
   while (!DONE){};
   delay_ms(10);
   BC_temp_data_h = ADCBUF0H;                                                    //read_adc();
   BC_temp_data_l = ADCBUF0L;
   //fprintf(PC,"%x%x\r\n",ADCBUF0H,BC_temp_data_l);
   BC_temp = BC_temp_data_h<<8 | BC_temp_data_l;                                 //convert to an int16
   //fprintf(PC,"HEX : %x%x\r\n",BC_temp>>8,BC_temp);

   temp = BC_temp;                                                               //convert to a float
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
   BC_SETUP();                                                                   //configuration for analog reading
   ADON = 1;
   SAMP = 1;
   delay_ms(1);
   
   SAMP = 0;                                                                     //start converting
   while (!DONE){};
   delay_ms(1);
   BC_temp_data_h = ADCBUF0H;                                                    //read_adc();
   BC_temp_data_l = ADCBUF0L;

   return;
}


void Turn_ON_BC()
{
   output_high(PIN_C2);                                                          //BC switch ON, RD5=1
   return;
}


void Turn_OFF_BC()
{
   output_low(PIN_C2);                                                           //BC switch OFF, RD5=0
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
   if(currenttime > 65 && BC_ATTEMPT_FLAG == 0)                                //if first attempt and 30 min(1800sec) passed // changed to 1 min for testing
   {
      fprintf(PC,"sending BC command to RESET PIC\r\n");
      for(int16 num = 0; num < 200; num++)
      {
         fputc(0xBC,reset);
         delay_ms(100);
         if(reset_bffr[0] == 0xCB)                                               //condition for it to stop sending when receiving the ACK
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
         delay_ms(20000);                                                        //wait until RESET goes back to normal loop
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

//------------------------------------HK Collection and forming CW----------------------------------------------------//

void Delete_in_HK()                                                              //Delete the array in_HK [118]
{
   int num;
   for(num = 0; num < HK_size; num++)
   {
      in_HK[num] = 0;
   }
   return;
}


void Delete_HKDATA()                                                             //function that deletes the HKDATA array [118]
{
   int num;
   for(num = 0; num < HK_size; num++)
   {
      HKDATA[num] = 0;
   }
   return;
}


void Send_COMMAND_TO_FAB(int8 cmd)                                               //function that sends commands to the FAB
{
   fputc(cmd,FAB);                                                               //the data and the port are indicated
   return;
}


void COMMAND_TO_FAB(int32 delaytime)                                             //send command to the FAB to request data
{
   FAB_DATA = 0;
   enable_interrupts(INT_rda3);                                                  //Enable UART FAB interrupt to load vector in_HK []
   Send_Command_to_FAB(0x61);                                                    //send command to fab
   int32 a = 0;
   while (FAB_DATA == 0)
   {                                                                             //Wait for the FAB_DATA flag to go high
      a++;                                                                       //the flag is raised when data is received on the UART
      if(a > 50000)
      {
         break;
      }
   }
   waiting(delaytime);                                                           //wait function
   disable_interrupts(INT_rda3);                                                 //disable UART FAB interrupt
   return;
}


int8 CHECK_SUNSHINE(int8 current)
{
   if(current > EX_PANEL_THRESHHOLD)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}


int8 OPERATION_MODE_VALUE(int8 Bat_Vol)
{
   if(Bat_Vol > 0x94)                                                            //0x94HKDATA[116] 
   {//more than 3.8
      OPERATION_MODE = 0x11;
      return 96;
   }
   else if((Bat_Vol > 0x88) && (Bat_Vol <= 0x94))
   {//3.5 to 3.8
      OPERATION_MODE = 0x10;
      return 64;
   }
   else if(Bat_Vol <= 0x88)
   {//less than 3.5
      OPERATION_MODE = 0x00;
      return 16;
   }
}


int8 CONVERT_16bit_GYRO_to_8bit_X(int8 HIGH_8bit, int8 LOW_8bit)
{
   signed int16 row_data = make16(HIGH_8bit, LOW_8bit);
   float ang_v = row_data*0.00875-0.11853;
   /*fprintf(PC,"ang:%.4f\r\n",ang_v);*/
   return (int8)ang_v; 
}


int8 CONVERT_16bit_GYRO_to_8bit_Y(int8 HIGH_8bit, int8 LOW_8bit)
{
   signed int16 row_data = make16(HIGH_8bit, LOW_8bit);
   float ang_v = row_data*0.00875-0.24552;
   /*fprintf(PC,"ang:%.4f\r\n",ang_v);*/
   return (int8)ang_v; 
}


int8 CONVERT_16bit_GYRO_to_8bit_Z(int8 HIGH_8bit, int8 LOW_8bit)
{
   signed int16 row_data = make16(HIGH_8bit, LOW_8bit);
   float ang_v = row_data*0.00875-0.40486;
   /*fprintf(PC,"ang:%.4f\r\n",ang_v);*/
   return (int8)ang_v; 
}


void REFRESH_CW_ACK_for_COM()                                                    //load the ACK_for_COM array with 0x00 [7]
{
   for(int i = 1; i < 7; i++)
   {
      ACK_for_COM[i] = 0x00;
   }
   return;
}


void REFRESH_MSN_ACK_for_COM()                                                   //clear array ACK_for_COM[i] from position 12 to 22
{
   for(int i = 12; i < 23; i++)
   {
      ACK_for_COM[i] = 0x00;
   }
   return;
}


void CW_RESPOND()                                                                //load array ACK_for_COM [] with data from array CW_FORMAT [] and send to COM PIC
{
   REFRESH_CW_ACK_for_COM();                                                     //load with 0x00 the array ACK_for_COM [] from pos 1 to 6
   delay_ms(100);
   ACK_for_COM[0] = 0xAA;
   ACK_for_COM[1] = 0x50;
   ACK_for_COM[2] = CW_FORMAT[0];
   ACK_for_COM[3] = CW_FORMAT[1];
   ACK_for_COM[4] = CW_FORMAT[2];
   ACK_for_COM[5] = CW_FORMAT[3];
   ACK_for_COM[6] = CW_FORMAT[4];
   ACK_for_COM[23] = 0xBB;
   for(int a = 0; a < 24; a++)                                                   //send the latest CW data to COM PIC
   {
      fputc(ACK_for_COM[a],COM);
   }
   
   fprintf(PC,"\r\nCW ANS:");
   for(a = 0; a < 23; a++)                                                       //send the latest CW data to PC
   {
      fprintf(PC,"%x,",ACK_for_COM[a]);
   }
   fprintf(PC,"%x\r\n",ACK_for_COM[23]);
   
//!   for(int k = 14; k < 23; k++)                                                   //clear RSV part
//!   {
//!      ACK_for_COM[k] = 0x00;
//!   }
   return;
}


void CHECK_50_and_CW_RESPOND()                                                   //check if command 0x50 arrived from COM PIC and send CW to COM
{
   if((in_bffr_main[4] == 0xAB) || (CMD_FROM_COMM[4] == 0xab))
   {
      CW_RESPOND();                                                              //load array ACK_for_COM [] with data from array CW_FORMAT [] and send to COM PIC
      Delete_Buffer();                                                           //delete in_bffr_main[] (COM command buffer)
      DELETE_CMD_FROM_COMM();
      COM_DATA = 0;                                                              //flag to zero
   }
   return;
}


void Delete_CW_FORMAT()                                                           //clear array CW_FORMAT [5]
{

   for(int num = 0; num < CW_size; num++)
   {
      CW_FORMAT[num] = 0;
   }
   return;
   
}


void MAKE_CW1_FORMAT()                                                           
{
   Delete_CW_FORMAT();                                                            //borra el array CW_FORMAT[5]
   
   CW_FORMAT[0] = HKDATA[44];                                                    //battery voltage
   CW_FORMAT[1] = HKDATA[45]<<4|HKDATA[46]>>4;                                   //battery current
   CW_FORMAT[2] = HKDATA[47];                                                    //battery temp
   
//   CW_FORMAT[3] = CW_FORMAT[3];//0:CW1
//   CW_FORMAT[3] = CW_FORMAT[3] + RESERVE_CHECK * 64;
   CW_FORMAT[3] = CW_FORMAT[3] + OPERATION_MODE_VALUE(HKDATA[68]);              //96(=64+32-->0x01100000):nomal, 64(0x01000000):low, 16(0x00100000):safe
   KILL_FLAG_FAB = (HKDATA[49] & 0x10)>>4;
   KILL_FLAG_MAIN = HKDATA[49] & 0x01;
   CW_FORMAT[3] = CW_FORMAT[3] + KILL_FLAG_MAIN * 16;
   CW_FORMAT[3] = CW_FORMAT[3] + KILL_FLAG_FAB * 8;
   CW_FORMAT[3] = CW_FORMAT[3] + ANT_DEP_STATUS * 4;
   CW_FORMAT[3] = CW_FORMAT[3] + CHECK_SUNSHINE(HKDATA[34]) * 2;                 //+Y Panel (new axis definition)
   CW_FORMAT[3] = CW_FORMAT[3] + CHECK_SUNSHINE(HKDATA[35]) * 1;                 //+X Panel (new axis definition)
   
   CW_FORMAT[4] = CW_FORMAT[4] + CHECK_SUNSHINE(HKDATA[36]) * 128;               //-Z Panel (new axis definition)
   CW_FORMAT[4] = CW_FORMAT[4] + CHECK_SUNSHINE(HKDATA[37]) * 64;                //-X Panel (new axis definition)
   CW_FORMAT[4] = CW_FORMAT[4] + CHECK_SUNSHINE(HKDATA[38]) * 32;                //+Z Panel (new axis definition)
   CW_FORMAT[4] = CW_FORMAT[4] + (HKDATA[4] & 0b00011111);                       //time data
   
   CW_IDENTIFIER = 0;
   CHECK_50_and_CW_RESPOND();
   ACK_for_COM[0] = 0xAA;                                                        //for safety (this byte should be always 0)
   ACK_for_COM[23] = 0xBB;                                                       //for safety (this byte should be always 0)
   return;
}


void MAKE_CW2_FORMAT()                                                           
{
   Delete_CW_FORMAT();
   CW_FORMAT[0] = CONVERT_16bit_GYRO_to_8bit_X(HKDATA[59], HKDATA[60]);          //GYRO X axis
   CW_FORMAT[1] = CONVERT_16bit_GYRO_to_8bit_Y(HKDATA[61], HKDATA[62]);          //GYRO Y axis
   CW_FORMAT[2] = CONVERT_16bit_GYRO_to_8bit_Z(HKDATA[63], HKDATA[64]);          //GYRO Z axis
   
   CW_FORMAT[3] = CW_FORMAT[3] + 128;                                            //1:CW2
   CW_FORMAT[3] = CW_FORMAT[3] + FIRST_HSSC_DONE * 64;                           //High Sampling Sensor Collection Flag 0:not done, 1:done
   CW_FORMAT[3] = CW_FORMAT[3] + AUTO_CAM_DONE * 32;                             //AUTO CAM MISSION DONE 0:not done, 1:done
   CW_FORMAT[3] = CW_FORMAT[3] + AUTO_MBP_DONE * 16;                             //AUTO MBP MISSION DONE 0:not done, 1:done
   CW_FORMAT[3] = CW_FORMAT[3] + AUTO_ADCS_DONE * 8;                             //AUTO ADCS MISSION DONE 0:not done, 1:done
   CW_FORMAT[3] = CW_FORMAT[3] + HKDATA[48] * 4;                                 //Heater 0:OFF, 1:ON
   CW_FORMAT[3] = CW_FORMAT[3] + RESERVE_CHECK * 2;                              //RSV Flag
   CW_FORMAT[3] = CW_FORMAT[3] + UPLINK_SUCCESS;                                 //UPLINK SUCCESS
   
   CW_FORMAT[4] = MISSION_STATUS<<4;                                             //MISSION STATUS FLAG
   CW_FORMAT[4] = CW_FORMAT[4] + (MISSION_OPERATING & 0x01);                     //MISSION OPERATION FLAG

   CW_IDENTIFIER = 1;
   CHECK_50_and_CW_RESPOND();
   ACK_for_COM[0] = 0xAA;                                                        //for safety (this byte should be always 0)
   ACK_for_COM[23] = 0xBB;                                                       //for safety (this byte should be always 0)
   
   return;
}


void MAKE_CW_FORMAT()
{
   if(CW_IDENTIFIER)
   {
      MAKE_CW1_FORMAT();
   }
   else
   {
      MAKE_CW2_FORMAT();
   }
}


void SAVE_HKDATA_TO_SCF(unsigned int32 Memory_Address)
{
   output_low(PIN_C4);
   for(int8 num = 0; num < HK_size; num++)
   {
      WRITE_DATA_BYTE_SCF(Memory_Address+num, HKDATA[num]);
   }
   output_high(PIN_C4);
   return;
}


void Send_HKDATA_to_SCF(int32 address)
{
   Save_HKDATA_to_SCF(address);  //save HK to COM PIC
   CHECK_50_and_CW_RESPOND();
   return;
}


void SAVE_HKDATA_TO_SMF(unsigned int32 Memory_Address)
{
   output_low(PIN_A5);
   for(int8 num = 0; num < HK_size; num++)
   {
      WRITE_DATA_BYTE_SMF(Memory_Address+num, HKDATA[num]);
   }
   return;
}


void Send_HKDATA_to_SMF(int32 address)
{
   Save_HKDATA_to_SMF(address);  //save HK to COM PIC
   CHECK_50_and_CW_RESPOND();
   fprintf(PC,"\r\nHK SAVED:\r\n");
   return;
}


void SAVE_HKDATA_TO_OF(unsigned int32 Memory_Address)
{
   int num;
   for(num = 0; num < HK_size; num++)
   {
      WRITE_DATA_BYTE_OF(Memory_Address+num, HKDATA[num]);
   }
   return;
}


void Send_HKDATA_to_OF(int32 address)
{
   Save_HKDATA_to_OF(address);  //save HK to COM PIC
   TRANSFER_DATA_NBYTE_TOPC_OF(address,HK_size);
   CHECK_50_and_CW_RESPOND();
   return;
}


void SAVE_CWFORMAT_TO_SCF(unsigned int32 Memory_Address)
{
   output_low(PIN_C4);
   for(int8 num = 0; num < CW_size; num++)
   {
      WRITE_DATA_BYTE_SCF(Memory_Address+num, CW_FORMAT[num]);
   }
   output_high(PIN_C4);
   return;
}


void SEND_CWFORMAT_TO_SCF(int32 address)
{
   SAVE_CWFORMAT_TO_SCF(address);                                                 //save HK to COM PIC
   CHECK_50_and_CW_RESPOND();
   return;
}


void SAVE_CWFORMAT_TO_SMF(unsigned int32 Memory_Address)
{
   output_low(PIN_A5);
   for(int8 num = 0; num < CW_size; num++)
   {
      WRITE_DATA_BYTE_SMF(Memory_Address+num, CW_FORMAT[num]);
   }
   return;
}


void SEND_CWFORMAT_TO_SMF(int32 address)
{
   SAVE_CWFORMAT_TO_SMF(address);  //save HK to COM PIC
   CHECK_50_and_CW_RESPOND();
   fprintf(PC,"\r\nCW SAVED:\r\n");
   return;
}


void SAVE_CWFORMAT_TO_OF(unsigned int32 Memory_Address)
{
   for(int8 num = 0; num < CW_size; num++)
   {
      WRITE_DATA_BYTE_OF(Memory_Address+num, CW_FORMAT[num]);
   }
   return;
}


void SEND_CWFORMAT_TO_OF(int32 address)
{
   SAVE_CWFORMAT_TO_OF(address);                                                 //save HK to COM PIC
   TRANSFER_DATA_NBYTE_TOPC_OF(address,CW_size);                                  //for checking whether the data saved correctly
   CHECK_50_and_CW_RESPOND();
   return;
}


void CHECK_HKDATA(int8 in,int32 delaytime)                                       //function that loads the HKDATA array []
{
   fprintf(PC,"\r\nGET FAB SENSOR DATA: ");
   Delete_HKDATA();                                                              //delete the HKDATA [] array
   waiting(delaytime);                                                           //waiting
   CHECK_50_and_CW_RESPOND();
   for(int num = 1; num < 11 - in; num++)                                        //[FAB] +X,-Y,-Z,+Y,-X temp_high,low(array[10] to [17])
   {
      HKDATA[num + 5+4] = in_HK[num + 2 - in];                                   //places the data sent by the FAB in the HKDATA [] array from position 10 to 17
      /* fputc(HKDATA[num + 5+4],PC); */                                         //prints the data from position 10 to 19
      fprintf(PC,"%x ",HKDATA[num + 5+4]);
   }
   
   MEASURE_BC_TEMP();                                                            //analog reading and loading of the variable BC_temp_data_h and BC_temp_data_l
   CHECK_50_and_CW_RESPOND();   
   HKDATA[14+4] = BC_temp_data_h;                                                //+X temp high[18]
   HKDATA[15+4] = BC_temp_data_l;                                                //+X temp low[19]
   
   for(num = 9; num < FAB_SENSOR_size - 2; num++)                                //[FAB] from CPLD temp to Kill status(array[20] to [49])
   {
      HKDATA[num + 7+4] = in_HK[num + 2 - in];
      /*fputc(HKDATA[num + 7+4],PC);*/                                           //prints the data from position 20 to 49
      fprintf(PC,"%x ",HKDATA[num + 7+4]);
   }
   fprintf(PC,"\r\n");
   FAB_DATA = 0;                                                                 //reset the flag
   return;
}


void VERIFY_FABDATA(int32 delaytime1,int32 delaytime2)
{
   for(int8 num = 0; num < 3; num++)
   {
      COMMAND_TO_FAB(delaytime1);                                                //function that sends command to the FAB and loads the array in_HK []
      CHECK_50_and_CW_RESPOND();
      //FAB_DATA = 0;
      if(in_HK[0] == 0x33)                                                       //gather sensor data by interrupt
      {
         CHECK_HKDATA(2,delaytime2);                                             //function that loads the HKDATA array [118]
         CHECK_50_and_CW_RESPOND();
         CHECK_FAB_RESPONSE = 1;                                                 //1 is succeeded to get response from FAB
         break;
      }
      else if(in_HK[1] == 0x33)
      {
         //delay_ms(200);
         CHECK_HKDATA(1,delaytime2);
         CHECK_50_and_CW_RESPOND();
         CHECK_FAB_RESPONSE = 1;                                                 //1 is succeeded to get response from FAB
         break;
      }
      else if(in_HK[2] == 0x33)
      {
         //delay_ms(200);
         CHECK_HKDATA(0,delaytime2);
         CHECK_50_and_CW_RESPOND();
         CHECK_FAB_RESPONSE = 1;                                                 //1 is succeeded to get response from FAB
         break;
      }
   }
   return;
}


void REPLY_TO_COM(int8 data1,int8 data2)
{
   if(ACK_for_COM[12] == 0x66 && ACK_for_COM[14] == 0x77)
   {
      for(int n = 0; n < 24; n++)                                                //send back the acknowledge
      {
         fputc(ACK_for_COM[n],COM);
      }
      for(n = 0; n < 24; n++)                                                    //send back the acknowledge
      {
         fprintf(PC,"%x,",ACK_for_COM[n]);
      }
      fprintf(PC,",AUTO\r\n");
   }
   else{
      //REFLESH_MSN_ACK_for_COM();
//!      for(int n = 7; n < 24; n++)//send back the acknowledge
//!      {
//!         ACK_for_COM[n] = 0xff;
//!      }
      ACK_for_COM[0] = 0xAA;
      ACK_for_COM[12] = data1;
      ACK_for_COM[13] = data2;
      ACK_for_COM[23] = 0xBB;
      for(int n = 0; n < 24; n++)                                                //send back the acknowledge
      {
         fputc(ACK_for_COM[n],COM);
      }
      for(n = 0; n < 24; n++)                                                    //send back the acknowledge
      {
         fprintf(PC,"%x,",ACK_for_COM[n]);
      }
      fprintf(PC,",NORMAL\r\n");
   }

}


void UPDATE_ACK_for_COM(int8 data1,int8 data2,int32 address, int16 size)         //send data to COM for automatical mission
{
   fprintf(PC,"address is %lx\r\n", address);
   fprintf(PC,"size is %lu\r\n",size);
   REFRESH_MSN_ACK_for_COM();                                                    //clear array ACK_for_COM[i] from position 12 to 22
   ACK_for_COM[15] = address >> 24;
   ACK_for_COM[16] = address >> 16;
   ACK_for_COM[17] = address >> 8;
   ACK_for_COM[18] = address;
   
   ACK_for_COM[19] = 0;
   ACK_for_COM[20] = 0;
   ACK_for_COM[21] = size >> 8;
   ACK_for_COM[22] = size;
   
   ACK_for_COM[0] = 0xAA;                                                        //header
   ACK_for_COM[12] = data1;                                                      //success ack indicator 0x66
   ACK_for_COM[14] = data2;                                                      //auto indicator 0x77
   ACK_for_COM[23] = 0xBB;                                                       //footer
   fprintf(PC,"sending to COM PIC: \r\n");
   for(int m = 0; m<24; m++)
   {
      fprintf(PC,"%x ",ACK_for_COM[m]);
   }
   fprintf(PC,"\r\n");
   for(int n = 0; n < 24; n++)                                                   //send back the acknowledge
   {
      fputc(ACK_for_COM[n],COM);
   }
}


void GET_RESET_DATA()                                                            //function that loads the HKDATA array with the Reset PIC data
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
   CHECK_50_and_CW_RESPOND();
   if(RESET_bffr[0] == 0x8e)                                                     //if the header byte is correct
   {
      fprintf(PC,"\r\nRESET DATA OBTAINED: ");
      for(int num = 0; num < 5; num++)                                           //load the HKDATA array with timedata in positions 2 to 6
      {
         HKDATA[num + 2] = reset_bffr[num + 1];
         /*
         fputc(HKDATA[num + 2],PC);
         */
         fprintf(PC,"%x ",HKDATA[num + 2]);                                      //show to serial monitor
      }
   
      for(num = 0; num < 5; num++)                                               //load the HKDATA array with reset sensor data in positions 110 to 114
      {
         HKDATA[num + 68] = reset_bffr[num + 6];                                //load the HKDATA [] with the reset data []
         /*
         fputc(HKDATA[num + 116],PC);
         */
         fprintf(PC,"%x ",HKDATA[num + 68]);                                    //show to serial monitor
      }
      fprintf(PC,"\r\n");
   }
   else
   {
      fprintf(PC,"\r\nRESET DATA NO OBTAINED\r\n");
   }
   //Delete_Reset();
   return;
}


void Turn_On_ADCS()
{
   
   fputc(0x4E, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   return;

}


void Delete_ADCS_data()
{

   for(int8 l = 0; l < 14; l++)
   {
      ADCS_SENSOR_DATA[l] = 0;
   }

}


void Turn_Off_ADCS()
{
   
   fputc(0x40, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   fputc(0x00, DC);
   delay_ms(10);
   return;

}


void GET_ADCS_SENSOR_DATA()                                                      //after that, method will changed (ADCS make format and just send to MAIN PIC)
{
   
   CHECK_50_and_CW_RESPOND();
   
   if (MISSION_STATUS == 0)
   {

      fputc(0x42, DC);                                                              //Request ADCS Data by sending command to Mission Boss to forward to ADCS MCU
      delay_ms(10);
      fputc(0xAA, DC);
      delay_ms(10);
      fputc(0x00, DC);
      delay_ms(10);
      fputc(0x00, DC);
      delay_ms(10);
      fputc(0x00, DC);
      delay_ms(10);
      fputc(0x00, DC);
      delay_ms(10);
      fputc(0x00, DC);
      delay_ms(10);
      fputc(0x00, DC);
      delay_ms(10);
      fputc(0x00, DC);
      delay_ms(10);
      
      int8 counter = 0;
      for(int32 adcs = 0; adcs < 100000; adcs++)
      {
         if(kbhit(DC))
         {
            int8 header = fgetc(DC);
            if (header == 0x55)
            {
               ADCS_SENSOR_DATA[counter] = header;
               counter++;
               for(int32 num = 0; num < 100000; num++)
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
      fprintf(PC,"\r\nADCS DATA: ");
      for(int l = 0; l < 14; l++)
      {
         fprintf(PC,"%x ",ADCS_SENSOR_DATA[l]);
      }
      fprintf(PC,"\r\n");
   }
   else
   {
      Delete_ADCS_data();
   }
   
//!   OitaTestArray[0] = ADCS_SENSOR_DATA[0];
//!   OitaTestArray[1] = ADCS_SENSOR_DATA[1];
//!   OitaTestArray[2] = ADCS_SENSOR_DATA[2];
//!   OitaTestArray[3] = ADCS_SENSOR_DATA[3];
//!   OitaTestArray[4] = ADCS_SENSOR_DATA[4];
//!   OitaTestArray[5] = ADCS_SENSOR_DATA[5];
//!   OitaTestArray[6] = ADCS_SENSOR_DATA[6];
//!   OitaTestArray[7] = ADCS_SENSOR_DATA[7];
//!   OitaTestArray[8] = ADCS_SENSOR_DATA[8];
//!   OitaTestArray[9] = ADCS_SENSOR_DATA[9];
//!   OitaTestArray[10] = ADCS_SENSOR_DATA[10];
//!   OitaTestArray[11] = ADCS_SENSOR_DATA[11];
//!   OitaTestArray[12] = ADCS_SENSOR_DATA[12];
//!   OitaTestArray[13] = ADCS_SENSOR_DATA[13];

   
//!   for(int lm = 0; lm < 14; lm++)
//!   {
//!      fprintf(PC, "%d ", OitaTestArray[lm]);
//!   }
//!   fprintf(PC,"\r\n");
   
//!   df = make16(ADCS_SENSOR_DATA[1], ADCS_SENSOR_DATA[2]);
//!   dg = make16(ADCS_SENSOR_DATA[3], ADCS_SENSOR_DATA[4]);
//!   dk = make16(ADCS_SENSOR_DATA[5], ADCS_SENSOR_DATA[6]);
//!      
      
//!   fprintf(PC, "%03f ", df);
//!   fprintf(PC, "%03f ", dg);
//!   fprintf(PC, "%03f ", dk);
//!   fprintf(PC,"\r\n");
//!   
//!   fprintf(PC, "%5.3f ", df);
//!   fprintf(PC, "%5.3f ", dg);
//!   fprintf(PC, "%5.3f ", dk);
//!   fprintf(PC,"\r\n");
//!   
//!   df = df/100;
//!   dg = dg/100;
//!   dk = dk/100;
//!   
//!   fprintf(PC, "%5.3f ", df);
//!   fprintf(PC, "%5.3f ", dg);
//!   fprintf(PC, "%5.3f ", dk);
//!   fprintf(PC,"\r\n");
//!
//!   if((df - 32768.00) > 0)
//!   {
//!      fprintf(PC, "%f ", ((df-32768)*-1)/100);
//!   }
//!   else
//!   {
//!      fprintf(PC, "%f ", df/100);
//!   }
//!      
//!   if((dg - 32768.00) > 0)
//!   {
//!      fprintf(PC, "%f ", ((dg-32768)*-1)/100);
//!   }
//!   else
//!   {
//!      fprintf(PC, "%f ", dg/100);
//!   }
//!      
//!   if((dk - 32768.00) > 0)
//!   {
//!      fprintf(PC, "%f ", ((dk-32768)*-1)/100);
//!   }
//!   else
//!   {
//!      fprintf(PC, "%f ", dk/100);
//!   }
      
//!   fprintf(PC, "%f ", dg);
//!   fprintf(PC, "%f ", dk);
//!   fprintf(PC,"\r\n");
  
   return;
}


void MAKE_ADCS_HKDATA()                                                          //loads into the HKDATA [] array the ADCS data in positions 53 to 106
{
   
   GET_ADCS_SENSOR_DATA();                                                       //function that loads the ADCS_SENSOR_DATA [] array with the ADCS data
   
   CHECK_50_and_CW_RESPOND();   
   
   for(int num = 53; num < 65; num++)                                           //12byte(MAG6,GYRO6)
   {
      HKDATA[num] = ADCS_SENSOR_DATA[num - 52];                                  //ADCS[1] to ADCS[12]
   }
   
   return;
}


void DISPLAY_CW()                                                               //function that prints the array CW_FORMAT []
{
   fprintf(PC,"\r\nCW Data:\r\n");
   for(int8 i = 0; i < 5; i++)
   {
      fprintf(PC,"%x",CW_FORMAT[i]);
   }
   fprintf(PC,"\r\n");
   CHECK_50_and_CW_RESPOND();   
   return;
}


void SET_IDENTIFIER()
{
   CHECK_50_and_CW_RESPOND();
   HKDATA[0] = 0x33;
   HKDATA[1] = 0x33;
   HKDATA[7] = 0xAA;
   HKDATA[8] = 0xAA;
   HKDATA[9] = 0xAA;
   HKDATA[50] = 0xBB;
   HKDATA[51] = 0xBB;
   HKDATA[52] = 0xBB;
   HKDATA[65] = 0xCC;
   HKDATA[66] = 0xCC;
   HKDATA[67] = 0xCC;
   HKDATA[74] = 0x44;
   HKDATA[75] = 0x44;
   return;
}


void FAB_TEST_OPERATION()
{
   Delete_in_HK();                                                               //Delete the array in_HK [124]
   LOOP_FAB_HK_ADDRESS();                                                        //Rotate the save positions of the data if it reaches the end of the allocated space 
   LOOP_FAB_CW_ADDRESS();                                                        //Rotate the save positions of the data if it reaches the end of the allocated space
   CHECK_50_and_CW_RESPOND();                                                    //check if command 0x50 arrived from COM PIC and send CW to COM
   LOOP_FAB_HK_ADDRESS();                                                        //Rotate the save positions of the data if it reaches the end of the allocated space
   CHECK_50_and_CW_RESPOND();                                                    //check if command 0x50 arrived from COM PIC and send CW to COM
   LOOP_FAB_CW_ADDRESS();                                                        //Rotate the save positions of the data if it reaches the end of the allocated space
   CHECK_50_and_CW_RESPOND();                                                    //check if command 0x50 arrived from COM PIC and send CW to COM
   Turn_On_MBP();
   //Turn_On_ADCS();                                                               //turn ON ADCS, RD6 = HIGH                                                               
   fprintf(PC,"FAB communication start\r\n");
   waiting(200000);                                                              //wait function, about 200.000=1s
   FAB_MEASURING_FLAG++;                                                         //count until 7(it means 10 min)
   FAB_DATA = 0;                                                                 //for initialize
   
   fprintf(PC,"SENDING COMMAND TO FAB\r\n");
   CHECK_50_and_CW_RESPOND();                                                    //check if command 0x50 arrived from COM PIC and send CW to COM
   VERIFY_FABDATA(130000,2000);//delaytime1 and delaytime2                       //send command to the FAB and load the array in_HK [] with the data from the FAB
   CHECK_50_and_CW_RESPOND();
   
   GET_RESET_DATA();                                                             //send command to reset PIC and load HKDATA [] array with Reset PIC data
   
   MAKE_ADCS_HKDATA();                                                           //send command to ADCS MCS and load HKDATA [] array with ADCS data
   
   Delete_ADCS_data();
   //Turn_Off_ADCS();                                                              //ADCS switch OFF
   //Turn_Off_MBP();                                                               //Turn off mission boss and CPLD
   
   CHECK_50_and_CW_RESPOND();
   MAKE_CW_FORMAT();                                                             //make CW format
   CHECK_50_and_CW_RESPOND();   
   SET_IDENTIFIER();                                                             //load the HKDATA [] with the identifiers (headers and footers)  
   if(CHECK_FAB_RESPONSE)                                                        //check if HKDATA [] was loaded with the FAB data and performed the CW format
   {
      fprintf(PC,"\r\nCollecting HK and Making CW Format Done\r\n");

      fprintf(PC,"\r\nHK data:\r\n");

      CHECK_50_and_CW_RESPOND();
      
      for(int num = 0; num < 65; num++)                                          //array[0] to [64](until gyro data)
      {
         fprintf(PC,"%x ",HKDATA[num]);
      }
      CHECK_50_and_CW_RESPOND();
      for(num = 65; num < HK_Size; num++)                                            //GPS 60 byte should be shown as char type
      {
      
         fprintf(PC,"%x ",HKDATA[num]);                                          //just for test
      }
      CHECK_50_and_CW_RESPOND();

      fprintf(PC,"\r\n");
      CHECK_50_and_CW_RESPOND();
      
      DISPLAY_CW();                                                              //function that prints the array CW_FORMAT []
      CHECK_FAB_RESPONSE = 0;                                                    //FAB flag to zero
      if (MISSION_STATUS == 0)
      {
         output_low(PIN_C4);                                                        //Shared COM Flash memory, Main side
         output_low(PIN_A5);                                                        //Shared Mission Flash, Main side
         
         SEND_HKDATA_to_SCF(FAB_HK_ADDRESS);                                        //save the HKDATA array in COM flash []
         SEND_HKDATA_to_SMF(FAB_HK_ADDRESS);                                        //save the HKDATA array in SMF flash []
         SEND_HKDATA_to_OF(FAB_HK_ADDRESS);                                         //save the HKDATA array in Main flash []
         
         SEND_CWFORMAT_TO_SCF(FAB_CW_ADDRESS);                                      //save the CWFORMAT array in COM flash []
         SEND_CWFORMAT_TO_SMF(FAB_CW_ADDRESS);                                      //save the CWFORMAT array in SMF flash []
         SEND_CWFORMAT_TO_OF(FAB_CW_ADDRESS);                                       //save the CWFORMAT array in Main flash []
         
         FAB_HK_ADDRESS = FAB_HK_ADDRESS + HK_size;                                 //prepare for next storing address
         FAB_CW_ADDRESS = FAB_CW_ADDRESS + CW_size;                                 //prepare for next storing address
         
         fprintf(PC,"\r\nSENSORS DATA SAVED ON FLASH\r\n");
         output_high(PIN_C4);                                                       //COM Flash memory COM side
         CHECK_50_and_CW_RESPOND();
      }

   }
   else
   {
      if (MISSION_STATUS == 0)
      {
         SEND_HKDATA_to_SCF(FAB_HK_ADDRESS);                                        //save the HKDATA array in COM flash []
         SEND_HKDATA_to_SMF(FAB_HK_ADDRESS);                                        //save the HKDATA array in SMF flash []
         SEND_HKDATA_to_OF(FAB_HK_ADDRESS);                                         //save the HKDATA array in Main flash []
         
         SEND_CWFORMAT_TO_SCF(FAB_CW_ADDRESS);                                      //save the CWFORMAT array in COM flash []
         SEND_CWFORMAT_TO_SMF(FAB_CW_ADDRESS);                                      //save the CWFORMAT array in SMF flash []
         SEND_CWFORMAT_TO_OF(FAB_CW_ADDRESS);                                       //save the CWFORMAT array in Main flash []
         
         FAB_HK_ADDRESS = FAB_HK_ADDRESS + HK_size;                                 //prepare for next storing address
         FAB_CW_ADDRESS = FAB_CW_ADDRESS + CW_size;                                 //prepare for next storing address
         
         fprintf(PC,"NO RESPONSE FROM FAB\r\n\r\n");  
         CHECK_50_and_CW_RESPOND();
         for(int num = 0; num < HK_size; num++)
         {
            fprintf(PC,"%x ",HKDATA[num]);
         }
      }
      CHECK_50_and_CW_RESPOND();
   
   }
   
   return;
}


//--------HIGH SAMPLING HK collection----------------------------------------//

void CHECK_HIGH_SAMP_FABDATA(int8 in)                                            //FAB sensor data collect
{
   fprintf(PC,"\r\nFAB DATA OBTAINED: ");
   Delete_HKDATA();
   for(int8 num_fab = 1; num_fab < 11 - in; num_fab++)                                             //Collect HK DATA
   {
      HKDATA[num_fab + 5+4] = in_HK[num_fab + 2 - in];
      fprintf(PC, "%x ", HKDATA[num_fab + 5 + 4]);
   }
   
   MEASURE_BC_TEMP();
   HKDATA[18] = BC_temp_data_h;                                                  //-X Panel Temp
   HKDATA[19] = BC_temp_data_l;                                                  //-X Panel Temp
   
   for(num_fab = 9; num_fab < FAB_SENSOR_size - 2; num_fab++)                                //[FAB] from CPLD temp to Kill status(array[20] to [49])
   {
      HKDATA[num_fab + 7+4] = in_HK[num_fab + 2 - in];
      fprintf(PC, "%x ", HKDATA[num_fab + 7+4]);
   }
   fprintf(PC,"\r\n");
   FAB_DATA = 0;
}


void VERIFY_HIGH_SAMP_FABDATA(int32 delaytime)
{
   for(int8 num = 0; num < 3; num++)
   {
      COMMAND_TO_FAB(delaytime);
      //FAB_DATA = 0;
      //waiting(1000);
      if(in_HK[0] == 0x33)                                                       //gather sensor data by interrupt
      {
         CHECK_HIGH_SAMP_FABDATA(2);
         CHECK_FAB_RESPONSE = 1;                                                 //1 is succeeded to get response from FAB
         break;
      }
      else if(in_HK[1] == 0x33)
      {
         //delay_ms(200);
         CHECK_HIGH_SAMP_FABDATA(1);
         CHECK_FAB_RESPONSE = 1;                                                 //1 is succeeded to get response from FAB
         break;
      }
      else if(in_HK[2] == 0x33)
      {
         //delay_ms(200);
         CHECK_HIGH_SAMP_FABDATA(0);
         CHECK_FAB_RESPONSE = 1;                                                 //1 is succeeded to get response from FAB
         break;
      }
      FAB_DATA = 0;
   }
   return;
}


void GET_HIGH_SAMP_RESET_DATA()
{
   RESET_DATA = 0;
   COLLECT_RESET_DATA();
   if(RESET_bffr[0] == 0x8e)
   {
      fprintf(PC,"\r\nGET RESET: ");
      for(int num = 0; num < 5; num++)                                           //timedata
      {
         HKDATA[num + 2] = reset_bffr[num + 1];
         //fputc(HKDATA[num + 2],PC);
         fprintf(PC,"%x ",HKDATA[num+2]);
      }
   
      for(num = 0; num < 5; num++)                                               //reset sensor data
      {
         HKDATA[num + 68] = reset_bffr[num + 6];                                //HKDATA[68] = reset[6]
         //fputc(HKDATA[num + 68],PC);
         fprintf(PC,"%x ",HKDATA[num + 68]);
      }
   fprintf(PC,"\r\n");
   }
   else
   {
      fprintf(PC,"\r\nNO RESET DATA\r\n");
   }
   //Delete_Reset();
}


void MAKE_HIGH_SAMP_ADCS_FORMAT()
{
   GET_ADCS_SENSOR_DATA();
   
   for(int num = 53; num < 65; num++)                                           //12byte(MAG6,GYRO6)
   {
      HKDATA[num] = ADCS_SENSOR_DATA[num - 52];                                  //ADCS[1] to ADCS[12]
   }
   
   Delete_ADCS_data();
   
   return;
}


void SAVE_HIGH_SAMP_DATA_TO_EACH_MEMORY()
{
   output_low(PIN_C4);
   CHECK_50_and_CW_RESPOND();
   for(int i = 0; i < HIGH_SAMP_HK_size; i++)
   {
      WRITE_DATA_BYTE_SCF(HIGH_SAMP_HK_ADDRESS + i,HKDATA[i]);
   }
   output_high(PIN_C4); 
   
   CHECK_50_and_CW_RESPOND();
   for(i = 0; i < HIGH_SAMP_HK_size; i++)
   {
      WRITE_DATA_BYTE_OF(HIGH_SAMP_HK_ADDRESS + i,HKDATA[i]);
   }
   
   CHECK_50_and_CW_RESPOND();
   output_low(PIN_A5);
   for(i = 0; i < HIGH_SAMP_HK_size; i++)
   {
      WRITE_DATA_BYTE_SMF(HIGH_SAMP_HK_ADDRESS + i,HKDATA[i]);
   }  
   
   CHECK_50_and_CW_RESPOND();
   HIGH_SAMP_HK_ADDRESS = HIGH_SAMP_HK_ADDRESS + HIGH_SAMP_HK_size;              //prepare for next storing address
   
   return;
}


void HIGH_SAMP_FAB_OPERATION()
{
   Turn_ON_ADCS();
   CHECK_50_and_CW_RESPOND();
   HIGH_SAMP_FAB_MEASURING_FLAG++;                                              //count until 90(it means 10 min)
   //disable_interrupts(INT_rda2);
   if(HIGH_SAMP_FAB_MEASURING_FLAG > 8)                                        //HIGH_SAMP_FAB_MEASUERING_FLAG=18 --> 18*5 = 90 (sec)
      {
         
         CHECK_50_and_CW_RESPOND();
         
         fprintf(PC,"\r\n\90sec\r\n");
         Delete_in_HK();                                                         //delete HK array
         VERIFY_FABDATA(10000,10);                                               //send command to the FAB and load the array in_HK [] with the data from the FAB
         GET_RESET_DATA();                                                       //funcion que carga el array HKDATA con los datos del Reset PIC
         MAKE_ADCS_HKDATA();                                                     //carga en el array HKDATA[] los datos del ADCS en las posiciones 53 al 106
         SET_IDENTIFIER();
         MAKE_CW_FORMAT();
         output_low(PIN_C4);
         output_low(PIN_A5);
         
         SEND_HKDATA_to_SCF(FAB_HK_ADDRESS);                                     //save HK in COM flash memory
         SEND_HKDATA_to_SMF(FAB_HK_ADDRESS);                                     //save HK in Mission flash memory
         SEND_HKDATA_to_OF(FAB_HK_ADDRESS);                                      //save HK in OBC flash memory
         SEND_CWFORMAT_TO_SCF(FAB_CW_ADDRESS);
         SEND_CWFORMAT_TO_SMF(FAB_CW_ADDRESS);
         SEND_CWFORMAT_TO_OF(FAB_CW_ADDRESS);
         
         CHECK_50_and_CW_RESPOND();
         FAB_HK_ADDRESS = FAB_HK_ADDRESS + HK_size;                              //prepare for next storing address
         FAB_CW_ADDRESS = FAB_CW_ADDRESS + CW_size;                              //prepare for next storing address
         
         SAVE_HIGH_SAMP_DATA_TO_EACH_MEMORY();
         
         CHECK_50_and_CW_RESPOND();
         
         fprintf(PC,"\r\nCOUNT:%d\r\n",HIGH_SAMP_FAB_MEASURING_FLAG);
         HIGH_SAMP_FAB_MEASURING_FLAG = 0;
         
      }else{
         Delete_in_HK();                                                         //delet HK array
         GET_RESET_DATA();                                                       //function that loads the HKDATA array with the Reset PIC data
         VERIFY_HIGH_SAMP_FABDATA(10000);                                        //get FAB data   
         GET_HIGH_SAMP_RESET_DATA();                                             //get reset data
         MAKE_HIGH_SAMP_ADCS_FORMAT();                                           //get ADCS data
         SET_IDENTIFIER();
         //MAKE_CW_FORMAT();
              
         fprintf(PC,"\r\n");
         for(int num = 0; num < 65; num++)                                       //array[0] to [64](until gyro data)
         {
            fprintf(PC,"%x ",HKDATA[num]);
         }
         CHECK_50_and_CW_RESPOND();
         for(num = 65; num < HK_Size; num++)
         {
          //fputc(HKDATA[num],PC);
            fprintf(PC,"%x ",HKDATA[num]);
         }
         CHECK_50_and_CW_RESPOND();
//!         for(num = 113; num < HK_Size; num++)
//!         {
//!            fprintf(PC," %x",HKDATA[num]);
//!         } 
//!         CHECK_50_and_CW_RESPOND();
         
         fprintf(PC,"\r\nCOUNT:%d\r\n",HIGH_SAMP_FAB_MEASURING_FLAG);
         CHECK_FAB_RESPONSE = 0;
         SAVE_HIGH_SAMP_DATA_TO_EACH_MEMORY();
      }    
      //Delete_in_HK();

   FAB_DATA = 0;
   output_high(PIN_C4);

   return;
}


void HIGHSAMP_SENSOR_COLLECTION(int16 times)
{
   HIGH_SAMP_FAB_MEASURING_FLAG = 0;
   Turn_On_MBP();
   Turn_On_ADCS();
   LOOP_HIGH_SAMP_HK_ADDRESS();                                                  //loop in memory to save data, keep first 3 sectors forever
   int32 num = 0;

   while(num < times)
   {
      CHECK_50_and_CW_RESPOND();                                                 //check cw cmd from COM PIC
      if(FAB_FLAG > 10)                                                           //HSSC repeats every 10secs
      {
         FAB_FLAG = 0;
         HIGH_SAMP_FAB_OPERATION();                                              //collect FAB, RESET and ADCS data
         
         num++;
         if((num % 20) == 0)                                                     //once in the 20 times(every 100sec save the address to the flash)
         {
            STORE_ADRESS_DATA_TO_FLASH();                                        //for store the flag
         } 
      }

      //BC_ON_30min();                                                             //check if first attempt and 30 min(1800sec) passed
      if((CMD_FROM_PC[0] == 0xAB)||(in_bffr_main[4] == 0xAB))                    //forced termination function
      {
         Delete_Buffer();
         break;
      }
      PC_DATA = 0;
      RESET_SATELLITE();                                                         //check reset command from RESET PIC
   }
   Turn_Off_ADCS();
   //Turn_Off_MBP();
   fprintf(PC,"HSSC DONE\r\n");
   return;
}


//--------Mission Boss PIC Data Collection-----------------------------------//


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
      
   return;
}

//--------Mission Execution Tests--------------------------------------------//

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
         
         for(a=0; a<12; a++)
         {
            GET_RESET_DATA_for_PINO();
            delay_ms(10000);
         }
         
         fprintf (PC, "Finish 0x9F\r\n") ;
         fprintf (PC, "Finish 0x9C\r\n") ;
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
      case 0x2E: //Turn on CAM1 RPi for MOSFET on MB1 to power RPI from 5V
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start - Turn ON MULTSPEC CAM1 0x2E\r\n") ;
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         
         CWtest[0] = MISSION_STATUS<<4;
         CWtest[0] = CWtest[0] + (MISSION_OPERATING & 0x01);
         fprintf(PC,"%x",CWtest[0]);
         fprintf (PC, "\r\n");
         missiontime = 0;
         output_high(pin_G3); //Turn on DIO for MULTSPEC CAM1
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x2E\r\n"); 
      
      break;
      
      
      case 0x20: //Turn off CAM1 RPi DIO for MOSFET on MB1 to power RPI from 5V      
         
         output_high (PIN_A5);                                                      //SFM2 mission side access
         fprintf (PC, "Start 0x20 - Turn OFF MULTSPEC CAM1 (MB1)\r\n") ;
         output_low(pin_G3);                                                        //Turn off DIO for MULTSPEC CAM1
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         
         CWtest[0] = MISSION_STATUS<<4;
         CWtest[0] = CWtest[0] + (MISSION_OPERATING & 0x01);
         fprintf(PC,"%x",CWtest[0]);
         fprintf (PC, "\r\n");
         missiontime = 0;
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x20\r\n");
         
      break;
      
      case 0x21: //Real time uplink command
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
         Forward_CMD_MBP();
         DELETE_CMD_ARRAY_DATA();
         MISSION_OPERATING = 0;
         fprintf (PC, "Finish 0x21\r\n");

      break;
      
      case 0x22: //Real time downlink command
      
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
         Forward_CMD_MBP();
         
         CWtest[0] = MISSION_STATUS<<4;
         CWtest[0] = CWtest[0] + (MISSION_OPERATING & 0x01);
         fprintf(PC,"%x",CWtest[0]);
         fprintf (PC, "\r\n");
         
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
         DELETE_CMD_ARRAY_DATA();
         
      break;
      
      case 0x23:
      
         //Capture in mode 1
         //(1 image, captured immediately, saved to specified address) 
         //eg. 23 is command, 00 02 06 08 is the address location, 01 is number of images to capture, 00 00 00 for remaining unused command bytes (command: 23 00 02 06 08 01 00 00 00)
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
         
         for (i = 1; i < 9; i++)
         {
            fputc(command[i], DC);
            delay_ms(20);
            fputc(command[i], PC);
         }
         MISSION_OPERATING = 0;
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x23\r\n");
      break;
      
      case 0x24: //Turn on CAM1 RPi trigger at specific time from Reservation time

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
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x24\r\n");    
      break;
      
      case 0x25: //Copy specific images based on selected thumbnails selected from GS. Forwward all mission command data (8 bytes) to MB, MB will send command data (specific image name) to MB1 RPI
      
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
         MISSION_OPERATING = 0;
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x25\r\n");
         
      break;
      
      case 0x26: //Transfer N packets of data from SFM2 to PC at the specified address locations
      
         fprintf (PC, "Start 0x12\r\n") ;
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
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
         MISSION_OPERATING = 0;
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x12\r\n") ;
         
      break;

 
      case 0x27:                                                                  //Uplink command to write the data on Flash Memory 2
      
         output_low (PIN_A5) ;//Main side
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
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
         MISSION_OPERATING = 0;
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x27\r\n");
      
      break;
      
      case 0x28://Erase the data on SFM2 at the given address. Specify how much data to erase eg. 16 00 01 02 03 FF 00 00 will erase the entire 64kB sector at address 00010203
      
         output_low (PIN_A5);
         fprintf(PC, "Start 0x28\r\n");
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
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
         MISSION_OPERATING = 0;
         DELETE_CMD_ARRAY_DATA();
         
      break;
      

      
////////////////////////////FOR CAM2 RPi on MB2////////////////////////////////
      case 0x3E: // Turn on MULTSPEC CAM2 (MB2)
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x3E - Turn ON MULTSPEC CAM2 (MB2)\r\n") ;
         output_high(pin_F6); //Turn on DIO for MULTSPEC CAM2
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         missiontime = 0;
         fprintf (PC, "Finish 0x3E\r\n");
         DELETE_CMD_ARRAY_DATA();
         
      break;
     
     
     case 0x30: //Turn off CAM2 RPi DIO for MOSFET on MB2 to power RPI from 5V
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x30 - Turn OFF MULTSPEC CAM2 (MB2)\r\n") ;
         output_low(pin_F6); //Turn off DIO for MULTSPEC CAM2
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         missiontime = 0;
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x30\r\n");        
         
      break;
      
      case 0x31: //Turn off CAM1 RPi for MOSFET on MB1 to power RPI from 5V
      

         
      break;
      
      case 0x32: //Turn on CAM1 RPi trigger

         fprintf (PC, "Start 0x32 - Request MULT-SPEC MB2 Downlink Data\r\n") ;
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
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
         MISSION_OPERATING = 0;
         DELETE_CMD_ARRAY_DATA();
         fprintf (PC, "Finish 0x32\r\n");

      break;
      
      case 0x33: 
         //Capture in mode 1
         //(1 image, captured immediately, saved to specified address) 
         //eg. 23 is command, 00 02 06 08 is the address location, 01 is number of images to capture, 00 00 00 for remaining unused command bytes (command: 23 00 02 06 08 01 00 00 00)
         
         fprintf (PC, "Start 0x33\r\n") ;
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
         output_high (PIN_A5); //SFM2 mission side access
         
         for (i = 1; i < 9; i++)
         {
            fputc(command[i], DC);
            delay_ms(20);
            fputc(command[i], PC);
         }
         MISSION_OPERATING = 0;
         fprintf (PC, "Finish 0x33\r\n");
         DELETE_CMD_ARRAY_DATA();
         
      break;
      
      case 0x34: //Turn on CAM1 RPi trigger at specific time from RESET_PIC time information

         fprintf (PC, "Start 0x34\r\n") ;
         output_low (PIN_A5);
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
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
         MISSION_OPERATING = 0;
         fprintf (PC, "Finish 0x34\r\n");
         DELETE_CMD_ARRAY_DATA();
      break;
      
      case 0x35: //Turn on CAM1 and CAM2 RPi trigger
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x35\r\n") ;
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
         output_high(pin_F7); //Turn on DIO for trigger MB2
         output_high(pin_G2); //Turn on DIO for trigger MB1
         delay_ms(10000);
         output_low(pin_F7); //Turn off DIO for trigger
         output_low(pin_G2); //Turn off DIO for trigger
         MISSION_OPERATING = 0;
         fprintf (PC, "Finish 0x35\r\n");   
         DELETE_CMD_ARRAY_DATA();
      break;
      
      case 0x36: //Turn on CAM1 and CAM2 RPi
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x36\r\n") ;
         output_high(pin_G3); //Turn on DIO for trigger MB2
         delay_ms(5000);
         output_high(pin_F6); //Turn on DIO for trigger MB1
         fprintf (PC, "Finish 0x36\r\n");
         DELETE_CMD_ARRAY_DATA();
      break;
      

      
      default:
      
         fprintf (PC, "Command:%x", command[0]);
         fprintf (PC, " does not exist\r\n");
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         DELETE_CMD_ARRAY_DATA();
      break;
   }
   
}


void OITA_Test()
{
   Turn_On_MBP();
   MAKE_ADCS_HKDATA();
   
//!   command[0] = 0x21;
//!   command[1] = ADCS_SENSOR_DATA[1];
//!   command[2] = ADCS_SENSOR_DATA[2];
//!   command[3] = ADCS_SENSOR_DATA[3];
//!   command[4] = ADCS_SENSOR_DATA[4];
//!   command[5] = ADCS_SENSOR_DATA[5];
//!   command[6] = ADCS_SENSOR_DATA[6];
//!   command[7] = ADCS_SENSOR_DATA[7];
//!   command[8] = 0x05;
//!   
//!   for(int8 ll = 0; ll < 9; ll++)
//!      {
//!         
//!         fprintf(PC,"%x ",command[ll]);
//!      }
   
   //Forward_CMD_MBP();
   
   //Delete_ADCS_data();
   
   //DELETE_CMD_ARRAY_DATA();
   
   
}


void DLP_test()
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
      case 0x7E:
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x7E - Turn ON DLP\r\n") ;
         Forward_CMD_MBP();
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         fprintf (PC, "Finish 0x7E\r\n");
         
      break;
      
      case 0x70: //Turn off DLP RPi DIO for MOSFET on RAB to power RPI from 5V line
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x70 - Turn off IMGCLS\r\n") ;
         Forward_CMD_MBP();
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         fprintf (PC, "Finish 0x70\r\n"); 
         
      break;
      
      case 0x71: //Real time uplink command
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x71 - Real time uplink DLP\r\n") ;
         MISSION_STATUS = 1;
         MISSION_OPERATING = 1;
         Forward_CMD_MBP();
         MISSION_OPERATING = 0;
         fprintf (PC, "Finish 0x71\r\n"); 
      break;
      
      case 0x72:
         output_high (PIN_A5);
         fprintf (PC, "Start 0x82 - Real time downlink DLP\r\n") ;
         MISSION_STATUS = 1;
         MISSION_OPERATING = 1;
         Forward_CMD_MBP();
         int8 counter_dlp = 0;
         for(int32 num_dlp = 0; num_dlp < 1500000; num_dlp++)
         {
            if(kbhit(DC))
            {
               DLP_DATA[counter_dlp] = fgetc(DC);
               counter_dlp++;
               if(counter_dlp == 81)
               {
                  break;
               }
            }
         }
         
         fprintf(PC,"Data Recieved: ");
         for(int8 c = 0; c < 81; c++)
         {
            fprintf(PC,"%x, ",DLP_DATA[c]);
         }
         fprintf(PC,"\r\n");
         fprintf (PC, "Finish 0x72\r\n");
         
         for(c = 0; c < 81; c++)
         {
            DLP_DATA[c] = 0;
         }
         
         MISSION_OPERATING = 0;
      break;
      
      
      case 0x73://Erase the data on SFM2 at the given address. Specify how much data to erase eg. 16 00 01 02 03 FF will erase the entire 64kB sector at address 00010203
      
         output_high (PIN_A5);
         fprintf (PC, "Start 0x73\r\n") ;
         Forward_CMD_MBP();
         fprintf (PC, "Finish 0x73\r\n"); 
         
      break;
      
      case 0x74:
         
         fprintf (PC, "Start 0x74 - DLP Deployment\r\n") ;     
         output_high (PIN_A5);
         MISSION_STATUS = 1;
         Forward_CMD_MBP(); 
         MISSION_OPERATING = 0;
         fprintf (PC, "Finish 0x74\r\n");
         
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
   
      case 0x8E:
         
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x8E - Turn ON IMGCLS\r\n") ;
         
         MISSION_STATUS = 1;
         MISSION_OPERATING = 0;
         Forward_CMD_MBP();
         CWtest[0] = MISSION_STATUS<<4;
         CWtest[0] = CWtest[0] + (MISSION_OPERATING & 0x01);
         fprintf(PC,"%x",CWtest[0]);
         fprintf (PC, "\r\n");
         missiontime = 0;
         fprintf (PC, "Finish 0x8E\r\n");
         

      break;
      
      case 0x80: //Turn off IMGCLS RPi DIO for MOSFET on RAB to power RPI from 5V line
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x80 - Turn off IMGCLS\r\n");
         Forward_CMD_MBP();
         MISSION_STATUS = 0;
         MISSION_OPERATING = 0;
         
         CWtest[0] = MISSION_STATUS<<4;
         CWtest[0] = CWtest[0] + (MISSION_OPERATING & 0x01);
         fprintf(PC,"%x",CWtest[0]);
         fprintf (PC, "\r\n");
         missiontime = 0;
         fprintf (PC, "Finish 0x80\r\n"); 
         
      break;
      
      case 0x81: //Real time uplink command
      
         output_high (PIN_A5); //SFM2 mission side access
         fprintf (PC, "Start 0x81 - Real time uplink IMGCLS\r\n") ;
         Forward_CMD_MBP();
         
         CWtest[0] = MISSION_STATUS<<4;
         CWtest[0] = CWtest[0] + (MISSION_OPERATING & 0x01);
         fprintf(PC,"%x",CWtest[0]);
         fprintf (PC, "\r\n");
         
         
         DELETE_CMD_ARRAY_DATA();
         
         fprintf (PC, "Finish 0x81\r\n"); 
      break;
      
      case 0x82: //Real time downlink IMGCLS
         output_high (PIN_A5);
         fprintf (PC, "Start 0x82 - Real time downlink IMGCLS\r\n") ;
         if (MISSION_STATUS == 1)                                                 //Mission operating flag will only go high if MISSION STATUS is high. Mission STATUS is high when the mission turns on
         {
            MISSION_OPERATING = 1;
         }
         else
         {
            MISSION_OPERATING = 0;
         }
         
         CWtest[0] = MISSION_STATUS<<4;
         CWtest[0] = CWtest[0] + (MISSION_OPERATING & 0x01);
         fprintf(PC,"%x",CWtest[0]);
         fprintf (PC, "\r\n");
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
         
         MISSION_OPERATING = 0;
         
         fprintf (PC, "Finish 0x82\r\n"); 
         for(c = 0; c < 81; c++)
         {
            IMGCLS_DATA[c] = 0;
         }
         
         DELETE_CMD_ARRAY_DATA();
      break;
      
      
      case 0x83:
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
         
         CWtest[0] = MISSION_STATUS<<4;
         CWtest[0] = CWtest[0] + (MISSION_OPERATING & 0x01);
         fprintf(PC,"%x",CWtest[0]);
         fprintf (PC, "\r\n");
         Forward_CMD_MBP();

         DELETE_CMD_ARRAY_DATA();
         MISSION_OPERATING = 0;
         
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
         

     
   }
   for(int m = 0; m < 9; m++)
   {
      command[m] = 0;
   }
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
            //GET_RESET_DATA_for_PINO();
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


void GET_RESET_DATA_for_PINO()
{

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

  }
      else
      {
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



