#pragma once

class NativeWindowHolder
{
protected:
    HINSTANCE Instance;
    WNDPROC BaseWindowProcedure;

    HWND Handle;
    wchar_t* ClassName;
    virtual void SpecifyWindowClass(WNDCLASSEX* windowClass);

    static LRESULT CALLBACK WindowProcedureWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CallBaseWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) const;
    virtual LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam);

public:
    NativeWindowHolder();
    virtual ~NativeWindowHolder();

    virtual void Initialize();

    HWND GetHandle() const;
};