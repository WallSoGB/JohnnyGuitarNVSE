#include "NamedSpellLights.hpp"
#include "GameEffects.h"
#include "GameRTTI.h"

#include "Bethesda/MagicSystem.hpp"

namespace NamedSpellLights {

	STACK_FRAME_OPT_DISABLE
		NiPointLight* __fastcall SetLightNameHook(NiPointer<NiPointLight>& arLight) {
		uint8_t* pEBP = GetParentBasePtr(_AddressOfReturnAddress());
		const ActiveEffect* pEffect = *reinterpret_cast<ActiveEffect**>(pEBP - 0x60);
		const MagicItem* pMagicItem = pEffect->magicItem;
		if (pMagicItem) {
			const TESForm* pMagicItemForm = DYNAMIC_CAST(pMagicItem, MagicItem, TESForm);
			if (pMagicItemForm == MagicSystem::pPipBoyLightSpell) {
				// For consistenty with Mobile Pipboy Light
				arLight->SetName("PipboyLight");
			}
			else {
				const char* pEDID = pMagicItemForm->GetFormEditorID();
				if (pEDID && pEDID[0]) {
					char cName[MAX_PATH];
					our_snprintf(cName, sizeof(cName), "%s_PointLight", pEDID);
					arLight->SetName(cName);
				}
			}
		}
		return arLight.m_pObject;
	}
	STACK_FRAME_OPT_RESET

	void Install() {
		HookUtils::ReplaceCall(0x80ECD8, SetLightNameHook);
	}

}