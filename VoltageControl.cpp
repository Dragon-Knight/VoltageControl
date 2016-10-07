/*
	VoltageControl.cpp - Библиотека для контроля напряжения на аналоговом контакте микроконтроллера.
	Разработчик: Dragon_Knight => https://vk.com/id5982674 <= 2015г.
	Версия: 1.0
	
	Описание методов в файле 'VoltageControl.h'.
*/

#include "Arduino.h"
#include "VoltageControl.h"

VoltageControl::VoltageControl(uint8_t pin, float coefficient, uint16_t interval, uint8_t vmin, uint8_t vmax)
{
	pinMode(pin, INPUT);
	
	this->_pin = pin;
	this->_coefficient = coefficient;
	this->_interval = interval;
	this->_vmin = vmin;
	this->_vmax = vmax;
	
	//this->Run(false);
	
	return;
}

void VoltageControl::AttachCallback(void (*callback)(uint8_t pin, uint8_t voltage))
{
	this->_callback = callback;
	this->_callback_interval = 0;
	
	return;
}

void VoltageControl::AttachCallback(void (*callback)(uint8_t pin, uint8_t voltage), uint32_t interval)
{
	this->_callback = callback;
	this->_callback_interval = interval;
	
	return;
}

void VoltageControl::Run(bool run)
{
	if(this->_run != run)
	{
		this->_run = run;
		this->_callback_first = true;
		this->_callback_delaying = 0;
	}
	
	return;
}

uint8_t VoltageControl::GetVoltage()
{
	return (analogRead(this->_pin) * this->_coefficient);
}

void VoltageControl::Processing()
{
	if(this->_run == true)
	{
		if(this->_lastProcessingTime + this->_interval <= millis())
		{
			this->_lastProcessingTime = millis();
			
			uint8_t voltage = this->GetVoltage();
			
			if(voltage < this->_vmin || voltage > this->_vmax)
			{
				if(this->_callback_first == false)
				{
					this->_callback_delaying++;
					if((this->_callback_delaying * this->_interval) >= this->_callback_interval)
					{
						this->_callback_first = true;
						this->_callback_delaying = 0;
					}
				}
				
				if(this->_callback_first == true)
				{
					this->_callback_first = false;
					this->_callback(this->_pin, voltage);
				}
			}
			else
			{
				this->_callback_first = true;
				this->_callback_delaying = 0;
			}
			
		}
	}
	
	return;
}
