/*
 * spi_encoder.c
 *
 *  Created on: Feb 25, 2020
 *      Author: Yahui
 */
#include <spi_encoder.h>
#include <uart_term.h>

//void spiCallbackFxn(SPI_Handle handle, SPI_Transaction *tran) {
////    uint8_t *semPtr = (uint8_t *)(tran->arg);
//
//    int32_t return_msg;
//
//    if (tran->status == SPI_STATUS_SUCCESS && tran->count > 4) { //check SPI status
//        return_msg = (masterRxBuffer[1] << 8) + masterRxBuffer[2];
//        return_msg =  (return_msg << 8) + masterRxBuffer[3];
//        return_msg = (return_msg << 8) +masterRxBuffer[4];
//    }
//
//    sendSPIMsgToEncoderQueue(return_msg, 0, 0);
//}



void initSPIEncoder(){
    SPI_Params      spiParams;

    /* Open SPI as master (default) */
    SPI_Params_init(&spiParams);
    spiParams.dataSize = 8;
//    spiParams.transferMode = SPI_MODE_CALLBACK; // non-blocking mode
    spiParams.mode = SPI_MASTER;
//    spiParams.transferCallbackFxn = spiCallbackFxn; // configure the callback function
    masterSpi = SPI_open(CONFIG_SPI_MASTER, &spiParams);


    /* Configure the encoders' pin  */
    GPIO_setConfig(Slave_Motor1_Encoder, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Slave_Motor2_Encoder, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(Slave_Motor3_Encoder, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /*Raise select pins */
    GPIO_write(Slave_Motor1_Encoder, HIGH);
    GPIO_write(Slave_Motor2_Encoder, HIGH);
    GPIO_write(Slave_Motor3_Encoder, HIGH);

    if (masterSpi == NULL) { // Fail to initialize the callback mode
//        outputUartBytes("Error initializing master SPI\n");
        while (1);
    }
    else {
//        outputUartBytes("Master SPI initialized\n");
    }

    clear_SPI_Encoder_Count(Slave_Motor1_Encoder); // clear the motor count
    GPIO_write(Slave_Motor1_Encoder, HIGH);
    clear_SPI_Encoder_Count(Slave_Motor2_Encoder);
    GPIO_write(Slave_Motor2_Encoder, HIGH);
    clear_SPI_Encoder_Count(Slave_Motor3_Encoder);
    GPIO_write(Slave_Motor3_Encoder, HIGH);
}


int32_t read_SPI_Encoder(int encoder){
    GPIO_write(encoder, LOW);


    uint8_t masterRxBuffer[5] = {0} ;
    uint8_t masterTxBuffer[5] = {0};
    static SPI_Transaction transaction;
    bool transferOK;

    masterTxBuffer[0] = 0x60; // read the register
    masterTxBuffer[1] = 0x00;
    masterTxBuffer[2] = 0x00;
    masterTxBuffer[3] = 0x00;
    masterTxBuffer[4] = 0x00;

    memset((void *) masterRxBuffer, 0, 5);
    transaction.count = 5;
    transaction.txBuf = (void *) masterTxBuffer;
    transaction.rxBuf = (void *) masterRxBuffer;


    /* Perform SPI transfer */
    transferOK = SPI_transfer(masterSpi, &transaction);
    if (transferOK) {
//        sprintf("WRITE SUCCESSFULLY");
    }
    else {
//        sprintf("SPI FAIL");
    }

    while (!transferOK) {};
     /* Sleep for a bit before starting the next SPI transfer  */
//    sleep(3);

    int32_t return_msg;
    return_msg = (masterRxBuffer[1] << 8) + masterRxBuffer[2];
    return_msg =  (return_msg << 8) + masterRxBuffer[3];
    return_msg = (return_msg << 8) +masterRxBuffer[4];


    //char string_UART[100];
    //sprintf (string_UART,"encoder= %d  ",init_SS1);
    //outputUartBytes("12345");

    return return_msg;
}

int reset_SPI_Encoder(int encoder){
    uint8_t masterRxBuffer[5] = {0xE0, 0x00, 0x00, 0x00, 0x00} ;
    uint8_t masterTxBuffer[5] = {0x00, 0x00, 0x00, 0x00, 0x00,};
    SPI_Transaction transaction;
    int transferOK;

    memset((void *) masterRxBuffer, 0, 5);
    transaction.count = 5;
    transaction.txBuf = (void *) masterTxBuffer;
    transaction.rxBuf = (void *) masterRxBuffer;


    /* Perform SPI transfer */
    transferOK = SPI_transfer(masterSpi, &transaction);
    if (transferOK) {
//        sprintf("WRITE SUCCESSFULLY");
    }
    else {
//        sprintf("SPI FAIL");
    }

    while (!transferOK) {};
     /* Sleep for a bit before starting the next SPI transfer  */
//    sleep(3);

    return transferOK;
}


void clear_SPI_Encoder_Count(int encoder) {
    GPIO_write(encoder, LOW);
    SPI_Transaction transaction;
    int transferOK;

    uint8_t masterRxBuffer[1] = {0};
    uint8_t masterTxBuffer[1] = {0};
    // Initialize temporary variables for SPI read

    masterTxBuffer[0] = CLEAR_CNTR; // read the register
    memset((void *) masterRxBuffer, 0, sizeof(masterRxBuffer));
    transaction.count = 1;
    transaction.txBuf = (void *) masterTxBuffer;
    transaction.rxBuf = (void *) masterRxBuffer;


    /* Perform SPI transfer */
    transferOK = SPI_transfer(masterSpi, &transaction);

}


void *spiThread(void *arg0) {
    encoder_msg receiveFromQueue;

    initEncoderQueue();
    initSPIEncoder();
    initEncoderTimer();

    while(1) {
        receiveFromQueue = readSPIMsgFromEncoderQueue();
        //obtain the queue and read from it
//        UART_write(uartDebug, receiveFromQueue.encoder1_val.encoderMsg, sizeof(receiveFromQueue.encoder1_val.encoderMsg));
//        UART_write(uartDebug, receiveFromQueue.encoder2_val.encoderMsg, sizeof(receiveFromQueue.encoder2_val.encoderMsg));
//        UART_write(uartDebug, receiveFromQueue.encoder3_val.encoderMsg, sizeof(receiveFromQueue.encoder3_val.encoderMsg));
        char string_UART[100];
        sprintf (string_UART,"encoder1= %d",receiveFromQueue.encoder1_val.encoderMsg);
//        outputUartBytes(string_UART);

        char string_UART2[100];
        sprintf (string_UART2,"encoder2= %d",receiveFromQueue.encoder2_val.encoderMsg);
//        outputUartBytes(string_UART);

        char string_UART3[100];
        sprintf (string_UART3,"encoder3= %d\n",receiveFromQueue.encoder3_val.encoderMsg);
//        outputUartBytes(string_UART);

   }

}
