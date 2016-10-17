#include "Application.h"
#include "Services\Translation\Translator.h"
#include "Services\Translation\TranslatePageParser.h"
#include "Services\Dictionary\Dictionary.h"
#include "Providers\RequestProvider.h"
#include "View\Content\MainWindow.h"
#include "ErrorHandling\ExceptionHelper.h"
#include "Controllers\AppController.h"

Application::Application()
{
    components = vector<void*>();
}

int Application::Run(HINSTANCE hInstance)
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

int Application::BootstrapApplication(Logger* logger, HINSTANCE hInstance)
{
    TrayIconProvider* trayIconProvider = RegisterComponent(new TrayIconProvider(logger, hInstance));
    HotkeyProvider* hotkeyProvider = RegisterComponent(new HotkeyProvider());

    Dictionary* dictionary = RegisterComponent(new Dictionary(logger));
    TextExtractor* textExtractor = RegisterComponent(new TextExtractor());
    RequestProvider* requestProvider = RegisterComponent(new RequestProvider(logger));
    TranslatePageParser* translatePageParser = RegisterComponent(new TranslatePageParser(logger, requestProvider));
    Translator* translator = RegisterComponent(new Translator(logger, requestProvider, translatePageParser, dictionary));
    TextPlayer* textPlayer = RegisterComponent(new TextPlayer(logger, translator, requestProvider, trayIconProvider));

    ScaleProvider* scaleProvider = RegisterComponent(new ScaleProvider());
    DeviceContextProvider* deviceContextProvider = RegisterComponent(new DeviceContextProvider());
    ScrollProvider* scrollProvider = RegisterComponent(new ScrollProvider());
    RenderingContext* renderingContext = RegisterComponent(new RenderingContext(scaleProvider, deviceContextProvider, scrollProvider));

    WindowContext* windowContext = RegisterComponent(new WindowContext(
        hInstance,
        scrollProvider,
        scaleProvider,
        deviceContextProvider,
        trayIconProvider,
        renderingContext,
        logger));

    MainWindow* mainWindow = RegisterComponent(new MainWindow(windowContext, GetMainWindowDescriptor(scaleProvider), hotkeyProvider));

    AppController* appController = RegisterComponent(new AppController(
        mainWindow,
        trayIconProvider,
        hotkeyProvider,
        translator,
        textPlayer,
        textExtractor,
        dictionary));

    trayIconProvider->Initialize();
    mainWindow->Initialize();
    appController->Initialize();

    logger->Log(L"Application initialized.");

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    logger->Log(L"Application shutdown.");

    DestroyComponents();

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

template <typename TComponent>
TComponent* Application::RegisterComponent(TComponent* component)
{
    components.push_back(component);
    return component;
}

void Application::DestroyComponents()
{
    for(size_t i = 0; i < components.size(); ++i)
    {
        delete components[i];
    }
}

Application::~Application()
{
}
