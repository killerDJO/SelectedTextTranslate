#include "Application.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "BusinessLogic\Translation\TranslationService.h"
#include "BusinessLogic\Translation\TranslatePageParser.h"
#include "BusinessLogic\Dictionary\DictionaryService.h"
#include "DataAccess\RequestProvider.h"
#include "BusinessLogic\Settings\SettingsProvider.h"
#include "Presentation\Framework\Providers\DeviceContextProvider.h"
#include "Presentation\Framework\Providers\ScrollProvider.h"
#include "Presentation\Framework\Rendering\RenderingContext.h"
#include "Infrastructure\CompositionRoot.h"
#include "Presentation\MessageBus.h"
#include "Presentation\Providers\HotkeyProvider.h"
#include "Presentation\Providers\TrayIconProvider.h"
#include "BusinessLogic\Translation\TextExtractor.h"
#include "BusinessLogic\Translation\TextPlayer.h"
#include "Presentation\Components\Main\MainComponent.h"

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
    mainComponent.SetLayout(GetMainWindowDescriptor(root->GetService<ScaleProvider>()));

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

LayoutDescriptor Application::GetMainWindowDescriptor(ScaleProvider* scaleProvider) const
{
    int padding = 5;

    RECT workarea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);

    int width = scaleProvider->Scale(300);
    int height = scaleProvider->Scale(400);

    int x = workarea.right - width - padding;
    int y = workarea.bottom - height - padding;

    LayoutDescriptor descriptor = LayoutDescriptor::CreateLayoutDescriptor(
        Point(x, y),
        Size(width, height),
        OverflowModes::Scroll,
        OverflowModes::Scroll);

    return descriptor;
}