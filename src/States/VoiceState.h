#pragma once

#include "IState.h"

class VoiceState : public IState
{
public:

	VoiceState();

	std::unique_ptr<IState> HandleAction(RE::Actor* actor, Action action) override;
	std::optional<float> GetProgress(RE::PlayerCharacter* player) override;

	const char* GetName() override { return "Shout"; };

private:
	using ShoutLevel = RE::TESShout::VariationID;

	float GetShoutTimeByLevel(int32_t a_level);

	float shoutTime1;
	float shoutTime2;
};
