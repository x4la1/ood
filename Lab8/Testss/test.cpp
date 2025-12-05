#include "pch.h"
#include "GumBallMachineWithDynamicallyCreatedState.h"
#include "NaiveMultiGumballMachine.h"
#include "MultiGumballMachineDynamic.h"

//========================================================
// -- GumBallMachineWithDynamicallyCreatedState Tests --
//========================================================
class MockGumballMachine : public with_dynamic_state::IGumballMachine
{
public:

	int releaseBallCount = 0;
	int setSoldOutStateCount = 0;
	int setNoQuarterStateCount = 0;
	int setSoldStateCount = 0;
	int setHasQuarterStateCount = 0;


	void ReleaseBall() override { releaseBallCount++; }
	unsigned GetBallCount() const override { return m_ballCount; }
	void SetSoldOutState() override { setSoldOutStateCount++; }
	void SetNoQuarterState() override { setNoQuarterStateCount++; }
	void Refill(unsigned count) { m_ballCount = count; }
	void SetSoldState() override { setSoldStateCount++; }
	void SetHasQuarterState() override { setHasQuarterStateCount++; }

	void SetBallCount(unsigned count) { m_ballCount = count; }
	void Reset()
	{
		releaseBallCount = 0;
		setSoldOutStateCount = 0;
		setNoQuarterStateCount = 0;
		setSoldStateCount = 0;
		setHasQuarterStateCount = 0;
	}

private:
	unsigned m_ballCount = 0;
};

class CoutRedirect
{
public:
	CoutRedirect(std::streambuf* new_buffer) : old_buf(std::cout.rdbuf(new_buffer)) {}
	~CoutRedirect() { std::cout.rdbuf(old_buf); }
private:
	std::streambuf* old_buf;
};

// NoQuarterState Tests
class NoQuarterStateTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		machine = std::make_unique<MockGumballMachine>();
		state = std::make_unique<with_dynamic_state::NoQuarterState>(*machine);
	}
	std::unique_ptr<MockGumballMachine> machine;
	std::unique_ptr<with_dynamic_state::NoQuarterState> state;
};

//Вставление монетки
TEST_F(NoQuarterStateTest, InsertQuarterTransitionsToHasQuarterState)
{
	state->InsertQuarter();
	EXPECT_EQ(machine->setHasQuarterStateCount, 1);
}

//Вызов невозможных комманд для текущего состояния
TEST_F(NoQuarterStateTest, NoCallsWithOtherCommandsInNoQuarterState)
{
	state->Dispense();
	state->EjectQuarter();
	state->TurnCrank();
	state->Refill(5);
	EXPECT_EQ(machine->releaseBallCount, 0);
	EXPECT_EQ(machine->setSoldOutStateCount, 0);
	EXPECT_EQ(machine->setNoQuarterStateCount, 0);
	EXPECT_EQ(machine->setSoldStateCount, 0);
	EXPECT_EQ(machine->setHasQuarterStateCount, 0);
}

// HasQuarterState Tests
class HasQuarterStateTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		machine = std::make_unique<MockGumballMachine>();
		state = std::make_unique<with_dynamic_state::HasQuarterState>(*machine);
	}
	std::unique_ptr<MockGumballMachine> machine;
	std::unique_ptr<with_dynamic_state::HasQuarterState> state;
};

//Возврат монеток
TEST_F(HasQuarterStateTest, EjectQuarterTransitionsToNoQuarterState)
{
	state->EjectQuarter();
	EXPECT_EQ(machine->setNoQuarterStateCount, 1);
}

//Дернуть рычаг
TEST_F(HasQuarterStateTest, TurnCrankTransitionsToSoldState)
{
	machine->SetBallCount(1);
	state->TurnCrank();
	EXPECT_EQ(machine->setSoldStateCount, 1);
}

//Вызов невозможных комманд для текущего состояния
TEST_F(HasQuarterStateTest, NoCallsWithOtherCommandsInHasQuarterState)
{
	state->Dispense();
	state->InsertQuarter();
	state->Refill(5);
	EXPECT_EQ(machine->releaseBallCount, 0);
	EXPECT_EQ(machine->setSoldOutStateCount, 0);
	EXPECT_EQ(machine->setNoQuarterStateCount, 0);
	EXPECT_EQ(machine->setSoldStateCount, 0);
	EXPECT_EQ(machine->setHasQuarterStateCount, 0);
}

// SoldState Tests
class SoldStateTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		machine = std::make_unique<MockGumballMachine>();
		state = std::make_unique<with_dynamic_state::SoldState>(*machine);
	}
	std::unique_ptr<MockGumballMachine> machine;
	std::unique_ptr<with_dynamic_state::SoldState> state;
};

