#include "CastingBar.h"
#include "Settings.h"
#include "Events.h"

#include <stddef.h>

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

namespace {

    void InitializeLogging()
    {
        auto path = log_directory();
        if (!path) {
            report_and_fail("Unable to lookup SKSE logs directory.");
        }
        *path /= PluginDeclaration::GetSingleton()->GetName();
        *path += L".log";

        std::shared_ptr<spdlog::logger> log;
        if (IsDebuggerPresent()) {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
        } else {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
        }

#ifndef NDEBUG
        const auto level = spdlog::level::trace;
#else
        const auto level = spdlog::level::info;
#endif

        log->set_level(level);
        log->flush_on(level);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%H:%M:%S][%l] %v");
    }

    void InitializeMessaging()
    {
        if (!GetMessagingInterface()->RegisterListener([](MessagingInterface::Message* message) {
				switch (message->type) {
				case MessagingInterface::kPostLoad:
				case MessagingInterface::kPostPostLoad:
				case MessagingInterface::kInputLoaded:
					break;
				case MessagingInterface::kDataLoaded:
					MenuModeEventHandler::Register();
					CastingBar::Register();
					break;
				}
            })) {
            stl::report_and_fail("Unable to register message listener.");
        }
    }
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse)
{
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();

	logger::info("{} {} is loading...",
        plugin->GetName(), plugin->GetVersion());

    SKSE::Init(skse);
    InitializeMessaging();

    Settings::GetSingleton().Load();

    return true;
}
