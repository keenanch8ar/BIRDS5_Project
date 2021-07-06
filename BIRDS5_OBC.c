#include <BIRDS5_OBC.h>

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
   enable_interrupts(INT_rda4);
   fprintf(PC,"Start Operating\r\n");
   output_high(PIN_A5);  //MUX MISSION SIDE
   //output_low(PIN_A5); //MUX MAIN SIDE
   output_high(sel);
   
   while(TRUE)
   {
      
      fprintf(fab,"\r\nHello World\r\n");
      fprintf(fab,"Operating\r\n");
      delay_ms(2500);
      MULT_SPEC_test();
      
   }

}
