#include "Person.h"
#include "Utility.h"

Person::Person(int timeToCrossBridge)
{
	static int next_prime = 3;

	this->timeToCrossBridge = timeToCrossBridge;
	this->id = next_prime;

	do {
		next_prime += 2;
	} while (!util::isPrime(next_prime));
}

bool Person::operator==(const Person& p) const
{
	return this->id == p.id;
}

bool Person::operator<(const Person& p) const
{
	return this->timeToCrossBridge < p.timeToCrossBridge;
}

bool Person::operator==(const Person* p) const
{
	return this->id == p->id;
}

bool Person::operator<(const Person* p) const
{
	return this->timeToCrossBridge < p->timeToCrossBridge;
}