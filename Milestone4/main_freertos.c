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

#include <ti/drivers/Board.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/SPI.h>

/* TI-DRIVERS Header files */
#include "ti_drivers_config.h"

extern void * serverThread(void *arg0);
extern void * motorThread(void *arg0);
extern void * spiThread(void *arg0);
extern void * controlThread(void *arg0);

/* Stack size in bytes */
#define THREADSTACKSIZE   4096

/*
 *  ======== main ========
 */
int main(void)
{
    pthread_t           serverthread;
    pthread_t           motorthread;
    pthread_t           spithread;
    pthread_t           controlthread;

    pthread_attr_t      serverattrs;
    pthread_attr_t      motorattrs;
    pthread_attr_t      spiattrs;
    pthread_attr_t      controlattrs;

    int                 serverretc;
    int                 motorretc;
    int                 spiretc;
    int                 controlretc;


    /* Call board init functions */
    Board_init();
    GPIO_init();
    UART_init();
    Timer_init();
    SPI_init();


    /* Initialize the attributes structure with default values */
    pthread_attr_init(&serverattrs);
    pthread_attr_init(&motorattrs);
    pthread_attr_init(&spiattrs);
    pthread_attr_init(&controlattrs);


    serverretc = pthread_create(&serverthread, &serverattrs, serverThread, NULL);
    if(serverretc != NULL)
    {
        /* pthread_create() failed */
        while(1) {}
    }

    motorretc = pthread_create(&motorthread, &motorattrs, motorThread, NULL);
    if (motorretc != NULL) {
        /* pthread_create() failed */
        while (1) {}
    }

    spiretc = pthread_create(&spithread, &spiattrs, spiThread, NULL);
    if (spiretc != NULL) {
        /* pthread_create() failed */
        while (1) {}
    }

    controlretc = pthread_create(&controlthread, &controlattrs, controlThread, NULL);
    if (controlretc != NULL) {
        /* pthread_create() failed */
        while (1) {}
    }

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
