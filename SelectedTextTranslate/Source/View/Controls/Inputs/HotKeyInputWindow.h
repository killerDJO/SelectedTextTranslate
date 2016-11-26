#pragma once
#include "View\Framework\Windows\ChildWindow.h"

class HotKeyInputWindow : public ChildWindow
{
private:
    DWORD currentHotkey;
    Point currentTextBottomRight;
    HFONT font;

    int padding;
    int borderWidth;
    int lineHeight;

    bool hasFocus;

    vector<pair<DWORD, wstring>> controlToDisplayNameMap;

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    void SubclassNativeControl();
    void ShowCustomCaret() const;
    wstring GetHotkeyDisplayString() const;

    void RenderBorder(Renderer* renderer) const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    HotKeyInputWindow(WindowContext* context, Window* parentWindow);
    virtual ~HotKeyInputWindow();

    void SetDescriptor(WindowDescriptor descriptor) override;
    void SetPosition(Point position);

    void SetFont(HFONT font);
    HFONT GetFont() const;

    void SetPadding(int padding);
    int GetPadding() const;

    void SetBorderWidth(int borderWidth);
    int GetBorderWidth() const;

    void SetLineHeight(int lineHeight);
    int GetLineHeight() const;

    void SetDefaultHotkey(DWORD hotkey);
    DWORD GetDefaultHotKey() const;

    void Initialize() override;

    Subscribeable<DWORD> OnHotkeyChanged;
};