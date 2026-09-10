#pragma once

#include "TESForm.hpp"
#include "BGSPreloadable.hpp"

class BGSImpactData;
class ImpactSwap;

class BGSImpactDataSet : public TESForm, public BGSPreloadable {
public:
	BGSImpactDataSet();
	~BGSImpactDataSet();

	struct _MaterialType {
		enum Type : uint32_t {
			NONE			= UINT32_MAX,
			STONE			= 0,
			DIRT			= 1,
			GRASS			= 2,
			GLASS			= 3,
			METAL			= 4,
			WOOD			= 5,
			ORGANIC			= 6,
			CLOTH			= 7,
			WATER			= 8,
			HOLLOW_METAL	= 9,
			ORGANIC_BUG		= 10,
			ORGANIC_GLOW	= 11,
			COUNT,
		};
	};
	using ImpactMaterialType = _MaterialType::Type;

	BGSImpactData* pImpactDatas[ImpactMaterialType::COUNT];

	TESFORM_TYPE(BGSImpactDataSet);

#ifdef GAME
	BGSImpactData* GetImpactData(ImpactMaterialType aeType) const;

	BGSImpactData* GetImpactData(HK_MATERIAL_TYPE aeHavokType) const;

	static void ApplyImpactSwap(ImpactSwap* apSwap);

	static ImpactMaterialType GetImpactMaterialType(HK_MATERIAL_TYPE aeHavokMaterial);
#endif

	static BGSImpactDataSet* GetDefaultImpactDataSet();

	static const char* GetImpactMaterialName(ImpactMaterialType aeType);

protected:
#ifdef GAME
	static constexpr AddressPtr<BGSImpactDataSet*, 0x11CA830> pDefaultImpactDataSet;
	static constexpr AddressPtr<const char*, 0x118C4A0, ImpactMaterialType::COUNT> pImpactMaterialString;
#else
	static constexpr AddressPtr<BGSImpactDataSet*, 0xED9674> pDefaultImpactDataSet;
	static constexpr AddressPtr<const char*, 0xE9B8BC, ImpactMaterialType::COUNT> pImpactMaterialString;
#endif
};

#ifdef GAME
ASSERT_SIZE(BGSImpactDataSet, 0x4C);
#else
ASSERT_SIZE(BGSImpactDataSet, 0x60);
#endif