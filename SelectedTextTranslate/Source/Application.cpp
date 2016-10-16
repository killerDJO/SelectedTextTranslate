#include "Application.h"
#include "Services\Translation\Translator.h"
#include "Services\Translation\TranslatePageParser.h"
#include "Services\Dictionary\Dictionary.h"
#include "Providers\RequestProvider.h"
#include "View\Content\MainWindow.h"
#include "Helpers\ExceptionHelper.h"

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

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    Logger* logger = new Logger();
    logger->Log(L"Application start.");

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

    GdiplusShutdown(gdiplusToken);

    return result;
}

int Application::BootstrapApplication(Logger* logger, HINSTANCE hInstance) const
{
    Dictionary* dictionary = new Dictionary(logger);
    TextExtractor* textExtractor = new TextExtractor();
    RequestProvider* requestProvider = new RequestProvider(logger);
    TranslatePageParser* translatePageParser = new TranslatePageParser(logger, requestProvider);
    Translator* translator = new Translator(logger, requestProvider, translatePageParser, dictionary);
    TextPlayer* textPlayer = new TextPlayer(logger, translator, requestProvider);

    ScaleProvider* scaleProvider = new ScaleProvider();
    DeviceContextProvider* deviceContextProvider = new DeviceContextProvider();
    ScrollProvider* scrollProvider = new ScrollProvider();
    RenderingContext* renderingContext = new RenderingContext(scaleProvider, deviceContextProvider, scrollProvider);

    AppController* appController = new AppController(translator, textPlayer, textExtractor, dictionary);

    HotkeyProvider* hotkeyProvider = new HotkeyProvider();
    TrayIconProvider* trayIconProvider = new TrayIconProvider(appController);
    ErrorStateProvider* errorStateProvider = new ErrorStateProvider(appController);

    WindowContext* windowContext = new WindowContext(
        hInstance,
        scrollProvider,
        scaleProvider,
        deviceContextProvider,
        errorStateProvider,
        renderingContext,
        logger);

    MainWindow* mainWindow = new MainWindow(windowContext, GetMainWindowDescriptor(scaleProvider), appController, hotkeyProvider, trayIconProvider);
    appController->SetMainWindow(mainWindow);

    mainWindow->Initialize();

    logger->Log(L"Application initialized.");

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    logger->Log(L"Application shutdown.");

    delete dictionary;
    delete textExtractor;
    delete requestProvider;
    delete translatePageParser;
    delete translator;
    delete appController;
    delete textPlayer;
    delete mainWindow;
    delete renderingContext;
    delete scrollProvider;
    delete scaleProvider;
    delete deviceContextProvider;
    delete hotkeyProvider;
    delete trayIconProvider;
    delete errorStateProvider;

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
        OverflowModes::Scroll,
        false);

    return descriptor;
}

Application::~Application()
{
}
