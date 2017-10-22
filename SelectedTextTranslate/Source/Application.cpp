#include "Application.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Services\Translation\TranslationService.h"
#include "Services\Translation\TranslatePageParser.h"
#include "Services\Dictionary\DictionaryService.h"
#include "Providers\RequestProvider.h"
#include "Services\Settings\SettingsProvider.h"
#include "View\Framework\Providers\DeviceContextProvider.h"
#include "View\Framework\Providers\ScrollProvider.h"
#include "View\Framework\Rendering\RenderingContext.h"
#include "CompositionRoot.h"
#include "View\Framework\MessageBus.h"
#include "View\Providers\HotkeyProvider.h"
#include "View\Providers\TrayIconProvider.h"
#include "Services\Translation\TextExtractor.h"
#include "Services\Translation\TextPlayer.h"
#include "View\Content\Main\MainComponent.h"

Application::Application()
{
}

int Application::Run(HINSTANCE hInstance) const
{
    HANDLE mutex = CreateMutex(nullptr, FALSE, _T("Selected text translate"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        FatalAppExit(0, TEXT("SelectedTextTranslate already started!"));
    }

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    Logger* logger = new Logger();
    logger->Log(LogLevels::Trace, L"Application start.");

    int result;
    try
    {
        ExceptionHelper::SetupStructuredExceptionsTranslation();
        result = BootstrapApplication(logger, hInstance);
    }
    catch (...)
    {
        ExceptionHelper::TerminateOnException(logger);
        return -1;
    }

    delete logger;

    ReleaseMutex(mutex);
    CloseHandle(mutex);

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return result;
}

int Application::BootstrapApplication(Logger* logger, HINSTANCE hInstance) const
{
    CompositionRoot* root = new CompositionRoot();
    root->RegisterService(logger);
    root->RegisterService(new SettingsProvider(root));
    
    Settings settings = root->GetService<SettingsProvider>()->GetSettings();
    root->RegisterService(new MessageBus());
    root->RegisterService(new HotkeyProvider(settings.GetHotkeySettings(), root));
    root->RegisterService(new TrayIconProvider(root, hInstance));

    root->RegisterService(new SqliteProvider());
    root->RegisterService(new DictionaryService(root));
    root->RegisterService(new TextExtractor());
    root->RegisterService(new RequestProvider(root));
    root->RegisterService(new TranslatePageParser(root));
    root->RegisterService(new TranslationService(root));
    root->RegisterService(new TextPlayer(root, root->GetService<TrayIconProvider>()));
    root->RegisterService(new ScaleProvider());
    root->RegisterService(new DeviceContextProvider());
    root->RegisterService(new ScrollProvider());
    root->RegisterService(new RenderingProvider(root));
    root->RegisterService(new RenderingContext(root));

    CommonContext viewContext = CommonContext(hInstance, root, root->GetService<TrayIconProvider>());

    MainComponent mainComponent = MainComponent(&viewContext);
    mainComponent.SetDescriptor(GetMainWindowDescriptor(root->GetService<ScaleProvider>()));

    root->GetService<TrayIconProvider>()->Initialize();
    mainComponent.Initialize();

    logger->Log(LogLevels::Trace, L"Application initialized.");

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete root;
    logger->Log(LogLevels::Trace, L"Application shutdown.");

    return msg.wParam;
}

WindowDescriptor Application::GetMainWindowDescriptor(ScaleProvider* scaleProvider) const
{
    int padding = 5;

    RECT workarea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);

    int width = scaleProvider->Scale(300);
    int height = scaleProvider->Scale(400);

    int x = workarea.right - width - padding;
    int y = workarea.bottom - height - padding;

    WindowDescriptor descriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(x, y),
        Size(width, height),
        OverflowModes::Scroll,
        OverflowModes::Scroll);

    return descriptor;
}