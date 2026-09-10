#include "TESActorBaseData.hpp"

// GAME - 0x5D8A70
const BSSimpleList<FactionRank*>* TESActorBaseData::GetFactionList() const {
	return &kFactions;
}

// GAME - 0x5D8A70
BSSimpleList<FactionRank*>* TESActorBaseData::GetFactionList() {
	return &kFactions;
}

// GAME - 0x47D680
// GECK - 0x4EAF70
int32_t TESActorBaseData::GetFactionRank(const TESFaction* apFaction, bool abPlayer) const {
#ifdef GAME
	return ThisCall<int32_t>(0x47D680, this, apFaction, abPlayer);
#else
	return ThisCall<int32_t>(0x4EAF70, this, apFaction, abPlayer);
#endif
}

// GAME - 0x47CDB0
// GECK - 0x4EAF20
bool TESActorBaseData::GetUsesLeveledTemplate() const {
#ifdef GAME
	return ThisCall<bool>(0x47CDB0, this);
#else
	return ThisCall<bool>(0x4EAF20, this);
#endif
}

// GAME - 0x47E040
// GECK - 0x4EA760
uint32_t TESActorBaseData::GetAlignmentForKarma(float afKarma) {
#ifdef GAME
	return CdeclCall<uint32_t>(0x47E040, afKarma);
#else
	return CdeclCall<uint32_t>(0x4EA760, afKarma);
#endif
}

#ifdef GAME
// GAME - 0x47E0E0
const char* TESActorBaseData::GetKarmicTitle(Actor* apActor) {
	return CdeclCall<const char*>(0x47E0E0, apActor);
}
#endif