//Выдача шарика с переходом в состояние soldout
TEST_F(SoldStateTest, DispenseLastGumballTransitionsToSoldOut)
{
	machine->SetBallCount(0);
	state->Dispense();

	EXPECT_EQ(machine->releaseBallCount, 1);
	EXPECT_EQ(machine->setSoldOutStateCount, 1);
}

//Выдача шарика с переходом в состояние no quarter
TEST_F(SoldStateTest, DispenseWithGumballsLeftTransitionsToNoQuarter)
{
	machine->SetBallCount(5);
	state->Dispense();

	EXPECT_EQ(machine->releaseBallCount, 1);
	EXPECT_EQ(machine->setNoQuarterStateCount, 1);
}

//Вызов невозможных комманд для текущего состояния
TEST_F(SoldStateTest, NoCallsWithOtherCommandsInSoldState)
{
	state->EjectQuarter();
	state->InsertQuarter();
	state->Refill(5);
	state->TurnCrank();
	EXPECT_EQ(machine->releaseBallCount, 0);
	EXPECT_EQ(machine->setSoldOutStateCount, 0);
	EXPECT_EQ(machine->setNoQuarterStateCount, 0);
	EXPECT_EQ(machine->setSoldStateCount, 0);
	EXPECT_EQ(machine->setHasQuarterStateCount, 0);
}

// SoldOutState Tests
class SoldOutStateTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		machine = std::make_unique<MockGumballMachine>();
		state = std::make_unique<with_dynamic_state::SoldOutState>(*machine);
	}
	std::unique_ptr<MockGumballMachine> machine;
	std::unique_ptr<with_dynamic_state::SoldOutState> state;
};

//Заполнение шариками с переходом в состояние soldout
TEST_F(SoldOutStateTest, RefillWithZeroGumballsTransitionsToSoldOutState)
{
	state->Refill(0);
	EXPECT_EQ(machine->setSoldOutStateCount, 0);
}

//Заполнение шариками с переходом в состояние no quarter
TEST_F(SoldOutStateTest, RefillGumballsTransitionsToNoQuarterState)
{
	state->Refill(5);
	EXPECT_EQ(machine->setNoQuarterStateCount, 1);
}

//Вызов невозможных комманд для текущего состояния
TEST_F(SoldOutStateTest, NoCallsWithOtherCommandInSoldOutState)
{
	state->Dispense();
	state->EjectQuarter();
	state->InsertQuarter();
	state->TurnCrank();
	EXPECT_EQ(machine->releaseBallCount, 0);
	EXPECT_EQ(machine->setSoldOutStateCount, 0);
	EXPECT_EQ(machine->setNoQuarterStateCount, 0);
	EXPECT_EQ(machine->setSoldStateCount, 0);
	EXPECT_EQ(machine->setHasQuarterStateCount, 0);
}


class GumballMachineIntegrationTest : public ::testing::Test
{
protected:
	void SetUp() override {}
	void TearDown() override {}
};

//Создание заполненного автомата
TEST_F(GumballMachineIntegrationTest, InitialStateWithBalls)
{
	with_dynamic_state::GumballMachine machine(5);
	std::string result = machine.ToString();
	EXPECT_TRUE(result.find("Inventory: 5 gumballs") != std::string::npos);
	EXPECT_TRUE(result.find("Machine is waiting for quarter") != std::string::npos);
}

//Создание пустого автомата
TEST_F(GumballMachineIntegrationTest, InitialStateWithoutBalls)
{
	with_dynamic_state::GumballMachine machine(0);
	std::string result = machine.ToString();
	EXPECT_TRUE(result.find("Inventory: 0 gumballs") != std::string::npos);
	EXPECT_TRUE(result.find("Machine is sold out") != std::string::npos);
}

//Использование автомата
TEST_F(GumballMachineIntegrationTest, FullWorkflow)
{
	with_dynamic_state::GumballMachine machine(2);
	std::ostringstream captured_output;

	machine.InsertQuarter();
	machine.TurnCrank();

	machine.InsertQuarter();
	machine.TurnCrank();

	std::string result = machine.ToString();
	EXPECT_TRUE(result.find("Inventory: 0 gumballs") != std::string::npos);
	EXPECT_TRUE(result.find("Machine is sold out") != std::string::npos);
}


//========================================================
// -- MultiGumballMachine Tests --
//========================================================

class MockMultiGumballMachine : public multi_with_dynamic_state::IGumballMachine
{
public:
	int releaseBallCount = 0;
	int setSoldOutStateCount = 0;
	int setNoQuarterStateCount = 0;
	int setSoldStateCount = 0;
	int setHasQuarterStateCount = 0;
	int refillCallCount = 0;
	unsigned lastRefillCount = 0;

	unsigned m_ballCount = 0;
	unsigned m_quartersCount = 0;

