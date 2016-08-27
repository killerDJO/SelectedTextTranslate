#pragma once
#include "Entities\TranslateResult.h"
#include "Windows\Content\Base\ContentWindow.h"
#include "Windows\Buttons\AudioButtonWindow.h"
#include "Helpers\Utilities.h"
#include "TranslateEngine\TextPlayer.h"

class HeaderWindow : public ContentWindow
{
	AudioButtonWindow* audioButton;

	TranslateResult translateResult;

	void InitializeAudioButton();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual POINT RenderDC();
	virtual void ResetWindow(POINT bottomRight);
	
public:
	HeaderWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~HeaderWindow();

	POINT RenderResult(TranslateResult translateResult);

	void PlayText();	
};