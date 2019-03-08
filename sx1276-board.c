/*!
 * \file      sx1276-board.c
 *
 * \brief     Target board SX1276 driver implementation
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
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "utilities.h"
#include "board-config.h"
#include "radio.h"
#include "sx1276-board.h"

/*!
 * \brief Gets the board PA selection configuration
 *
 * \param [IN] power Selects the right PA according to the wanted power.
 * \retval PaSelect RegPaConfig PaSelect value
 */
static uint8_t SX1276GetPaSelect( int8_t power );

/*!
 * Flag used to set the RF switch control pins in low power mode when the radio is not active.
 */
static bool RadioIsActive = false;

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
    SX1276Init,
    SX1276GetStatus,
    SX1276SetModem,
    SX1276SetChannel,
    SX1276IsChannelFree,
    SX1276Random,
    SX1276SetRxConfig,
    SX1276SetTxConfig,
    SX1276CheckRfFrequency,
    SX1276GetTimeOnAir,
    SX1276Send,
    SX1276SetSleep,
    SX1276SetStby,
    SX1276SetRx,
    SX1276StartCad,
    SX1276SetTxContinuousWave,
    SX1276ReadRssi,
    SX1276Write,
    SX1276Read,
    SX1276WriteBuffer,
    SX1276ReadBuffer,
    SX1276SetMaxPayloadLength,
    SX1276SetPublicNetwork,
    SX1276GetWakeupTime,
    NULL, // void ( *IrqProcess )( void )
    NULL, // void ( *RxBoosted )( uint32_t timeout ) - SX126x Only
    NULL, // void ( *SetRxDutyCycle )( uint32_t rxTime, uint32_t sleepTime ) - SX126x Only
};

/*!
 * TCXO power control pin
 */
//Gpio_t TcxoPower;

/*!
 * Antenna switch GPIO pins objects
 */
//Gpio_t AntSwitchRx;
//Gpio_t AntSwitchTxBoost;
//Gpio_t AntSwitchTxRfo;

/*!
 * Debug GPIO pins objects
 */


void SX1276IoInit( void )
{

	if(access(RADIO_DIO_0,F_OK)) system(OPEN_DIO_0); //dio_0
	if(access(RADIO_DIO_1,F_OK)) system(OPEN_DIO_1); //dio_1
	if(access(RADIO_DIO_2,F_OK)) system(OPEN_DIO_2); //dio_2
	if(access(RADIO_DIO_3,F_OK)) system(OPEN_DIO_3); //dio_3
	if(access(RADIO_DIO_5,F_OK)) system(OPEN_DIO_5); //dio_5

}
//
void SX1276IoIrqInit( DioIrqHandler **irqHandlers )
{


}

//void SX1276IoDeInit( void )
//{
//    GpioInit( &SX1276.Spi.Nss, RADIO_NSS, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 1 );
//
//    GpioInit( &SX1276.DIO0, RADIO_DIO_0, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX1276.DIO1, RADIO_DIO_1, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX1276.DIO2, RADIO_DIO_2, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX1276.DIO3, RADIO_DIO_3, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX1276.DIO4, RADIO_DIO_4, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &SX1276.DIO5, RADIO_DIO_5, PIN_INPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//}

/*!
 * \brief Enables/disables the TCXO if available on board design.
 *
 * \param [IN] state TCXO enabled when true and disabled when false.
 */
//static void SX1276SetBoardTcxo( uint8_t state )
//{
//    if( state == true )
//    {
//        // Power ON the TCXO
//        GpioWrite( &TcxoPower, 1 );
//        DelayMs( BOARD_TCXO_WAKEUP_TIME );
//    }
//    else
//    {
//        // Power OFF the TCXO
//        GpioWrite( &TcxoPower, 0 );
//    }
//}

//uint32_t SX1276GetBoardTcxoWakeupTime( void )
//{
//    return BOARD_TCXO_WAKEUP_TIME;
//}

void SX1276Reset( void )
{
	system("echo 2 > /sys/class/gpio/export");
	usleep(1000);
	system("echo out > /sys/class/gpio/gpio2/direction");
	usleep(1000);
	system("echo 0 > /sys/class/gpio/gpio2/value");
	usleep(2000);
	system("echo 1 > /sys/class/gpio/gpio2/value");
	usleep(6000);
	system("echo in > /sys/class/gpio/gpio2/direction");
	usleep(1000);
	system("echo 2 > /sys/class/gpio/unexport");
	usleep(1000);
}

