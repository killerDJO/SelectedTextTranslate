#pragma once

/// <summary>
/// Provides methods to work with hotkeys.
/// </summary>
class HotkeyProvider
{
private:
    /// <summary>
    /// The translate hotkey identifier.
    /// </summary>
    const int TranslateHotkeyId = 3003;

    /// <summary>
    /// The play text hotkey identifier.
    /// </summary>
    const int PlayTextHotkeyId = 3004;

    /// <summary>
    /// The zoom in hotkey identifier.
    /// </summary>
    const int ZoomInHotkeyId = 3006;

    /// <summary>
    /// The zoom out hotkey identifier.
    /// </summary>
    const int ZoomOutHotkeyId = 3007;

    /// <summary>
    /// The map which holds registered callbacks for hotkeys.
    /// </summary>
    map<int, function<void()>> hotkeyCallbacks;

    /// <summary>
    /// Registers the hotkey callback.
    /// </summary>
    /// <param name="hotkeyId">The hotkey identifier.</param>
    /// <param name="pressedCallback">The pressed callback.</param>
    void RegisterHotkeyCallback(int hotkeyId, function<void()> pressedCallback);

    /// <summary>
    /// Unregisters the hotkey callback.
    /// </summary>
    /// <param name="hotkeyId">The hotkey identifier.</param>
    void UnregisterHotkeyCallback(int hotkeyId);

public:
    /// <summary>
    /// Initializes a new instance of the <see cref="HotkeyProvider"/> class.
    /// </summary>
    HotkeyProvider();

    /// <summary>
    /// Finalizes an instance of the <see cref="HotkeyProvider"/> class.
    /// </summary>
    ~HotkeyProvider();

    /// <summary>
    /// Registers the translate hotkey.
    /// </summary>
    /// <param name="windowHandle">The window handle.</param>
    /// <param name="pressedCallback">The pressed callback.</param>
    void RegisterTranslateHotkey(HWND windowHandle, function<void()> pressedCallback);

    /// <summary>
    /// Registers the play text hotkey.
    /// </summary>
    /// <param name="windowHandle">The window handle.</param>
    /// <param name="pressedCallback">The pressed callback.</param>
    void RegisterPlayTextHotkey(HWND windowHandle, function<void()> pressedCallback);
    
    /// <summary>
    /// Registers the zoom in hotkey.
    /// </summary>
    /// <param name="windowHandle">The window handle.</param>
    /// <param name="pressedCallback">The pressed callback.</param>
    void RegisterZoomInHotkey(HWND windowHandle, function<void()> pressedCallback);

    /// <summary>
    /// Registers the zoom out hotkey.
    /// </summary>
    /// <param name="windowHandle">The window handle.</param>
    /// <param name="pressedCallback">The pressed callback.</param>
    void RegisterZoomOutHotkey(HWND windowHandle, function<void()> pressedCallback);

    /// <summary>
    /// Unregisters the zoom in hotkey.
    /// </summary>
    /// <param name="windowHandle">The window handle.</param>
    void UnregisterZoomInHotkey(HWND windowHandle);

    /// <summary>
    /// Unregisters the zoom out hotkey.
    /// </summary>
    /// <param name="windowHandle">The window handle.</param>
    void UnregisterZoomOutHotkey(HWND windowHandle);

    /// <summary>
    /// Processes the hotkey.
    /// </summary>
    /// <param name="hotkeyId">The hotkey identifier.</param>
    void ProcessHotkey(DWORD hotkeyId);
};