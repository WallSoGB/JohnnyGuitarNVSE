#pragma once

#include "ActorValue.hpp"
#include "BSSimpleList.hpp"
#include "EffectArchetypes.hpp"
#include "EffectItem.hpp"

class Actor;

class EffectItemList : public BSSimpleList<EffectItem*> {
public:
	EffectItemList();
	~EffectItemList();

	virtual bool		IsMedicine() const;
	virtual bool		IsFood() const;
	virtual float		GetCost(Actor* apActor) const;
	virtual uint32_t	GetMaxEffectCount() const;
	virtual uint32_t	GetLevel() const;

	uint32_t uiHostileCount;

	bool CanBePoison() const;

#ifdef GAME
	float GetEffectScore(EffectArchetypes::Type aeType, MagicSystem::Range aeRange, ActorValue::Index auiActorValue) const;

	void GetEffectsString(char* apBuffer, uint32_t auiBufferSize) const;
#endif
};

ASSERT_SIZE(EffectItemList, 0x10);