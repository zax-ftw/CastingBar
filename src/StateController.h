#pragma once

#include "Utils/Singleton.h"
#include "Actions.h"

using EventResult = RE::BSEventNotifyControl;

class CastingBar;
class IState;

namespace case_insensitive
{
	struct equals
	{
		bool operator()(const std::string& lhs, const std::string& rhs) const
		{
			return _stricmp(lhs.c_str(), rhs.c_str()) == 0;
		}
	};

	struct hash
	{
		std::size_t operator()(const std::string str) const
		{
			std::string lower;
			std::transform(str.begin(), str.end(), std::back_inserter(lower), 
				[](char c) { return std::tolower(c); });
			return std::hash<std::string>()(lower);
		}
	};
};

class StateController :
	public RE::BSTEventSink<RE::BSAnimationGraphEvent>,
	public RE::BSTEventSink<RE::TESObjectLoadedEvent>,
	public RE::BSTEventSink<RE::TESLoadGameEvent>
{
public:
	using AnimationGraphEventSource = RE::BSTEventSource<RE::BSAnimationGraphEvent>;
	using ObjectLoadedEventSource = RE::BSTEventSource<RE::TESObjectLoadedEvent>;
	using LoadGameEventSource = RE::BSTEventSource<RE::TESLoadGameEvent>;

	StateController(RE::PlayerCharacter* a_player);

	EventResult ProcessEvent(const RE::BSAnimationGraphEvent* a_event, AnimationGraphEventSource* a_source) override;
	EventResult ProcessEvent(const RE::TESObjectLoadedEvent* a_event, ObjectLoadedEventSource* a_source) override;
	EventResult ProcessEvent(const RE::TESLoadGameEvent* a_event, LoadGameEventSource* a_source) override;

	void Dispatch(Action action);
	void Update(CastingBar* meter);

private:
	using ActionMap = std::unordered_map<std::string, Action, 
		case_insensitive::hash, case_insensitive::equals>;

	static Action AnimEventToAction(const RE::BSFixedString& a_event);

	std::unique_ptr<IState> state;
	RE::PlayerCharacter* m_actor;

	static inline bool firstLoad{ true };
};
