#pragma once

#include <unordered_set>
#include <vector>
#include <set>

#include "Person.h"

class State
{
public:
	State(const std::vector<Person*>& people);
	State(State* copyState);

	bool isFinal(void) const noexcept;

	std::vector<State*> getChildStates(void) noexcept;

	int getScore() const { return astarScore; }
	int getCostFromRoot() const { return costFromRootToSelf; }
	int getId(void) const noexcept { return id; }

	State* getParent(void) const;
	void SetParent(State* state);

	void print(void) const;

	bool operator==(const State& state) const;
	bool operator<(const State& state) const;

private:
	using Side = std::multiset<Person*, PersonComp>;

	inline void EvaluateAStarScore(void) noexcept;
	inline void ChangeBridgeSides(Person* p) noexcept;
	inline void ReturnWithLampFromLeft(Person* p) noexcept;
	inline void CrossWithLampFromRight(Person* p1, Person* p2) noexcept;

	int CalculateID(void) noexcept;

	inline bool IsPersonOnSide(const Person* person, const Side& side);

private:
	Side leftSide;
	Side rightSide;

	State* parentState = nullptr;

	int astarScore = 0, costFromRootToSelf = 0;
	bool isLampOnRight = true;

	int id = 0;
};

/*
*  Used as an std::unordered_set template argument
*/
struct StatePtrHash
{
	std::size_t operator()(const State* state) const
	{
		return state->getId();
	}
};

/*
*  Used as an std::priority_queue template argument
*/
struct StateComparator
{
	bool operator()(const State* lhs, const State* rhs)
	{
		return lhs->getScore() > rhs->getScore();
	}
};