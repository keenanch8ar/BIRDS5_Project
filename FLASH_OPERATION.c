

void REFRESH_SECTOR_OF(int8 data1,int8 data2,int8 data3,int8 data4)
{
   fprintf(PC,"ERASE OF:%x%x%x%x\r\n",data1,data2,data3,data4);
   int32 address = make32(data1,data2,data3,data4);
   sector_erase_OF(address);                                                     //erase 1 sector
   
   int16 DATA = address/SECT;                                                    //maximum 2048, data almacena el nro de sector borrado
   int8 D1 = DATA >> 8;
   int8 D2 = DATA;
   
   SAVE_SAT_LOG(0xe0, D1, D2);                                                   //0xE0+16bit sector info
   
   return;
}
void REFRESH_SECTOR_SCF(int8 data1,int8 data2,int8 data3,int8 data4)
{
   fprintf(PC,"ERASE SCF:%x%x%x%x\r\n",data1,data2,data3,data4);
   int32 address = make32(data1,data2,data3,data4);
   output_low(PIN_C4);                                                           //multiplexor OBC side
   sector_erase_SCF(address);                                                    //erase 1 sector
   output_high(PIN_C4);
   
   int16 DATA = address/SECT;                                                    //maximum 2048, data almacena el nro de sector borrado
   int8 D1 = DATA >> 8;
   int8 D2 = DATA;
   
   SAVE_SAT_LOG(0xe1, D1, D2);                                                   //0xE0+16bit sector info
   return;
}
void REFRESH_SECTOR_SMF(int8 data1,int8 data2,int8 data3,int8 data4)
{
   fprintf(PC,"ERASE SMF:%x%x%x%x\r\n",data1,data2,data3,data4);
   int32 address = make32(data1,data2,data3,data4);
   output_low(PIN_A5);                                                           //multiplexor OBC side
   sector_erase_SMF(address);                                                    //erase 1 sector
   
   int16 DATA = address/SECT;                                                    //maximum 2048, data almacena el nro de sector borrado
   int8 D1 = DATA >> 8;
   int8 D2 = DATA;
   
   SAVE_SAT_LOG(0xe2, D1, D2);                                                   //0xE0+16bit sector info
   return;
}





void TRANSFER_SECTOR_OF2SCF(int8 data1,int8 data2,int8 data3,int8 data4)         //command 0x12
{
   REFRESH_SECTOR_SCF(data1,data2,data3,data4);                                  //erase 1 sector SCF
   delay_ms(500);
   fprintf(PC,"transfer OF2SCF:%x%x%x%x\r\n",data1,data2,data3,data4);
   int32 address = make32(data1,data2,data3,data4);
   address = address - (address%SECT);                                           //si la direccion no es exacta la de un sector, transfiere desde la direccion donde comienza el sector
   output_low(PIN_C4);                                                           //OBC side
   for(int32 i = 0; i < SECT; i++)
   {
      WRITE_DATA_BYTE_SCF(address + i, READ_DATA_BYTE_OF(address + i));
   }
   delay_ms(100);
   output_high(PIN_C4);                                                          //COM side
   
   int16 DATA = address/SECT;                                                    //maximum 2048, data almacena el nro de sector copiado
   int8 D1 = DATA >> 8;
   int8 D2 = DATA;
   
   SAVE_SAT_LOG(0x12, D1, D2);                                                   //0xE0+16bit sector info
   return;
}

void TRANSFER_SECTOR_OF2SMF(int8 data1,int8 data2,int8 data3,int8 data4)
{
   REFRESH_SECTOR_SMF(data1,data2,data3,data4);                                  //erase 1 sector SMF
   delay_ms(500);
   fprintf(PC,"transfer OF2SMF:%x%x%x%x\r\n",data1,data2,data3,data4);
   int32 address = make32(data1,data2,data3,data4);
   address = address - (address%SECT);                                           //si la direccion no es exacta la de un sector, transfiere desde la direccion donde comienza el sector
   output_low(PIN_A5);                                                           //OBC side
   for(int32 i = 0; i < SECT; i++)
   {
      WRITE_DATA_BYTE_SMF(address + i, READ_DATA_BYTE_OF(address + i));
   }
   delay_ms(100);
   
   int16 DATA = address/SECT;                                                    //maximum 2048, data almacena el nro de sector copiado
   int8 D1 = DATA >> 8;
   int8 D2 = DATA;
   
   SAVE_SAT_LOG(0x13, D1, D2);                                                   //0xE0+16bit sector info
   
   return;
}


