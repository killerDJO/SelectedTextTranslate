#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

HotKeyInputWindow::HotKeyInputWindow(ViewContext* context, View* parentWindow)
    : ChildView(context, parentWindow)
{
    this->className = HOTKEY_CLASS;
    this->currentHotkey = 0;
    this->defaultFont = context->GetRenderingProvider()->CreateCustomFont(FontSizes::Medium);
    this->font = nullptr;
    this->padding = context->GetScaleProvider()->Scale(3);
    this->borderWidth = context->GetScaleProvider()->Scale(1);
    this->width = context->GetScaleProvider()->Scale(200);
    this->lineHeight = this->defaultFont->GetHeight();
    this->hasFocus = false;
    this->isValid = true;

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
    AssertViewNotInitialized();
    descriptor = WindowDescriptor::CreateStretchWindowDescriptor(position);
}

void HotKeyInputWindow::SetFont(Font* font)
{
    AssertViewNotInitialized();
    this->font = font;
}

Font* HotKeyInputWindow::GetFont() const
{
    return font == nullptr ? defaultFont : font;
}

void HotKeyInputWindow::SetPadding(int padding)
{
    AssertViewNotInitialized();
    this->padding = padding;
}

int HotKeyInputWindow::GetPadding() const
{
    return padding;
}

void HotKeyInputWindow::SetBorderWidth(int borderWidth)
{
    AssertViewNotInitialized();
    this->borderWidth = borderWidth;
}

int HotKeyInputWindow::GetBorderWidth() const
{
    return borderWidth;
}

void HotKeyInputWindow::SetLineHeight(int lineHeight)
{
    AssertViewNotInitialized();
    this->lineHeight = lineHeight;
}

int HotKeyInputWindow::GetLineHeight() const
{
    return lineHeight;
}

void HotKeyInputWindow::SetHotkey(DWORD hotkey)
{
    AssertViewNotInitialized();
    currentHotkey = hotkey;
}

DWORD HotKeyInputWindow::GetHotKey() const
{
    return currentHotkey;
}

void HotKeyInputWindow::MakeValid()
{
    isValid = true;
}

void HotKeyInputWindow::MakeInvalid()
{
    isValid = false;
}

bool HotKeyInputWindow::IsValid() const
{
    return isValid;
}

void HotKeyInputWindow::Initialize()
{
    Size hotkeyInputSize = Size(width, lineHeight + padding * 2 + borderWidth * 2);
    descriptor = WindowDescriptor::CreateFixedWindowDescriptor(descriptor.GetPosition(), hotkeyInputSize);

    // Ensure that the common control DLL is loaded.
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_HOTKEY_CLASS;
    AssertCriticalWinApiResult(InitCommonControlsEx(&icex));

    ChildView::Initialize();

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
    int fontAscent = GetFont()->GetAscent();

    if (currentHotkey == 0)
    {
        currentTextPoistion = Point(textOffset, textOffset);
        renderer->PrintText(
            L"Not assigned",
            GetFont(),
            Colors::Gray,
            Point(textOffset, fontAscent + textOffset));
    }
    else
    {
        currentTextPoistion = renderer->PrintText(
            GetHotkeyDisplayString().c_str(),
            GetFont(),
            Colors::Black,
            RenderPosition(textOffset, fontAscent + textOffset));
    }

    return renderer->GetSize();
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
        context->GetMessageBus()->SuspendHotkeys();
        return TRUE;
    }

    case WM_KILLFOCUS:
    {
        hasFocus = false;
        RenderToBuffer();
        AssertCriticalWinApiResult(HideCaret(windowHandle));
        InvalidateRect(windowHandle, nullptr, TRUE);
        context->GetMessageBus()->EnableHotkeys();
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

    return ChildView::WindowProcedure(message, wParam, lParam);
}

void HotKeyInputWindow::ShowCustomCaret() const
{
    AssertCriticalWinApiResult(CreateCaret(windowHandle, (HBITMAP)nullptr, 1, lineHeight));
    AssertCriticalWinApiResult(SetCaretPos(currentTextPoistion.GetX(), borderWidth + padding));
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
        GetClientSize());

    Colors borderColor = Colors::Gray;
    if(!isValid)
    {
        borderColor = Colors::Red;
    }
    else if(hasFocus)
    {
        borderColor = Colors::Blue;
    }

    renderer->DrawBorderedRect(borderRect, nullptr, borderWidth, borderColor);
}

HotKeyInputWindow::~HotKeyInputWindow()
{
    context->GetMessageBus()->EnableHotkeys();
    delete defaultFont;
}