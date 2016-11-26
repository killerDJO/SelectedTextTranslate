#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

HotKeyInputWindow::HotKeyInputWindow(WindowContext* context, Window* parentWindow)
    : ChildWindow(context, parentWindow)
{
    this->className = HOTKEY_CLASS;
    this->currentHotkey = 0;
    this->font = context->GetRenderingContext()->CreateCustomFont(FontSizes::Medium);
    this->padding = 3;
    this->borderWidth = 1;
    this->lineHeight = context->GetScaleProvider()->Downscale(context->GetRenderingContext()->GetFontMetrics(this->font).tmHeight);
    this->hasFocus = false;

    this->controlToDisplayNameMap = vector<pair<DWORD, wstring>>();
    this->controlToDisplayNameMap.push_back(make_pair(HOTKEYF_CONTROL, L"CTRL"));
    this->controlToDisplayNameMap.push_back(make_pair(HOTKEYF_ALT, L"ALT"));
    this->controlToDisplayNameMap.push_back(make_pair(HOTKEYF_SHIFT, L"SHIFT"));
    this->controlToDisplayNameMap.push_back(make_pair(HOTKEYF_EXT, L"EXT"));
}

void HotKeyInputWindow::SetDescriptor(WindowDescriptor descriptor)
{
    throw new SelectedTextTranslateFatalException(L"SetDescriptor is unsupported");
}

void HotKeyInputWindow::SetPosition(Point position)
{
    AssertWindowNotInitialized();
    this->position = position;
}

void HotKeyInputWindow::SetFont(HFONT font)
{
    AssertWindowNotInitialized();
    this->font = font;
}

HFONT HotKeyInputWindow::GetFont() const
{
    return font;
}

void HotKeyInputWindow::SetPadding(int padding)
{
    AssertWindowNotInitialized();
    this->padding = padding;
}

int HotKeyInputWindow::GetPadding() const
{
    return padding;
}

void HotKeyInputWindow::SetBorderWidth(int borderWidth)
{
    AssertWindowNotInitialized();
    this->borderWidth = borderWidth;
}

int HotKeyInputWindow::GetBorderWidth() const
{
    return borderWidth;
}

void HotKeyInputWindow::SetLineHeight(int lineHeight)
{
    AssertWindowNotInitialized();
    this->lineHeight = lineHeight;
}

int HotKeyInputWindow::GetLineHeight() const
{
    return lineHeight;
}

void HotKeyInputWindow::SetDefaultHotkey(DWORD hotkey)
{
    AssertWindowNotInitialized();
    currentHotkey = hotkey;
}

DWORD HotKeyInputWindow::GetDefaultHotKey() const
{
    return currentHotkey;
}

void HotKeyInputWindow::Initialize()
{
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(position, Size(200, lineHeight + padding * 2 + borderWidth * 2));

    // Ensure that the common control DLL is loaded.
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_HOTKEY_CLASS;
    AssertCriticalWinApiResult(InitCommonControlsEx(&icex));

    ChildWindow::Initialize();

    SubclassNativeControl();

    Render();
}

void HotKeyInputWindow::SubclassNativeControl()
{
    baseWindowProcedure = (WNDPROC)SetWindowLongPtr(windowHandle, GWLP_WNDPROC, (LONG_PTR)WindowProcedureWrapper);
    AssertCriticalWinApiResult(baseWindowProcedure);

    LONG exStyle = GetWindowLong(windowHandle, GWL_EXSTYLE);
    AssertCriticalWinApiResult(exStyle);

    exStyle &= ~WS_EX_CLIENTEDGE;
    AssertCriticalWinApiResult(SetWindowLong(windowHandle, GWL_EXSTYLE, exStyle));

    AssertCriticalWinApiResult(SetWindowPos(windowHandle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER));

    SendMessage(windowHandle, HKM_SETHOTKEY, currentHotkey, 0);
}

Size HotKeyInputWindow::RenderContent(Renderer* renderer)
{
    RenderBorder(renderer);

    int textOffset = borderWidth + padding;
    int fontAscent = renderer->GetFontAscent(font);

    if (currentHotkey == 0)
    {
        currentTextBottomRight = Point(textOffset, textOffset);
        renderer->PrintText(
            L"Not assigned",
            font,
            Colors::Gray,
            Point(textOffset, fontAscent + textOffset));
    }
    else
    {
        currentTextBottomRight = renderer->PrintText(
            GetHotkeyDisplayString().c_str(),
            font,
            Colors::Black,
            Point(textOffset, fontAscent + textOffset));
    }

    return renderer->GetScaledSize();
}

