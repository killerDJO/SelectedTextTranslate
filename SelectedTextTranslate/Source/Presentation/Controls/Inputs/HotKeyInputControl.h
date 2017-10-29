#pragma once
#include "Presentation\Framework\Views\ControlView.h"
#include "Utilities\Subscribable\Subscribable.h"

class HotKeyInputControl : public ControlView
{
private:
    DWORD currentHotkey;
    RenderPosition currentTextPoistion;
    Font* defaultFont;
    Font* font;

    int padding;
    int borderWidth;
    int lineHeight;
    int width;

    bool hasFocus;
    bool isValid;

    vector<pair<DWORD, wstring>> controlToDisplayNameMap;

    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    void SubclassNativeControl();
    void ShowCustomCaret() const;
    wstring GetHotkeyDisplayString() const;

    void RenderBorder(Renderer* renderer) const;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    HotKeyInputControl(ViewContext* context, View* parentWindow);
    virtual ~HotKeyInputControl();

    void SetPosition(Point position);

    void SetFont(Font* font);
    Font* GetFont() const;

    void SetPadding(int padding);
    int GetPadding() const;

    void SetBorderWidth(int borderWidth);
    int GetBorderWidth() const;

    void SetLineHeight(int lineHeight);
    int GetLineHeight() const;

    void SetHotkey(DWORD hotkey);
    DWORD GetHotKey() const;

    void MakeValid();
    void MakeInvalid();
    bool IsValid() const;

    void Initialize() override;

    Subscribable<DWORD> OnHotkeyChanged;
};
