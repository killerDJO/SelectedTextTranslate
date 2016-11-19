#include "View\Content\Settings\SettingsWindow.h"
#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"

SettingsWindow::SettingsWindow(WindowContext* context, WindowDescriptor descriptor, wstring name, Window* parentWindow)
    : ContentWindow(context, descriptor, name, parentWindow)
{
    this->hotkeyControlFont = nullptr;
}

void SettingsWindow::Initialize()
{
    ContentWindow::Initialize();
    hotkeyControlFont = context->GetRenderingContext()->CreateCustomFont(windowHandle, FontSizes::Normal);
}

Size SettingsWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);
    DestroyChildWindows();

    renderer->SetBackground(context->GetRenderingContext()->CreateCustomBrush(Colors::Background));

    int curY = paddingY;

    curY = RenderHotkeysEditControl(renderer, L"Transalte selected text hotkey:", curY);
    curY = RenderHotkeysEditControl(renderer, L"Play selected text hotkey:", curY);

    HoverFlatButtonWindow* saveButton = new HoverFlatButtonWindow(
        context,
        WindowDescriptor::CreateFixedWindowDescriptor(
            Point(paddingX, curY),
            Size(50, 21)),
        L"SaveButton",
        this,
        fontNormal,
        L"Save");
    AddChildWindow(saveButton);

    renderer->IncreaseWidth(paddingX);
    renderer->IncreaseHeight(paddingY);

    return renderer->GetScaledSize();
}

int SettingsWindow::RenderHotkeysEditControl(Renderer* renderer, wstring title, int curY)
{
    int normalFontAscent = renderer->GetFontAscent(fontNormal);
    renderer->PrintText(title.c_str(), fontNormal, Colors::Black, Point(paddingX, curY + normalFontAscent));

    curY += paddingY + 5;

    int hotkeyControlFontHeight = renderer->GetFontHeight(hotkeyControlFont);
    HotKeyInputWindow* hotKeyInputWindow = new HotKeyInputWindow(
        context,
        Point(paddingX, curY),
        L"HotKeyInput",
        this,
        0,
        hotkeyControlFont,
        hotkeyControlFontHeight);

    AddChildWindow(hotKeyInputWindow);

    renderer->UpdateSize(Size(
        hotKeyInputWindow->GetSize().Width + hotKeyInputWindow->GetPosition().X,
        hotKeyInputWindow->GetSize().Height + hotKeyInputWindow->GetPosition().Y));

    curY += hotKeyInputWindow->GetSize().Height + paddingY;

    return curY;
}

SettingsWindow::~SettingsWindow()
{
}