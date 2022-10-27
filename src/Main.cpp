#include "Settings.h"
#include "SKSE/API.h"
#include "CastingBar.h"
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

        log->set_level(spdlog::level::info);
        log->flush_on(spdlog::level::info);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%H:%M:%S][%l] %v");
    }

    void InitializeMessaging()
    {
        if (!GetMessagingInterface()->RegisterListener(
            [](MessagingInterface::Message* message) {

                switch (message->type) 
                {
                    case MessagingInterface::kPostLoad:
                    case MessagingInterface::kPostPostLoad:
                    case MessagingInterface::kInputLoaded:
                        break;
                    case MessagingInterface::kDataLoaded:                   
                        MenuOpenCloseEventHandler::Register();
                        CastingBar::Register();
                        break;
                    case MessagingInterface::kNewGame:
                        CastingBar::Show();
                        break;
                    case MessagingInterface::kPreLoadGame:
                        break;
                    case MessagingInterface::kPostLoadGame:
                        CastingBar::Show();
                        break;
                    case MessagingInterface::kSaveGame:
                        break;
                    case MessagingInterface::kDeleteGame:
                        break;
                }
        })) {
            stl::report_and_fail("Unable to register message listener.");
        }
    }
}

SKSEPluginLoad(const LoadInterface* skse) 
{
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);

    Init(skse);
    InitializeMessaging();
    Settings::GetSingleton()->Load();

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
