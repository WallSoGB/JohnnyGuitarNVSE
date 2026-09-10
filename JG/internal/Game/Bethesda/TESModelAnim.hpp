#pragma once

#include "TESModel.hpp"

class TESModelAnim : public TESModel {
public:
	TESModelAnim();
	~TESModelAnim();
};

ASSERT_SIZE(TESModelAnim, sizeof(TESModel));