#include "VoiceState.h"
#include "DefaultState.h"

#include "Utils/GameSettings.h"

using namespace RE;

class DefaultState;

VoiceState::VoiceState()
{
	logger::trace("VoiceState::Ctor");

	shoutTime1 = Utils::GetGameSettingFloat("fShoutTime1", 0.20);
	shoutTime2 = Utils::GetGameSettingFloat("fShoutTime2", 0.90);
}

std::unique_ptr<IState> VoiceState::HandleAction(Actor* actor, Action action)
{
	logger::trace("VoiceState::HandleAction {}", 
		magic_enum::enum_name(action));

	switch (action) {
	case Action::kVoiceFire:
	case Action::kVoiceStop:
	case Action::kWeaponSheathe:
		return std::make_unique<DefaultState>();
	}

	return nullptr;
}

std::optional<float> VoiceState::GetProgress(PlayerCharacter* player)
{
	auto& runtime = player->GetActorRuntimeData();
	auto data = runtime.currentProcess->high;

	float elapsed = data->voiceTimeElapsed;
	if (elapsed > 0.1) {
		
		auto level = player->GetCurrentShoutLevel();	
		auto required = GetShoutTimeByLevel(level);

		if (required > 0.0) {
			return elapsed / required;
		}
	}
	return std::nullopt;
}

float VoiceState::GetShoutTimeByLevel(int32_t a_level)
{
	switch (a_level) {
	case ShoutLevel::kOne:
		return 0.0f;
	case ShoutLevel::kTwo:
		return shoutTime1;
	case ShoutLevel::kThree:
		return shoutTime2;
	default:
		return 0.0f;
	}
}

