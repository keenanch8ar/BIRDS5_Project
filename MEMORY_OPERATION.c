//-------------------------ADDRESS (PREVIOUS)----------------------------------
//!/////////////////previous//////////////////////////////
//!Static int32 ADCS_SENSOR_ADDRESS = 65536*500;
//!Static int32 FAB_HK_ADDRESS = 65536 * 200;
//!Static int32 FAB_CW_ADDRESS = 65536 * 50;
//!Static int32 FAB_CW_ADDRESS_FOR_MF = 65536; //sector1
//!Static int32 HIGH_SAMP_HK_ADDRESS = 65536 * 1800;
//!Static int32 CAM_ADDRESS = 65536;
//!Static int32 DC_STATUS_ADDRESS = 65536*300;
//!///////////////////////////////////////////////////////
#define SECT 65536                                                               //1 Sector = 65536 Bytes

Static int32 ADD_INFO_ADDRESS = 4096;                                            //for keeping the address data to flash memory
Static int32 ADDRESS_WRITING_COUNTER = 0;                                        //if address writing more than 30000 times, change the store address

Static int32 FLAG_DATA_ADDRESS = SECT*4;                                         //1sector
Static int32 RSV_DATA_ADDRESS = SECT*5;                                          //1sector (65536 Bytes)
Static int32 SAT_LOG = SECT*6;                                                   //2sectors
Static int32 IMGCLS_ADDRESS = SECT*8;                                            //100sectors
Static int32 MCAM1_THMB_ADDRESS = SECT*108;                                      //50sectors
Static int32 MCAM1_IMG_ADDRESS = SECT*158;                                       //50sectors
Static int32 MCAM2_THMB_ADDRESS = SECT*208;                                      //50sectors
Static int32 MCAM2_IMG_ADDRESS = SECT*258;                                       //50sectors
Static int32 FAB_HK_ADDRESS = SECT*308;                                          //300sectors
Static int32 FAB_CW_ADDRESS = SECT*608;                                          //50sectors
Static int32 HIGH_SAMP_HK_ADDRESS = SECT*658;                                    //253sectors
Static int32 DC_STATUS_ADDRESS = SECT*912;                                       //1sector
Static int32 STORE_FORWARD_ADDRESS = SECT*913;                                   //9sectors
Static int32 ADCS_SENSOR_ADDRESS = SECT*922;                                     //100sectors
Static int32 PINO_ADDRESS = SECT*1023;                                           //1025sectors


Static int32 FLAG_ADDRESS_EEPROM = 0x18000;                                      //from 75 percent of the programming memory
Static int32 FLASH_AD_ADDRESS_EEPROM = 0x18000 + 28;                             //after flag info
                                                                                 //Defining memory space addresses (end)
#define FLAG_DATA_ADDRESS_END  SECT*5-26                                         //consider the length of each piece of data to determine the direction of completion
#define RSV_DATA_ADDRESS_END  SECT*6-160
#define SAT_LOG_END  SECT*8-22
#define IMGCLS_ADDRESS_END  SECT*108-SECT*2
#define FAB_HK_ADDRESS_END  SECT*608-SECT-248
#define FAB_CW_ADDRESS_END  SECT*658-10
#define FAB_CW_ADDRESS_FOR_MF_END FAB_CW_ADDRESS_END
#define DC_STATUS_ADDRESS_END SECT*913-415
#define HIGH_SAMP_HK_ADDRESS_END  SECT*912-178560                               //178560 is for 2 hour data(124*3600*2/5)
#define ADCS_SENSOR_ADDRESS_END  SECT*1023-14                                   //14 bytes per ADCS data collection
                                                                                //Flags definition
int8 BC_ATTEMPT_FLAG = 0;
int16 PASSED_DAYS = 0;
int8 RESERVE_CHECK = 0;
int8 RESERVE_MIN_FLAG = 0;
int8 RESERVE_TARGET_FLAG = 0;
int8 MISSION_CONTENTS = 0;
int8 RESERVE_DATA_0 = 0;
int8 RESERVE_DATA_1 = 0;
int8 RESERVE_DATA_2 = 0;
int8 RESERVE_DATA_3 = 0;
int8 RESERVE_DATA_4 = 0;
int8 RESERVE_DATA_5 = 0;
int8 RESERVE_DATA_6 = 0;
int8 Kill_FLAG_MAIN = 0;
int8 Kill_FLAG_FAB = 0;
int8 FIRST_HSSC_DONE = 0;
int8 ANT_DEP_STATUS = 0;
int8 UPLINK_SUCCESS = 0;
int8 MISSION_STATUS = 0;                                                         //MISSION STATUS FLAG
int8 MISSION_OPERATING = 0;                                                      //MISSION OPERATING FLAG
int8 COM_TO_MAIN_FLAG = 0;
int8 RESET_TO_MAIN_FLAG = 0;
int8 CHECK_FAB_RESPONSE = 0;

int8 RESERVE_SEC_FLAG = 0;

//--------MAIN PIC Buffer------------------------------------------------------
int8 CMD_FROM_PC[9] = {};
unsigned int8 CMD_FROM_COMM[16] = {};
unsigned int8 in_bffr_main[16] = {};
int8 COM_DATA= 0;
static int8 CW_IDENTIFIER = 1;
int8 OPERATION_MODE = 0x00;
int8 PC_DATA = 0;                                  


#define buffer_from_com  (in_bffr_main[0]==0xAA) && (in_bffr_main[15]==0xBB)
#define buffer_flash  (in_bffr_main[7]==0x73)
#define START_ADCS_MISSION  (in_bffr_main[1]==0x73)
#define ADCS_SENSOR_SIZE 14                                                      //for testing additional 6byte for checking duty


int8 ADCS_SENSOR_DATA[ADCS_SENSOR_SIZE] = {};
int8 ADCS_ACK = 0;
#define ATTEMPT_TIME 8


#define EX_PANEL_THRESHHOLD 0x0019
#define HIGH_SAMP_TIMES 12                                                       //for 2 hours

//----------RESET--------------------------------------------------------------
#define Reset_size 11                                                            //1byte ack, 4byte time, 10byte sensor
int8 RESET_DATA = 0;
int8 reset_bffr[Reset_size] = {};
int8 reset_flag = 0;

//---------OTHER FUNCTION------------------------------------------------------
#define FLASH_ADD_SIZE 37                                                        //8 kinds of address(32byte), 1 flag(1byte), 1 WRITING COUNTER(2byte)
#define FLAG_INFO_SIZE 15
unsigned int8 sec_add_bfr[FLASH_ADD_SIZE] = {};                                  //vector used to store addresses read from flash
unsigned int8 flag_info_bffr[FLAG_INFO_SIZE] = {};