LRESULT HotKeyInputWindow::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_SETFOCUS:
    {
        hasFocus = true;
        RenderToBuffer();
        ShowCustomCaret();
        InvalidateRect(windowHandle, nullptr, TRUE);
        context->GetHotkeyProvider()->SuspendHotkeys();
        return TRUE;
    }

    case WM_KILLFOCUS:
    {
        hasFocus = false;
        RenderToBuffer();
        AssertCriticalWinApiResult(HideCaret(windowHandle));
        InvalidateRect(windowHandle, nullptr, TRUE);
        context->GetHotkeyProvider()->EnableHotkeys();
        return TRUE;
    }

    case WM_SETCURSOR:
    {
        SetCursor(LoadCursor(nullptr, IDC_IBEAM));
        return TRUE;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        AssertCriticalWinApiResult(BeginPaint(GetHandle(), &ps));
        Draw(false);
        EndPaint(GetHandle(), &ps);

        if (GetFocus() == windowHandle)
        {
            ShowCustomCaret();
        }

        return TRUE;
    }

    case WM_KEYDOWN:
    case WM_KEYUP:
    {
        int procedureResult = CallBaseWindowProcedure(message, wParam, lParam);
        currentHotkey = SendMessage(windowHandle, HKM_GETHOTKEY, 0, 0);

        int virtualCode = LOBYTE(LOWORD(currentHotkey));
        int modifier = HIBYTE(LOWORD(currentHotkey));

        if(message == WM_KEYUP)
        {
            OnHotkeyChanged.Notify(currentHotkey);
        }
        else
        {
            bool isModifierInvalid = modifier == 0 || (modifier & HOTKEYF_EXT) == HOTKEYF_EXT;
            bool isKeyInvalid = virtualCode != 0 && MapVirtualKey(virtualCode, MAPVK_VK_TO_CHAR) == 0;
            if (isModifierInvalid || isKeyInvalid)
            {
                currentHotkey = 0;
                SendMessage(windowHandle, HKM_SETHOTKEY, 0, 0);
            }
        }

        RenderToBuffer();
        return procedureResult;
    }

    }

    return ChildWindow::WindowProcedure(message, wParam, lParam);
}

void HotKeyInputWindow::ShowCustomCaret() const
{
    ScaleProvider* scaleProvider = context->GetScaleProvider();
    AssertCriticalWinApiResult(CreateCaret(windowHandle, (HBITMAP)nullptr, 1, scaleProvider->Scale(lineHeight)));
    AssertCriticalWinApiResult(SetCaretPos(scaleProvider->Scale(currentTextBottomRight.X), context->GetScaleProvider()->Scale(borderWidth + padding)));
    AssertCriticalWinApiResult(ShowCaret(windowHandle));
}

wstring HotKeyInputWindow::GetHotkeyDisplayString() const
{
    wstring hotkeyDisplay = L"";

    int modifier = HIBYTE(LOWORD(currentHotkey));
    int key = LOBYTE(LOWORD(currentHotkey));

    for(size_t i = 0; i < controlToDisplayNameMap.size(); ++i)
    {
        DWORD control = controlToDisplayNameMap[i].first;
        if((modifier & control) == control)
        {
            hotkeyDisplay += controlToDisplayNameMap[i].second + L" + ";
        }
    }

    DWORD keyChar = MapVirtualKey(key, MAPVK_VK_TO_CHAR);
    hotkeyDisplay += (wchar_t)keyChar;

    return hotkeyDisplay;
}

void HotKeyInputWindow::RenderBorder(Renderer* renderer) const
{
    Rect borderRect = Rect(
        Point(0, 0),
        context->GetScaleProvider()->Downscale(GetAvailableClientSize()));

    renderer->DrawBorderedRect(
        borderRect,
        nullptr,
        borderWidth,
        hasFocus ? Colors::Blue : Colors::Gray);
}

HotKeyInputWindow::~HotKeyInputWindow()
{
    context->GetHotkeyProvider()->EnableHotkeys();
}