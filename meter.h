#ifndef __NB_METER
#define __NB_METER

#include "nworkbench.h"

class nBlock_Meter: public nBlockNode {
public:
    nBlock_Meter(PinName pinPWM, uint32_t Frequency, float meterVcc, float meterVmax, float leftValue, float rightValue);
    void triggerInput(nBlocks_Message message);
    void step(void);
    float _meterVcc; 
    float _meterVmax; 
    float _leftValue; 
    float _rightValue;
    float tmp;
private:
    PwmOut _pwm;
	float duty_float;
	uint32_t is_updated;
};



#endif
