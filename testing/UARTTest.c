#include <BIRDS5_OBC.h>

void main()
{

   fprintf(PC,"IMG Start operation\r\n");
   fputc(0x70,IMGCLS);                        //send command handshake to IMGCLS
   CHECK_ACK_FROM_IMGCLS_10k()                //check for handshake respose ack for 10s
   
   if(IMGCLS_ACK == 0x61)                                                           //IMGCLS ACK
   {
      IMGCLS_ACK = 0;
      fprintf(PC,"Handshake ACK received from IMGCLS\r\n\r\n");
      fputc(0x70,IMGCLS);															//send mission mode command
      CHECK_ACK_FROM_IMGCLS_10k();                                                  //check for mission mode response ack for 10s
	  
	  if(IMGCLS_ACK == 0x71)
	  {
		  IMGCLS_ACK = 0;
		  fprintf(PC,"Mode ACK received from IMGCLS\r\n\r\n");
		  CHECK_ACK_FROM_IMGCLS_10k();                                              //check for mission mode completion response ack for 10s
		  
		  if(IMGCLS_ACK == 0x72)
		  {
			  IMGCLS_ACK = 0;
			  fprintf(PC,"Finished ACK received from IMGCLS\r\n\r\n")
		  }
		  
	  }
   }
}