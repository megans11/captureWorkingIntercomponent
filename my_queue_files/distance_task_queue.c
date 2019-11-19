/*
 * distanceTaskQueue.c
 *
 */

#include <my_queue_files/distance_task_queue.h>
#include <my_driver_files/distance_sensor_driver.h>


/**
 * Wrapper for createQueue to create predefined queue1
 */
int create_DistanceTaskQueue(){
    return createQueue(&distanceTaskQueue);
}

int timerExpired_DistanceTaskQueue(unsigned long int time_elapsed) {

    message_type msg;
    msg[MSG_TYPE] = MSG_TYPE_TIMER;
    msg[MSG_DATA] = time_elapsed;

    return sendMsgToQueue_ISR(distanceTaskQueue, &msg);
}

int captureRiseDistanceTaskQueue(struct timespec currentTime)
{

    long riseSeconds = currentTime.tv_sec;
    long riseNanoseconds = currentTime.tv_nsec;
    long riseTime = 0;

//    char buffer2[60];
//    snprintf(buffer2, sizeof buffer2, "nano %ld\n", riseNanoseconds);
//    UART_write(distanceSensorUART, buffer2, sizeof(buffer2));

    riseTime = (riseSeconds * 1000000000) + riseNanoseconds;

    message_type msg;
    msg[MSG_TYPE] = MSG_TYPE_RISE;
//    msg[MSG_DATA] = time_elapsed;
    msg[MSG_DATA] = riseTime;

    return sendMsgToQueue_ISR(distanceTaskQueue, &msg);
}

int captureFallDistanceTaskQueue(struct timespec lastTime)
{
    long fallSeconds = lastTime.tv_sec;
    long fallNanoseconds = lastTime.tv_nsec;
    long fallTime = 0;

//    fallSeconds = lastTime.tv_sec;
//    fallNanoseconds = lastTime.tv_nsec;
    fallTime = (fallSeconds * 1000000000) + fallNanoseconds;

    message_type msg;
    msg[MSG_TYPE] = MSG_TYPE_FALL;
//    msg[MSG_DATA] = time_elapsed;
    msg[MSG_DATA] = fallTime;

    return sendMsgToQueue_ISR(distanceTaskQueue, &msg);
}


/*
 * Message send from timer callback.
 */
int stop_DistanceTaskQueue() {

    message_type msg;
    msg[MSG_TYPE] = MSG_TYPE_STOP;
    msg[MSG_DATA] = 0;

    return sendMsgToQueue_ISR(distanceTaskQueue, &msg);
}

/**
 * Blocking read from Queue 1. Fills correct buffer, return type specifies type received.
 */
int readMsg_DistanceTaskQueue(long *amount){
    message_type msg_buffer;

    // Debug before receiving from queue in ISR
    int read_status = readMsqFromQueue(distanceTaskQueue, &msg_buffer);

    // Block until message, check if valid
    if (read_status == READ_SUCCESS) {

        // Fill correct buffer
        switch (msg_buffer[MSG_TYPE]){
        case MSG_TYPE_TIMER:
            *amount = msg_buffer[MSG_DATA];
            return MSG_TYPE_TIMER;
        case MSG_TYPE_RISE:
            *amount = msg_buffer[MSG_DATA];
            return MSG_TYPE_RISE;
        case MSG_TYPE_FALL:
            *amount = msg_buffer[MSG_DATA];
            return MSG_TYPE_FALL;
        case MSG_TYPE_STOP:
            *amount = NULL;
            return MSG_TYPE_STOP;
        default:
            errorRoutine(READ_QUEUE_FAILED);
        }
    }

    //if no returns were called earlier, then read fail
    return READ_FAILURE;
}