	void ReleaseBall() override { releaseBallCount++; }
	unsigned GetBallCount() const override { return m_ballCount; }
	void SetSoldOutState() override { setSoldOutStateCount++; }
	void SetNoQuarterState() override { setNoQuarterStateCount++; }
	void SetSoldState() override { setSoldStateCount++; }
	void SetHasQuarterState() override { setHasQuarterStateCount++; }
	void AddQuarter() override { m_quartersCount++; }
	unsigned GetQuartersCount() const override { return m_quartersCount; }
	void EjectQuarters() override { m_quartersCount = 0; }
	void Refill(unsigned count) override
	{
		refillCallCount++;
		lastRefillCount = count;
		m_ballCount = count;
	}

	void SetBallCount(unsigned count) { m_ballCount = count; }
	void SetQuartersCount(unsigned count) { m_quartersCount = count; }
	void Reset() {
		releaseBallCount = 0; setSoldOutStateCount = 0; setNoQuarterStateCount = 0;
		setSoldStateCount = 0; setHasQuarterStateCount = 0; refillCallCount = 0;
		lastRefillCount = 0; m_ballCount = 0; m_quartersCount = 0;
	}
};

// NoQuarterState Tests
class MultiNoQuarterStateTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		machine = std::make_unique<MockMultiGumballMachine>();
		state = std::make_unique<multi_with_dynamic_state::NoQuarterState>(*machine);
	}
	std::unique_ptr<MockMultiGumballMachine> machine;
	std::unique_ptr<multi_with_dynamic_state::NoQuarterState> state;
};

TEST_F(MultiNoQuarterStateTest, RefillWithGumballsStaysInNoQuarterState)
{
	state->Refill(5);
	EXPECT_EQ(machine->refillCallCount, 1);
	EXPECT_EQ(machine->lastRefillCount, 5);
	EXPECT_EQ(machine->GetBallCount(), 5);
	EXPECT_EQ(machine->setNoQuarterStateCount, 0);
	EXPECT_EQ(machine->setSoldOutStateCount, 0);
}

TEST_F(MultiNoQuarterStateTest, RefillWithZeroGumballsTransitionsToSoldOut)
{
	state->Refill(0);
	EXPECT_EQ(machine->refillCallCount, 1);
	EXPECT_EQ(machine->lastRefillCount, 0);
	EXPECT_EQ(machine->GetBallCount(), 0);
	EXPECT_EQ(machine->setSoldOutStateCount, 1);
}

// HasQuarterState Tests
class MultiHasQuarterStateTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		machine = std::make_unique<MockMultiGumballMachine>();
		machine->SetQuartersCount(2);
		state = std::make_unique<multi_with_dynamic_state::HasQuarterState>(*machine);
	}
	std::unique_ptr<MockMultiGumballMachine> machine;
	std::unique_ptr<multi_with_dynamic_state::HasQuarterState> state;
};

TEST_F(MultiHasQuarterStateTest, InsertMoreThanFiveQuartersInHasQuarterState)
{
	state->InsertQuarter();
	state->InsertQuarter();
	state->InsertQuarter();
	EXPECT_EQ(machine->m_quartersCount, 5);
	state->InsertQuarter();
	EXPECT_EQ(machine->m_quartersCount, 5);
}

TEST_F(MultiHasQuarterStateTest, EjectAllQuartersInHasQuarterState)
{
	state->EjectQuarter();
	EXPECT_EQ(machine->m_quartersCount, 0);
	EXPECT_EQ(machine->setNoQuarterStateCount, 1);
}

TEST_F(MultiHasQuarterStateTest, RefillWithGumballsStaysInHasQuarterState)
{
	state->Refill(5);
	EXPECT_EQ(machine->refillCallCount, 1);
	EXPECT_EQ(machine->GetBallCount(), 5);
	EXPECT_EQ(machine->setHasQuarterStateCount, 0);
	EXPECT_EQ(machine->setSoldOutStateCount, 0);
	EXPECT_EQ(machine->GetQuartersCount(), 2);
}

TEST_F(MultiHasQuarterStateTest, RefillWithZeroGumballsTransitionsToSoldOut)
{
	state->Refill(0);
	EXPECT_EQ(machine->refillCallCount, 1);
	EXPECT_EQ(machine->GetBallCount(), 0);
	EXPECT_EQ(machine->setSoldOutStateCount, 1);
	EXPECT_EQ(machine->GetQuartersCount(), 2);
}

// SoldOutState Tests
class MultiSoldOutStateTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		machine = std::make_unique<MockMultiGumballMachine>();
		state = std::make_unique<multi_with_dynamic_state::SoldOutState>(*machine);
	}
	std::unique_ptr<MockMultiGumballMachine> machine;
	std::unique_ptr<multi_with_dynamic_state::SoldOutState> state;
};

