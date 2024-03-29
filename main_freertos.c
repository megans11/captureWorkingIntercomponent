/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_freertos.c ========
 */
#include <stdint.h>

/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include "FreeRTOS.h"
#include "task.h"

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/Timer.h>

/* Example/Board Header files */
#include "Board.h"
#include "debug.h"
//#include "timer.h"
//#include "pwm.h"

#include "my_driver_files/distance_sensor_driver.h"
#include "my_callback_files/distance_sensor_timer.h"

extern void *mainThread(void *arg0);
extern void *uartThread(void *arg0);
//extern void *findDistanceTaskThread(void *arg0);
extern void *sensorMainThread(void *arg0);
//extern void *armControlThread(void *arg0);
//extern void *servoPositionThread(void *arg0);

/* Stack size in bytes */
#define THREADSTACKSIZE   4096
#define UARTTHREADSIZE 1024

/*
 *  ======== main ========
 */
int main(void)
{
//    pthread_t           thread;
//    pthread_attr_t      attrs;
//    struct sched_param  priParam;
//    int                 retc;
//
//    Board_init();
//
//    SPI_init();
//    UART_init();
//    GPIO_init();
//    Capture_init();
//    Timer_init();
//
//    UART_Params_init(&uartDistanceSensorParams);
//    uartDistanceSensorParams.writeMode = UART_MODE_BLOCKING;
//    uartDistanceSensorParams.writeDataMode = UART_DATA_BINARY;
//    uartDistanceSensorParams.baudRate = 115200;
//    uartDistanceSensorParams.readEcho = UART_ECHO_OFF;
//
//    /* Open UART with header pin selected in sysconfig i.e. UART0 */
//        UART0 = UART_open(UART_USB, &uartParams);
//        if (UART0 == NULL) {
//            /* Error creating UART */
//            while (1);
//        }
//
//        /* Block read since UART is write-only for this application */
//        UART_control(UART0, UART_CMD_RXDISABLE, NULL);
//
//    /* Setting up the Capture driver to detect two rising edges and report
//    * the result in microseconds
////    */
////    Capture_Params_init(&distanceCaptureParams);
////    distanceCaptureParams.mode = Capture_ANY_EDGE;
////    distanceCaptureParams.periodUnit = Capture_PERIOD_US;
////    distanceCaptureParams.callbackFxn = distanceCaptureCallback;
////
////    // init timer
////     Timer_Params_init(&distance_sensor_params);
////     distance_sensor_params.period = DISTANCE_SENSOR_TIMER_PERIOD;
////     distance_sensor_params.periodUnits = Timer_PERIOD_US;
////     distance_sensor_params.timerMode = Timer_CONTINUOUS_CALLBACK;
////     distance_sensor_params.timerCallback = distanceSensorTimerCallback;
//
//    /* Initialize the attributes structure with default values */
//    pthread_attr_init(&attrs);
//
//    /* Set priority, detach state, and stack size attributes */
//    priParam.sched_priority = 1;
//    retc = pthread_attr_setschedparam(&attrs, &priParam);
//    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
//    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
//    if (retc != 0) {
//        /* failed to set attributes */
//        while (1) {}
//    }
//
//#ifdef UART_DEBUGGING
//    retc = pthread_create(&thread, &attrs, uartThread, NULL);
//    if (retc != 0) {
//        /* pthread_create() failed */
//        while (1) {}
//    }
//#endif
//
//    retc = pthread_create(&thread, &attrs, sensorMainThread, NULL);
//    if (retc != 0) {
//        /* pthread_create() failed */
//        while (1) {}
//    }
//
//    retc = pthread_create(&thread, &attrs, mainThread, NULL);
//    if (retc != 0) {
//        /* pthread_create() failed */
//        while (1) {}
//    }
//
//    /* Start the FreeRTOS scheduler */
//    vTaskStartScheduler();
//
//    dbgUARTVal("here");
//
//    return (0);


    pthread_t thread;
    pthread_attr_t pAttrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    /* Call board init functions */
    Board_initGeneral();

    //Init everything here
//    SPI_init();
//    GPIO_init();
//    UART_init();

    SPI_init();
    UART_init();
    GPIO_init();
    Capture_init();
//    dbgUARTInit();
    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.baudRate = 115200;
    uartParams.readEcho = UART_ECHO_OFF;

    /* Open UART with header pin selected in sysconfig i.e. UART0 */
    UART0 = UART_open(UART_USB, &uartParams);
    if (UART0 == NULL) {
        /* Error creating UART */
        while (1);
    }

    /* Block read since UART is write-only for this application */
    UART_control(UART0, UART_CMD_RXDISABLE, NULL);
//
//    /* Configure the LED pin */
    GPIO_setConfig(Board_TRIG, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_ECHO, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_GPIO0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
//    GPIO_setConfig(Board_GPIO1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn off user LED */
    GPIO_write(Board_TRIG, 0);
    GPIO_write(Board_ECHO, 0);
    GPIO_write(Board_GPIO0, 0);
//    GPIO_write(Board_GPIO1, 0);

    Capture_Params_init(&distanceCaptureParams);
    distanceCaptureParams.mode = Capture_ANY_EDGE;
    distanceCaptureParams.periodUnit = Capture_PERIOD_US;
    distanceCaptureParams.callbackFxn = distanceCaptureCallback;

    Timer_init();
    Timer_Params_init(&distance_sensor_params);
    distance_sensor_params.period = DISTANCE_SENSOR_TIMER_PERIOD;
    distance_sensor_params.periodUnits = Timer_PERIOD_US;
    distance_sensor_params.timerMode = Timer_CONTINUOUS_CALLBACK;
    distance_sensor_params.timerCallback = distanceSensorTimerCallback;

    /* Set priority and stack size attributes */
    pthread_attr_init(&pAttrs);
    priParam.sched_priority = 2;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&pAttrs, detachState);
    if(retc != 0)
    {
        /* pthread_attr_setdetachstate() failed */
        while(1)
        {
            ;
        }
    }

    pthread_attr_setschedparam(&pAttrs, &priParam);

    retc |= pthread_attr_setstacksize(&pAttrs, THREADSTACKSIZE);
    if(retc != 0)
    {
        /* pthread_attr_setstacksize() failed */
        while(1)
        {
            ;
        }
    }

    retc = pthread_create(&thread, &pAttrs, mainThread, NULL);
    if(retc != 0)
    {
        /* pthread_create() failed */
        while(1)
        {
            ;
        }
    }

//pthread_t Fthread;
//   pthread_attr_t FAttrs;
//   struct sched_param FpriParam;
//
//   /* Set priority and stack size attributes */
//   pthread_attr_init(&FAttrs);
//   FpriParam.sched_priority = 2;
//
//   detachState = PTHREAD_CREATE_DETACHED;
//   retc = pthread_attr_setdetachstate(&FAttrs, detachState);
//   if(retc != 0)
//   {
//       /* pthread_attr_setdetachstate() failed */
//       while(1)
//       {
//           ;
//       }
//   }
//
//   pthread_attr_setschedparam(&FAttrs, &FpriParam);
//
//   retc |= pthread_attr_setstacksize(&FAttrs, 2048);
//   if(retc != 0)
//   {
//       /* pthread_attr_setstacksize() failed */
//       while(1)
//       {
//           ;
//       }
//   }
//
//   retc = pthread_create(&Fthread, &FAttrs, findDistanceTaskThread, NULL);
//   if(retc != 0)
//   {
//       /* pthread_create() failed */
//       while(1)
//       {
//           ;
//       }
//   }


#ifdef UART_DEBUGGING
    pthread_t uthread;
    pthread_attr_t uAttrs;
    struct sched_param upriParam;

    /* Set priority and stack size attributes */
    pthread_attr_init(&uAttrs);
    upriParam.sched_priority = 1;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&uAttrs, detachState);
    if(retc != 0)
    {
        /* pthread_attr_setdetachstate() failed */
        while(1)
        {
            ;
        }
    }

    pthread_attr_setschedparam(&uAttrs, &upriParam);

    retc |= pthread_attr_setstacksize(&uAttrs, UARTTHREADSIZE);
    if(retc != 0)
    {
        /* pthread_attr_setstacksize() failed */
        while(1)
        {
            ;
        }
    }

    retc = pthread_create(&uthread, &uAttrs, uartThread, NULL);
    if(retc != 0)
    {
        /* pthread_create() failed */
        while(1)
        {
            ;
        }
    }
