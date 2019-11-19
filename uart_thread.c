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
#include "my_callback_files/distance_sensor_timer.h"

/*
 *  ======== uartThread ========
 */
void *uartThread(void *arg0)
{
    // turn usb uart
//    sleep(5);

//    dbgUARTVal("in uart thread");

    createUartQueue();

    if (create_DistanceTaskQueue() == CREATE_QUEUE_FAILURE)
    {
//        errorRoutine(CONTROL_QUEUE_CREATE_FAILURE);
//        printf("Queue creation failed.\n");
    }
    else
    {
//        printf("Queue creation succeeded\n");
    }

//    dbgUARTInit();

    // Command variables
    char msg[UART_QUEUE_WIDTH] = "\r\nUart thread opened\r\n\0";
    dbgUARTVal(msg);

    sendMsgToUart("blocking read in uart \r\n\0");
//
//    sleep(2);
//
//
//    init_distanceSensorTimer();

    distance_sensor_timer = Timer_open(DISTANCE_SENSOR_TIMER, &distance_sensor_params);

    if (distance_sensor_timer == NULL) {
        /* Failed to initialized timer */
//         errorRoutine(ENCODER_BUFFER_TIMER_ERROR);
        return TIMER_FAILURE;
    }

     if (Timer_start(distance_sensor_timer) == Timer_STATUS_ERROR) {
         /* Failed to start timer */
 //         errorRoutine(ENCODER_BUFFER_TIMER_ERROR);
         return TIMER_FAILURE;
     }

     distanceCapture = Capture_open(Board_CAPTURE0, &distanceCaptureParams);

      if (distanceCapture == NULL)
      {
          /* Failed to initialize capture */
          while(1) {}
      }

       if (Capture_start(distanceCapture) == Capture_STATUS_ERROR) {
            /* Failed to start capture */
            while (1) {}
        }

    while (1) {
//        sendMsgToUart("blocking read in uart \r\n\0");
//         blocking read from queue
        readMsqFromUartQueue(msg);
//
//        // write to UART
        dbgUARTVal(msg);
    }
}


