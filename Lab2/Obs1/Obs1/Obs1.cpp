#include "WeatherData.h"

int main()
{
	auto weatherStation1 = std::make_shared<WeatherData>();
	auto weatherStation2 = std::make_shared<WeatherData>();
	auto outsideWeatherStation = std::make_shared<WeatherDataOutside>();
	

    auto monoStatsDisplay = std::make_shared<MonoStatsDisplay>(*outsideWeatherStation);
	auto monoDisplay = std::make_shared<MonoDisplay>(*outsideWeatherStation);
	auto duoStatsDisplay = std::make_shared<DuoStatsDisplay>(*weatherStation1, *weatherStation2);

	weatherStation1->RegisterObserver(duoStatsDisplay, 1);
	weatherStation2->RegisterObserver(duoStatsDisplay, 1);
	
	outsideWeatherStation->RegisterObserver(monoStatsDisplay, 1);
	outsideWeatherStation->RegisterObserver(monoDisplay, 2);


	weatherStation1->SetMeasurements(20, 20, 800);
	weatherStation2->SetMeasurements(25, 30, 760);

	weatherStation1->SetMeasurements(30, 20, 780);
	weatherStation2->SetMeasurements(22, 25, 790);


	std::cout << "DUO\n";
	outsideWeatherStation->SetMeasurements(20, 20, 820, 5, 270);
	outsideWeatherStation->SetMeasurements(20, 20, 820, 11, 0);

	outsideWeatherStation->RemoveObserver(monoStatsDisplay);

	outsideWeatherStation->SetMeasurements(20, 20, 820, 17, 0);
	
	return 0;
}
