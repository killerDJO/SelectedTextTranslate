#pragma once
#include "TranslateResult.h"
#include "ContentWindow.h"
#include "AudioButtonWindow.h"
#include "Utilities.h"
#include "TextPlayer.h"

class HeaderWindow : public ContentWindow
{
	AudioButtonWindow* audioButton;

	void InitializeAudioButton();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
public:
	HeaderWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y);
	~HeaderWindow();

	void PlayText();
	virtual void Draw();
	
};