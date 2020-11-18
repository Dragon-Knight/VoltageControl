#include <VoltageControl.h>

VoltageControl<A0> MyVolt;

void setup()
{
	Serial.begin(115200);
	
	randomSeed(analogRead(A7));
	
	MyVolt.SetRequest(OnRequestVoltage, 1000);
	MyVolt.SetResponse(OnResponseVoltage);
	//MyVolt.SetResponse(OnResponseVoltage, true);
	//MyVolt.SetCoefficient( VoltageControl<>::GetCoefficient(5100000, 15100, 2395) );
	MyVolt.SetCoefficient( VoltageControl<>::GetCoefficient(5100000) );
	MyVolt.SetMin(3000);
	MyVolt.SetMax(3600);
	MyVolt.SetStart();
	//MyVolt.SetStop();
	MyVolt.GetValid();
	MyVolt.GetVoltage();
	
	return;
}

void loop()
{
	MyVolt.Processing();
	
	return;
}

int32_t OnRequestVoltage(uint8_t id)
{
	Serial.print(" > OnRequestVoltage(");
	Serial.print(id);
	Serial.println(");");
	
	return analogRead(id);
}

void OnResponseVoltage(uint8_t id, int32_t value, int8_t state)
{
	Serial.print(" > OnResponseVoltage(");
	Serial.print(id);
	Serial.print(", ");
	Serial.print(value);
	Serial.print(", ");
	Serial.print(state);
	Serial.println(");");
	Serial.println();
	
	return;
}
