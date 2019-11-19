/*
 * distance_sensor_driver.c
 *
 */

#include <my_driver_files/distance_sensor_driver.h>
#include <my_callback_files/distance_sensor_timer.h>

/*
 * Initialize GPIO used for sensor values
 */
int init_distance_sensor_GPIO()
{

//    /* Configure the LED pin */
//    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_TRIG, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Board_ECHO, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn off user LED */
//    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_OFF);
    GPIO_write(Board_TRIG, 0);
    GPIO_write(Board_ECHO, 0);

    return 1;
}


/*
 * Initialize Capture used for sensor values
 */
int init_distance_sensor_capture()
{

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

    return (NULL);
}

/*
 * Initialize UART used for sensor values
 */
int init_distance_sensor_UART() {

    /* Open UART with header pin selected in sysconfig i.e. UART0 */
//    distanceSensorUART = UART_open(DISTANCE_SENSOR_UART, &uartDistanceSensorParams);
//    if (distanceSensorUART == NULL) {
//        /* Error creating UART */
//        return UART_INIT_FAIL;
//    }
//
//    /* Block read since UART is write-only for this application */
//    UART_control(distanceSensorUART, UART_CMD_RXDISABLE, NULL);
//
//    // send baud rate init message
//    char cmd = INIT_UART_MSG;
//
////
////    const char  echoPrompt[] = "Echoing characters:\r\n";
////    UART_write(distanceSensorUART, echoPrompt, sizeof(echoPrompt));
//
//    // write message
//    if (UART_write(distanceSensorUART, (void *) &cmd, sizeof(cmd)) == UART_STATUS_ERROR)
//        return UART_INIT_FAIL;
//    return UART_INIT_SUCCESS;
}