void SX1276SetRfTxPower( int8_t power )
{
    uint8_t paConfig = 0;
    uint8_t paDac = 0;

    paConfig = SX1276Read( REG_PACONFIG );
    paDac = SX1276Read( REG_PADAC );

    paConfig = ( paConfig & RF_PACONFIG_PASELECT_MASK ) | SX1276GetPaSelect( power );

    if( ( paConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
    {
        if( power > 17 )
        {
            paDac = ( paDac & RF_PADAC_20DBM_MASK ) | RF_PADAC_20DBM_ON;
        }
        else
        {
            paDac = ( paDac & RF_PADAC_20DBM_MASK ) | RF_PADAC_20DBM_OFF;
        }
        if( ( paDac & RF_PADAC_20DBM_ON ) == RF_PADAC_20DBM_ON )
        {
            if( power < 5 )
            {
                power = 5;
            }
            if( power > 20 )
            {
                power = 20;
            }
            paConfig = ( paConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 5 ) & 0x0F );
        }
        else
        {
            if( power < 2 )
            {
                power = 2;
            }
            if( power > 17 )
            {
                power = 17;
            }
            paConfig = ( paConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 2 ) & 0x0F );
        }
    }
    else
    {
        if( power > 0 )
        {
            if( power > 15 )
            {
                power = 15;
            }
            paConfig = ( paConfig & RF_PACONFIG_MAX_POWER_MASK & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( 7 << 4 ) | ( power );
        }
        else
        {
            if( power < -4 )
            {
                power = -4;
            }
            paConfig = ( paConfig & RF_PACONFIG_MAX_POWER_MASK & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( 0 << 4 ) | ( power + 4 );
        }
    }
    SX1276Write( REG_PACONFIG, paConfig );
    SX1276Write( REG_PADAC, paDac );
}

static uint8_t SX1276GetPaSelect( int8_t power )
{
    if( power > 14 )
    {
        return RF_PACONFIG_PASELECT_PABOOST;
    }
    else
    {
        return RF_PACONFIG_PASELECT_RFO;
    }
}

void SX1276SetAntSwLowPower( bool status )
{
    if( RadioIsActive != status )
    {
        RadioIsActive = status;

        if( status == false )
        {
//            SX1276SetBoardTcxo( true );
//            SX1276AntSwInit( );
        }
        else
        {
//            SX1276SetBoardTcxo( false );
//            SX1276AntSwDeInit( );
        }
    }
}

//void SX1276AntSwInit( void )
//{
//    GpioInit( &AntSwitchRx, RADIO_ANT_SWITCH_RX, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &AntSwitchTxBoost, RADIO_ANT_SWITCH_TX_BOOST, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//    GpioInit( &AntSwitchTxRfo, RADIO_ANT_SWITCH_TX_RFO, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
//}
//
//void SX1276AntSwDeInit( void )
//{
//    GpioInit( &AntSwitchRx, RADIO_ANT_SWITCH_RX, PIN_ANALOGIC, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
//    GpioInit( &AntSwitchTxBoost, RADIO_ANT_SWITCH_TX_BOOST, PIN_ANALOGIC, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
//    GpioInit( &AntSwitchTxRfo, RADIO_ANT_SWITCH_TX_RFO, PIN_ANALOGIC, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
//}

//void SX1276SetAntSw( uint8_t opMode )
//{
//    uint8_t paConfig =  SX1276Read( REG_PACONFIG );
//    switch( opMode )
//    {
//    case RFLR_OPMODE_TRANSMITTER:
//        if( ( paConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
//        {
//            GpioWrite( &AntSwitchTxBoost, 1 );
//        }
//        else
//        {
//            GpioWrite( &AntSwitchTxRfo, 1 );
//        }
//        break;
//    case RFLR_OPMODE_RECEIVER:
//    case RFLR_OPMODE_RECEIVER_SINGLE:
//    case RFLR_OPMODE_CAD:
//    default:
//        GpioWrite( &AntSwitchRx, 1 );
//        break;
//    }
//}

bool SX1276CheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}

#if defined( USE_RADIO_DEBUG )
void SX1276DbgPinTxWrite( uint8_t state )
{
    GpioWrite( &DbgPinTx, state );
}

void SX1276DbgPinRxWrite( uint8_t state )
{
    GpioWrite( &DbgPinRx, state );
}
#endif
/*
 * ------GPIO function------
 *
 */
int SX1276ReadDio0(void)
{
	int fd ;
	fd = open(VALUE_DIO_0,O_RDONLY);
	if(fd<0)
	{
		perror("open Dio0:");
		return 0;
	}
	char buf[1];
	read(fd,buf,1);
	if(buf[0] == '1')
	{
		close(fd);
		return 1;
	}
	else{
		close(fd);
		return 0;
	}
}
int SX1276ReadDio1(void)
{
	int fd ;
	fd = open(VALUE_DIO_1,O_RDONLY);
	if(fd<0)
	{
		perror("open Dio1:");
		return 0;
	}
	char buf[1];
	read(fd,buf,1);
	if(buf[0] == '1')
	{
		close(fd);
		return 1;
	}
	else{
		close(fd);
		return 0;
	}
}
int SX1276ReadDio2(void)
{
	int fd ;
	fd = open(VALUE_DIO_2,O_RDONLY);
	if(fd<0)
	{
		perror("open Dio2:");
		return 0;
	}
	char buf[1];
	read(fd,buf,1);
	if(buf[0] == '1')
	{
		close(fd);
		return 1;
	}
	else{
		close(fd);
		return 0;
	}
}
int SX1276ReadDio3(void)
{
	int fd ;
	fd = open(VALUE_DIO_3,O_RDONLY);
	if(fd<0)
	{
		perror("open Dio3:");
		return 0;
	}
	char buf[1];
	read(fd,buf,1);
	if(buf[0] == '1')
	{
		close(fd);
		return 1;
	}
	else{
		close(fd);
		return 0;
	}
}
int SX1276ReadDio5(void)
{
	int fd ;
	fd = open(VALUE_DIO_5,O_RDONLY);
	if(fd<0)
	{
		perror("open Dio5:");
		return 0;
	}
	char buf[1];
	read(fd,buf,1);
	if(buf[0] == '1')
	{
		close(fd);
		return 1;
	}
	else{
		close(fd);
		return 0;
	}
}







