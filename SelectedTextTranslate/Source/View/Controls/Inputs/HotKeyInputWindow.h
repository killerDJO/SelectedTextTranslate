#pragma once
#include "View\Framework\Windows\ChildWindow.h"

class HotKeyInputWindow : public ChildWindow
{
private:
    DWORD currentHotkey;
    Point currentTextBottomRight;
    HFONT font;
    HPEN borderPen;

    int padding;
    int borderWidth;
    int lineHeight;

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    void SubclassNativeControl();
    void ShowCustomCaret() const;
    wstring GetHotkeyDisplayString() const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    HotKeyInputWindow(
        WindowContext* context,
        Point postion,
        wstring name,
        Window* parentWindow,
        int defaultHotKey,
        HFONT font,
        int lineHeight,
        int padding = 3,
        int borderWidth = 1);

    virtual ~HotKeyInputWindow();

    void Initialize() override;
};