void CURRENT_FLAG_STATUS()                                                      //Print on PC port current flag status
{
   fprintf(PC,"\r\nPASSED DAYS:%x\r\n",PASSED_DAYS);
   fprintf(PC,"RESERVE CHECK:%x\r\n",RESERVE_CHECK);
   fprintf(PC,"Kill FLAG MAIN:%x\r\n",Kill_FLAG_MAIN);
   fprintf(PC,"Kill FLAG FAB:%x\r\n",Kill_FLAG_FAB);
   fprintf(PC,"AUTO HIGH SAMPLING:%x\r\n",FIRST_HSSC_DONE);
   fprintf(PC,"ANTENNA DEPLOY:%x\r\n",ANT_DEP_STATUS);
   fprintf(PC,"ANTENNA DEPLOY ATTEMPT:%x\r\n",BC_ATTEMPT_FLAG);
   fprintf(PC,"UPLINK SUCCESS:%x\r\n",UPLINK_SUCCESS);
   fprintf(PC,"MISSION STATUS:%x\r\n",MISSION_STATUS);
   fprintf(PC,"MISSION OPERATING:%x\r\n\r\n",MISSION_OPERATING);
   return;
}


void CURRENT_ADDRESS_OF_FLASH()                                                  //print the Flash memory address assignment
{  
   fprintf(PC,"\r\nADD INFO ADDRESS:%lx\r\n\r\n",ADD_INFO_ADDRESS);
   fprintf(PC,"FLAG DATA ADDRESS:%x%x%x%x\r\n",FLAG_DATA_ADDRESS>>24,FLAG_DATA_ADDRESS>>16,FLAG_DATA_ADDRESS>>8,FLAG_DATA_ADDRESS);
   fprintf(PC,"RESERVATION TABLE ADDRESS:%x%x%x%x\r\n",RSV_DATA_ADDRESS>>24,RSV_DATA_ADDRESS>>16,RSV_DATA_ADDRESS>>8,RSV_DATA_ADDRESS);
   fprintf(PC,"SATELLITE LOG ADDRESS:%x%x%x%x\r\n",SAT_LOG>>24,SAT_LOG>>16,SAT_LOG>>8,SAT_LOG);
   fprintf(PC,"FAB HK ADDRESS:%x%x%x%x\r\n",FAB_HK_ADDRESS>>24,FAB_HK_ADDRESS>>16,FAB_HK_ADDRESS>>8,FAB_HK_ADDRESS);
   fprintf(PC,"FAB CW ADDRESS:%x%x%x%x\r\n",FAB_CW_ADDRESS>>24,FAB_CW_ADDRESS>>16,FAB_CW_ADDRESS>>8,FAB_CW_ADDRESS);
   fprintf(PC,"ADCS SENSOR ADDRESS:%x%x%x%x\r\n",ADCS_SENSOR_ADDRESS>>24,ADCS_SENSOR_ADDRESS>>16,ADCS_SENSOR_ADDRESS>>8,ADCS_SENSOR_ADDRESS);
   fprintf(PC,"MISSION BOSS ADDRESS:%x%x%x%x\r\n",DC_STATUS_ADDRESS>>24,DC_STATUS_ADDRESS>>16,DC_STATUS_ADDRESS>>8,DC_STATUS_ADDRESS);
   fprintf(PC,"HIGH SAMP HK ADDRESS:%x%x%x%x\r\n",HIGH_SAMP_HK_ADDRESS>>24,HIGH_SAMP_HK_ADDRESS>>16,HIGH_SAMP_HK_ADDRESS>>8,HIGH_SAMP_HK_ADDRESS);
   fprintf(PC,"WRITE %lx TIMES\r\n",ADDRESS_WRITING_COUNTER);
   return;
}


void ERASE_EEPROM_INFO(int8 CMD1, int8 CMD2, int8 CMD3)
{
   if (CMD1 == 0xAA && CMD2 == 0xBB && CMD3 == 0xCC)
   {
      erase_program_eeprom(FLAG_ADDRESS_EEPROM);//erase 512byte(from 0x18000 to 0x181ff)
   }
   return;
   
}


void WRITE_FLAG_to_EEPROM()     //saves the flags in the EEPROM from the address 0x18000 (75%)
{
//total programming memory size is 128KB
//start writing the important flag from 75 percent of the programming memory(from the address of 96KB --> 0x18000)
   int16 DATA;
   DATA = (int16)BC_ATTEMPT_FLAG;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM, DATA);
   DATA = (int16)PASSED_DAYS;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+2, DATA);
   DATA = (int16)RESERVE_CHECK;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+4, DATA);
   DATA = (int16)RESERVE_MIN_FLAG;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+6, DATA);
   DATA = (int16)RESERVE_TARGET_FLAG;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+8, DATA);
   DATA = (int16)MISSION_CONTENTS;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+10, DATA);
   //DATA = (int16)MISSION_DETAIL;
   //WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+12, DATA);
   DATA = (int16)Kill_FLAG_MAIN;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+14, DATA);
   DATA = (int16)Kill_FLAG_FAB;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+16, DATA);
   DATA = (int16)FIRST_HSSC_DONE;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+18, DATA);
   DATA = (int16)ANT_DEP_STATUS;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+20, DATA);
   DATA = (int16)UPLINK_SUCCESS;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+22, DATA);
   DATA = (int16)MISSION_STATUS;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+24, DATA);
   DATA = (int16)MISSION_OPERATING;
   WRITE_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+26, DATA);
   return;
}


/*
void WRITE_AD_INFO_to_EEPROM()
{
   int16 DATA;
   DATA = BC_ATTEMPT_FLAG >> 16;
   WRITE_PROGRAM_EEPROM(FLASH_AD_ADDRESS_EEPROM, DATA);
   DATA = BC_ATTEMPT_FLAG;
   WRITE_PROGRAM_EEPROM(FLASH_AD_ADDRESS_EEPROM+2, DATA);
   return;
}
*/


void MAKE_FLAG_from_EEPROM()                                                     //takes the info of the flags from the eeprom and loads them in their respective variables
{
   if((READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM)!=0xffff)&&(READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+26)!=0xffff))
   {
      BC_ATTEMPT_FLAG = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM);
      PASSED_DAYS = (int16)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+2);
      RESERVE_CHECK = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+4);
      RESERVE_MIN_FLAG = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+6);
      RESERVE_TARGET_FLAG = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+8);
      MISSION_CONTENTS = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+10);
      //MISSION_DETAIL = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+12);
      Kill_FLAG_MAIN = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+14);
      Kill_FLAG_FAB = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+16);
      FIRST_HSSC_DONE = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+18);
      ANT_DEP_STATUS = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+20);
      UPLINK_SUCCESS = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+22);
      MISSION_STATUS = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+24);
      MISSION_OPERATING = (int8)READ_PROGRAM_EEPROM(FLAG_ADDRESS_EEPROM+26);
   }
   
   fprintf(PC,"\r\nPASSED DAYS:%lx\r\n",PASSED_DAYS);
   fprintf(PC,"RESERVE CHECK:%x\r\n",RESERVE_CHECK);
   fprintf(PC,"Kill FLAG MAIN:%x\r\n",Kill_FLAG_MAIN);
   fprintf(PC,"Kill FLAG FAB:%x\r\n",Kill_FLAG_FAB);
   fprintf(PC,"AUTO HIGH SAMPLING:%x\r\n",FIRST_HSSC_DONE);
   fprintf(PC,"ANTENNA DEPLOY:%x\r\n",ANT_DEP_STATUS);
   fprintf(PC,"UPLINK SUCCESS:%x\r\n",UPLINK_SUCCESS);
   fprintf(PC,"MISSION STATUS:%x\r\n",MISSION_STATUS);
   fprintf(PC,"MISSION OPERATING:%x\r\n\r\n",MISSION_OPERATING);
   
   return;
}


