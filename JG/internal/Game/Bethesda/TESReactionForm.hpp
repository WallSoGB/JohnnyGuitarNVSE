#pragma once

#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

class TESFaction;

struct GroupReaction {
	TESForm*		pForm;
	int32_t			iReaction;
	FIGHT_REACTION	eFightReaction;
};

class TESReactionForm : public BaseFormComponent {
public:
	TESReactionForm();
	~TESReactionForm();

	BSSimpleList<GroupReaction*> kReactions;
	uint8_t						 ucGroupFormType;

	BSSimpleList<GroupReaction*>* GetReactionList();
	const BSSimpleList<GroupReaction*>* GetReactionList() const;

	int32_t GetReaction(const TESForm* apForm) const;
	void SetReaction(TESForm* apForm, int32_t aiReaction);

	FIGHT_REACTION GetFactionCombatRelation(const TESForm* apForm) const;
#ifdef GAME
	void SetFightReaction(TESForm* apForm, FIGHT_REACTION aeFightReaction);
#endif
};

ASSERT_SIZE(TESReactionForm, 0x10);