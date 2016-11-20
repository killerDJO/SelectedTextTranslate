#include "Application.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Controllers\AppController.h"
#include "Services\Translation\TranslationService.h"
#include "Services\Translation\TranslatePageParser.h"
#include "Services\Dictionary\DictionaryService.h"
#include "Providers\RequestProvider.h"
#include "View\Content\MainWindow.h"

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

    GdiplusShutdown(gdiplusToken);

    return result;
}

int Application::BootstrapApplication(Logger* logger, HINSTANCE hInstance) const
{
    HotkeyProvider hotkeyProvider = HotkeyProvider();
    TrayIconProvider trayIconProvider = TrayIconProvider(logger, &hotkeyProvider, hInstance);

    SqliteProvider sqliteProvider = SqliteProvider();
    DictionaryService dictionarySerivce = DictionaryService(logger, &sqliteProvider);
    TextExtractor textExtractor = TextExtractor();
    RequestProvider requestProvider = RequestProvider(logger);
    TranslatePageParser translatePageParser = TranslatePageParser(logger, &requestProvider);
    TranslationService translationService = TranslationService(logger, &requestProvider, &translatePageParser, &dictionarySerivce);
    TextPlayer textPlayer = TextPlayer(logger, &translationService, &requestProvider, &trayIconProvider);

    ScaleProvider scaleProvider = ScaleProvider();
    DeviceContextProvider deviceContextProvider = DeviceContextProvider();
    ScrollProvider scrollProvider = ScrollProvider();
    RenderingContext renderingContext = RenderingContext(&scaleProvider, &deviceContextProvider, &scrollProvider);

    WindowContext windowContext = WindowContext(
        hInstance,
        &scrollProvider,
        &scaleProvider,
        &deviceContextProvider,
        &trayIconProvider,
        &renderingContext,
        logger);

    MainWindow mainWindow = MainWindow(&windowContext, &hotkeyProvider);
    mainWindow.SetDescriptor(GetMainWindowDescriptor(&scaleProvider));

    AppController appController = AppController(
        &mainWindow,
        &trayIconProvider,
        &translationService,
        &textPlayer,
        &textExtractor,
        &dictionarySerivce);

    trayIconProvider.Initialize();
    mainWindow.Initialize();
    appController.Initialize();

    logger->Log(LogLevels::Trace, L"Application initialized.");

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

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
        OverflowModes::Scroll,
        false);

    return descriptor;
}

Application::~Application()
{
}
