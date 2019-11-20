/*
    distance_sensor_timer.c
 */
#include <my_callback_files/distance_sensor_timer.h>

/*
 * This callback sends the time since last ISR call in ms to queue 1
 */
void distanceSensorTimerCallback(Timer_Handle myHandle) {

//    GPIO_toggle(Board_GPIO0);
    static int start = 0;

//    sendMsgToUart("Timer Callback\r\n\0");

//    GPIO_toggle(Board_GPIO0);

   if(start == 0)
   {
       GPIO_toggle(Board_TRIG);
   }
   if (start < 2)
   {
       start++;
   }
   if (start == 2)
   {
       GPIO_toggle(Board_TRIG);
       start = 3;
   }
   if (start >= 3)
   {
       start++;
   }
   if (start == 10000) // do this every 100 ms
   {
       start = 0;
   }

}

int init_distanceSensorTimer() {

     // start timer
//    sendMsgToUart("before timer open \r\n\0");

     distance_sensor_timer = Timer_open(DISTANCE_SENSOR_TIMER, &distance_sensor_params);

     if (distance_sensor_timer == NULL) {
         /* Failed to initialized timer */
         errorRoutine(ENCODER_BUFFER_TIMER_ERROR);
         return TIMER_FAILURE;
     }

     if (Timer_start(distance_sensor_timer) == Timer_STATUS_ERROR) {
         /* Failed to start timer */
         errorRoutine(ENCODER_BUFFER_TIMER_ERROR);
         return TIMER_FAILURE;
     }

//     sendMsgToUart("after start \r\n\0");

     return TIMER_SUCCESS;
}

/* Callback function that displays the interval */
void distanceCaptureCallback(Capture_Handle handle, uint32_t interval)
{

        static int echo_rise = 1;
        static struct timespec current_time, last_time = {0,0};

        if (echo_rise == 1)
        {
//            dbgUARTVal("before clock");
            clock_gettime(CLOCK_REALTIME, &current_time);

            //SEND THIS TO NEW THREAD

//            dbgUARTVal("before first queue");
            captureRiseDistanceTaskQueue(current_time);
//            dbgUARTVal("after first queue");


//            GPIO_toggle(Board_ECHO);
            echo_rise = 0;

        }
        else
        {
            clock_gettime(CLOCK_REALTIME, &last_time);
            //SEND THIS TO NEW THREAD

            captureFallDistanceTaskQueue(last_time);


//            GPIO_toggle(Board_ECHO);
            echo_rise = 1;

        }

    GPIO_toggle(Board_GPIO0);

}


