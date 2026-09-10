#pragma once

#include "BSSimpleList.hpp"

class TESPackage;
class Actor;

class PackageList : public BSSimpleList<TESPackage*> {
public:
	TESPackage* FindCurrentPackage(Actor* apActor) const;
	void FindMissedPackages(Actor* apActor, BSSimpleList<TESPackage*>& arOut, float afStartDate, float afhour) const;
};

ASSERT_SIZE(PackageList, 0x8);