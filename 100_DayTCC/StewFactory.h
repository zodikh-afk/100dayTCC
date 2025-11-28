#pragma once
#include "ItemFactory.h"
#include "Stew.h"

class StewFactory : public ItemFactory
{
	std::unique_ptr<Item> CreateItem() const override;

};

