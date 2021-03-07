#include "meter.h"

nBlock_Meter::nBlock_Meter(PinName pinPWM, uint32_t Frequency, float meterVcc, float meterVmax, float leftValue, float rightValue): 
    _pwm(pinPWM) {
	duty_float = 0.0;
	is_updated = 0;
    _pwm.period(1.0/((float)Frequency));
    _pwm.write(0);
    _leftValue  = leftValue;
    _rightValue = rightValue;
    _meterVcc   = meterVcc;
    _meterVmax  = meterVmax;
    return;
}

void nBlock_Meter::triggerInput(nBlocks_Message message) {
    // Stores the duty cycle we would like to use
    // when updating in the end of this frame
    float tmp;
	if (message.dataType == OUTPUT_TYPE_FLOAT) {
		// If received a float, assign directly
		tmp = message.floatValue;
		is_updated = 1;		// flag as updated
	}
	else if (message.dataType == OUTPUT_TYPE_INT) {
		tmp = (message.intValue & 0x0000FFFF);	// get the message, mask huge values and store it to a float variable
		is_updated = 1;		// flag as updated
	}
		if(is_updated){							        // scale the output
			tmp = tmp - _leftValue;
			if (tmp < 0) tmp = 0;				        // send to meter 0 for values =< leftValue
			tmp = tmp / (_rightValue - _leftValue);		// normalize so the desired [voltage_span value] becomes 1 (100% of the meter scale)
			tmp = tmp * _meterVmax/_meterVcc;		    // Reduce for Vcc larger that the meter Max voltage (100% doesn't exceed meter scale)
			duty_float = tmp;
		}
	
	// Other types are ignored
}
void nBlock_Meter::step(void) {
	if (is_updated) {
		// de-flag updated
		is_updated = 0;
		// Apply the value
		_pwm.write(duty_float);
	}
    return;
}
