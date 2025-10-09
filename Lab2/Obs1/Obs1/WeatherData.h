#include "NumericStats.h"
#include "Observer.h"
#include "WeatherInfo.h"
#include "WindDirectionStats.h"
#include <iostream>
#include <string>
struct WeatherStats
{
	NumericStats<double> temperature{ "temperature" };
	NumericStats<double> pressure{ "pressure" };
	NumericStats<double> humidity{ "humidity" };
};

struct OutsideWeatherStats
{
	NumericStats<double> temperature{ "temperature" };
	NumericStats<double> pressure{ "pressure" };
	NumericStats<double> humidity{ "humidity" };
	NumericStats<double> windSpeed{ "wind speed" };
	WindDirectionStats windDirectionStats;
};

class MonoDisplay : public MonoObserver<OutsideWeatherInfo>
{
public:
	MonoDisplay(IObservable<OutsideWeatherInfo>& observable)
		: MonoObserver<OutsideWeatherInfo>{ observable }
	{
	}

private:
	void OnUpdate(const IObservable<OutsideWeatherInfo>& observable) override
	{
		OutsideWeatherInfo data = observable.GetData();

		std::cout << "Current Temp " << data.temperature << "\n";
		std::cout << "Current Hum " << data.humidity << "\n";
		std::cout << "Current Pressure " << data.pressure << "\n";
		std::cout << "Current Wind Speed " << data.windSpeed << "\n";
		std::cout << "Current Wind Direction " << data.windDirection << "\n";
		std::cout << "----------------\n";
	}
};

class MonoStatsDisplay : public MonoObserver<OutsideWeatherInfo>
{
public:
	MonoStatsDisplay(IObservable<OutsideWeatherInfo>& observable)
		: MonoObserver<OutsideWeatherInfo>{ observable }
	{
	}

protected:
	void OnUpdate(const IObservable<OutsideWeatherInfo>& observable) override
	{
		OutsideWeatherInfo data = observable.GetData();

		m_stats.temperature.Update(data.temperature);
		m_stats.pressure.Update(data.pressure);
		m_stats.humidity.Update(data.humidity);
		m_stats.windSpeed.Update(data.windSpeed);
		m_stats.windDirectionStats.Update(data.windDirection);
	}

private:
	OutsideWeatherStats m_stats;
};

class DuoStatsDisplay : public DuoObserver<WeatherInfo>
{
public:
	DuoStatsDisplay(IObservable<WeatherInfo>& inside, IObservable<WeatherInfo>& outside)
		: DuoObserver<WeatherInfo>{ inside, outside }
	{
	}

protected:
	void OnFirstUpdate(const IObservable<WeatherInfo>& observable) override
	{
		WeatherInfo data = observable.GetData();
		std::cout << "Inside:\n";
		m_insideStats.temperature.Update(data.temperature);
		m_insideStats.pressure.Update(data.pressure);
		m_insideStats.humidity.Update(data.humidity);
	}

	void OnSecondUpdate(const IObservable<WeatherInfo>& observable) override
	{
		WeatherInfo data = observable.GetData();
		std::cout << "Outside:\n";
		m_outsideStats.temperature.Update(data.temperature);
		m_outsideStats.pressure.Update(data.pressure);
		m_outsideStats.humidity.Update(data.humidity);
	}

private:
	WeatherStats m_insideStats;
	WeatherStats m_outsideStats;
};

class WeatherData : public Observable<WeatherInfo>
{
public:
	double GetTemperature() const
	{
		return m_temperature;
	};

	double GetHumidity() const
	{
		return m_humidity;
	};

	double GetPressure() const
	{
		return m_pressure;
	}

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(double temp, double humidity, double pressure)
	{
		m_temperature = temp;
		m_humidity = humidity;
		m_pressure = pressure;

		MeasurementsChanged();
	}

	WeatherInfo GetData() const override
	{
		return { m_temperature, m_humidity, m_pressure };
	}

private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
};

class WeatherDataOutside : public Observable<OutsideWeatherInfo>
{
public:
	double GetTemperature() const
	{
		return m_temperature;
	};

	double GetHumidity() const
	{
		return m_humidity;
	};

	double GetPressure() const
	{
		return m_pressure;
	}

	double GetWindSpeed() const
	{
		return m_windSpeed;
	}

	double GetWindDirection() const
	{
		return m_windDirection;
	}

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(double temp, double humidity, double pressure, double windSpeed, double windDirection)
	{
		m_temperature = temp;
		m_humidity = humidity;
		m_pressure = pressure;
		m_windSpeed = windSpeed;
		m_windDirection = windDirection;

		MeasurementsChanged();
	}

	OutsideWeatherInfo GetData() const override
	{
		return { m_temperature, m_humidity, m_pressure, m_windSpeed, m_windDirection };
	}

private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;
	double m_pressure = 760.0;
	double m_windSpeed = 0.0;
	double m_windDirection = 0.0;
};