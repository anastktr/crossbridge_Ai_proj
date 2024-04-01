#include "SpaceSearcher.h"

std::vector<State*> SpaceSearcher::Search(State* initial, int maxCost)
{
	Frontier frontier;
	frontier.Push(initial);

	while (!frontier.IsEmpty())
	{
		State* currentState = frontier.Pop();

		if (currentState->getCostFromRoot() <= maxCost)
		{
			if (currentState->isFinal())
			{
				return GetPathToState(currentState);
			}

			frontier.Push(currentState->getChildStates());
		}
	}

	return std::vector<State*>();
}

std::vector<State*> SpaceSearcher::GetPathToState(State* state)
{
	std::vector<State*> path;
	do {
		path.insert(path.begin(), state);
		state = state->getParent();
	} while (state != nullptr);

	return path;
}