void TAKE_AD_INFO_from_EEPROM()                                                  //Reads address data from de program memory
{
   int16 DATA_high = READ_PROGRAM_EEPROM(FLASH_AD_ADDRESS_EEPROM);               
   int16 DATA_low = READ_PROGRAM_EEPROM(FLASH_AD_ADDRESS_EEPROM+2);
   int32 DATA = (((int32)DATA_high<<16) | (int32)DATA_low);
   if(DATA != 0xffffffff)
   {
      ADD_INFO_ADDRESS = DATA;                                                   
   }
   return;
}


void STORE_FLAG_INFO()                                                           //save flag data to flash memory
{
   flag_info_bffr[0] = BC_ATTEMPT_FLAG;
   flag_info_bffr[1] = PASSED_DAYS >> 8;
   flag_info_bffr[2] = PASSED_DAYS;
   flag_info_bffr[3] = RESERVE_CHECK;
   flag_info_bffr[4] = RESERVE_TARGET_FLAG;
   flag_info_bffr[5] = RESERVE_MIN_FLAG;
   flag_info_bffr[6] = MISSION_CONTENTS;
   //flag_info_bffr[7] = MISSION_DETAIL;
   flag_info_bffr[8] = Kill_FLAG_MAIN;
   flag_info_bffr[9] = Kill_FLAG_FAB;
   flag_info_bffr[10] = FIRST_HSSC_DONE;
   flag_info_bffr[11] = ANT_DEP_STATUS;
   flag_info_bffr[12] = UPLINK_SUCCESS;
   flag_info_bffr[13] = MISSION_STATUS;
   flag_info_bffr[14] = MISSION_OPERATING;
   
   if(MISSION_STATUS == 0)
   {
      output_low(PIN_A5);                                                           //MISSION_MUX MAINSIDE
   }
   output_low(PIN_C4);                                                           //COM_MUX MAINSIDE
   delay_ms(2000);
   
   SUBSECTOR_4KB_ERASE_OF(FLAG_DATA_ADDRESS);
   delay_ms(200);
   for(int8 num = 0; num < FLAG_INFO_SIZE; num++)
   {
      WRITE_DATA_BYTE_OF(FLAG_DATA_ADDRESS + num,flag_info_bffr[num]);
      delay_us(10);
   }
   
   delay_ms(10);
   SUBSECTOR_4KB_ERASE_SCF(FLAG_DATA_ADDRESS);
   delay_ms(200);
   for(num = 0; num < FLAG_INFO_SIZE; num++)
   {
      WRITE_DATA_BYTE_SCF(FLAG_DATA_ADDRESS + num,flag_info_bffr[num]);
      delay_us(10);
   }
   
   delay_ms(10);
   if(MISSION_STATUS == 0)
   {
      SUBSECTOR_4KB_ERASE_SMF(FLAG_DATA_ADDRESS);
      delay_ms(200);
      for(num = 0; num < FLAG_INFO_SIZE; num++)
      {
         WRITE_DATA_BYTE_SMF(FLAG_DATA_ADDRESS + num,flag_info_bffr[num]);
         delay_us(10);
      }
   }
   
   output_high(PIN_C4);                                                          //COM_MUX COMSIDE
   output_high(PIN_A5);                                                           //MISSION_MUX MAINSIDE
   fprintf(PC,"FLAG INFO STORED IN FLASH\r\n");
   return;
   
}


void RESET_FLAG_DATA()
{
   BC_ATTEMPT_FLAG = 0;
   PASSED_DAYS = 0;
   //RESERVE_CHECK = 0;
   //RESERVE_MIN_FLAG = 0;
   //RESERVE_TARGET_FLAG = 0;
   //MISSION_CONTENTS = 0;
   //MISSION_DETAIL = 0;
   Kill_FLAG_MAIN = 0;
   Kill_FLAG_FAB = 0;
   FIRST_HSSC_DONE = 0;
   ANT_DEP_STATUS = 0;
   UPLINK_SUCCESS = 0;
   MISSION_STATUS = 0;
   MISSION_OPERATING = 0;
   STORE_FLAG_INFO();
   WRITE_FLAG_to_EEPROM();
   return;
}


void READ_WRITING_ADDRESS_LOCATION()                                            //load the variable ADD_INFO_ADDRESS, look for the data in OF, SCF, SMF and EEPROM (carga la variable ADD_INFO_ADDRESS, busca el dato en OF, SCF, SMF y EEPROM)
{                                                                                
   int i;                                                                        //if you can't find it use the initial value (si no lo encuentra usa el valor inicial)
   int8 ad_location_bfr[4];
   output_low(PIN_C4);                                                           //Main side
   output_low(PIN_A5);                                                           //Main side
   if(READ_DATA_BYTE_OF(0) == 0xff)                                              //this means there is no address data in OBC Flash
   {
      if(READ_DATA_BYTE_SCF(0) == 0xff)                                          //this means there is no address data in Shared COM Flash
      {
         if(READ_DATA_BYTE_SMF(0) == 0xff)                                       //if all memory was empty, location should be initial value
         {
            TAKE_AD_INFO_from_EEPROM();                                          //if inside of eeprom is also empty, use the initial value
            ad_location_bfr[0] = ADD_INFO_ADDRESS >> 24;
            ad_location_bfr[1] = ADD_INFO_ADDRESS >> 16;
            ad_location_bfr[2] = ADD_INFO_ADDRESS >> 8;
            ad_location_bfr[3] = ADD_INFO_ADDRESS;
         }
         else
         {
            ad_location_bfr[0] = READ_DATA_BYTE_SMF(0);
            ad_location_bfr[1] = READ_DATA_BYTE_SMF(1);
            ad_location_bfr[2] = READ_DATA_BYTE_SMF(2);
            ad_location_bfr[3] = READ_DATA_BYTE_SMF(3);
            ADD_INFO_ADDRESS = make32(ad_location_bfr[0],ad_location_bfr[1],ad_location_bfr[2],ad_location_bfr[3]);
         }
      }
      else
      {
         ad_location_bfr[0] = READ_DATA_BYTE_SCF(0);
         ad_location_bfr[1] = READ_DATA_BYTE_SCF(1);
         ad_location_bfr[2] = READ_DATA_BYTE_SCF(2);
         ad_location_bfr[3] = READ_DATA_BYTE_SCF(3);
         ADD_INFO_ADDRESS = make32(ad_location_bfr[0],ad_location_bfr[1],ad_location_bfr[2],ad_location_bfr[3]);
      }
   }
   else
   {
      ad_location_bfr[0] = READ_DATA_BYTE_OF(0);
      ad_location_bfr[1] = READ_DATA_BYTE_OF(1);
      ad_location_bfr[2] = READ_DATA_BYTE_OF(2);
      ad_location_bfr[3] = READ_DATA_BYTE_OF(3);
      ADD_INFO_ADDRESS = make32(ad_location_bfr[0],ad_location_bfr[1],ad_location_bfr[2],ad_location_bfr[3]);
   }
   
   for(i = 0; i < 4; i++)                                                        //store the subsector(location) information
   {
      WRITE_DATA_BYTE_OF(i,ad_location_bfr[i]);
      WRITE_DATA_BYTE_SCF(i,ad_location_bfr[i]);
      WRITE_DATA_BYTE_SMF(i,ad_location_bfr[i]);
   }
   output_high(PIN_C4);
   fprintf(PC,"\r\naddress info location:%lx\r\n",ADD_INFO_ADDRESS);
   return;
}


