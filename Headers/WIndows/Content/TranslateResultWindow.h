#pragma once
#include "Windows\Content\Base\ContentWindow.h"
#include "Entities\TranslateResult.h"
#include "Helpers\Utilities.h"
#include "Windows\Buttons\ExpandButtonWindow.h"
#include "Windows\MainWindow.h"

class MainWindow;

class TranslateResultWindow : public ContentWindow
{
private:
	MainWindow* mainWindow;

	virtual POINT RenderDC();

	vector<ExpandButtonWindow*> expandButtons;
	void DestroyButtons();
	void ExpandDictionary(int index);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	TranslateResultWindow(MainWindow* mainWindow, DWORD x, DWORD y, DWORD width, DWORD height);
	~TranslateResultWindow();	
};