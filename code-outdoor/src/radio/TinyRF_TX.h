#ifndef TINYRF_TX_H
#define TINYRF_TX_H

void setupTransmitter(uint8_t pin);

// Sends an array of bytes
// Also appends error checking and sequence numbering
void send(byte *data, uint8_t len, bool incrementSeq = true);

#endif
