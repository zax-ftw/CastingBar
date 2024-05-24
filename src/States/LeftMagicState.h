#pragma once

#include "MagicState.h"

class LeftMagicState : public MagicState
{
public:
	LeftMagicState();

	std::unique_ptr<IState> HandleAction(RE::Actor* actor, Action action) override;
};
