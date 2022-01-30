//TODO: change reservation commands to reserve 8 byte commands not 7 bytes

#define table_size 90
unsigned int8 reserve_table[table_size+1] = {};
unsigned int8 table_num = 0;

void initialize_table()                                                          //Function that initializes a table
{                                                                                //fill all positions with 0xff
   for(int num = 0; num < table_size; num++)
   {
      reserve_table[num] = 0xff;
   }
   reserve_table[90] = 0;                                                        //Place in the last position 0 as the amount of spaces used
   return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//                          ***Sort Function***
//This is for sorting the saved reservation commands in ascending order.
//
//reserved_table[j-9] reserved_table[j-8] reserved_table[j-7] reserved_table[j-6] reserved_table[j-5] reserved_table[j-4] reserved_table[j-3] reserved_table[j-2]
//reserved_table[j-1]  reserved_table[j]  reserved_table[j+1] reserved_table[j+2] reserved_table[j+3] reserved_table[j+4] reserved_table[j+5] reserved_table[j+6] 
//
//j-9 and j-1 data are the contents information(like mission)
//j-8 and j data are the time data(execute the command reserved_table[j] min later )
//j-7 and j+1 data are the contents detail information
//
//by comapring j-8 and j data,if it is needed, swhich j-9 and j-1, j-8 and j, j-7 and j+1....etc
////////////////////////////////////////////////////////////////////////////////////////////////

int8 count_reservation()                                                         //Function that counts the number of positions in the table with data other than 0xFF
{
   int8 counter = 0;
   for(int num = 1; num < table_size - 7; num = num + 9)
   {
      if(reserve_table[num-1] != 0xff)
      {
         counter++;
      }
   }
   return counter * 9;
}
//changing is not yet.
void sort_table()                                                                //(command info, time info, mission mode)~10¨compare the time and so in ascending formrt
{
   for(int i = 1; i < table_size - 8; i = i + 9){

         // compare from lower site
      for(int j = table_size - 8; j > i; j = j - 9){

      // if upper value is larger, switch the values
         if(reserve_table[j] < reserve_table[j-9]){                              //compare the time of [j]th and [j-9]th
            //fprintf(PC, "compare:%x and %x", reserve_table[j], reserve_table[j-9]);
            int8 t1 = reserve_table[j+7];
            int8 t2 = reserve_table[j+6];
            int8 t3 = reserve_table[j+5];
            int8 t4 = reserve_table[j+4];
            int8 t5 = reserve_table[j+3];
            int8 t6 = reserve_table[j+2];
            int8 t7 = reserve_table[j+1];
            int8 t8 = reserve_table[j];
            int8 t9 = reserve_table[j-1];
//            fprintf(PC, "bottom_command:%x%x%x%x%x%x%x%x%x", reserve_table[j-1], reserve_table[j], reserve_table[j+1], reserve_table[j+2], reserve_table[j+3], reserve_table[j+4], reserve_table[j+5], reserve_table[j+6], reserve_table[j+7]);            
 //           fprintf(PC, "t_array:%x%x%x%x%x%x%x%x%x", t9, t8, t7, t6, t5, t4, t3, t2, t1);
            
            reserve_table[j+7] = reserve_table[j-2];
            reserve_table[j+6] = reserve_table[j-3];
            reserve_table[j+5] = reserve_table[j-4];
            reserve_table[j+4] = reserve_table[j-5];
            reserve_table[j+3] = reserve_table[j-6];
            reserve_table[j+2] = reserve_table[j-7];
            reserve_table[j+1] = reserve_table[j-8];
            reserve_table[j] = reserve_table[j-9];
            reserve_table[j-1] = reserve_table[j-10];
            //fprintf(PC, "bottom_command:%x%x%x%x%x%x%x%x%x", reserve_table[j-10], reserve_table[j-9], reserve_table[j-8], reserve_table[j-7], reserve_table[j-6], reserve_table[j-5], reserve_table[j-4], reserve_table[j-3], reserve_table[j-2]);            
            //fprintf(PC, "up_command:%x%x%x%x%x%x%x%x%x", reserve_table[j-1], reserve_table[j], reserve_table[j+1], reserve_table[j+2], reserve_table[j+3], reserve_table[j+4], reserve_table[j+5], reserve_table[j+6], reserve_table[j+7]);            
            
            reserve_table[j-2] = t1;
            reserve_table[j-3] = t2;
            reserve_table[j-4] = t3;
            reserve_table[j-5] = t4;
            reserve_table[j-6] = t5;
            reserve_table[j-7] = t6;
            reserve_table[j-8] = t7;
            reserve_table[j-9] = t8;
            reserve_table[j-10] = t9;
            
            //fprintf(PC, "bottom_command:%x%x%x%x%x%x%x%x%x", reserve_table[j-10], reserve_table[j-9], reserve_table[j-8], reserve_table[j-7], reserve_table[j-6], reserve_table[j-5], reserve_table[j-4], reserve_table[j-3], reserve_table[j-2]);            
            //fprintf(PC, "t_array:%x%x%x%x%x%x%x%x%x", t9, t8, t7, t6, t5, t4, t3, t2, t1);
            //fprintf(PC, "up_command:%x%x%x%x%x%x%x%x%x", reserve_table[j-1], reserve_table[j], reserve_table[j+1], reserve_table[j+2], reserve_table[j+3], reserve_table[j+4], reserve_table[j+5], reserve_table[j+6], reserve_table[j+7]);
       
         
         }
         else{
           // fprintf(PC, "No change\r\n");
         }
      }
   }
   return;
}

void Save_RSV()
{
   output_low(PIN_A5);                                                           //CAM_MUX MAINSIDE
   output_low(PIN_C4);                                                           //COM_MUX MAINSIDE
   SUBSECTOR_4KB_ERASE_OF(RSV_DATA_ADDRESS);
   delay_ms(100);
   SUBSECTOR_4KB_ERASE_SCF(RSV_DATA_ADDRESS);
   delay_ms(100);
   SUBSECTOR_4KB_ERASE_SMF(RSV_DATA_ADDRESS);
   delay_ms(100);
   
   for(int num = 0; num < table_size + 1; num++)                                 //save in flash memory reserve_table[] content
   {
      WRITE_DATA_BYTE_OF(RSV_DATA_ADDRESS+num,reserve_table[num]);
      WRITE_DATA_BYTE_SMF(RSV_DATA_ADDRESS+num,reserve_table[num]);
      WRITE_DATA_BYTE_SCF(RSV_DATA_ADDRESS+num,reserve_table[num]);
   }
   
   WRITE_DATA_BYTE_OF(RSV_DATA_ADDRESS+91,RESERVE_MIN_FLAG);                     // save reserve flag
   WRITE_DATA_BYTE_SCF(RSV_DATA_ADDRESS+91,RESERVE_MIN_FLAG);
   WRITE_DATA_BYTE_SMF(RSV_DATA_ADDRESS+91,RESERVE_MIN_FLAG);
   for(num = 0; num < table_size + 1; num++)                                     //show in PC RSV DATA
   {
      fprintf(PC,"%x",READ_DATA_BYTE_OF(RSV_DATA_ADDRESS + num));
   }
   output_high(PIN_C4);                                                          //COM_MUX MAINSIDE
   fprintf(PC,"\r\n");
   return;
}

void table_refresh()                                                             //table and flags are come back to default 
{
   for(int i = 0; i < table_size; i++)
   {
      reserve_table[i] = 0xff;
   }
   reserve_table[90] = 0;
   RESERVE_MIN_FLAG = 0;
   RESERVE_SEC_FLAG = 0;
   RESERVE_CHECK = 0;
   MISSION_CONTENTS = 0;                                                         //closest mission command
   RESERVE_TARGET_FLAG = 0;                                                      //closest time data
   RESERVE_DATA_0 = 0;                                                           //closest reserve command data
   RESERVE_DATA_1 = 0;                                                           //closest reserve command data
   RESERVE_DATA_2 = 0;                                                           //closest reserve command data
   RESERVE_DATA_3 = 0;                                                           //closest reserve command data
   RESERVE_DATA_4 = 0;                                                           //closest reserve command data
   RESERVE_DATA_5 = 0;                                                           //closest reserve command data
   RESERVE_DATA_6 = 0;                                                           //closest reserve command data
   Save_RSV();
   return;
}

void Get_RSV()
{
   //fprintf(PC,"%x");
   for(int num = 0; num < table_size+1; num++)                                   //Loads the data read from flash memory into the reserve_table [] vector 
   {                                                                             //from RSV_DATA_ADDRESS position to table size
      reserve_table[num]=READ_DATA_BYTE_OF(RSV_DATA_ADDRESS + num);              //import reservation table info
   }
   reserve_table[90] = count_reservation();                                      //Loads in position 30 the number of positions occupied with data
//   RESERVE_MIN_FLAG = READ_DATA_BYTE_OF(RSV_DATA_ADDRESS+31);
   if(reserve_table[90] == 0xFF || reserve_table[90] == 0x00)                    //If the table has no stored data
   {
      initialize_table();                                                        //fill the entire table with 0xff
   }
   table_num = reserve_table[90];                                                //saves in table_num the amount of data that the table contains
   if(table_num != 0x00)                                                         //yes-->get reserved command and target time
   {
      RESERVE_CHECK = 1;                                                         //The table is used
      MISSION_CONTENTS = reserve_table[0];                                       //closest mission command
      RESERVE_TARGET_FLAG = reserve_table[1];                                    //closest time data
      RESERVE_DATA_0 = reserve_table[2];                                         //closest reserve command data
      RESERVE_DATA_1 = reserve_table[3];                                         //closest reserve command data
      RESERVE_DATA_2 = reserve_table[4];                                         //closest reserve command data
      RESERVE_DATA_3 = reserve_table[5];                                         //closest reserve command data
      RESERVE_DATA_4 = reserve_table[6];                                         //closest reserve command data
      RESERVE_DATA_5 = reserve_table[7];                                         //closest reserve command data
      RESERVE_DATA_6 = reserve_table[8];                                         //closest reserve command data
   }
   else
   {
      RESERVE_CHECK = 0;                                                         //The table is not used
      MISSION_CONTENTS = 0xff;                                                   //closest mission command
      RESERVE_TARGET_FLAG = 0xff;                                                //closest time data
      RESERVE_DATA_0 = 0xff;                                                     //closest reserve command data
      RESERVE_DATA_1 = 0xff;                                                     //closest reserve command data
      RESERVE_DATA_2 = 0xff;                                                     //closest reserve command data
      RESERVE_DATA_3 = 0xff;                                                     //closest reserve command data
      RESERVE_DATA_4 = 0xff;                                                     //closest reserve command data
      RESERVE_DATA_5 = 0xff;                                                     //closest reserve command data
      RESERVE_DATA_6 = 0xff;                                                     //closest reserve command data
   }
   return;
}

void Disp_RSV()                                                                  //show the reserve status (reserve command and reserve table)
{
   Get_RSV();                                                                    //get the info from the main flash
   for(int num = 0; num < table_size + 1; num++)
   {
      fprintf(PC,"%x",reserve_table[num]);
   }
   fprintf(PC,"\r\n");
   fprintf(PC,"RESERVE_CHECK = %x\r\n",RESERVE_CHECK);
   fprintf(PC,"NEXT FUNCTION_CONTENTS = %x\r\n",MISSION_CONTENTS);
   fprintf(PC,"RESERVE_TARGET_FLAG = %x\r\n",RESERVE_TARGET_FLAG);
   //fprintf(PC,"NEXT FUNCTION DETAIL = %x\r\n",MISSION_DETAIL);
   fprintf(PC,"NEXT FUNCTION Command DATA = %x%x%x%x%x%x%x\r\n",RESERVE_DATA_0,RESERVE_DATA_1,RESERVE_DATA_2, RESERVE_DATA_3,RESERVE_DATA_4, RESERVE_DATA_5, RESERVE_DATA_6);
   //fprintf(PC,"NEXT FUNCTION PCKT NUMBER = %x\r\n",RESERVE_MIN_FLAG);
   fprintf(PC,"RESERVE_MIN_FLAG = %x\r\n",RESERVE_MIN_FLAG);
   fprintf(PC,"TABLE NUM = %x\r\n",table_num);
   
   return;
}


//TODO: ADD FINAL COMM COMMAND BYTE WHEN INPUTTING RESERVATION COMMAND, RIGHT NOW IT DOES NOT SAVE CMD_FROM_COMM[11]


void input_reservation_COM()                                                     //put the command from COM into the table and sort in ascending form
{
//!   reserve_table[table_num] = CMD_FROM_PC[0];                                 //input the data about mission content
//!   reserve_table[table_num + 1] = CMD_FROM_PC[1];                             //input the data about time data
//!   reserve_table[table_num + 2] = CMD_FROM_PC[2];                             //input the data about mission detail mode
   //fprintf(PC,"Table num: %x\r\n",reserve_table[80]);                          //show the amount of bytes ocupied in the table
   table_num = reserve_table[90];
   if(reserve_table[90] == 90)
   {
      fprintf(PC,"Reservation table is Full!\r\n\r\n"); 
   }
   else
   {
      if(RESERVE_CHECK != 0x00)                                                  //if the table is in use
      {
         for(int num = 1; num < table_size - 7; num = num + 9)                   //check all command data placed in the table and update the time
         {
            if(reserve_table[num-1] != 0xff)                                     
            {
               if(reserve_table[num] > RESERVE_MIN_FLAG)
               {
                  reserve_table[num] = reserve_table[num] - RESERVE_MIN_FLAG;    //update the time of all commands
               }
               else
               {
                  reserve_table[num] = 0;
               }
            }
         }
         reserve_table[table_num] = CMD_FROM_COMM[3];                             //input the data abou mission content
         reserve_table[table_num + 1] = CMD_FROM_COMM[4];                         //input data about time
         reserve_table[table_num + 2] = CMD_FROM_COMM[5];                         //input reserve command data
         reserve_table[table_num + 3] = CMD_FROM_COMM[6];                         //input reserve command data
         reserve_table[table_num + 4] = CMD_FROM_COMM[7];                         //input reserve command data
         reserve_table[table_num + 5] = CMD_FROM_COMM[8];                         //input reserve command data
         reserve_table[table_num + 6] = CMD_FROM_COMM[9];                         //input reserve command data
         reserve_table[table_num + 7] = CMD_FROM_COMM[10];                        //input reserve comamnd data
         reserve_table[table_num + 8] = CMD_FROM_COMM[11];                        //input reserve command data
         if(RESERVE_MIN_FLAG != 0)                                               //RESERVE_MIN_FLAG = 0 --> no need to update the target flag
         {
            RESERVE_TARGET_FLAG = RESERVE_TARGET_FLAG - RESERVE_MIN_FLAG;
            RESERVE_MIN_FLAG = 0;
         }                                               
      }
      else
      {
         RESERVE_MIN_FLAG = 0;
         RESERVE_SEC_FLAG = 0;
         reserve_table[table_num] = CMD_FROM_COMM[3];                             //input the data about mission content
         reserve_table[table_num + 1] = CMD_FROM_COMM[4];                         //input data about time
         reserve_table[table_num + 2] = CMD_FROM_COMM[5];                         //input reserve command data
         reserve_table[table_num + 3] = CMD_FROM_COMM[6];                         //input reserve command data
         reserve_table[table_num + 4] = CMD_FROM_COMM[7];                         //input reserve command data
         reserve_table[table_num + 5] = CMD_FROM_COMM[8];                         //input reserve command data
         reserve_table[table_num + 6] = CMD_FROM_COMM[9];                         //input reserve command data
         reserve_table[table_num + 7] = CMD_FROM_COMM[10];                        //input reserve command data
         reserve_table[table_num + 7] = CMD_FROM_COMM[11];                        //input reserve command data
      }
      //table_num = table_num + 3;
      //reserve_table[30] = table_num;
      sort_table();
//!      for(int num = 0; num < table_size+1; num++)
//!      {
//!         fprintf(PC,"%x",reserve_table[num]);//show for the test
//!      }
   }
   reserve_table[90] = count_reservation();                                      //update the table number data
   return;
}



void input_reservation_PC()                                                      //put the command from PC into the table and sort
{
//!   reserve_table[table_num] = in_bffr_main[4];
//!   reserve_table[table_num + 1] = in_bffr_main[5];
//!   reserve_table[table_num + 2] = in_bffr_main[6];
   fprintf(PC,"Table num: %x\r\n",reserve_table[90]);                            //print the table
   table_num = reserve_table[90];
   if(reserve_table[90] == 90)
   {
      fprintf(PC,"Reservation table is Full!\r\n\r\n");
   }
   else
   {
      if(RESERVE_CHECK != 0x00)                                                  //if new reservation is faster than target time
      {
         for(int num = 1; num < table_size - 7; num = num + 9)
         {
            if(reserve_table[num-1] != 0xff)                                     //check all command data placed in the table and update the time
            {
               if(reserve_table[num] > RESERVE_MIN_FLAG)
               {
                  reserve_table[num] = reserve_table[num] - RESERVE_MIN_FLAG;    //update the time of all commands
               }
               else
               {
                  reserve_table[num] = 0;
               }
            }
         }
         reserve_table[table_num] = CMD_FROM_PC[0];                              //input the data about mission content
         reserve_table[table_num + 1] = CMD_FROM_PC[1];                          //input reservation time
         reserve_table[table_num + 2] = CMD_FROM_PC[2];                          //input reserve command data
         reserve_table[table_num + 3] = CMD_FROM_PC[3];                          //input reserve command data
         reserve_table[table_num + 4] = CMD_FROM_PC[4];                          //input reserve command data
         reserve_table[table_num + 5] = CMD_FROM_PC[5];                          //input reserve command data
         reserve_table[table_num + 6] = CMD_FROM_PC[6];                          //input reserve command data
         reserve_table[table_num + 7] = CMD_FROM_PC[7];                          //input reserve command data
         reserve_table[table_num + 8] = CMD_FROM_PC[8];                          //input reserve command data
         
         if(RESERVE_MIN_FLAG != 0)                                               //RESERVE_MIN_FLAG = 0 --> no need to update the target flag
         {
            RESERVE_TARGET_FLAG = RESERVE_TARGET_FLAG - RESERVE_MIN_FLAG;
            RESERVE_MIN_FLAG = 0;
         }
      }
      else
      {                                                                     //if first reservation
         RESERVE_MIN_FLAG = 0;
         RESERVE_SEC_FLAG = 0;
         reserve_table[table_num] = CMD_FROM_PC[0];                              //input the data about mission content
         reserve_table[table_num + 1] = CMD_FROM_PC[1];
         reserve_table[table_num + 2] = CMD_FROM_PC[2];                          //input reserve command data
         reserve_table[table_num + 3] = CMD_FROM_PC[3];                          //input reserve command data
         reserve_table[table_num + 4] = CMD_FROM_PC[4];                          //input reserve command data
         reserve_table[table_num + 5] = CMD_FROM_PC[5];                          //input reserve command data
         reserve_table[table_num + 6] = CMD_FROM_PC[6];                          //input reserve command data
         reserve_table[table_num + 7] = CMD_FROM_PC[7];                          //input reserve command data
         reserve_table[table_num + 8] = CMD_FROM_PC[8];                          //input reserve command data         
      }
      //table_num = table_num + 3;
      //reserve_table[30] = table_num;
      sort_table();
//!      for(int num = 0; num < table_size+1; num++)
//!      {
//!         fprintf(PC,"%x",reserve_table[num]);//show for the test
//!      }
   }
   reserve_table[90] = count_reservation();                                      //update the table number data
   return;
}



void Remove_1_Reservation()                                                      //after executing the mission, it should be finished
{
   for(int i = 1; i < table_size - 7; i = i + 9)
   {
      if(reserve_table[i-1] != 0xff)                                             //check whether command data is placed
      {
         reserve_table[i] = reserve_table[i] - RESERVE_TARGET_FLAG;              //update the time of all commands
      }
   }
   reserve_table[0] = 0xff;                                                      //delete the finished mission data, put 0xFF to move at the end of the table
   reserve_table[1] = 0xff;                                                      //delete the finished reserve command data
   reserve_table[2] = 0xff;                                                      //delete the finished reserve command data
   reserve_table[3] = 0xff;                                                      //delete the finished reserve command data
   reserve_table[4] = 0xff;                                                      //delete the finished reserve command data
   reserve_table[5] = 0xff;                                                      //delete the finished reserve command data
   reserve_table[6] = 0xff;                                                      //delete the finished reserve command data
   reserve_table[7] = 0xff;                                                      //delete the finished reserve command data
   reserve_table[8] = 0xff;                                                      //delete the finished reserve command data          
   sort_table();
   reserve_table[90] = count_reservation();                                      //update the table number data 
   
   fprintf(PC,"Removed finished mission command:");
   Save_RSV();
   fprintf(PC,"\r\n");
   return;
}

void Reserve_command_COM()                                                       //reservation from COM
{
   fprintf(PC,"Reserve CMD from COM\r\n");
   input_reservation_COM();                                                      //put the command from COM into the table and sort in ascending form
   Save_RSV();                                                                   //save reserve table to flash memory
   RESERVE_CHECK = 1;                                                            // table check flag in HIGH
   if(reserve_table[90] == 0x00 || reserve_table[90] == 0xff)
   {
      RESERVE_MIN_FLAG = 0;                                                      //reset time flag
   }
   MISSION_CONTENTS = reserve_table[0];                                          //closest mission command
   RESERVE_TARGET_FLAG = reserve_table[1];                                       //closest time data
   RESERVE_DATA_0 = reserve_table[2];                                            //closest reserve command data
   RESERVE_DATA_1  = reserve_table[3];                                           //closest reserve command data
   RESERVE_DATA_2 = reserve_table[4];                                            //closest reserve command data
   RESERVE_DATA_3 = reserve_table[5];                                            //closest reserve command data
   RESERVE_DATA_4 = reserve_table[6];                                            //closest reserve command data
   RESERVE_DATA_5 = reserve_table[7];                                            //closest reserve command data
   RESERVE_DATA_6 = reserve_table[8];                                            //closest reserve command data
}



void Reserve_command_PC()                                                        //reservation from PC
{
   fprintf(PC,"Reserve CMD from PC\r\n");
   fprintf(PC, "%x%x%x%x%x%x%x%x",CMD_FROM_PC[0], CMD_FROM_PC[2], CMD_FROM_PC[3], CMD_FROM_PC[4], CMD_FROM_PC[5], CMD_FROM_PC[6], CMD_FROM_PC[7], CMD_FROM_PC[8]);
   input_reservation_PC();                                                       //put the command from PC into the table and sort in ascending form
   //fprintf(PC,"Reserve CMD from PC: %x%x%x%x%x%x%x%x%x", reserve_table[0], reserve_table[1], reserve_table[2], reserve_table[3], reserve_table[4], reserve_table[5], reserve_table[6], reserve_table[7], reserve_table[8]);
   fprintf(PC, "input_reservation_PC()\r\n");
   Save_RSV();//save reserve table to flash memory
   //printf(PC, "OK!");
   fprintf(PC,"Reserve CMD from PC: %x%x%x%x%x%x%x%x%x", reserve_table[0], reserve_table[1], reserve_table[2], reserve_table[3], reserve_table[4], reserve_table[5], reserve_table[6], reserve_table[7], reserve_table[8]);
   RESERVE_CHECK = 1;
   if(reserve_table[90] == 0x00 || reserve_table[90] == 0xff)                    //for only first reservation
   {
      RESERVE_MIN_FLAG = 0;                                                      //reset time flag
   }
   
   MISSION_CONTENTS = reserve_table[0];                                          //closest mission command
   RESERVE_TARGET_FLAG = reserve_table[1];                                       //closest time data
   RESERVE_DATA_0 = reserve_table[2];                                            //closest reserve command data
   RESERVE_DATA_1  = reserve_table[3];                                           //closest reserve command data
   RESERVE_DATA_2 = reserve_table[4];                                            //closest reserve command data
   RESERVE_DATA_3 = reserve_table[5];                                            //closest reserve command data
   RESERVE_DATA_4 = reserve_table[6];                                            //closest reserve command data
   RESERVE_DATA_5 = reserve_table[7];                                            //closest reserve command data
   RESERVE_DATA_6 = reserve_table[8];                                            //closest reserve command data
   
}



void Reserve_next()                                                              //after finish execution of reservation command, this function should be execute
{
   fprintf(PC,"Reserve next\r\n");
   //Save_RSV();//save reserve table to flash memory
   
   //RESERVE_CHECK = 1;
   RESERVE_MIN_FLAG = 0;                                                         //reset time flag
   MISSION_CONTENTS = reserve_table[0];                                          //closest mission command
   RESERVE_TARGET_FLAG = reserve_table[1];                                       //closest time data
   RESERVE_DATA_0 = reserve_table[2];                                            //closest reserve command data
   RESERVE_DATA_1  = reserve_table[3];                                           //closest reserve command data
   RESERVE_DATA_2 = reserve_table[4];                                            //closest reserve command data
   RESERVE_DATA_3 = reserve_table[5];                                            //closest reserve command data
   RESERVE_DATA_4 = reserve_table[6];                                            //closest reserve command data
   RESERVE_DATA_5 = reserve_table[7];                                            //closest reserve command data
   RESERVE_DATA_6 = reserve_table[8];                                            //closest reserve command data
}


