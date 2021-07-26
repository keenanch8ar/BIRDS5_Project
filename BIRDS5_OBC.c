#include <BIRDS5_OBC.h>

#int_timer0                                                                      //Interrupcion del TMR0
void intval()
{   
   static int32 counter = 0;                                                     //la variable se activa como global, mantiene su valor al entrar y salir de la funcion
   counter++;                                                                    //se incrementa cada 16.384ms
   if (counter > 60)                                                             //los contadores dentro del if se incrementan aprox cada 1s
   {
      counter = 0;
      FAB_FLAG++;                                                             //contador de tiempo en segundos, a los 90 segundos pide datos al FAB
      RESERVE_SEC_FLAG++;                                                     //contador de tiempo en segundos (used in Table)
      currenttime++;                                                          //contador de tiempo en segundos (Reset Function)
      //fprintf(PC,"currenttime:%ld \r\n", currenttime);                        //Check TMR0 operation
      //fprintf(PC,"Done\r\n");
      
      if(RESERVE_SEC_FLAG > 59)                                                  //los contadores dentro del if se incrementan cada 60s
      {
         RESERVE_SEC_FLAG = 0;
         RESERVE_MIN_FLAG++;                                                     //contador de tiempo en minutos (used in Table)
      }
   }
}

#INT_rda                                                                         //PC Interrupt, RS232 receive data available in buffer 1
void UART1_RXD(void)
{
   CMD_FROM_PC[PC_DATA] = fgetc(PC);                                             //carga el array CMD_FROM_PC[] con los datos enviados por PC
   PC_DATA = ((PC_DATA + 1) % 9);                                                //cuando se obtenga el dato en la posicion 6, PC_DATA=0
}

#INT_rda2 HIGH                                                                   //COM Interrupt, RS232 receive data available in buffer 2
void UART2_RXD(void)
{
   //SerialDataReceive();  //get 32 byte
   //COM_ONEBYTE_COMMAND = fgetc(COM);
   in_bffr_main[COM_DATA] = fgetc(COM);                                          //carga el array in_bffr_main[] con los datos enviados por COM PIC
   fprintf(PC,"I receive from COM PIC: %x \r\n", in_bffr_main[COM_DATA]);
   COM_DATA = ((COM_DATA + 1) % 16);                                             //cuando se obtenga el dato en la posicion 16, COM_DATA=0
} 

#INT_rda4                                                                        //Reset PIC Interrupt, RS232 receive data available in buffer 4
void UART4_RXD(void)
{
   reset_bffr[RESET_DATA] = fgetc(reset);                                        //loads the reset_bffr array with the data sent by the Reset PIC (carga el array reset_bffr con los datos enviados por el Reset PIC)
   RESET_DATA = ((RESET_DATA + 1) % RESET_size);                                 //when the data is obtained in position 11 RESET_DATA = 0 (cuando se obtenga el dato en la posicion 11 RESET_DATA=0)
   if(reset_bffr[0] == 0xaa)                                                     //get ready for reset satellite
   {
      reset_flag = 1;                                                            //raise flag to reset (pone a alto bandera para reseteo)
      RESET_DATA = 0;                                                            //position indicator within reset_data vector (indicador de posicion dentro del vector reset_data)
   }
   
} 

void main()
{

   //settings();
   set_tris_b(0b11010000);
   enable_interrupts(global);
   enable_interrupts(INT_rda);
   enable_interrupts(INT_rda2);
   enable_interrupts(INT_rda4);
   
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_256 | RTCC_8_BIT);                     //Config TMR0, internal clock, prescaler 256, clock/counter 8bit
   set_timer0(0);   //this sets timer0 register to 0
   enable_interrupts(INT_TIMER0);
   
   fprintf(PC,"Start Operating\r\n");
   fprintf(fab,"Start Operating\r\n");
   output_high(PIN_A5);  //MUX MISSION SIDE
   //output_low(PIN_A5); //MUX MAIN SIDE
   output_high(sel);

   while(TRUE)
   {

      //PINO_Test();

      //MULT_SPEC_Test();  
      
      if(CMD_FROM_PC[0])
      {
         fprintf(PC,"COMMAND RECEIVED FROM PC: ");
         for(int m = 0; m < 9; m++)
         {
            fprintf(PC,"%x",CMD_FROM_PC[m]);
         }
         fprintf(PC,"\r\n");
         //0000 0000 to 0001 FFFF is MAIN PIC/MB
         
         BYTE command_ID = CMD_FROM_PC[0];
         command_ID &= 0xF0;
         fprintf(PC,"%x",command_ID);
         fprintf(PC,"\r\n");
         
         if(command_ID == 0x00 || command_ID == 0x10)
         {
            fprintf(PC,"Main PIC or MB Command only\r\n");
            //execute MP or MB commands
         }
         
         if(command_ID == 0x20 || command_ID == 0x30)
         {
            fprintf(PC,"MULT-SPEC Command\r\n");
            MULT_SPEC_Test();
         }
         
         if(command_ID == 0x90)
         {
            fprintf(PC,"PINO Command\r\n");
            //PINO_test();
         }

         DELETE_CMD_FROM_PC();                                                   //clear CMD_FROM_PC[] array
         Delete_Buffer();                                                        //clear in_bffr_main[] array
         CMD_FROM_PC[1] = 0;
         COM_DATA = 0;                                                           //clear COM correct receiving data flag
         PC_DATA = 0;                                                            //clear PC correct receiving data flag
      }
      
      delay_ms(400); 
   }

}
