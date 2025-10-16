#include <cstdint>
#include <map>
#include <memory>
#include <vector>

//TODO реализовать MonoObserver
//сделать наследников
//доделать диаграмму
//

template <typename T>
class IObservable;

template <typename T>
class IObserver
{
public:
	virtual void Update(IObservable<T>& subject) = 0;
	virtual ~IObserver() = default;
};

template <typename T>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(const std::weak_ptr<IObserver<T>>& observer, uint32_t priority) = 0;
	virtual void NotifyObservers() = 0;
	virtual void RemoveObserver(const std::weak_ptr<IObserver<T>>& observer) = 0;
	virtual T GetData() const = 0;
};

template <typename T>
class DuoObserver : public IObserver<T>
{
public:
	DuoObserver(IObservable<T>& first, IObservable<T>& second)
		: m_first{ &first }
		, m_second{ &second }
	{
	}

	void Update(IObservable<T>& observable) override
	{
		if (&observable == m_first)
		{
			OnFirstUpdate(observable);
		}
		else if (&observable == m_second)
		{
			OnSecondUpdate(observable);
		}
	}

	virtual ~DuoObserver() = default;

protected:
	virtual void OnFirstUpdate(const IObservable<T>& observable) = 0;
	virtual void OnSecondUpdate(const IObservable<T>& observable) = 0;

private:
	IObservable<T>* m_first;
	IObservable<T>* m_second;
};

template <typename T>
class MonoObserver : public IObserver<T>
{
public:
	MonoObserver(IObservable<T>& observable)
		: m_observable{ &observable }
	{
	}

	void Update(IObservable<T>& observable) override
	{
		if (&observable == m_observable)
		{
			OnUpdate(observable);
		}
	}

	virtual ~MonoObserver() = default;

protected:
	virtual void OnUpdate(const IObservable<T>& observable) = 0;
	IObservable<T>* m_observable;
};

template <typename T>
class Observable : public IObservable<T>
{
public:
	typedef IObserver<T> ObserverType;

	void RegisterObserver(const std::weak_ptr<ObserverType>& observer, uint32_t priority) override
	{
		if (observer.expired())
		{
			return;
		}

		if (m_observerPriorities.find(observer) != m_observerPriorities.end())
		{
			return;
		}

		if (m_priorityObservers.find(priority) != m_priorityObservers.end())
		{
			return;
		}

		m_observerPriorities.emplace(observer, priority);
		m_priorityObservers.emplace(priority, observer);
	}

	void NotifyObservers() override
	{
		std::vector<std::pair<uint32_t, std::weak_ptr<ObserverType>>> observers;
		observers.assign(m_priorityObservers.begin(), m_priorityObservers.end());

		for (const auto& [priority, weakObserver] : observers)
		{
			if (auto observer = weakObserver.lock())
			{
				observer->Update(*this);
			}
			else
			{
				RemoveObserver(weakObserver);
			}
		}
	}

	void RemoveObserver(const std::weak_ptr<ObserverType>& observer) override
	{
		auto it = m_observerPriorities.find(observer);
		if (it == m_observerPriorities.end())
		{
			return;
		}

		auto priority = it->second;
		m_priorityObservers.erase(priority);
		m_observerPriorities.erase(it);
	}

	uint32_t GetObserverCount() const
	{
		return m_priorityObservers.size();
	}

	virtual ~Observable() = default;

private:
	std::map<uint32_t, std::weak_ptr<ObserverType>> m_priorityObservers;
	std::map<std::weak_ptr<ObserverType>, uint32_t, std::owner_less<std::weak_ptr<ObserverType>>> m_observerPriorities;
};
