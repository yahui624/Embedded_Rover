

#include "encoder_interface.h"

int encoder_interface_set_mode(SPI_Handle *encoder_SPI)
{
    // Write to MDR0 and Configure to 4 byte mode
    const static uint8_t tx_buffer[] = {ENCODER_WRITE_TO_MDR0, ENCODER_4_BYTE_MODE};
    static uint8_t rx_buffer[] = {JUNK, JUNK};

    // Transfer
    int transfer_ok = SPI_interface_transfer(encoder_SPI, rx_buffer, tx_buffer, sizeof(tx_buffer));

    if (!transfer_ok)
    {
        code(ENCODER_SET_MODE_FAILED);
        stop_everything();
    }
    return transfer_ok;
}

int encoder_interface_read(SPI_Handle *encoder_SPI, uint8_t *rx_buffer)
{
    // Write 0x60 first to read count
    const static uint8_t tx_buffer[] = {ENCODER_READ_COUNT, JUNK, JUNK, JUNK, JUNK};

    int transfer_ok = SPI_interface_transfer(encoder_SPI, rx_buffer, tx_buffer, sizeof(tx_buffer));

    if (!transfer_ok)
    {
        code(ENCODER_READ_FAIL);
        stop_everything();
    }

    return transfer_ok;
}

long encoder_interface_assemble_bytes(uint8_t *rx_buffer)
{
    long count_value;

    // Assembly higher order byte to low for count value
    count_value = (rx_buffer[1] << 8) + rx_buffer[2];
    count_value = (count_value << 8) + rx_buffer[3];
    count_value = (count_value << 8) + rx_buffer[4];

    return count_value;
}

int encoder_interface_reset_data_register(SPI_Handle *encoder_SPI)
{
    // Write to DTR all zeroes using 0x98
    const static uint8_t tx_buffer[] = {ENCODER_WRITE_TO_DTR, ZERO, ZERO, ZERO, ZERO};
    static uint8_t rx_buffer[] = {JUNK, JUNK, JUNK, JUNK, JUNK};

    int transfer_ok = SPI_interface_transfer(encoder_SPI, rx_buffer, tx_buffer, sizeof(tx_buffer));

    if (!transfer_ok)
    {
        code(ENCODER_RESET_DTR_FAIL);
        stop_everything();
    }
    return transfer_ok;
}

int encoder_interface_clear_counter(SPI_Handle *encoder_SPI)
{
    // Write to set to CTR using 0xE0
    const static uint8_t tx_message = ENCODER_SET_TO_CTR;
    static uint8_t rx_message = JUNK;

    int transfer_ok = SPI_interface_transfer(encoder_SPI, &rx_message, &tx_message, ONE);

    if (!transfer_ok)
    {
        code(ENCODER_CLEAR_CTR_FAIL);
        stop_everything();
    }

    // Collect all bytes
    return transfer_ok;
}
