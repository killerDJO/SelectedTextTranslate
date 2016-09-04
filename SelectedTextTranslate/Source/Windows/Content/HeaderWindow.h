#pragma once
#include "Entities\TranslateResult.h"
#include "Windows\Content\Base\ContentWindow.h"
#include "Windows\Buttons\HoverIconButtonWindow.h"
#include "Windows\Buttons\HoverTextButtonWindow.h"

class HeaderWindow : public ContentWindow
{
protected:
	POINT RenderDC() override;
	void InitializeFonts() override;
	
private:
	HFONT fontSmallUnderscored;
	
	void PlayText();
public:
	HeaderWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~HeaderWindow();
};