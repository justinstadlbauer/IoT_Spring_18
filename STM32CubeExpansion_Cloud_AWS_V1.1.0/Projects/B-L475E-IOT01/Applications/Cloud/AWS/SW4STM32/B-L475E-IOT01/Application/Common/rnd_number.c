/**
  ******************************************************************************
  * @file    rnd_number.c
  * @author  Justin Stadlbauer
  * @version V1.1.0
  * @date    19-February-2018
  * @brief   Publishes a random number to AWS
  ******************************************************************************
  */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rnd_number.h"

#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"
#include "msg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  fill the payload with a random number
  * @param  none
  * @param PayloadBuffer is the char pointer for the Payload buffer to be filled
  * @param PayloadSize size of the above buffer
  * @retval 0 in case of success
  *         -1 in case of failure
  */
int PrepareMqttPayload1(char * PayloadBuffer, int PayloadSize, char * deviceID)
{
  char * Buff = PayloadBuffer;
  int BuffSize = PayloadSize;
  int snprintfreturn = 0;

 #ifdef BLUEMIX
    snprintfreturn = snprintf( Buff, BuffSize, "{\"d\":{"
             "\"Random Number\": %d"
               "}}",
             rand() );

 #else
  if (deviceID != NULL)
  {
    snprintfreturn = snprintf( Buff, BuffSize, "{\"deviceId\":\"%s\","
 	       "   \"Random Number\": %d\n"
 	       "  }\n }\n}",
             deviceID,
             rand() );
  }
  else
  {
  snprintfreturn = snprintf( Buff, BuffSize, "{\n \"state\": {\n  \"reported\": {\n"
	       "   \"Random Number\": %d\n"
	       "  }\n }\n}",
	           (rand()%1000) );
  }
 #endif
  /* Check total size to be less than buffer size
            * if the return is >=0 and <n, then
            * the entire string was successfully formatted; if the return is
            * >=n, the string was truncated (but there is still a null char
            * at the end of what was written); if the return is <0, there was
            * an error.
            */
  if (snprintfreturn >= 0 && snprintfreturn < PayloadSize)
  {
      return 0;
  }
  else if(snprintfreturn >= PayloadSize)
  {
      msg_error("Data Pack truncated\n");
      return 0;
  }
  else
  {
      msg_error("Data Pack Error\n");
      return -1;
  }
}