void CHANGE_ADDRESS_WRITING_LOCATION()                                           //change the writing address of address locations when 95,000 cycles are exceeded
{
   int32 AD_COUNTER = make32(READ_DATA_BYTE_OF(ADD_INFO_ADDRESS+33),READ_DATA_BYTE_OF(ADD_INFO_ADDRESS+34),READ_DATA_BYTE_OF(ADD_INFO_ADDRESS+35),READ_DATA_BYTE_OF(ADD_INFO_ADDRESS+36));//check counter value
   fprintf(PC,"AD COUNTER:%lx\r\n",AD_COUNTER);
   if((AD_COUNTER > 95000) && (AD_COUNTER != 0xffffffff))
   {
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         SUBSECTOR_4KB_ERASE_SMF(0);
      }

      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      SUBSECTOR_4KB_ERASE_OF(0);
      SUBSECTOR_4KB_ERASE_SCF(0);

      ADDRESS_WRITING_COUNTER = 0;                                               //reset counter
      ADD_INFO_ADDRESS = ADD_INFO_ADDRESS + 0x00001000;                          //use next subsector
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      
      unsigned int8 address_place[4] = {};
      address_place[0] = ADD_INFO_ADDRESS >> 24;
      address_place[1] = ADD_INFO_ADDRESS >> 16;
      address_place[2] = ADD_INFO_ADDRESS >> 8;
      address_place[3] = ADD_INFO_ADDRESS;
      
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         for(int i = 0; i < 4; i++)                                                 //store the new subsector(location) information
         {
            WRITE_DATA_BYTE_SMF(i,address_place[i]);
            delay_us(10);
         }
      }
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      for(int i = 0; i < 4; i++)                                                 //store the new subsector(location) information
      {
         WRITE_DATA_BYTE_OF(i,address_place[i]);                                 //address and data is passed to OF
         delay_us(10);
         WRITE_DATA_BYTE_SCF(i,address_place[i]);
         delay_us(10);
      }
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      output_high(PIN_A5);
      fprintf(PC,"CHANGED MEMORY ADDRESS:");
      for(i = 0; i < 4; i++)                                                     //store the subsector(location) information
      {
         fprintf(PC,"%x",READ_DATA_BYTE_OF(i));
      }
      fprintf(PC,"\r\n");
   }
   
   return;
}


void TAKE_FLAG_INFO_FROM_OF()                                                    //Function that reads data from FLAGS from memory
{
   for(int8 num = 0; num < FLAG_INFO_SIZE; num++)
   {
      flag_info_bffr[num] = READ_DATA_BYTE_OF(FLAG_DATA_ADDRESS + num);          //save the data in the vector flag_info_bffr []
      //delay_ms(1);
   }
   return;
}


void STORE_ADRESS_DATA_TO_FLASH()                                                //save the address data in a new sector if the R / W cycle is reached
{
   fprintf(PC,"\r\nStoring Address Data in Flash\r\n");
   ADDRESS_WRITING_COUNTER++;                                                    //write counter in one address
   CHANGE_ADDRESS_WRITING_LOCATION();                                            //changes the address of the writing location if 95000 cycles are exceeded
   sec_add_bfr[0] = FLAG_DATA_ADDRESS >> 24;                                     //the sec_add_bfr [] array is loaded with the address data
   sec_add_bfr[1] = FLAG_DATA_ADDRESS >> 16;
   sec_add_bfr[2] = FLAG_DATA_ADDRESS >> 8;
   sec_add_bfr[3] = FLAG_DATA_ADDRESS;
      
   sec_add_bfr[4] = RSV_DATA_ADDRESS >> 24;
   sec_add_bfr[5] = RSV_DATA_ADDRESS >> 16;
   sec_add_bfr[6] = RSV_DATA_ADDRESS >> 8;
   sec_add_bfr[7] = RSV_DATA_ADDRESS;
   
   sec_add_bfr[8] = SAT_LOG >> 24;
   sec_add_bfr[9] = SAT_LOG >> 16;
   sec_add_bfr[10] = SAT_LOG >> 8;
   sec_add_bfr[11] = SAT_LOG;
   
   sec_add_bfr[12] = FAB_HK_ADDRESS >> 24;
   sec_add_bfr[13] = FAB_HK_ADDRESS >> 16;
   sec_add_bfr[14] = FAB_HK_ADDRESS >> 8;
   sec_add_bfr[15] = FAB_HK_ADDRESS;
   
   sec_add_bfr[16] = FAB_CW_ADDRESS >> 24;
   sec_add_bfr[17] = FAB_CW_ADDRESS >> 16;
   sec_add_bfr[18] = FAB_CW_ADDRESS >> 8;
   sec_add_bfr[19] = FAB_CW_ADDRESS;
   
   sec_add_bfr[20] = ADCS_SENSOR_ADDRESS >> 24;
   sec_add_bfr[21] = ADCS_SENSOR_ADDRESS >> 16;
   sec_add_bfr[22] = ADCS_SENSOR_ADDRESS >> 8;
   sec_add_bfr[23] = ADCS_SENSOR_ADDRESS;
   
   sec_add_bfr[24] = DC_STATUS_ADDRESS >> 24;
   sec_add_bfr[25] = DC_STATUS_ADDRESS >> 16;
   sec_add_bfr[26] = DC_STATUS_ADDRESS >> 8;
   sec_add_bfr[27] = DC_STATUS_ADDRESS;
   
   sec_add_bfr[28] = HIGH_SAMP_HK_ADDRESS >> 24;
   sec_add_bfr[29] = HIGH_SAMP_HK_ADDRESS >> 16;
   sec_add_bfr[30] = HIGH_SAMP_HK_ADDRESS >> 8;
   sec_add_bfr[31] = HIGH_SAMP_HK_ADDRESS;
   
   sec_add_bfr[32] = BC_ATTEMPT_FLAG;
   
   
   sec_add_bfr[33] = ADDRESS_WRITING_COUNTER >> 24;
   sec_add_bfr[34] = ADDRESS_WRITING_COUNTER >> 16;
   sec_add_bfr[35] = ADDRESS_WRITING_COUNTER >> 8;
   sec_add_bfr[36] = ADDRESS_WRITING_COUNTER;
   

   output_low(PIN_C4);                                                           //COM_MUX MAINSIDE
   int8 num = 0;
   SUBSECTOR_4KB_ERASE_OF(ADD_INFO_ADDRESS);
   delay_ms(200);
   for(num = 0; num < FLASH_ADD_SIZE; num++)                                     //saves all address data in OBC FLASH
   {
      WRITE_DATA_BYTE_OF(ADD_INFO_ADDRESS + num,sec_add_bfr[num]);
      delay_us(10);
   }
   SUBSECTOR_4KB_ERASE_SCF(ADD_INFO_ADDRESS);
   delay_ms(200);
   for(num = 0; num < FLASH_ADD_SIZE; num++)                                     //saves all address data in COM FLASH
   {
      WRITE_DATA_BYTE_SCF(ADD_INFO_ADDRESS + num,sec_add_bfr[num]);
      delay_us(10);
   }
   if(MISSION_STATUS == 0)
   {     
      output_low(PIN_A5);                                                           //CAM_MUX MAINSIDE
      delay_ms(10);
      SUBSECTOR_4KB_ERASE_SMF(ADD_INFO_ADDRESS);
      delay_ms(200);
      for(num = 0; num < FLASH_ADD_SIZE; num++)                                     //gsaves all address data in MISSION FLASH
      {
         WRITE_DATA_BYTE_SMF(ADD_INFO_ADDRESS + num,sec_add_bfr[num]);
         delay_us(10);
      }
   }
   output_high(PIN_C4);                                                          //COM_MUX COMSIDE
  
   fprintf(PC,"ADDRESS DATA STORE DONE:%lx\r\n",ADD_INFO_ADDRESS);
   return;
}


