#pragma once
#include "Entities\TranslateResult.h"
#include "Windows\Content\Base\ContentWindow.h"
#include "Windows\Buttons\HoverIconButtonWindow.h"
#include "Windows\Buttons\HoverTextButtonWindow.h"
#include "Helpers\Utilities.h"
#include "TranslateEngine\TextPlayer.h"

class HeaderWindow : public ContentWindow
{
protected:
	POINT RenderDC() override;
	void InitializeFonts() override;
	
private:
	TranslateResult translateResult;
	HFONT fontSmallUnderscored;
	
public:
	HeaderWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~HeaderWindow();

	POINT RenderResult(TranslateResult translateResult);

	void PlayText();
};