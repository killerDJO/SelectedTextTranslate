#pragma once
#include "Windows\Base\ChildWindow.h"
#include "AppModel.h"

class ContentWindow : public ChildWindow
{
protected:
	const	UINT	FONT_HEIGHT = 20;
	UINT	PADDING_X;
	UINT	PADDING_Y;
	UINT	LINE_HEIGHT;
	double	kX;
	double	kY;
	
	COLORREF COLOR_GRAY;
	COLORREF COLOR_BLACK;

	HFONT fontNormal, fontHeader, fontItalic, fontSmall;
	HBRUSH grayBrush;

	AppModel* appModel;

	void ComputeParameters() override;
	void InitializeFonts() override;
	void InitializeBrushes() override;

public:
	ContentWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	virtual ~ContentWindow();

	virtual POINT RenderResult();
};