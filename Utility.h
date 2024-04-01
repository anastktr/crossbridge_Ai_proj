#pragma once

#include <list>
#include <set>

namespace util
{
	template <typename T>
	using pair_list = std::list<std::pair<T, T>>;

	template <typename T, typename Comp> 
	pair_list<T> GenerateAllPairs(const std::multiset<T, Comp>& s)
	{
		pair_list<T> pairs;

		// match the 1st with 2nd, 3rd, 4rth etc
		// match the 2nd with 3rd, 4rth etc.
		// match the 3rd with 4rth etc.
		// etc.
		for (auto it = s.begin(); it != s.end(); ++it)
		{
			for (auto right = std::next(it); right != s.end(); ++right)
			{
				pairs.emplace_back(std::make_pair(*it, *right));
			}
		}

		return pairs;
	}

	bool isPrime(int p);
}