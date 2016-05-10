/*  DDS.cpp

    Derived from examples found on the Internet and the Analog Devices
    AD9850 datasheet. Thanks to Anthony Good and Richard Visokey for
    creating useful examples.

    Note: this code is known to work with the AD9850 only.

    Created November 2015 for the book "Arduino in a Nutshell" by
    John M. Hughes
*/

#include "Arduino.h"
#include <DDS.h>

//-----------------------------------------------------------------------------
DDS::DDS(unsigned long clockfreq, uint8_t data_pin, uint8_t load_pin, uint8_t wclock_pin, uint8_t reset_pin)
{
    dds_clock = clockfreq;
    dds_data  = data_pin;
    dds_load  = load_pin;
    dds_wclk  = wclock_pin;
    dds_reset = reset_pin;

    dds_cal = 0.0;

    pinMode(dds_load, OUTPUT);
    pinMode(dds_wclk, OUTPUT);
    pinMode(dds_data, OUTPUT);
    pinMode(dds_reset, OUTPUT);

    // set all control pins low initially
    digitalWrite(dds_load,LOW);
    digitalWrite(dds_wclk,LOW);
    digitalWrite(dds_data,LOW);
    digitalWrite(dds_reset,LOW);

    // pulse the control pins to set up AD9850
    pulseHigh(dds_reset);
    pulseHigh(dds_wclk);
    pulseHigh(dds_load);
}

// Private methods ------------------------------------------------------------

void DDS::pulseHigh(int pin)
{
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
}


unsigned long DDS::freqData(unsigned long freq)
{
    // refer to AD9850 datasheet for explanation
    return (unsigned long)(((float) freq * pow(2,32) ) / (float)(dds_clock - (dds_clock * dds_cal)));
}


void DDS::sendBit (uint8_t outbit)
{
    // Each bit is clocked into the DDS with the dds_wclk pin
    digitalWrite(dds_data,outbit);
    pulseHigh(dds_wclk);
}


// Public methods -------------------------------------------------------------

// Load caller-supplied calibration value. Refer to the AD9850 datasheet
// for more information about how this is computed and used.
void DDS::calibrate(float calval)
{
    dds_cal = calval;
}


// The one and only way to set the frequency of the AD9850. Passing in a zero
// value will cause the output to stop. And value between 1 and 40,000,000 is
// valid (and possibly higher).
void DDS::setFrequency(unsigned long freq)
{
    // convert target frequency to control word
    unsigned long freq_val = freqData(freq);

    // set load pin low before sending data
    digitalWrite(dds_load,LOW);

    // Send the 32 bit word, least significant bit first. Uses the
    // library function bitRead().
    for (int x = 0; x < 32; x++) {
        sendBit(bitRead(freq_val,x));
    }

    // Send the control byte at the end, all zero bits
    // pulse dds_wclk for each bit
    digitalWrite(dds_data,0);
    for (int y = 0; y < 8; y++) {
        pulseHigh(dds_wclk);
    }

    // pulse the load pin to signal end of data word
    pulseHigh(dds_load);
}

