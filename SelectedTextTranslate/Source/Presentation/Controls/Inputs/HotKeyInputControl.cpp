#include "Presentation\Controls\Inputs\HotKeyInputControl.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Presentation\MessageBus.h"

HotKeyInputControl::HotKeyInputControl(ViewContext* context, View* parentWindow)
    : ControlView(context, parentWindow)
{
    this->ClassName = HOTKEY_CLASS;
    this->currentHotkey = 0;
    this->defaultFont = RenderingProvider->CreateCustomFont(FontSizes::Medium);
    this->font = nullptr;
    this->padding = ScaleProvider->Scale(3);
    this->borderWidth = ScaleProvider->Scale(1);
    this->width = ScaleProvider->Scale(200);
    this->lineHeight = this->defaultFont->GetHeight();
    this->hasFocus = false;
    this->isValid = true;

    this->controlToDisplayNameMap = vector<pair<DWORD, wstring>>();
    this->controlToDisplayNameMap.push_back(make_pair(HOTKEYF_CONTROL, L"CTRL"));
    this->controlToDisplayNameMap.push_back(make_pair(HOTKEYF_ALT, L"ALT"));
    this->controlToDisplayNameMap.push_back(make_pair(HOTKEYF_SHIFT, L"SHIFT"));
    this->controlToDisplayNameMap.push_back(make_pair(HOTKEYF_EXT, L"EXT"));
}

void HotKeyInputControl::SetPosition(Point position)
{
    AssertViewNotInitialized();
    Layout = LayoutDescriptor::CreateLayoutWindowDescriptor(position);
}

void HotKeyInputControl::SetFont(Font* font)
{
    AssertViewNotInitialized();
    this->font = font;
}

Font* HotKeyInputControl::GetFont() const
{
    return font == nullptr ? defaultFont : font;
}

void HotKeyInputControl::SetPadding(int padding)
{
    AssertViewNotInitialized();
    this->padding = padding;
}

int HotKeyInputControl::GetPadding() const
{
    return padding;
}

void HotKeyInputControl::SetBorderWidth(int borderWidth)
{
    AssertViewNotInitialized();
    this->borderWidth = borderWidth;
}

int HotKeyInputControl::GetBorderWidth() const
{
    return borderWidth;
}

void HotKeyInputControl::SetLineHeight(int lineHeight)
{
    AssertViewNotInitialized();
    this->lineHeight = lineHeight;
}

int HotKeyInputControl::GetLineHeight() const
{
    return lineHeight;
}

void HotKeyInputControl::SetHotkey(DWORD hotkey)
{
    AssertViewNotInitialized();
    currentHotkey = hotkey;
}

DWORD HotKeyInputControl::GetHotKey() const
{
    return currentHotkey;
}

void HotKeyInputControl::MakeValid()
{
    isValid = true;
}

void HotKeyInputControl::MakeInvalid()
{
    isValid = false;
}

bool HotKeyInputControl::IsValid() const
{
    return isValid;
}

void HotKeyInputControl::Initialize()
{
    Size hotkeyInputSize = Size(width, lineHeight + padding * 2 + borderWidth * 2);
    Layout = LayoutDescriptor::CreateFixedLayoutDescriptor(Layout.GetPosition(), hotkeyInputSize);

    // Ensure that the common control DLL is loaded.
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_HOTKEY_CLASS;
    AssertCriticalWinApiResult(InitCommonControlsEx(&icex));

    ChildView::Initialize();

    SubclassNativeControl();

    Render();
}

void HotKeyInputControl::SubclassNativeControl()
{
    BaseWindowProcedure = (WNDPROC)SetWindowLongPtr(Handle, GWLP_WNDPROC, (LONG_PTR)WindowProcedureWrapper);
    AssertCriticalWinApiResult(BaseWindowProcedure);

    LONG exStyle = GetWindowLong(Handle, GWL_EXSTYLE);
    AssertCriticalWinApiResult(exStyle);

    exStyle &= ~WS_EX_CLIENTEDGE;
    AssertCriticalWinApiResult(SetWindowLong(Handle, GWL_EXSTYLE, exStyle));

    AssertCriticalWinApiResult(SetWindowPos(Handle, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER));

    SendMessage(Handle, HKM_SETHOTKEY, currentHotkey, 0);
}

Size HotKeyInputControl::RenderContent(Renderer* renderer)
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

LRESULT HotKeyInputControl::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_SETFOCUS:
    {
        hasFocus = true;
        RenderToBuffer();
        ShowCustomCaret();
        InvalidateRect(Handle, nullptr, TRUE);
        Context->GetMessageBus()->OnSuspendHotkeys.Notify();
        return TRUE;
    }

    case WM_KILLFOCUS:
    {
        hasFocus = false;
        RenderToBuffer();
        AssertCriticalWinApiResult(HideCaret(Handle));
        InvalidateRect(Handle, nullptr, TRUE);
        Context->GetMessageBus()->OnEnableHotkeys.Notify();
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

        if (GetFocus() == Handle)
        {
            ShowCustomCaret();
        }

        return TRUE;
    }

    case WM_KEYDOWN:
    case WM_KEYUP:
    {
        int procedureResult = CallBaseWindowProcedure(message, wParam, lParam);
        currentHotkey = SendMessage(Handle, HKM_GETHOTKEY, 0, 0);

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
                SendMessage(Handle, HKM_SETHOTKEY, 0, 0);
            }
        }

        RenderToBuffer();
        return procedureResult;
    }

    }

    return ControlView::WindowProcedure(message, wParam, lParam);
}

void HotKeyInputControl::ShowCustomCaret() const
{
    AssertCriticalWinApiResult(CreateCaret(Handle, (HBITMAP)nullptr, 1, lineHeight));
    AssertCriticalWinApiResult(SetCaretPos(currentTextPoistion.GetX(), borderWidth + padding));
    AssertCriticalWinApiResult(ShowCaret(Handle));
}

wstring HotKeyInputControl::GetHotkeyDisplayString() const
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

void HotKeyInputControl::RenderBorder(Renderer* renderer) const
{
    Rect borderRect = Rect(
        Point(0, 0),
        GetAvailableClientSize());

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

HotKeyInputControl::~HotKeyInputControl()
{
    Context->GetMessageBus()->OnEnableHotkeys.Notify();
    delete defaultFont;
}