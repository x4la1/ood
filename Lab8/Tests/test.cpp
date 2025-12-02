#include "pch.h"
#include "GumBallMachineWithDynamicallyCreatedState.h"

// GumBallMachineWithDynamicallyCreatedState Tests
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

TEST_F(NoQuarterStateTest, InsertQuarterTransitionsToHasQuarterState)
{
	state->InsertQuarter();
	EXPECT_EQ(machine->setHasQuarterStateCount, 1);
}

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

TEST_F(HasQuarterStateTest, EjectQuarterTransitionsToNoQuarterState)
{
	state->EjectQuarter();
	EXPECT_EQ(machine->setNoQuarterStateCount, 1);
}

TEST_F(HasQuarterStateTest, TurnCrankTransitionsToSoldState)
{
	machine->SetBallCount(1);
	state->TurnCrank();
	EXPECT_EQ(machine->setSoldStateCount, 1);
}

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

TEST_F(SoldStateTest, DispenseLastGumballTransitionsToSoldOut)
{
	machine->SetBallCount(0);
	state->Dispense();

	EXPECT_EQ(machine->releaseBallCount, 1);
	EXPECT_EQ(machine->setSoldOutStateCount, 1);
}

TEST_F(SoldStateTest, DispenseWithGumballsLeftTransitionsToNoQuarter)
{
	machine->SetBallCount(5);
	state->Dispense();

	EXPECT_EQ(machine->releaseBallCount, 1);
	EXPECT_EQ(machine->setNoQuarterStateCount, 1);
}

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

TEST_F(SoldOutStateTest, RefillWithZeroGumballsTransitionsToSoldOutState)
{
	state->Refill(0);
	EXPECT_EQ(machine->setSoldOutStateCount, 1);
}

TEST_F(SoldOutStateTest, RefillGumballsTransitionsToNoQuarterState)
{
	state->Refill(5);
	EXPECT_EQ(machine->setNoQuarterStateCount, 1);
}

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

TEST_F(GumballMachineIntegrationTest, InitialStateWithBalls)
{
	with_dynamic_state::GumballMachine machine(5);
	std::string result = machine.ToString();
	EXPECT_TRUE(result.find("Inventory: 5 gumballs") != std::string::npos);
	EXPECT_TRUE(result.find("Machine is waiting for quarter") != std::string::npos);
}

TEST_F(GumballMachineIntegrationTest, FullWorkflow)
{
	with_dynamic_state::GumballMachine machine(2);
	std::ostringstream captured_output;
	{
		CoutRedirect redirect(captured_output.rdbuf());
		machine.InsertQuarter();
	}
	EXPECT_EQ(captured_output.str(), "You inserted a quarter\n");
	EXPECT_TRUE(machine.ToString().find("waiting for turn of crank") != std::string::npos);

	captured_output.str("");
	{
		CoutRedirect redirect(captured_output.rdbuf());
		machine.TurnCrank();
	}
	EXPECT_EQ(captured_output.str(), "You turned...\nA gumball comes rolling out the slot...\n");
	EXPECT_TRUE(machine.ToString().find("Inventory: 1 gumball") != std::string::npos);
	EXPECT_TRUE(machine.ToString().find("waiting for quarter") != std::string::npos);
}