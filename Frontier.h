#pragma once

#include "State.h"

#include <queue>

class Frontier
{
public:
	void Push(State* state) noexcept;
	void Push(const std::vector<State*>&) noexcept;
	State* Pop(void) noexcept;

	bool IsEmpty(void) const noexcept;
	
private:
	std::priority_queue<State*, std::vector<State*>, StateComparator> frontier;
};

