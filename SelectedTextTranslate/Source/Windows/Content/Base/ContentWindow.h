#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Entities\TranslateResult.h"
#include "Helpers\Utilities.h"
#include "Windows\Base\WindowBase.h"

class ContentWindow : public WindowBase
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

	void ComputeParameters() override;
	void InitializeFonts() override;
	void InitializeBrushes() override;

public:
	ContentWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
	~ContentWindow();

	virtual POINT RenderResult();
};