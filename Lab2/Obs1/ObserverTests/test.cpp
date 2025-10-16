#include "pch.h"
#include "../../Obs1/Obs1/WeatherData.h"


class SelfRemovingObserver : public MonoObserver<OutsideWeatherInfo>, public std::enable_shared_from_this<SelfRemovingObserver>
{
public:
	SelfRemovingObserver(IObservable<OutsideWeatherInfo>& observable)
		: MonoObserver<OutsideWeatherInfo>(observable) {}

	int update_count = 0;

	void OnUpdate(const IObservable<OutsideWeatherInfo>&) override
	{
		++update_count;
		m_observable->RemoveObserver(shared_from_this());
	}
};

TEST(ObservableTest, ObserverSelfRemoving) {
	WeatherDataOutside subject;
	auto obs1 = std::make_shared<SelfRemovingObserver>(subject);
	auto obs2 = std::make_shared<SelfRemovingObserver>(subject);

	subject.RegisterObserver(obs1, 1);
	subject.RegisterObserver(obs2, 2);
	EXPECT_NO_THROW(subject.SetMeasurements(25.0, 60.0, 1013.0, 5.0, 90.0));
	EXPECT_EQ(obs1->update_count, 1);
	EXPECT_EQ(obs2->update_count, 1);
	EXPECT_EQ(subject.GetObserverCount(), 0);
}


class MockPriorityObserver : public MonoObserver<OutsideWeatherInfo>
{
public:
	static int callCounter;
	int updateOrder = 0;

	MockPriorityObserver(IObservable<OutsideWeatherInfo>& observable)
		: MonoObserver<OutsideWeatherInfo>(observable) {}

	void OnUpdate(const IObservable<OutsideWeatherInfo>&) override
	{
		updateOrder = ++callCounter;
	}
};
int MockPriorityObserver::callCounter = 0;

TEST(PriorityObservers, NotificationOrderAndNoDuplicates) {
	WeatherDataOutside subject;
	auto high = std::make_shared<MockPriorityObserver>(subject);
	auto med = std::make_shared<MockPriorityObserver>(subject);
	auto low = std::make_shared<MockPriorityObserver>(subject);
	auto observer = std::make_shared<MockPriorityObserver>(subject);

	subject.RegisterObserver(high, 1);
	subject.RegisterObserver(med, 2);
	subject.RegisterObserver(low, 3);

	EXPECT_EQ(subject.GetObserverCount(), 3);
	subject.RegisterObserver(high, 20);
	EXPECT_EQ(subject.GetObserverCount(), 3);
	subject.RegisterObserver(observer, 1);
	subject.RegisterObserver(observer, 2);
	subject.RegisterObserver(observer, 3);
	EXPECT_EQ(subject.GetObserverCount(), 3);

	MockPriorityObserver::callCounter = 0;
	subject.SetMeasurements(25.0, 60.0, 1013.0, 5.0, 90.0);
	EXPECT_EQ(high->updateOrder, 1);
	EXPECT_EQ(med->updateOrder, 2);
	EXPECT_EQ(low->updateOrder, 3);

	subject.RemoveObserver(med);
	EXPECT_EQ(subject.GetObserverCount(), 2);

	MockPriorityObserver::callCounter = 0;
	subject.SetMeasurements(20.0, 55.0, 1000.0, 7.0, 180.0);
	EXPECT_EQ(high->updateOrder, 1);
	EXPECT_EQ(low->updateOrder, 2);
}


class DuoTestObserver : public DuoObserver<WeatherInfo>
{
public:
	DuoTestObserver(IObservable<WeatherInfo>& first, IObservable<WeatherInfo>& second)
		: DuoObserver<WeatherInfo>(first, second) {}

	bool isFirstUpdateCalled = false;
	bool isSecondUpdateCalled = false;

	void OnFirstUpdate(const IObservable<WeatherInfo>&) override
	{
		isFirstUpdateCalled = true;
	}

	void OnSecondUpdate(const IObservable<WeatherInfo>&) override
	{
		isSecondUpdateCalled = true;
	}
};

TEST(DuoObservers, UpdateCalls)
{
	WeatherData insideStation;
	WeatherData outsideStation;
	auto duo = std::make_shared<DuoTestObserver>(insideStation, outsideStation);

	insideStation.RegisterObserver(duo, 0);
	outsideStation.RegisterObserver(duo, 1);

	insideStation.SetMeasurements(20, 50, 1000);
	EXPECT_TRUE(duo->isFirstUpdateCalled);
	EXPECT_FALSE(duo->isSecondUpdateCalled);

	duo->isFirstUpdateCalled = false;
	duo->isSecondUpdateCalled = false;

	outsideStation.SetMeasurements(25, 55, 1005);
	EXPECT_FALSE(duo->isFirstUpdateCalled);
	EXPECT_TRUE(duo->isSecondUpdateCalled);

	insideStation.RemoveObserver(duo);
	outsideStation.RemoveObserver(duo);

	duo->isFirstUpdateCalled = false;
	duo->isSecondUpdateCalled = false;

	insideStation.SetMeasurements(22, 55, 1000);
	outsideStation.SetMeasurements(26, 60, 1001);
	EXPECT_FALSE(duo->isFirstUpdateCalled);
	EXPECT_FALSE(duo->isSecondUpdateCalled);
}


