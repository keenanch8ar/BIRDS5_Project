#include <18F67J94.h>
#FUSES NOWDT, NOBROWNOUT, NOPROTECT
#use delay(crystal=16Mhz, clock=16Mhz)
#include<PIC18F67J94_registers.h>

#device HIGH_INTS=TRUE

//-------------UART and SPI Setting--------------------------------------------------------------------------------------

#pin_select TX1=PIN_C6                                                           //TX PIN EXT +/- 6v
#pin_select RX1=PIN_C7                                                           //RX PIN EXT +/- 6V
#use rs232(baud=115200,parity=N,UART1,bits=8,stream=pc)                      // MAIN to Ext +/- 6v

#pin_select TX2=PIN_D3
#pin_select RX2=PIN_D2
#use rs232(baud=9600,parity=N,UART2,bits=8,stream=com,ERRORS)                           //UART Main to COM PIC

#pin_select TX4=PIN_E3  
#pin_select RX4=PIN_F2
#use rs232(baud=9600,parity=N,UART4,bits=8,stream=reset,ERRORS)                         //UART MAIN to RESET PIC

#pin_select TX3=PIN_E5  
#pin_select RX3=PIN_E4
#use rs232(baud=9600,parity=N,UART3,bits=8,stream=fab,ERRORS)                           //UART MAIN to FAB PIC


#use rs232(baud=9600,parity=N,xmit=pin_G0,rcv=pin_G1,bits=8,stream=DC,ERRORS)           //UART MAIN to MB DCM


#use spi(MASTER, CLK=PIN_E1, DI=PIN_E0, DO=PIN_E6,  BAUD=1000000, BITS=8, STREAM=MAIN_FM, MODE=0)        //MAIN flash memory port
#use spi(MASTER, CLK=PIN_B2, DI=PIN_B5, DO=PIN_B4,  BAUD=1000000, BITS=8, STREAM=COM_FM, MODE=0)         //COM flash memory port
#use spi(MASTER, CLK=PIN_A3, DI=PIN_A0, DO=PIN_A1,  BAUD=1000000, BITS=8, STREAM=MISSION_FM, MODE=0)     //MISSION flash memory port

//--------------Pin Assignment---------------------------------------------------------------------------------------------
#define sel PIN_D6
#define hvs PIN_D7
#define PINO_POWER PIN_D5


//-------------mainpic_function.c-----------------------------------------------------------------------------------------

void Check_UART();
void waiting(int32 counterval);

/////////////MAIN PIC Buffer/////////////
void SerialDataReceive();                                                        //get buffer data one by one
void Delete_Buffer();                                                            //get buffer data one by one
void SAVE_SAT_LOG(int8 data, int8 data2, int8 data3);
void RESET_SATELLITE();
void MAIN_MB_CMD();


/////////////ANTENNA DEPLOYMENT/////////////
void BC_SETUP();
void BC_READ_TO_PC();
void CHECK_BC_TEMP();
void Turn_ON_BC();
void Turn_OFF_BC();
void BC_OPERATION();
void CLEAR_BC_FLAG();
void MAKE_BC_FLAG_1();
void MAKE_BC_FLAG_2();
void MAKE_BC_FLAG_3();
void MAKE_BC_FLAG_4();

/////////////CAM MISSION/////////////
void Turn_On_CAM();
void Turn_Off_CAM();

/////////////RESET SENSOR collection/////////////
void COLLECT_RESET_DATA();

/////////////MBP Data Collection MISSION/////////////
void Turn_On_MBP();
void Turn_Off_MBP();

/////////////ADCS MISSION/////////////
void ADCS_Test();

/////////////Testing Stuff/////////////
void Flash_Memory_Access();
void PINO_test();
void GET_RESET_DATA();
void MULT_SPEC_Test();
void GET_TIME();
void IMGCLS_test();
#include <mainpic_function.h>
