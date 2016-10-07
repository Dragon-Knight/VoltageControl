/*
	demo.ino - Пример работы библиотеки для контроля напряжения на аналоговом контакте микроконтроллера.
	Разработчик: Dragon_Knight => https://vk.com/id5982674 <= 2015г.
	Версия: 1.0
	
	Описание методов в файле 'VoltageControl.h'.
*/

#include <VoltageControl.h>

VoltageControl vol1 = VoltageControl(A1, 0.13469705784574468085106382978724, 1000, 75, 85);

void setup()
{
	Serial.begin(115200);
	
	vol1.AttachCallback(OnVoltageError, 5000);
	vol1.Run(true);
	
	Serial.println("INIT OK!");
	
	delay(250);
}

void loop()
{
	vol1.Processing();
}

void OnVoltageError(uint8_t pin, uint8_t voltage)
{
	Serial.print("callback: voltage='");
	Serial.print(voltage);
	Serial.println();
}
