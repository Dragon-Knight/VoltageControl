/*
 *	VoltageControl.h
 *	Voltage Control class
 *
 *	@author		Nikolai Tikhonov aka Dragon_Knight <dubki4132@mail.ru>, https://vk.com/globalzone_edev
 *	@licenses	MIT https://opensource.org/licenses/MIT
 *	@repo		https://github.com/Dragon-Knight/VoltageControl
*/

#pragma once

template<uint8_t _id = 0>
class VoltageControl
{
	public:
		using request_t = int32_t (*)(uint8_t id);
		using response_t = void (*)(uint8_t id, int32_t value, int8_t state);
		
		VoltageControl()
		{
			return;
		};
		
		VoltageControl(request_t callback_req, uint16_t interval_req, response_t callback_res, bool once, uint32_t coefficient, int32_t vmin, int32_t vmax) : _data{callback_req, interval_req, 0, callback_res, once, 0, coefficient, vmin, vmax, false}
		{
			return;
		};
		
		// Указать колбэк-функцию запроса напряжения и интервал запроса в мс.
		void SetRequest(request_t callback, uint16_t interval)
		{
			_data.request_callback = callback;
			_data.request_interval = interval;
			
			return;
		};
		
		// Указать колбэк-функцию изменения напряжения и интервал оповещения в мс.
		void SetResponse(response_t callback, bool once = false)
		{
			_data.response_callback = callback;
			_data.response_once = once;
			
			return;
		};
		
		// Указать коэффициент пересчёта АЦП в мВ.
		void SetCoefficient(uint32_t coefficient)
		{
			_data.coefficient = coefficient;
			
			return;
		};
		
		// Указать минимально допустимое напряжение в мВ.
		void SetMin(uint32_t vmin)
		{
			_data.vmin = vmin;
			
			return;
		};
		
		// Указать максимально допустимое напряжение в мВ.
		void SetMax(uint32_t vmax)
		{
			_data.vmax = vmax;
			
			return;
		};
		
		// Запустить класс.
		void SetStart()
		{
			_data.active = true;
			
			return;
		};
		
		// Остановить класс.
		void SetStop()
		{
			_data.active = false;
			
			return;
		};
		
		// Проверить текущее напряжение на нахождение между vmin и vmax.
		int8_t GetValid()
		{
			int32_t voltage = GetVoltage();
			
			return ((voltage < _data.vmin) ? -1 : ((voltage > _data.vmax) ? 1 : 0));
		};
		
		// Вернуть текущее напряжение, в мВ.
		int32_t GetVoltage()
		{
			uint32_t raw = _data.request_callback(_id);
			
			return (raw * _data.coefficient) / 1000;
		};
		
		// Обработка класса.
		void Processing(uint32_t currentTime = millis())
		{
			if( _data.active == true && (_data.request_time + _data.request_interval) <= currentTime )
			{
				int32_t voltage = GetVoltage();
				int8_t state = ((voltage < _data.vmin) ? -1 : ((voltage > _data.vmax) ? 1 : 0));
				if( _data.response_once == false || (_data.response_once == true && _data.old_state != state) )
				{
					_data.response_callback(_id, voltage, state);
					
					_data.old_state = state;
				}
				
				_data.request_time = currentTime;
			}
			
			return;
		};
		
		// Утилита расчёта коэффициента по VRev, R1, R2 и уровню квантования.
		// PS: Не стоит обращать внимание на тип float. Эта функция полностью просчитывается на этапе компиляции и возвращает тип uint32_t.
		static inline uint32_t GetCoefficient(float vref, float r1 = 0, float r2 = 1, float quantization = 1024)
		{
			return ((vref / quantization) * ((r1 + r2) / r2)) + 0.5;
		}
		
	private:
		struct data_t
		{
			request_t request_callback;		// Колбэк-функция запроса напряжения.
			uint16_t request_interval;		// Интервал вызова колбэк-функции запроса напряжения.
			uint32_t request_time;			// Время последнего запроса напряжения.
			
			response_t response_callback;	// Колбэк-функция изменения напряжения.
			bool response_once;				// Флаг одиночного вызова при изменении состояния напряжения.
			int8_t old_state;				// Состояние предыдущего измерения. Только при response_once == true
			
			uint32_t coefficient;			// Коэффициент пересчёта значения АЦП в мВ.
			int32_t vmin;					// Минимально допустимое напряжение, в мВ.
			int32_t vmax;					// Максимально допустимое напряжение, в мВ.
			bool active;					// Флаг активности класса.
		} _data;
};
