#ifndef ShiftRegisterIO_h
#define ShiftRegisterIO_h

struct SR_IO
{
    bool PT100Reader_Enable = true;
    bool TMuxS0 = false;
    bool TMuxS1 = false;
    bool TMuxS2 = false;
    bool RMuxS2 = false;
    bool RMuxS1 = false;
    bool RMuxS0 = false;
    bool Distance_Enable = false;

    bool LED_Wifi = false;
    bool LED_Error = false;
    bool LED_Ready = false;
    bool LED_RJ4 = false;
    bool LED_RJ3 = false;
    bool LED_RJ2 = false;
    bool LED_RJ1 = false;
    bool OLED_Enbable = true;

    bool RJ4_Counter = true; // Keep this mostly true using pnp-transostor
    bool RJ4_Status = false;
    bool RJ3_Counter = true; // Keep this mostly true using pnp-transostor
    bool RJ3_Status = false;
    bool RJ2_Counter = true; // Keep this mostly true using pnp-transostor
    bool RJ2_Status = false;
    bool RJ1_Counter = true; // Keep this mostly true using pnp-transostor
    bool RJ1_Status = true;
};

class ShiftRegisterIO
{
public:
    //Constructor
    ShiftRegisterIO();

    //Methodes
    void init();
    void allOne();
    void allZero();

    void ledBlink(int time);
    SR_IO r_MuxSelect(SR_IO sr_io, int channel);
    void t_MuxSelect(struct SR_IO *sr_io, int channel);
    void write(struct SR_IO *sr_io);

    void led_ERROR(struct ShiftRegisterIO *shiftRegisterIO, struct SR_IO *sr_io, bool toggle);
    void led_READY(struct ShiftRegisterIO *shiftRegisterIO, struct SR_IO *sr_io, bool toggle);
    void led_WIFI(struct ShiftRegisterIO *shiftRegisterIO, struct SR_IO *sr_io, bool toggle);
    void led_RJ1(struct ShiftRegisterIO *shiftRegisterIO, struct SR_IO *sr_io, bool toggle);
    void led_RJ2(struct ShiftRegisterIO *shiftRegisterIO, struct SR_IO *sr_io, bool toggle);
    void led_RJ3(struct ShiftRegisterIO *shiftRegisterIO, struct SR_IO *sr_io, bool toggle);
    void led_RJ4(struct ShiftRegisterIO *shiftRegisterIO, struct SR_IO *sr_io, bool toggle);
};
#endif