void TAKE_ADDRESS_DATA_FROM_OF()                                                 //Function that takes from memory the addresses stored in the first places
{  
   
   for(int8 num = 0; num < FLASH_ADD_SIZE; num++)
   {
      sec_add_bfr[num] = READ_DATA_BYTE_OF(ADD_INFO_ADDRESS + num);
   }
   return;
}


void TAKE_ADDRESS_DATA_FROM_SCF()
{  
   output_low(PIN_C4);  
   for(int8 num = 0; num < FLASH_ADD_SIZE; num++)
   {
      sec_add_bfr[num] = READ_DATA_BYTE_SCF(ADD_INFO_ADDRESS + num);
   }
   output_high(PIN_C4);
   return;
}


void TAKE_ADDRESS_DATA_FROM_SMF()
{  
   output_low(PIN_A5);  
   for(int8 num = 0; num < FLASH_ADD_SIZE; num++)
   {
      sec_add_bfr[num] = READ_DATA_BYTE_SMF(ADD_INFO_ADDRESS + num);
   }
   return;
}


void MAKE_FLAG_INFO()                                                            //Saves in each variable the information of flags retrieved from memory
{
   BC_ATTEMPT_FLAG     = flag_info_bffr[0];
   PASSED_DAYS         = make16(flag_info_bffr[1],flag_info_bffr[2]);
   RESERVE_CHECK       = flag_info_bffr[3];
   RESERVE_TARGET_FLAG = flag_info_bffr[4];
   RESERVE_MIN_FLAG    = flag_info_bffr[5];
   MISSION_CONTENTS    = flag_info_bffr[6];
   //MISSION_DETAIL      = flag_info_bffr[7];
   Kill_FLAG_MAIN      = flag_info_bffr[8];
   Kill_FLAG_FAB       = flag_info_bffr[9];
   FIRST_HSSC_DONE     = flag_info_bffr[10];
   ANT_DEP_STATUS      = flag_info_bffr[11];
   UPLINK_SUCCESS      = flag_info_bffr[12];
   MISSION_STATUS      = flag_info_bffr[13];
   MISSION_OPERATING   = flag_info_bffr[14];
   if(PASSED_DAYS == 0xffff)
   {
      COLLECT_RESET_DATA();                                                      //function that sends command 0x28 to reset PIC and loads reset_bffr []
      PASSED_DAYS = make16(reset_bffr[4],reset_bffr[5]);
   }
   if(PASSED_DAYS == 0xffff)
   {
      PASSED_DAYS = make16(0x00,0x00);
   }
   return;
}


void MAKE_ADDRESS_DATA()                                                         //Function that separates the address data in each corresponding variable
{                                                                                
                                                                                 //sec_add_bfr [] vector containing the address data
                                                                                 
   FLAG_DATA_ADDRESS = make32(sec_add_bfr[0],sec_add_bfr[1],sec_add_bfr[2],sec_add_bfr[3]);//FLAG_DATA_ADDRESS_1 | FLAG_DATA_ADDRESS_2 | FLAG_DATA_ADDRESS_3 | FLAG_DATA_ADDRESS_4;
   
   RSV_DATA_ADDRESS = make32(sec_add_bfr[4],sec_add_bfr[5],sec_add_bfr[6],sec_add_bfr[7]);//RSV_DATA_ADDRESS_1| RSV_DATA_ADDRESS_2 | RSV_DATA_ADDRESS_3 | RSV_DATA_ADDRESS_4;
   
   SAT_LOG = make32(sec_add_bfr[8],sec_add_bfr[9],sec_add_bfr[10],sec_add_bfr[11]);//SAT_LOG_1 | SAT_LOG_2 | SAT_LOG_3 | SAT_LOG_4;
   
   FAB_HK_ADDRESS = make32(sec_add_bfr[12],sec_add_bfr[13],sec_add_bfr[14],sec_add_bfr[15]);//FAB_HK_ADDRESS_1 | FAB_HK_ADDRESS_2 | FAB_HK_ADDRESS_3 | FAB_HK_ADDRESS_4;

   FAB_CW_ADDRESS = make32(sec_add_bfr[16],sec_add_bfr[17],sec_add_bfr[18],sec_add_bfr[19]);//FAB_CW_ADDRESS_1| FAB_CW_ADDRESS_2 | FAB_CW_ADDRESS_3 | FAB_CW_ADDRESS_4;
   
   ADCS_SENSOR_ADDRESS = make32(sec_add_bfr[20],sec_add_bfr[21],sec_add_bfr[22],sec_add_bfr[23]);//ADCS_SENSOR_ADDRESS_1 | ADCS_SENSOR_ADDRESS_2 | ADCS_SENSOR_ADDRESS_3 | ADCS_SENSOR_ADDRESS_4;
   
   DC_STATUS_ADDRESS = make32(sec_add_bfr[24],sec_add_bfr[25],sec_add_bfr[26],sec_add_bfr[27]);//DC_STATUS_ADDRESS_1 | DC_STATUS_ADDRESS_2 | DC_STATUS_ADDRESS_3 | DC_STATUS_ADDRESS_4;
   
   HIGH_SAMP_HK_ADDRESS = make32(sec_add_bfr[28],sec_add_bfr[29],sec_add_bfr[30],sec_add_bfr[31]);//HIGH_SAMP_HK_ADDRESS = HIGH_SAMP_HK_ADDRESS_1 | HIGH_SAMP_HK_ADDRESS_2 | HIGH_SAMP_HK_ADDRESS_3 | HIGH_SAMP_HK_ADDRESS_4;
   
   ADDRESS_WRITING_COUNTER = make32(sec_add_bfr[33],sec_add_bfr[34],sec_add_bfr[35],sec_add_bfr[36]);//ADDRESS_WRITING_COUNTER_1 | ADDRESS_WRITING_COUNTER_2
   
   return;
}


