#include "ApplicationServiceRegistry.h"
#include "BusinessLogic\Settings\SettingsProvider.h"
#include "Presentation\MessageBus.h"
#include "Presentation\Providers\HotkeysRegistry.h"
#include "Presentation\Providers\TrayIcon.h"
#include "DataAccess\SqliteProvider.h"
#include "BusinessLogic\Dictionary\DictionaryService.h"
#include "BusinessLogic\Translation\TextExtractor.h"
#include "DataAccess\RequestProvider.h"
#include "BusinessLogic\Translation\TranslatePageParser.h"
#include "BusinessLogic\Translation\TranslationService.h"
#include "BusinessLogic\Translation\TextPlayer.h"
#include "Presentation\Framework\Providers\ScaleProvider.h"
#include "Presentation\Framework\Providers\DeviceContextProvider.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"
#include "Presentation\Framework\CommonContext.h"

ServiceRegistry* ApplicationServiceRegistry::GetServiceRegistry()
{
    ServiceRegistry* applicationRegistry = new ServiceRegistry();

    RegisterInfrastructure(applicationRegistry);
    RegisterDataAccess(applicationRegistry);
    RegisterBusinessLogic(applicationRegistry);
    RegisterPresentation(applicationRegistry);

    return applicationRegistry;
}

void ApplicationServiceRegistry::RegisterInfrastructure(ServiceRegistry* applicationRegistry)
{
    applicationRegistry->RegisterSingleton<Logger>([](ServiceRegistry* registry) { return new Logger(); });
    applicationRegistry->RegisterSingleton<ErrorHandler>([](ServiceRegistry* registry) { return registry->Get<TrayIcon>(); }, false);
}

void ApplicationServiceRegistry::RegisterDataAccess(ServiceRegistry* applicationRegistry)
{
    applicationRegistry->RegisterSingleton<SqliteProvider>([](ServiceRegistry* registry) { return new SqliteProvider(); });
    applicationRegistry->RegisterSingleton<RequestProvider>([](ServiceRegistry* registry) { return new RequestProvider(registry); });
}

void ApplicationServiceRegistry::RegisterBusinessLogic(ServiceRegistry* applicationRegistry)
{
    applicationRegistry->RegisterSingleton<DictionaryService>([](ServiceRegistry* registry) { return new DictionaryService(registry); });

    applicationRegistry->RegisterSingleton<TextExtractor>([](ServiceRegistry* registry) { return new TextExtractor(); });
    applicationRegistry->RegisterSingleton<TranslatePageParser>([](ServiceRegistry* registry) { return new TranslatePageParser(registry); });
    applicationRegistry->RegisterSingleton<TranslationService>([](ServiceRegistry* registry) { return new TranslationService(registry); });
    applicationRegistry->RegisterSingleton<TextPlayer>([](ServiceRegistry* registry) { return new TextPlayer(registry, registry->Get<TrayIcon>()); });

    applicationRegistry->RegisterSingleton<SettingsProvider>([](ServiceRegistry* registry) { return new SettingsProvider(registry); });
    applicationRegistry->RegisterSingleton<HotkeysRegistry>([](ServiceRegistry* registry) { return new HotkeysRegistry(registry); });
}

void ApplicationServiceRegistry::RegisterPresentation(ServiceRegistry* applicationRegistry)
{
    applicationRegistry->RegisterSingleton<MessageBus>([](ServiceRegistry* registry) { return new MessageBus(); });
    applicationRegistry->RegisterSingleton<TrayIcon>([](ServiceRegistry* registry) { return new TrayIcon(registry); });
    applicationRegistry->RegisterSingleton<CommonContext>([](ServiceRegistry* registry) { return new CommonContext(registry); });

    applicationRegistry->RegisterSingleton<ScaleProvider>([](ServiceRegistry* registry) { return new ScaleProvider(); });
    applicationRegistry->RegisterSingleton<DeviceContextProvider>([](ServiceRegistry* registry) { return new DeviceContextProvider(); });
    applicationRegistry->RegisterSingleton<ScrollProvider>([](ServiceRegistry* registry) { return new ScrollProvider(); });
    applicationRegistry->RegisterSingleton<RenderingProvider>([](ServiceRegistry* registry) { return new RenderingProvider(registry); });
    applicationRegistry->RegisterSingleton<RenderingContext>([](ServiceRegistry* registry) { return new RenderingContext(registry); });
}