void TRANSFER_SECTOR_SCF2OF(int8 data1,int8 data2,int8 data3,int8 data4)
{
   REFRESH_SECTOR_OF(data1,data2,data3,data4);                                   //erase 1 sector OBC flash
   delay_ms(500);
   fprintf(PC,"transfer SCF2OF:%x%x%x%x\r\n",data1,data2,data3,data4);
   int32 address = make32(data1,data2,data3,data4);
   address = address - (address%SECT);                                           //si la direccion no es exacta la de un sector, transfiere desde la direccion donde comienza el sector
   output_low(PIN_C4);                                                           //SCF OBC side
   for(int32 i = 0; i < SECT; i++)
   {
      WRITE_DATA_BYTE_OF(address + i, READ_DATA_BYTE_SCF(address + i));
   }
   delay_ms(100);
   output_high(PIN_C4);
   
   int16 DATA = address/SECT;                                                    //maximum 2048, data almacena el nro de sector copiado
   int8 D1 = DATA >> 8;
   int8 D2 = DATA;
   
   SAVE_SAT_LOG(0x21, D1, D2);                                                   //0xE0+16bit sector info
   
   return;
}

void TRANSFER_SECTOR_SCF2SMF(int8 data1,int8 data2,int8 data3,int8 data4)
{
   REFRESH_SECTOR_SMF(data1,data2,data3,data4);                                  //erase 1 sector SMF
   delay_ms(500);
   fprintf(PC,"transfer SCF2SMF:%x%x%x%x\r\n",data1,data2,data3,data4);
   int32 address = make32(data1,data2,data3,data4);
   address = address - (address%SECT);                                           //si la direccion no es exacta la de un sector, transfiere desde la direccion donde comienza el sector
   output_low(PIN_C4);                                                           //SCF OBC side
   output_low(PIN_A5);                                                           //SMF OBC side
   for(int32 i = 0; i < SECT; i++)
   {
      WRITE_DATA_BYTE_SMF(address + i, READ_DATA_BYTE_SCF(address + i));
   }
   delay_ms(100);
   output_high(PIN_C4);
   
   int16 DATA = address/SECT;                                                    //maximum 2048, data almacena el nro de sector copiado
   int8 D1 = DATA >> 8;
   int8 D2 = DATA;
   
   SAVE_SAT_LOG(0x23, D1, D2);                                                   //0xE0+16bit sector info
   
   return;
}

void TRANSFER_SECTOR_SMF2OF(int8 data1,int8 data2,int8 data3,int8 data4)
{
   REFRESH_SECTOR_OF(data1,data2,data3,data4);                                   //erase 1 sector OBC flash
   delay_ms(500);
   fprintf(PC,"transfer SMF2OF:%x%x%x%x\r\n",data1,data2,data3,data4);
   int32 address = make32(data1,data2,data3,data4);
   address = address - (address%SECT);                                           //si la direccion no es exacta la de un sector, transfiere desde la direccion donde comienza el sector
   output_low(PIN_A5);                                                           //SMF OBC side
   for(int32 i = 0; i < SECT; i++)
   {
      WRITE_DATA_BYTE_OF(address + i, READ_DATA_BYTE_SMF(address + i));
   }
   delay_ms(100);
   
   int16 DATA = address/SECT;                                                    //maximum 2048, data almacena el nro de sector copiado
   int8 D1 = DATA >> 8;
   int8 D2 = DATA;
   
   SAVE_SAT_LOG(0x31, D1, D2);                                                   //0xE0+16bit sector info
   
   return;
}

void TRANSFER_SECTOR_SMF2SCF(int8 data1,int8 data2,int8 data3,int8 data4)        //0x32
{
   REFRESH_SECTOR_SCF(data1,data2,data3,data4);                                  //erase 1 sector SCF
   delay_ms(500);
   fprintf(PC,"transfer SMF2SCF:%x%x%x%x\r\n",data1,data2,data3,data4);
   int32 address = make32(data1,data2,data3,data4);
   address = address - (address%SECT);                                           //si la direccion no es exacta la de un sector, transfiere desde la direccion donde comienza el sector
   output_low(PIN_C4);                                                           //SCF OBC side
   output_low(PIN_A5);                                                           //SMF OBC side
   for(int32 i = 0; i < SECT; i++)
   {
      WRITE_DATA_BYTE_SCF(address + i, READ_DATA_BYTE_SMF(address + i));
   }
   delay_ms(100);
   output_high(PIN_C4);                                                          //SCF COM side
   
   int16 DATA = address/SECT;                                                    //maximum 2048, data almacena el nro de sector copiado
   int8 D1 = DATA >> 8;
   int8 D2 = DATA;
   
   SAVE_SAT_LOG(0x32, D1, D2);                                                   //0xE0+16bit sector info
   return;
}

