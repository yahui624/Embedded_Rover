/*
 * SPI_interface.c
 *
 *  Created on: Feb 25, 2020
 *      Author: hungn
 */

#include "SPI_interface.h"
#define BYTE 8
SPI_Handle SPI_interface_initialize(unsigned mode)
{
    SPI_Handle spi;
    SPI_Params spi_params;

    // SPI_Transaction transaction;
    SPI_Params_init(&spi_params);
    spi_params.transferMode        = SPI_MODE_CALLBACK;
    spi_params.bitRate             = SPI_BITRATE;
    spi_params.mode                = SPI_MASTER;
    spi_params.dataSize = BYTE;
    spi_params.transferCallbackFxn = SPI_transfer_callback;

    spi = SPI_open(CONFIG_SPI_0, &spi_params);

    if (spi == NULL)
    {
        code(SPI_OPEN_FAILED);
        stop_everything();
    }

    return spi;
}

void SPI_transfer_callback(SPI_Handle spi, SPI_Transaction *transaction)
{
    if (transaction->status == SPI_TRANSFER_COMPLETED)
    {
        //  Send to queue every transfer to Encoder Queue
        code(SPI_CALLBACK_BEGIN);
        Encoder_Queue_send_SPI_transfer_completed();
        code(SPI_CALLBACK_END);
    }

}

int SPI_interface_transfer(SPI_Handle *spi, uint8_t *rx_buffer, uint8_t *tx_buffer, size_t count)
{
    static SPI_Transaction transaction;

    transaction.count = count;
    transaction.txBuf = (void *)tx_buffer;
    transaction.rxBuf = (void *)rx_buffer;

    int transfer_ok = SPI_transfer(*spi, &transaction);

    // Handle transfer_ok error in caller functions
    return transfer_ok;
}
