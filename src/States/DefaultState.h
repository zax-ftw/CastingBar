#pragma once

#include "IState.h"

class DefaultState : public IState
{
public:
	DefaultState();

	std::unique_ptr<IState> HandleAction(RE::Actor* actor, Action action) override;
	std::optional<float> GetProgress(RE::PlayerCharacter* player) override { return std::nullopt; };

	const char* GetName() override { return "Default"; };
};
