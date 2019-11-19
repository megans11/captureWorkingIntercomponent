/*
 * uart_thread.c
 *
 *  Created on: Oct 9, 2019
 *      Author: lukeb
 */

#include "uart_thread.h"
//#include <pthread.h>
//extern void *mainThread(void *arg0);
//#define THREADSTACKSIZE   2048

/*
 *  ======== uartThread ========
 */
void *uartThread(void *arg0)
{
    // turn usb uart
//    sleep(5);

    createUartQueue();
    dbgUARTInit();

    // Command variables
    char msg[UART_QUEUE_WIDTH] = "\r\nUart thread opened\r\n\0";
    dbgUARTVal(msg);
//
//    sleep(2);
//
//
//    init_distanceSensorTimer();

    while (1) {
//        sendMsgToUart("blocking read in uart \r\n\0");
        // blocking read from queue
        readMsqFromUartQueue(msg);

        // write to UART
        dbgUARTVal(msg);
    }
}


