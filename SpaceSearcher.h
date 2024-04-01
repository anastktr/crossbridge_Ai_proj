#pragma once

#include <vector>

#include "Frontier.h"

class SpaceSearcher
{
public:
	std::vector<State*> Search(State* initial, int maxCost);

private:
	std::vector<State*> GetPathToState(State* state);
};

