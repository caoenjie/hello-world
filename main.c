/*!
 * \file      main.c
 *
 * \brief     LoRaMac classA device implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */

/*! \file classA/B-L072Z-LRWAN1/main.c */

#include <stdio.h>
#include "utilities.h"
#include "board.h"
#include "radio.h"


/*!
 * Prints the provided buffer in HEX
 * 
 * \param buffer Buffer to be printed
 * \param size   Buffer size to be printed
 */
void PrintHexBuffer( uint8_t *buffer, uint8_t size )
{
    uint8_t newline = 0;

    for( uint8_t i = 0; i < size; i++ )
    {
        if( newline != 0 )
        {
            printf( "\r\n" );
            newline = 0;
        }

        printf( "%02X ", buffer[i] );

        if( ( ( i + 1 ) % 16 ) == 0 )
        {
            newline = 1;
        }
    }
    printf( "\r\n" );
}


/**
 * Main application entry point.
 */

int main()
{

	BoardInitMcu();
	RadioEvents_t *event;
	Radio.Init(event);
	srand1( Radio.Random( ) );
    Radio.SetPublicNetwork( true );
    Radio.Sleep( );

    int8_t txPower = 20;
    uint32_t freq = 502300000;

    /*
     * example for send
     */
//    while(1)
//    {
//		Radio.SetChannel( freq );
//	   //                                          BW SF 4/5            crc         iq
//		Radio.SetTxConfig( MODEM_LORA, txPower, 0, 0, 7, 1, 8, false, true, 0, 0, true, 3000  );
//
//		Radio.SetMaxPayloadLength( MODEM_LORA, sizeof("hello,world!") );
//
//		Radio.Send( "hello,world!", sizeof("hello,world!") );
//		if(SX1276ReadDio0())
//		{
//			SX1276OnDio0Irq(NULL);
//
//		}
//		sleep(1);
//    }

    /*
     * example for receive
     */

    uint8_t maxPayload = 128;
    int8_t phyDr = 7;
    uint32_t frequency = 502300000;
//	MacCtx.RxWindow1Config.Channel = MacCtx.NvmCtx->Channel;
//	MacCtx.RxWindow1Config.DrOffset = MacCtx.NvmCtx->MacParams.Rx1DrOffset;
//	MacCtx.RxWindow1Config.DownlinkDwellTime = MacCtx.NvmCtx->MacParams.DownlinkDwellTime;
//	MacCtx.RxWindow1Config.RepeaterSupport = MacCtx.NvmCtx->RepeaterSupport;
//	MacCtx.RxWindow1Config.RxContinuous = false;
//	MacCtx.RxWindow1Config.RxSlot = MacCtx.RxSlot;
    Radio.Sleep( );
    if( Radio.GetStatus( ) != RF_IDLE )
    {
    	printf("status = %d\n",Radio.GetStatus( ));
        return false;
    }
    printf("radio status is idle\n");
    Radio.SetChannel( frequency );
//    printf("set channel !\n");
    // Radio configuration         BW  sf   4/5
    Radio.SetRxConfig( MODEM_LORA, 0, phyDr, 1, 0, 8, 8, false, 0, true, 0, 0, true, true );

    Radio.SetMaxPayloadLength( MODEM_LORA, maxPayload );

    printf("start to receive LoRa data.....\n");
    while(1)
    {

	    Radio.Rx( 0 );
		if(SX1276ReadDio0())
		{
			printf("dio0 is high!\n");
			SX1276OnDio0Irq(NULL);

		}

    }

}

