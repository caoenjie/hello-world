/*!
 * \file      board-config.h
 *
 * \brief     Board configuration
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
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 *
 * \author    Johannes Bruder ( STACKFORCE )
 */
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

/*!
 * Defines the time required for the TCXO to wakeup [ms].
 */
//#define BOARD_TCXO_WAKEUP_TIME                      5

/*!
 * Board MCU pins definitions
 */
#define RADIO_RESET                                 "/sys/class/gpio/gpio2/value"
//
//#define RADIO_MOSI                                  PA_7
//#define RADIO_MISO                                  PA_6
//#define RADIO_SCLK                                  PB_3

//#define RADIO_NSS                                   PA_15

#define RADIO_DIO_0                                 "/sys/class/gpio/gpio39"
#define RADIO_DIO_1                                 "/sys/class/gpio/gpio36"
#define RADIO_DIO_2                                 "/sys/class/gpio/gpio35"
#define RADIO_DIO_3                                 "/sys/class/gpio/gpio37"
#define RADIO_DIO_5                                 "/sys/class/gpio/gpio38"

#define VALUE_DIO_0									"/sys/class/gpio/gpio39/value"
#define VALUE_DIO_1									"/sys/class/gpio/gpio36/value"
#define VALUE_DIO_2									"/sys/class/gpio/gpio35/value"
#define VALUE_DIO_3									"/sys/class/gpio/gpio37/value"
#define VALUE_DIO_5									"/sys/class/gpio/gpio38/value"

#define OPEN_DIO_0 									"echo 39  > /sys/class/gpio/export"
#define OPEN_DIO_1 									"echo 36  > /sys/class/gpio/export"
#define OPEN_DIO_2 									"echo 35  > /sys/class/gpio/export"
#define OPEN_DIO_3 									"echo 37  > /sys/class/gpio/export"
#define OPEN_DIO_5 									"echo 38  > /sys/class/gpio/export"




#endif // __BOARD_CONFIG_H__
