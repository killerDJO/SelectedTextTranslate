#pragma once
#include "Windows\Content\Base\ContentWindow.h"
#include "Entities\TranslateResult.h"
#include "Helpers\Utilities.h"
#include "Windows\Buttons\HoverTextButtonWindow.h"
#include "Windows\MainWindow.h"

class MainWindow;

class TranslateResultWindow : public ContentWindow
{
protected:
	POINT RenderDC() override;
	void InitializeFonts() override;

private:
	MainWindow* mainWindow;
	TranslateResult translateResult;

	HFONT fontUnderscored;

	void ExpandDictionary(int index);

public:
	TranslateResultWindow(MainWindow* mainWindow, DWORD x, DWORD y, DWORD width, DWORD height);
	~TranslateResultWindow();	

	POINT RenderResult(TranslateResult translateResult);
};