#pragma once
class Item
{
private:
	int chance_to_spawning;
	int weight;
public:
	virtual ~Item() = default;
	virtual int GetChanceToSpawning() const = 0;
	virtual int GetWeight() const = 0;

	virtual void SetChanceToSpawning(int chance_to_spawning);
	virtual void SetWeight(int weight);
	
};

