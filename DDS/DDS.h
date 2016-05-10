/*  DDS.h

    Derived from examples found on the Internet and the Analog Devices
    AD9850 datasheet. Thanks to Anthony Good and Richard Visokey for
    creating useful examples.

    After DDS object is instantiated the method setFrequency is used to
    set the output frequency.

    Created November 2015 for the book "Arduino in a Nutshell" by
    John M. Hughes
*/

#ifndef DDS_H
#define DDS_H

class DDS {
    private:
        float dds_cal;

        unsigned long dds_clock;    // ext clock, in Hz

        uint8_t dds_reset;          // DDS reset pin
        uint8_t dds_data;           // DDS data pin
        uint8_t dds_load;           // DDS data load pin
        uint8_t dds_wclk;           // DDS data load clock pin

        void pulseHigh(int pin);
        unsigned long freqData(unsigned long freq);
        void sendBit(uint8_t outbit);

    public:
        DDS(unsigned long clockfreq, uint8_t dds_data, uint8_t dds_load, uint8_t dds_wclock, uint8_t dds_reset);
        void setFrequency(unsigned long freqval);
        void calibrate(float calval);
};

#endif