#endif



//    pthread_t Fthread;
//    pthread_attr_t FAttrs;
//    struct sched_param FpriParam;
//
//    /* Set priority and stack size attributes */
//    pthread_attr_init(&FAttrs);
//    FpriParam.sched_priority = 4;
//
//    detachState = PTHREAD_CREATE_DETACHED;
//    retc = pthread_attr_setdetachstate(&FAttrs, detachState);
//    if(retc != 0)
//    {
//        /* pthread_attr_setdetachstate() failed */
//        while(1)
//        {
//            ;
//        }
//    }
//
//    pthread_attr_setschedparam(&FAttrs, &FpriParam);
//
//    retc |= pthread_attr_setstacksize(&FAttrs, UARTTHREADSIZE);
//    if(retc != 0)
//    {
//        /* pthread_attr_setstacksize() failed */
//        while(1)
//        {
//            ;
//        }
//    }
//
//    retc = pthread_create(&Fthread, &FAttrs, findDistanceTaskThread, NULL);
//    if(retc != 0)
//    {
//        /* pthread_create() failed */
//        while(1)
//        {
//            ;
//        }
//    }


    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();



    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask,
                                   char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}

void vApplicationTickHook(void)
{
    /*
     * This function will be called by each tick interrupt if
     * configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
     * added here, but the tick hook is called from an interrupt context, so
     * code must not attempt to block, and only the interrupt safe FreeRTOS API
     * functions can be used (those that end in FromISR()).
     */
}

void vPreSleepProcessing(uint32_t ulExpectedIdleTime)
{
}

//*****************************************************************************
//
//! \brief Application defined idle task hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void
vApplicationIdleHook(void)
{
    /* Handle Idle Hook for Profiling, Power Management etc */
}

//*****************************************************************************
//
//! \brief  Overwrite the GCC _sbrk function which check the heap limit related
//!         to the stack pointer.
//!         In case of freertos this checking will fail.
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
#if defined (__GNUC__)
void * _sbrk(uint32_t delta)
{
    extern char _end;     /* Defined by the linker */
    extern char __HeapLimit;
    static char *heap_end;
    static char *heap_limit;
    char *prev_heap_end;

    if(heap_end == 0)
    {
        heap_end = &_end;
        heap_limit = &__HeapLimit;
    }

    prev_heap_end = heap_end;
    if(prev_heap_end + delta > heap_limit)
    {
        return((void *) -1L);
    }
    heap_end += delta;
    return((void *) prev_heap_end);
}

#endif
