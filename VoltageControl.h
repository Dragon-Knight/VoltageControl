/*
	VoltageControl.h - Библиотека для контроля напряжения на аналоговом контакте микроконтроллера.
	Разработчик: Dragon_Knight => https://vk.com/id5982674 <= 2015г.
	Версия: 1.0
	
	Описание методов:
		VoltageControl - Конструктор класса, принимает следующие параметры:
			uint8_t pin - Контакт микроконтроллера. Например: 'A1'
			float coefficient - Коэффициент пересчёта значения АЦП в Вольты. Например: '0.13469705784574468085106382978724'
			uint16_t interval - Интервал измерения напряжения, в мс. Например: '1000'
			uint8_t vmin - Минимальное допустимое напряжение в вольтах умноженное на 10. Например: '114' - равно 11.4 Вольта.
			uint8_t vmax - Максимальное допустимое напряжение в вольтах умноженное на 10. Например: '146' - равно 14.6 Вольта.
			Возвращает: Ничего.
		
		void AttachCallback - Метод указания ссылки на колбек, принимает следующие параметры:
			void (*callback)(uint8_t voltage) - Имя функции, которая будет выполнена, если значение напряжения выйдет за рамки vmin и vmax. Например: 'OnVoltageError'.
			uint32_t interval - (Опционально) Интервал вызова функции, в мс. Т.е. она будет вызываться не чаще указанного интервала. Например: '60000'. Для лучшей работы параметр должен быть кратный интервалу измерения напряжения.
			Функция должна иметь два параметра: 'uint8_t pin' и 'uint8_t voltage', и ничего не возвращать: 'void'. Например: 'OnVoltageError(uint8_t pin, uint8_t voltage){ }'.
			Возвращает: Ничего.
		
		void Run - Метод запуска или остановки контроля напряжения, принимает следующие параметры:
			bool run - Параметр запуска или остановки контроля напряжения. Например: 'true'.
			Возвращает: Ничего.
		
		uint8_t GetVoltage - Метод ручного измерения напряжения.
			Возвращает: Напряжение на контакте микроконтроллера, умноженное на 10. Например: '127' - равно 12.7 Вольта.
		
		void Processing - Метод обновления класса. Оптимизирован для работы в 'void loop(){ }'.
			Возвращает: Ничего.
		
		
		Формула расчёта коэффициента пересчёта значения АЦП и схема подключения резистивного делителя:
			coefficient = ((Vref / 1024) * ((R1 + R2) / R2)) * 10
				Vref - Опорное напряжение, в вольтах.
			
			(Vin)---[ R1 ]---*---[ R2 ]---(GND)
							 |
							 `---(AnalogInput)
*/

#ifndef VoltageControl_h
#define VoltageControl_h

#include "Arduino.h"

class VoltageControl
{
	public:
		VoltageControl(uint8_t pin, float coefficient, uint16_t interval, uint8_t vmin, uint8_t vmax);
		void AttachCallback(void (*callback)(uint8_t pin, uint8_t voltage));
		void AttachCallback(void (*callback)(uint8_t pin, uint8_t voltage), uint32_t interval);
		void Run(bool run);
		uint8_t GetVoltage();
		void Processing();
	private:
		uint8_t _pin;
		float _coefficient;
		uint16_t _interval;
		uint8_t _vmin;
		uint8_t _vmax;
		void (*_callback)(uint8_t pin, uint8_t voltage);
		uint32_t _callback_interval;
		uint32_t _lastProcessingTime;
		bool _run = false;
		uint32_t _callback_delaying = 0;
		bool _callback_first = true;
};

#endif
