#pragma once
#include "Presentation\Framework\Dto\Positioning\Rect.h"
#include "Infrastructure\ErrorHandling\ErrorHandler.h"

class NativeWindowHolder
{
private:
    Logger* logger;
    ErrorHandler* errorHandler;
    HINSTANCE instance;

    WNDPROC baseWindowProcedure;
    HWND handle;

    wstring className;
    DWORD styles;
    DWORD extendedStyles;
    HWND parent;
    Rect boundingRect;
    bool isSubClass;
    vector<function<void(WNDCLASSEX*)>> classSpecifiers;
    map<UINT, function<LRESULT(WPARAM, LPARAM)>> messageHandlers;
    vector<function<void(UINT, WPARAM, LPARAM)>> commonMessageHandlers;

    static LRESULT CALLBACK WindowProcedureWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CallBaseWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) const;
    LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam);

public:
    NativeWindowHolder(ErrorHandler* errorHandler, Logger* logger);
    ~NativeWindowHolder();

    NativeWindowHolder* SetClassName(wstring className);
    NativeWindowHolder* AddStyles(DWORD styles);
    NativeWindowHolder* AddExtendedStyles(DWORD extendedStyles);
    NativeWindowHolder* SetParent(HWND parent);
    NativeWindowHolder* SetBoundingRect(Rect boundingRect);
    NativeWindowHolder* EnableSubclassing();
    NativeWindowHolder* AddClassSpecifier(function<void(WNDCLASSEX*)> classSpecifier);
    NativeWindowHolder* SetMessageHandler(UINT message, function<LRESULT(WPARAM, LPARAM)> messageHandler);
    NativeWindowHolder* SetMessageHandler(UINT message, function<void(WPARAM, LPARAM)> messageHandler, LRESULT result);
    NativeWindowHolder* SetMessageHandler(UINT message, LRESULT result);
    NativeWindowHolder* SetProxyMessageHandler(UINT message, function<LRESULT(WPARAM, LPARAM, function<LRESULT()>)> messageHandler);
    NativeWindowHolder* SetMessageHandler(function<void(UINT, WPARAM, LPARAM)> messageHandler);
    NativeWindowHolder* Initialize();

    HINSTANCE GetInstance() const;
    HWND GetHandle() const;
    wstring GetClass() const;

    void Show();
    void Hide();
    void Move(Rect boundingRect);
    void DrawNonClientArea();
    void BringToFront();
    void Focus();
    void ApplCursor(LPCWSTR name);
    Rect GetBoundingRect();
};
