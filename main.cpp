#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "SpaceSearcher.h"

#define THROW_IF_NON_POSITIVE(x) if ((x) <= 0) { throw std::out_of_range(""); }

using time_point = std::chrono::steady_clock::time_point;

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		puts("Usage: <program name> [N] [max_time] [time_1, time_2, ..., time_N]");
		puts("e.g. <program name> 5 30 1 3 6 8 12");

		return EXIT_FAILURE;
	}

	try 
	{
		const int n = std::stoi(argv[1]);
		THROW_IF_NON_POSITIVE(n);

		if (argc != n + 3)
		{
			puts("Invalid number of parameters");
			return EXIT_FAILURE;
		}

		const int max_time = std::stoi(argv[2]);
		THROW_IF_NON_POSITIVE(max_time);

		std::vector<Person*> people;
		
		for (int i = 3; i < argc; ++i)
		{
			const int timeToCross = std::stoi(argv[i]);
			THROW_IF_NON_POSITIVE(timeToCross);
			people.emplace_back(new Person(std::stoi(argv[i])));
		}

		State* initialState = new State(people);

		if (!initialState)
		{
			throw std::bad_alloc();
		}

		SpaceSearcher spaceSearcher;

		time_point begin = std::chrono::steady_clock::now();
		std::vector<State*> solution = std::move(spaceSearcher.Search(initialState, max_time));
		time_point end = std::chrono::steady_clock::now();

		if (solution.empty())
		{
			puts("No solution found");
		}

		else
		{
			for (State* state : solution)
			{
				state->print();
				puts("\n========================================");
			}

			printf("\nSolution Cost: %d", solution.back()->getScore());
			printf("\nSolution found in %lf seconds\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0);
		}

		delete initialState;

		for (Person* person : people)
		{
			delete person;
		}

		return EXIT_SUCCESS;
	} 
	catch (std::out_of_range& e)
	{
		puts("Number out of range");
	}
	catch (std::invalid_argument& e)
	{
		puts("Cannot convert to integer");
	}
	catch (std::bad_alloc& e)
	{
		puts("Out of memory");
	}

	return EXIT_FAILURE;
}