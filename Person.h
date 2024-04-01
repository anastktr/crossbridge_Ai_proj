#pragma once

class Person
{
public:
	Person(int timeToCrossBridge);

	inline int getTimeToCrossBridge(void) const noexcept { return timeToCrossBridge; }
	inline int getId(void) const noexcept { return id; }

	bool operator==(const Person& p) const;
	bool operator<(const Person& p) const;

	bool operator==(const Person* p) const;
	bool operator<(const Person* p) const;

private:
	int id = 0;
	int timeToCrossBridge = 0;
};

/*
*  Used as an std::set template argument
*/
struct PersonComp
{
	bool operator()(const Person* lhs, const Person* rhs) const
	{
		return lhs->getTimeToCrossBridge() > rhs->getTimeToCrossBridge();
	}
};
