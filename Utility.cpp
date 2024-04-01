#include "Utility.h"

#include <cmath>

bool util::isPrime(int p)
{
	if (p < 2)
	{
		return false;
	}

	if (p == 2)
	{
		return true;
	}

	const int nSquareRoot = static_cast<int>(sqrt(p));

	for (int i = 3; i <= nSquareRoot; ++i)
	{
		if (p % i == 0)
		{
			return false;
		}
	}

	return true;
}