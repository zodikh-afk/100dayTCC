#pragma once
#include <memory>
#include "Item.h"

class ItemFactory
{
public:
	virtual ~ItemFactory() = default;
	virtual std::unique_ptr<Item> CreateItem() const = 0;
};

