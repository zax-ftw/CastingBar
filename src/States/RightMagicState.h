#pragma once

#include "MagicState.h"

class RightMagicState : public MagicState
{
public:
	RightMagicState();

	std::unique_ptr<IState> HandleAction(RE::Actor* actor, Action action) override;
};
