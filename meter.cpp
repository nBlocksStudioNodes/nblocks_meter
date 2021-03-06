#include "meter.h"

nBlock_Meter::nBlock_Meter(PinName pinPWM, uint32_t Frequency, float meterVcc, float meterVmax, float leftV, float rightV): 
    _pwm(pinPWM) {
	duty_float = 0.0;
	is_updated = 0;
    _pwm.period(1.0/((float)Frequency));
    _pwm.write(0);
    return;
}

void nBlock_Meter::triggerInput(nBlocks_Message message) {
    // Stores the duty cycle we would like to use
    // when updating in the end of this frame
	if (message.dataType == OUTPUT_TYPE_FLOAT) {
		// If received a float, assign directly
		duty_float = message.floatValue;
		// flag as updated
		is_updated = 1;
	}
	else if (message.dataType == OUTPUT_TYPE_INT) {
		// If received an integer, normalize it
		float tmp;
		tmp = tmp - leftV;
		if (tmp < 0) tmp = 0;							// send to meter 0 for values =< leftV
		tmp = (message.intValue & 0x0000FFFF);			// mask huge values
		tmp = tmp / (rightV - leftV);					// normalize so the desired [voltage_span value] becomes 1
		tmp = tmp * meterVmax/meterVcc					// Reduce for Vcc larger that the meter Max voltage
		duty_float = tmp;
		// flag as updated
		is_updated = 1;
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
