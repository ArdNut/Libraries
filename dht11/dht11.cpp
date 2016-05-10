#include <stdint.h>
#include "Arduino.h"
#include "dht11.h"

int dht11::readDHT(int pin)
{
    uint8_t  cnt     = 7;
    uint8_t  idx     = 0;
    uint8_t  chksum  = 0;
    uint16_t loopcnt = 0;
    uint32_t tmbit   = 0;

//    unsigned int loopcnt = 0;
//    unsigned long tmbit = 0;

    // clear input buffer
    for (int i = 0; i < INBUFFSZ; i++)
        bits[i] = 0;

    // signal DHT11 to start stream (18 ms)
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(18);

    // release pin and wait for DHT11 to respond (40 us)
    digitalWrite(pin, HIGH);
    delayMicroseconds(40);
    pinMode(pin, INPUT);

    // look for low->high response from DHT11
    loopcnt = MAXWAIT;
    while(digitalRead(pin) == LOW) {
        if (loopcnt-- == 0)
            return DHT_TIMEOUT_ERR;
    }

    loopcnt = MAXWAIT;
    while(digitalRead(pin) == HIGH) {
        if (loopcnt-- == 0)
            return DHT_TIMEOUT_ERR;
    }

    // read data bits: 40 BITS => 5 bytes
    // bit stream starts with low signal
    for (int i = 0; i < MAXBITS; i++) {
        // wait for low level
        loopcnt = MAXWAIT;
        while(digitalRead(pin) == LOW) {
            if (loopcnt-- == 0)
                return DHT_TIMEOUT_ERR;
        }

        // detect high and time it
        tmbit = micros();
        loopcnt = MAXWAIT;
        while(digitalRead(pin) == HIGH) {
            if (loopcnt-- == 0)
                return DHT_TIMEOUT_ERR;
        }

        if ((micros() - tmbit) > 40)
            bits[idx] |= (1 << cnt);

        if (cnt == 0) {
            cnt = 7;
            idx++;
        }
        else
            cnt--;
    }

    // as bits[1] and bits[3] are allways zero they are omitted in formulas.
    humR = bits[0];
    tmpC = bits[2];
    tmpF = CtoF(tmpC);
    tmpK = CtoK(tmpC);
    dwpt = dewPnt(tmpC, humR);

    chksum = bits[0] + bits[2];

    if (bits[4] != chksum)
        return DHT_CHKSUM_ERR;

    return DHT_OK;
}


//C to F conversion
double dht11::CtoF(double tmpC)
{
    return 1.8 * tmpC + 32;
}


//C to K conversion
double dht11::CtoK(double tmpC)
{
    return tmpC + 273.15;
}


// reference: http://en.wikipedia.org/wiki/Dew_point
double dht11::dewPnt(double tmpC, double humR)
{
    double a = 17.271;
    double b = 237.7;
    double temp = (a * tmpC) / (b + tmpC) + log(humR/100);
    double Td = (b * temp) / (a - temp);

    return Td;
}
