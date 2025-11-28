#include "StewFactory.h"

std::unique_ptr<Item> StewFactory::CreateItem() const
{
	return std::make_unique<Stew>();
}
