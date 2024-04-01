#include "Frontier.h"

void Frontier::Push(State* state) noexcept
{
	frontier.push(state);
}

void Frontier::Push(const std::vector<State*>& states) noexcept
{
	for (State* state : states)
	{
		frontier.push(state);
	}
}

State* Frontier::Pop(void) noexcept
{
	State* top = frontier.top();
	frontier.pop();
	return top;
}

bool Frontier::IsEmpty(void) const noexcept
{
	return frontier.empty();
}