TEST_F(MultiSoldOutStateTest, EjectAllQuartersInSoldOutState)
{
	machine->SetQuartersCount(4);
	state->EjectQuarter();
	EXPECT_EQ(machine->m_quartersCount, 0);
	EXPECT_EQ(machine->releaseBallCount, 0);
	EXPECT_EQ(machine->setSoldOutStateCount, 0);
	EXPECT_EQ(machine->setNoQuarterStateCount, 0);
	EXPECT_EQ(machine->setSoldStateCount, 0);
	EXPECT_EQ(machine->setHasQuarterStateCount, 0);
	EXPECT_EQ(machine->refillCallCount, 0);
	EXPECT_EQ(machine->GetQuartersCount(), 0);
}

TEST_F(MultiSoldOutStateTest, RefillWithZeroGumballsStaysInSoldOutState)
{
	state->Refill(0);
	EXPECT_EQ(machine->refillCallCount, 1);
	EXPECT_EQ(machine->GetBallCount(), 0);
	EXPECT_EQ(machine->setSoldOutStateCount, 0);
	EXPECT_EQ(machine->setNoQuarterStateCount, 0);
	EXPECT_EQ(machine->setHasQuarterStateCount, 0);
	EXPECT_EQ(machine->GetQuartersCount(), 0);
}

TEST_F(MultiSoldOutStateTest, RefillWithGumballsAndNoQuartersTransitionsToNoQuarterState)
{
	machine->SetQuartersCount(0);
	state->Refill(5);
	EXPECT_EQ(machine->refillCallCount, 1);
	EXPECT_EQ(machine->GetBallCount(), 5);
	EXPECT_EQ(machine->setNoQuarterStateCount, 1);
}

TEST_F(MultiSoldOutStateTest, RefillWithGumballsAndQuartersTransitionsToHasQuarterState)
{
	machine->SetQuartersCount(2);
	state->Refill(5);
	EXPECT_EQ(machine->refillCallCount, 1);
	EXPECT_EQ(machine->GetBallCount(), 5);
	EXPECT_EQ(machine->setHasQuarterStateCount, 1);
}

// SoldState Tests
class MultiSoldStateTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		machine = std::make_unique<MockMultiGumballMachine>();
		state = std::make_unique<multi_with_dynamic_state::SoldState>(*machine);
	}
	std::unique_ptr<MockMultiGumballMachine> machine;
	std::unique_ptr<multi_with_dynamic_state::SoldState> state;
};

TEST_F(MultiSoldStateTest, RefillDoesNothing)
{
	state->Refill(5);
	EXPECT_EQ(machine->refillCallCount, 0);
}

// -- NaiveGumballMachine Tests --

class NaiveGumballMachineTest : public ::testing::Test
{
protected:
	void SetUp() override {}
	void TearDown() override {}
};

TEST_F(NaiveGumballMachineTest, InsertMoreThanFiveQuarters)
{
	multi_naive::MultiGumballMachine machine(1);
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.InsertQuarter();
	std::string result = machine.ToString();
	EXPECT_TRUE(result.find("Inventory: 1 gumball 5 quarters") != std::string::npos);
	machine.InsertQuarter();
	result = machine.ToString();
	EXPECT_TRUE(result.find("Inventory: 1 gumball 5 quarters") != std::string::npos);
}

TEST_F(NaiveGumballMachineTest, RefillFromNoQuarterState)
{
	multi_naive::MultiGumballMachine machine(0);
	machine.Refill(5);
	std::string result = machine.ToString();
	EXPECT_TRUE(result.find("Inventory: 5 gumballs 0 quarters") != std::string::npos);
	EXPECT_TRUE(result.find("Machine is waiting for quarter") != std::string::npos);
}

TEST_F(NaiveGumballMachineTest, RefillFromHasQuarterState)
{
	multi_naive::MultiGumballMachine machine(1);
	machine.InsertQuarter();
	machine.Refill(5);
	std::string result = machine.ToString();
	EXPECT_TRUE(result.find("Inventory: 5 gumballs 1 quarter") != std::string::npos);
	EXPECT_TRUE(result.find("Machine is waiting for turn of crank") != std::string::npos);
}

TEST_F(NaiveGumballMachineTest, RefillFromSoldOutStateWithQuarters)
{
	multi_naive::MultiGumballMachine machine(1);
	machine.InsertQuarter();
	machine.InsertQuarter();
	machine.TurnCrank();
	machine.Refill(5);
	std::string result = machine.ToString();
	std::cout << result;
	EXPECT_TRUE(result.find("Inventory: 5 gumballs 1 quarter") != std::string::npos);
	EXPECT_TRUE(result.find("Machine is waiting for turn of crank") != std::string::npos);
}