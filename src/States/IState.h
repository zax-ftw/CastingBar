#pragma once

#include "Actions.h"

class IState
{
public:

	virtual ~IState() { logger::trace("IState::Dtor"); }

	virtual std::unique_ptr<IState> HandleAction(RE::Actor* actor, Action action) = 0;
	virtual std::optional<float> GetProgress(RE::PlayerCharacter* player) = 0;

	virtual const char* GetName() = 0;
};