void CHECK_FLAG_INFO()                                                           //function that prints the data of the flags and separates them in each variable
{
   int8 checksum = 0;
   TAKE_FLAG_INFO_FROM_OF();                                                     //function that reads from the Main flash the state of the flags and loads the flag_info_bffr array
   for(int8 num = 0; num < FLAG_INFO_SIZE; num++)
   {
      if(flag_info_bffr[num] == 0xff)                                            //if the information retrieved is 0xff (no information) increases the checksum
      {
         checksum++;
      }
   }
   //fprintf(PC,"%d",checksum);
   
   if((flag_info_bffr[0]!=0xff)&&(flag_info_bffr[FLAG_INFO_SIZE-1]!=0xff))       //if something stored
   {  
      fprintf(PC,"Flag Data: ");
      for(num = 0; num < FLAG_INFO_SIZE; num++)                                  //print the information of the flags
      {
         fprintf(PC,"%x",flag_info_bffr[num]);
      }
      MAKE_FLAG_INFO();                                                          //separate the state of the flags in each variable
   }
   else
   {                                                                        //if nothing stored
      MAKE_FLAG_from_EEPROM();                                                   //read from the eeprom the state of the flags
   }
   checksum = 0;
   return;
}

void CHECK_ADDRESS_DATA()                                                        //prints the address data and separates it into each corresponding variable
{
   int8 checksum = 0;
   TAKE_ADDRESS_DATA_FROM_OF();                                                  //load the array sec_add_bfr [] with address location info and print it                                                
   for(int8 num = 33; num < 37; num++)                                           //check the data of last 4 byte
   {
      if(sec_add_bfr[num] == 0xff)                                               //if nothing, count checksum
      {
         checksum++;
      }
   }
   
   if(checksum != 4)                                                             //if something stored (if last 4 byte were not 0xff)
   {  
      fprintf(PC,"Address Data: ");
      for(num = 0; num < FLASH_ADD_SIZE; num++)
      {
         fprintf(PC,"%x",sec_add_bfr[num]);                                      //prints the content of sec_add_bfr [] containing the addresses
      }
      fprintf(PC,"\r\n");
      MAKE_ADDRESS_DATA();                                                       //separates them into the variables corresponding to each address
   }
   else
   {                                                                        //if there is nothing, check from SCF
      output_low(PIN_C4);
      TAKE_ADDRESS_DATA_FROM_SCF();
      output_high(PIN_C4);
      checksum = 0;
      for(num = 33; num < 37; num++)                                             //check the data of last 4 byte
      {
         if(sec_add_bfr[num] == 0xff)                                            //if nothing, count checksum
         {
            checksum++;
         }
      }
      if(checksum != 4)
      {
         for(num = 0; num < FLASH_ADD_SIZE; num++)
         {
            fprintf(PC,"%x",sec_add_bfr[num]);
         }
         fprintf(PC,"\r\n");
         MAKE_ADDRESS_DATA();
      }
      else
      {                                                                     //if there is nothing, check from SMF
         output_low(PIN_A5);
         TAKE_ADDRESS_DATA_FROM_SMF();
         checksum = 0;
         for(num = 33; num < 37; num++)                                          //check the data of last 4 byte
         {
            if(sec_add_bfr[num] == 0xff)                                         //if nothing, count checksum
            {
               checksum++;
            }
         }
         if(checksum != 4)
         {
            for(num = 0; num < FLASH_ADD_SIZE; num++)
            {
               fprintf(PC,"%x",sec_add_bfr[num]);
            }
            fprintf(PC,"\r\n");
            MAKE_ADDRESS_DATA();
         }
      }
   }  
   checksum = 0;  
   return;
}


//////////////////////////
//FAB_HK_ADDRESS        //
//FAB_CW_ADDRESS        //
//ADCS_SENSOR_ADDRESS   //
//CAM_ADDRESS           //
//DC_STATUS_ADDRESS     //
//////////////////////////


