#pragma once

class NativeWindowHolder
{
protected:
    HINSTANCE instance;

    HWND windowHandle;
    wchar_t* className;
    virtual void SpecifyWindowClass(WNDCLASSEX* windowClass);

    static LRESULT CALLBACK WindowProcedureWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam);

public:
    NativeWindowHolder(HINSTANCE instance);
    virtual ~NativeWindowHolder();

    virtual void Initialize();

    HWND GetHandle() const;
};