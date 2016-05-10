#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

#define DHT_OK              0
#define DHT_CHKSUM_ERR      -1
#define DHT_TIMEOUT_ERR     -2
#define DHT_INVALID_VAL     -999

#define INBUFFSZ            5
#define MAXBITS             40
#define MAXWAIT             10000

class dht11
{
    private:
        uint8_t bits[INBUFFSZ];
        double CtoF(double tmpC);
        double CtoK(double tmpC);
        double dewPnt(double tmpC, double humR);

    public:
        int     readDHT(int pin);
        double  humR;
        double  tmpC;
        double  tmpF;
        double  tmpK;
        double  dwpt;
};

#endif

