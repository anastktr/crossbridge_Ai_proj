#include "State.h"
#include "Utility.h"

#include <cassert>
#include <algorithm>
#include <set>
#include <vector>
#include <cstdio>

class StateManager
{
public:
	State* Create(State* parent)
	{
		State* newState = new State(parent);
		if (!newState)
			throw std::bad_alloc();
		createdStates.emplace_back(newState);
		return newState;
	}

	~StateManager(void)
	{
		for (State* state : createdStates)
		{
			delete state;
		}
	}

private:
	std::vector<State*> createdStates;
};

static StateManager s_stateManager;

// We'll use these globally in order to reduce memory usage
static int fastestTimeToCross = 0;
static int n = 0;

State::State(const std::vector<Person*>& people)  //gia tin prwti fora arxiki katastasi
	: rightSide(people.begin(), people.end())
{
	id = CalculateID(); //gia tin katastasi

	if (!people.empty())
	{
		// We'll calculate the minimum time it takes for someone to cross the bridge so 
		// we can use it later on in our heuristic function
		fastestTimeToCross = (*std::min_element(people.begin(), people.end(), [](Person* a,Person* b) {
			return a->getTimeToCrossBridge() < b->getTimeToCrossBridge();
		}))->getTimeToCrossBridge();
	}

	// We are also going to need this, so as not to calculate it each time the heuristic function is calculated
	n = people.size();
}

State::State(State* copyState)
{
	assert(copyState); //debugging

	costFromRootToSelf = copyState->costFromRootToSelf;
	id = copyState->id;

	leftSide  = copyState->leftSide;
	rightSide = copyState->rightSide;

	parentState = copyState;
}

bool State::operator<(const State& state) const
{
	return astarScore < state.astarScore;
}

bool State::operator==(const State& state) const
{
	return id == state.id;
}

bool State::isFinal(void) const noexcept
{
	return rightSide.empty();
}

std::vector<State*> State::getChildStates(void) noexcept
{
	std::vector<State*> children;

	if (isLampOnRight)
	{
		// Here we generate all possible pairs of people on the right side
		// For example if rightSide = { 3, 5, 7 } (IDs) then the generated
		// pairs are {3, 5}, {3, 7}, {5, 7}
		util::pair_list<Person*> crossingPairs = std::move(util::GenerateAllPairs(rightSide));  //move gia optimize

		// Here we'll create a clone state for each pair, make the said
		// pair cross the bridge, and add the new state to the children vector
		for (auto& pair : crossingPairs)
		{
			State* state = s_stateManager.Create(this);
			state->CrossWithLampFromRight(pair.first, pair.second);
			children.emplace_back(std::move(state));
		}
	}

	else
	{
		// For each person on the left side, we create a clone state
		// and make said person cross the bridge and add the new state to the vector
		for (Person* person : leftSide)
		{
			State* state = s_stateManager.Create(this);
			state->ReturnWithLampFromLeft(person);
			children.emplace_back(std::move(state));
		}
	}

	return children;
}

void State::CrossWithLampFromRight(Person* p1, Person* p2) noexcept
{
	ChangeBridgeSides(p1);
	ChangeBridgeSides(p2);
	isLampOnRight = false;

	id = CalculateID();
	costFromRootToSelf += std::max(p1->getTimeToCrossBridge(), p2->getTimeToCrossBridge());  //g

	EvaluateAStarScore();
}

void State::ReturnWithLampFromLeft(Person* p) noexcept
{
	ChangeBridgeSides(p);
	isLampOnRight = true;

	id = CalculateID();
	costFromRootToSelf += p->getTimeToCrossBridge();  //g

	EvaluateAStarScore();
}

void State::ChangeBridgeSides(Person* p) noexcept
{
	if (IsPersonOnSide(p, leftSide)) {
		leftSide.erase(std::find(leftSide.begin(), leftSide.end(), p));
		rightSide.insert(p);
	} else {
		rightSide.erase(std::find(rightSide.begin(), rightSide.end(), p));
		leftSide.insert(p);
	}
}

bool State::IsPersonOnSide(const Person* person, const Side& side)
{
	return std::find(side.begin(), side.end(), person) != side.end();
}

void State::EvaluateAStarScore(void) noexcept
{
	int i = 0, costFromSelfToGoal = 0;

	// Here we calculate Zs as described in the PDF.
	// A brief explanation: We match each person x to the slowest person y
	// that is faster than x, as long as both x and y aren't matched up.
	// For example, for {1, 6, 12, 3, 8} we get
	// {12, 8}, {6, 3}, {1}
	// Then we sum the time it takes for each pair to cross the bridge.
	// The way we do this is iterating over every person in a descending order
	// (based on the time it takes to cross the bridge) and only account for the
	// time of every other person, starting from the first one, since this way
	// we basically add the max time of each pair
	// for example for 12, 8, 6, 3, 1 we skip 8 and 3, which don't change
	// the crossing times of {12, 8} and {6, 3}, and add 12, 6 and 1
	for (const Person* person : rightSide)
	{
		if (i % 2 == 0)
		{
			costFromSelfToGoal += person->getTimeToCrossBridge();
		}

		++i;
	}

	int movesRemainingFromLeftToRight = rightSide.size();

	if (isLampOnRight && movesRemainingFromLeftToRight >= 2)
	{
		movesRemainingFromLeftToRight -= 2; 
	}

	costFromSelfToGoal += movesRemainingFromLeftToRight * fastestTimeToCross; 

	astarScore = costFromRootToSelf + costFromSelfToGoal;
}

/*
*  Each person is assigned a unique prime number as their id, starting from 3.
* 
*  The state ID is the product of the IDs of the people on the least populated side, plus
*  one if the lamp is on the right side. By calculating the state ID using this method,
*  it is ensured by the fundamental theorem of arithmetic that the state ID will be unique.
* 
*  If the least populated side happens to be empty, then the state ID is either 1 or 2,
*  depending on the side the lamp is located.
*/
int State::CalculateID(void) noexcept
{
	Side* leastPopulatedSide = (rightSide.size() < leftSide.size()) ? &rightSide : &leftSide;

	int id = 1;

	for (const Person* person : *leastPopulatedSide)
	{
		id *= person->getId();
	}

	return isLampOnRight ? id + 1 : id;
}

State* State::getParent(void) const
{
	return parentState;
}

void State::SetParent(State* state)
{
	this->parentState = state;
}

void State::print(void) const
{
	printf("\nLamp is on the %s\n", isLampOnRight ? "right" : "left");

	printf("\nRight: ");

	for (const Person* person : rightSide) 
	{
		printf("%d ", person->getTimeToCrossBridge());
	}

	printf("\nLeft:  ");

	for (const Person* person : leftSide)
	{
		printf("%d ", person->getTimeToCrossBridge());
	}

	printf("\nG: %d\n", costFromRootToSelf);
}