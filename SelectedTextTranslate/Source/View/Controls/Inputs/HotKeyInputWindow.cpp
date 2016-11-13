#include "View\Controls\Inputs\HotKeyInputWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

HotKeyInputWindow::HotKeyInputWindow(
    WindowContext* context,
    Point postion,
    wstring name,
    Window* parentWindow,
    int defaultHotKey,
    HFONT font,
    int lineHeight,
    int padding,
    int borderWidth)
    : ChildWindow(context, WindowDescriptor::CreateFixedWindowDescriptor(postion, Size(200, lineHeight + padding * 2 + borderWidth * 2)), name, parentWindow)
{
    this->className = HOTKEY_CLASS;
    this->currentHotkey = defaultHotKey;
    this->font = font;
    this->padding = padding;
    this->borderWidth = borderWidth;
    this->lineHeight = lineHeight;
    this->borderPen = nullptr;
}

void HotKeyInputWindow::Initialize()
{
    // Ensure that the common control DLL is loaded.
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_HOTKEY_CLASS;
    AssertCriticalWinApiResult(InitCommonControlsEx(&icex));

    ChildWindow::Initialize();

    SubclassNativeControl();

    borderPen = context->GetRenderingContext()->CreateCustomPen(Colors::Gray, borderWidth);

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
        ShowCustomCaret();
        return TRUE;
    }

    case WM_KILLFOCUS:
    {
        AssertCriticalWinApiResult(HideCaret(windowHandle));
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

    if ((modifier & HOTKEYF_CONTROL) == HOTKEYF_CONTROL)
    {
        hotkeyDisplay += L"CTRL + ";
    }

    if ((modifier & HOTKEYF_ALT) == HOTKEYF_ALT)
    {
        hotkeyDisplay += L"ALT + ";
    }

    if ((modifier & HOTKEYF_SHIFT) == HOTKEYF_SHIFT)
    {
        hotkeyDisplay += L"SHIFT + ";
    }

    if ((modifier & HOTKEYF_EXT) == HOTKEYF_EXT)
    {
        hotkeyDisplay += L"EXT + ";
    }

    DWORD keyChar = MapVirtualKey(key, MAPVK_VK_TO_CHAR);
    hotkeyDisplay += (wchar_t)keyChar;

    return hotkeyDisplay;
}

void HotKeyInputWindow::RenderBorder(Renderer* renderer) const
{
    Size windowSize = GetAvailableClientSize();
    int scaledBorderWidth = context->GetScaleProvider()->Scale(borderWidth);
    int borderOffset = scaledBorderWidth - 1;
    Rect borderRect = Rect(
        Point(borderOffset, borderOffset),
        Size(windowSize.Width - borderOffset, windowSize.Height - borderOffset));

    renderer->DrawRectUnscaled(
        borderRect,
        borderPen);
}

HotKeyInputWindow::~HotKeyInputWindow()
{
}