void MEMORY_ERASE(int8 CMD1, int8 CMD2, int8 CMD3)
{
   if (CMD1 == 0xAA && CMD2 == 0xBB && CMD3 == 0xCC)                             //Unnecesarily convolouted as sometimes the flash erase will take forever or not at all so it is
                                                                                 //broken into steps, usually takes ~5 minutes
   {
      fprintf(PC,"FLASH MEMORY ERASE START\r\n");
      
      output_low(PIN_A5);                                                           //CAM_MUX MAINSIDE
      output_low(PIN_C4);                                                           //COM_MUX MAINSIDE
      //DIE_ERASE_OF();
      //DIE_ERASE_SCF();
      //DIE_ERASE_SMF();
      
      
      sector_erase_OF(0);
      sector_erase_SCF(0);
      sector_erase_SMF(0);
      
      sector_erase_OF(FAB_CW_ADDRESS);
      sector_erase_SCF(FAB_CW_ADDRESS);
      sector_erase_SMF(FAB_CW_ADDRESS);
      
      
      for(int8 num = 0; num < 5; num++)
      {
         sector_erase_OF(FLAG_DATA_ADDRESS + SECT*num);                             //65536*num
         sector_erase_OF(IMGCLS_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(MCAM1_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(MCAM1_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(MCAM2_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(MCAM2_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(FAB_HK_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(ADCS_SENSOR_ADDRESS + SECT*num);                           //65536*num
         sector_erase_OF(DC_STATUS_ADDRESS + SECT*num);                             //65536*num
         sector_erase_OF(HIGH_SAMP_HK_ADDRESS + SECT*num);                          //65536*num
         
         sector_erase_SCF(FLAG_DATA_ADDRESS + SECT*num);                             //65536*num
         sector_erase_SCF(IMGCLS_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SCF(MCAM1_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SCF(MCAM1_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SCF(MCAM2_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SCF(MCAM2_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SCF(FAB_HK_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SCF(ADCS_SENSOR_ADDRESS + SECT*num);                           //65536*num
         sector_erase_SCF(DC_STATUS_ADDRESS + SECT*num);                             //65536*num
         sector_erase_SCF(HIGH_SAMP_HK_ADDRESS + SECT*num);                          //65536*num
         
         sector_erase_SMF(FLAG_DATA_ADDRESS + SECT*num);                             //65536*num
         sector_erase_SMF(IMGCLS_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SMF(MCAM1_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SMF(MCAM1_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SMF(MCAM2_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SMF(MCAM2_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SMF(FAB_HK_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SMF(ADCS_SENSOR_ADDRESS + SECT*num);                           //65536*num
         sector_erase_SMF(DC_STATUS_ADDRESS + SECT*num);                             //65536*num
         sector_erase_SMF(HIGH_SAMP_HK_ADDRESS + SECT*num);                          //65536*num
      }
      for(num = 5; num < 10; num++)
      {
         sector_erase_OF(IMGCLS_ADDRESS + SECT*num);                                   //65536*num
         sector_erase_OF(MCAM1_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(MCAM1_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(MCAM2_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(MCAM2_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(FAB_HK_ADDRESS + SECT*num);                                //65536*num
         sector_erase_OF(HIGH_SAMP_HK_ADDRESS + SECT*num);                          //65536*num
         
         sector_erase_SCF(IMGCLS_ADDRESS + SECT*num);                                  //65536*num
         sector_erase_SCF(MCAM1_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SCF(MCAM1_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SCF(MCAM2_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SCF(MCAM2_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SCF(FAB_HK_ADDRESS + SECT*num);                               //65536*num
         sector_erase_SCF(HIGH_SAMP_HK_ADDRESS + SECT*num);                         //65536*num
         
         sector_erase_SMF(IMGCLS_ADDRESS + SECT*num);                                  //65536*num
         sector_erase_SMF(MCAM1_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SMF(MCAM1_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SMF(MCAM2_THMB_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SMF(MCAM2_IMG_ADDRESS + SECT*num);                                //65536*num
         sector_erase_SMF(FAB_HK_ADDRESS + SECT*num);                               //65536*num
         sector_erase_SMF(HIGH_SAMP_HK_ADDRESS + SECT*num);                         //65536*num
      }
      
      sector_erase_OF(0x06650000);                                                  //erase ADCS TLE address
      sector_erase_SCF(0x06650000);
      sector_erase_SMF(0x06650000);
      
      fprintf(PC,"\r\nFlag Data:\r\n");
      TRANSFER_DATA_NBYTE_TOPC_SCF(FLAG_DATA_ADDRESS, 1);
      delay_ms(10);
      fprintf(PC,"\r\nSATLOG Data:\r\n");
      TRANSFER_DATA_NBYTE_TOPC_SCF(SAT_LOG, 81);
      delay_ms(10);
      fprintf(PC,"\r\nCW Data:\r\n");
      TRANSFER_DATA_NBYTE_TOPC_SCF(FAB_CW_ADDRESS, 81*5);
      delay_ms(10);
      fprintf(PC,"\r\nIMGCLS Data:\r\n");
      TRANSFER_DATA_NBYTE_TOPC_SCF(IMGCLS_ADDRESS, 81*5);
      delay_ms(10);
      fprintf(PC,"\r\nHSSC Data:\r\n");      
      TRANSFER_DATA_NBYTE_TOPC_SCF(HIGH_SAMP_HK_ADDRESS, 81*5);
      delay_ms(10);
      fprintf(PC,"\r\nHK Data:\r\n");      
      TRANSFER_DATA_NBYTE_TOPC_SCF(FAB_HK_ADDRESS, 81*5);
      delay_ms(10);
      fprintf(PC,"\r\nMCAM Data:\r\n");      
      TRANSFER_DATA_NBYTE_TOPC_SCF(MCAM1_IMG_ADDRESS, 81*5);
      
      output_high(PIN_C4);                                                          //COM_MUX COMSIDE
      delay_ms(10000);
      fprintf(PC,"FLASH MEMORY ERASE DONE\r\n");
      for(int t=60; t>0; t--){
      fprintf(PC,"Remaining time: %d  \r", t);
      delay_ms(1000);
      }
      //delay_ms(60000);
   }
   return;
}


/*
//@@@@@@@@@@@@@@@@@@MISSION LOOP@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void LOOP_FLAG_DATA_ADDRESS()
{
   
   if(FLAG_DATA_ADDRESS > FLAG_DATA_ADDRESS_END)
   {
   }
   return;
}

void LOOP_RSV_DATA_ADDRESS()
{
   if(RSV_DATA_ADDRESS > RSV_DATA_ADDRESS_END)
   {
      
   }
   return;
}
*/


void LOOP_SAT_LOG()                                                              //loops memory locations to store SAT_LOG
{
   if(SAT_LOG >= SAT_LOG_END)                                                    //if the write position of SAT_LOG reaches SAT_LOG_END
   {
      sector_erase_OF(SECT*7);                                                   //deletes a sector of the MAIN flash, 65536 * 7
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      sector_erase_SCF(SECT*7);                                                  //clears a sector from the COM flash, 65536 * 7
      output_high(PIN_C4);                                                       //CAM_MUX MAINSIDE
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         sector_erase_SMF(SECT*7);                                                  //deletes a sector of the MISSION flash, 65536 * 7
      }
      SAT_LOG = SECT*7;                                                          //Change the address of SAT_LOG and keep the first sector, 65536 * 7
      STORE_ADRESS_DATA_TO_FLASH();                                              //saves the address data, it will be in a new sector if the R / W cycle is fulfilled
   }
   //output_high(PIN_C4);//COM_MUX COMSIDE
   return;
}


//!void LOOP_CAM_ADDRESS()                                                          //loops memory locations to store CAM data
//!{
//!   if(CAM_ADDRESS >= CAM_ADDRESS_END)                                            //if the write position of CAM reaches CAM_ADDRESS_END
//!   {
//!      sector_erase_OF(SECT*9);                                                   //deletes 1 sector of the MAIN flash, 65536 * 9
//!      
//!      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
//!      sector_erase_SCF(SECT*9);                                                  //clears 1 sector of COM flash, 65536 * 9
//!      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
//!      
//!      output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
//!      sector_erase_SMF(SECT*9);                                                  //deletes 1 sector of the MISSION flash, 65536 * 9
//!      
//!      CAM_ADDRESS = SECT*9;                                                      //CAM address is now located at the beginning of the space designated to CAM
//!      STORE_ADRESS_DATA_TO_FLASH();
//!   }
//!   return;
//!}


void LOOP_FAB_HK_ADDRESS()                                                       //Rotate data save positions if it reaches the end of the allocated space                                                      
{
   int32 checksum = FAB_HK_ADDRESS && 0x0000ffff;
   if(FAB_HK_ADDRESS >= FAB_HK_ADDRESS_END)                                      
   {
      sector_erase_OF(SECT*308);
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      sector_erase_SCF(SECT*308);
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         sector_erase_SMF(SECT*308);
      }
      FAB_HK_ADDRESS = 65536*309;                                                  //the new address is again the beginning of the allocated space
      STORE_ADRESS_DATA_TO_FLASH();                                              //saves the address data and in a new sector if the R / W cycle is fulfilled
   }
   else if(65536 - checksum < 119)
   {                                                                             //if value will be close to the sector
      fprintf(PC,"erasing next sector\r\n");
      
      sector_erase_OF(FAB_HK_ADDRESS + SECT);                                    //erase next sector in advance
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      sector_erase_SCF(FAB_HK_ADDRESS + SECT);
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         sector_erase_SMF(FAB_HK_ADDRESS + SECT);
      }
   }
   return;
}


void LOOP_FAB_CW_ADDRESS()                                                       //Rotate data save positions if it reaches the end of the allocated space
{
   int32 checksum = FAB_CW_ADDRESS && 0x0000ffff;
   if(FAB_CW_ADDRESS >= FAB_CW_ADDRESS_END)
   {  
      sector_erase_OF(SECT*608);
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      sector_erase_SCF(SECT*608);
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         sector_erase_SMF(SECT*608);
      }
      FAB_CW_ADDRESS = SECT*608;
      STORE_ADRESS_DATA_TO_FLASH();
   }
   else if(SECT - checksum < 5)
   {                                                                             //if value will be close to the sector
      fprintf(PC,"erasing next sector\r\n");
      
      sector_erase_OF(FAB_CW_ADDRESS + SECT);                                    //erase next sector in advance
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      sector_erase_SCF(FAB_CW_ADDRESS + SECT);                                   //erase next sector in advance
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      
      if(MISSION_STATUS == 0) //prevent SMF interruption during mission running
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         sector_erase_SMF(FAB_CW_ADDRESS + SECT);                                   //erase next sector in advance
      }
      
   }
   return;
}


void LOOP_ADCS_SENSOR_ADDRESS()
{
   if(ADCS_SENSOR_ADDRESS >= ADCS_SENSOR_ADDRESS_END)                            //for 15min operation, data size will be more than 1sector
   {
      sector_erase_OF(SECT*922);
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      sector_erase_SCF(SECT*922);
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         sector_erase_SMF(SECT*922);
      }
      
      ADCS_SENSOR_ADDRESS = SECT*922;                                           //keep first 2 sector forever
      STORE_ADRESS_DATA_TO_FLASH();
   }
   else
   {                                                                        //if value will be close to the sector
      fprintf(PC,"erasing next sector\r\n");
      
      int i;
      for(i = 1; i < 10; i++)
      {
         sector_erase_OF(ADCS_SENSOR_ADDRESS + SECT*i);                          //erase next sector in advance
      }
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      for(i = 1; i < 10; i++)
      {
         sector_erase_SCF(ADCS_SENSOR_ADDRESS + SECT*i);                         //erase next sector in advance
      }
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         for(i = 1; i < 10; i++)
         {
            sector_erase_SMF(ADCS_SENSOR_ADDRESS + SECT*i);                         //erase next sector in advance
         }
      }
   }
   return;
}


void LOOP_DC_STATUS_ADDRESS()
{
   int32 checksum = DC_STATUS_ADDRESS && 0x00000fff;
   if(DC_STATUS_ADDRESS >= DC_STATUS_ADDRESS_END)
   {
      SUBSECTOR_4KB_ERASE_OF(SECT*912+4096);
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      SUBSECTOR_4KB_ERASE_SCF(SECT*912+4096);
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         SUBSECTOR_4KB_ERASE_SMF(SECT*912+4096);
      }
      
      DC_STATUS_ADDRESS = SECT*912+4096;                                        //keep first 1 SUBSECTOR forever
      STORE_ADRESS_DATA_TO_FLASH();
   }
   else if(4096 - checksum < 800)
   {                                                                            //if value will be close to the sector
      fprintf(PC,"erasing next sector\r\n");
      
      SUBSECTOR_4KB_ERASE_OF(DC_STATUS_ADDRESS + 4096);                          //erase next SUBSECTOR in advance
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      SUBSECTOR_4KB_ERASE_SCF(DC_STATUS_ADDRESS + 4096);                         //erase next SUBSECTOR in advance
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         SUBSECTOR_4KB_ERASE_SMF(DC_STATUS_ADDRESS + 4096);                         //erase next SUBSECTOR in advance
      }
   }
   return;
}


void LOOP_HIGH_SAMP_HK_ADDRESS()
{
   if(HIGH_SAMP_HK_ADDRESS >= HIGH_SAMP_HK_ADDRESS_END)                          //for 15min operation, data size will be more than 2sector
   {
      sector_erase_OF(SECT*660);                                                //delet from 066A0000 (keep first 3 sectors)
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      sector_erase_SCF(SECT*660);
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //CAM_MUX MAINSIDE
         sector_erase_SMF(SECT*660);
      }
      
      HIGH_SAMP_HK_ADDRESS = SECT*660;                                          //keep first 2 sector forever
   }
   else
   {                                                                             //if value will be close to the sector
      fprintf(PC,"erasing next sector\r\n");
      
      sector_erase_OF(HIGH_SAMP_HK_ADDRESS + SECT);                              //erase next sector in advance
      sector_erase_OF(HIGH_SAMP_HK_ADDRESS + SECT*2);                            //erase next sector in advance
      sector_erase_OF(HIGH_SAMP_HK_ADDRESS + SECT*3);                            //erase next sector in advance
      
      output_low(PIN_C4);                                                        //COM_MUX MAINSIDE
      sector_erase_SCF(HIGH_SAMP_HK_ADDRESS + SECT);                             //erase next sector in advance
      sector_erase_SCF(HIGH_SAMP_HK_ADDRESS + SECT*2);                           //erase next sector in advance
      sector_erase_SCF(HIGH_SAMP_HK_ADDRESS + SECT*3);                           //erase next sector in advance
      output_high(PIN_C4);                                                       //COM_MUX COMSIDE
      
      if(MISSION_STATUS == 0)
      {
         output_low(PIN_A5);                                                        //SMF_MUX MAINSIDE
         sector_erase_SMF(HIGH_SAMP_HK_ADDRESS + SECT);                             //erase next sector in advance
         sector_erase_SMF(HIGH_SAMP_HK_ADDRESS + SECT*2);                           //erase next sector in advance
         sector_erase_SMF(HIGH_SAMP_HK_ADDRESS + SECT*3);                           //erase next sector in advance
      }
   }
   return;
}


