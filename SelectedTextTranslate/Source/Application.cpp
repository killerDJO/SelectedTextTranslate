#include "Application.h"
#include "Service\Translation\Translator.h"
#include "Service\Translation\TranslatePageParser.h"
#include "Service\Dictionary\Dictionary.h"
#include "Providers\RequestProvider.h"
#include "View\Content\MainWindow.h"
#include "Exceptions\SelectedTextTranslateException.h"
#include "Exceptions\SelectedTextTranslateFatalException.h"

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
        result = BootstrapApplication(logger, hInstance);
    }
    catch (const SelectedTextTranslateFatalException& exception)
    {
        return TerminateOnException(logger, exception.GetErrorMessage());
    }
    catch (...)
    {
        return TerminateOnException(logger, wstring());
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
    WindowContext* windowContext = new WindowContext(
        hInstance,
        scrollProvider,
        scaleProvider,
        deviceContextProvider,
        renderingContext);

    AppController* appController = new AppController(translator, textPlayer, textExtractor, dictionary);

    HotkeyProvider* hotkeyProvider = new HotkeyProvider();
    TrayIconProvider* trayIconProvider = new TrayIconProvider(appController);
    MainWindow* mainWindow = new MainWindow(windowContext, GetMainWindowDescriptor(scaleProvider), appController, hotkeyProvider, trayIconProvider);
    appController->SetMainWindow(mainWindow);

    mainWindow->Initialize();

    logger->Log(L"Application initialized.");

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        try
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        catch (const SelectedTextTranslateException& error)
        {
            logger->LogFormatted(L"Error occurred. Message: '%ls'.", error.GetErrorMessage().c_str());
            appController->ShowError();
        }
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

int Application::TerminateOnException(Logger* logger, wstring message) const
{
    logger->LogFormatted(L"Unhandled exception occurred. Message: '%ls'.", message.c_str());
    FatalAppExit(0, TEXT("Unhandled exception occurred. See log for details."));
    return -1;
}

Application::~Application()
{
}
