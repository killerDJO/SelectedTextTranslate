#include "Application.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ServiceRegistry\ServiceRegistry.h"
#include "Presentation\MessageBus.h"
#include "Presentation\TrayIcon.h"
#include "Presentation\Components\Main\MainComponent.h"
#include "ServiceRegistryCreator.h"

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

    ServiceRegistry* serviceRegistry = ServiceRegistryCreator::GetServiceRegistry();
    Logger* logger = serviceRegistry->Get<Logger>();

    logger->Log(LogLevels::Trace, L"Application start.");

    int result;
    try
    {
        ExceptionHelper::SetupStructuredExceptionsTranslation();
        result = BootstrapApplication(serviceRegistry);
        delete serviceRegistry;
    }
    catch (...)
    {
        ExceptionHelper::TerminateOnException(logger);
        return -1;
    }

    ReleaseMutex(mutex);
    CloseHandle(mutex);

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return result;
}

int Application::BootstrapApplication(ServiceRegistry* serviceRegistry) const
{
    Logger* logger = serviceRegistry->Get<Logger>();
    MainComponent mainComponent = MainComponent(serviceRegistry);
    mainComponent.SetLayout(GetMainComponentLayout(serviceRegistry->Get<ScaleProvider>()));

    serviceRegistry->Get<TrayIcon>()->Initialize();
    mainComponent.Initialize();

    serviceRegistry->Get<MessageBus>()->OnExit.Subscribe(bind(&Application::Exit, this));

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

void Application::Exit() const
{
    PostQuitMessage(0);
}

LayoutDescriptor Application::GetMainComponentLayout(ScaleProvider* scaleProvider) const
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
