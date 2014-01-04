#include "stdafx.h"
#include "ScrollbarManager.h"

int ScrollbarManager::xPos;        
int ScrollbarManager::yPos;
 
double ScrollbarManager::xChar;       
double ScrollbarManager::yChar;        	

void ScrollbarManager::InitializeScrollbars(HWND hwnd, int windowWidth, int windowHeight, int horizontalChars, int verticalChars)
{
	HDC hdc = GetDC (hwnd);	
	SCROLLINFO si;  
	
	xChar = 8; 
	yChar = 20;  // Line height
  
	ReleaseDC (hwnd, hdc); 
 
	si.cbSize = sizeof(si); 
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
	si.nMin   = 0; 
	si.nPos	  = 0;
	si.nMax   = verticalChars; 
	si.nPage  = (int)(windowHeight / yChar); 
	SetScrollInfo(hwnd, SB_VERT, &si, TRUE); 
 
	si.cbSize = sizeof(si); 
	si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS; 
	si.nMin   = 0; 
	si.nPos	  = 0;
	si.nMax   = horizontalChars; 
	si.nPage  = (int)(windowWidth / xChar); 
	SetScrollInfo(hwnd, SB_HORZ, &si, TRUE); 
}

void ScrollbarManager::OnHorizontalScroll(HWND hwnd,  WPARAM wParam, LPARAM lParam)
{
	SCROLLINFO si;
	si.cbSize = sizeof (si);
	si.fMask  = SIF_ALL;

	GetScrollInfo (hwnd, SB_HORZ, &si);
	xPos = si.nPos;
	switch (LOWORD (wParam))
	{
		case SB_LINELEFT: 
			si.nPos -= 1;
			break;
              
		case SB_LINERIGHT: 
			si.nPos += 1;
			break;
              
		case SB_LEFT:
		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;
              
		case SB_RIGHT:
		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;
              		
		case SB_THUMBTRACK: 
			si.nPos = si.nTrackPos;
			break;
              
		default:
			break;
	}

	si.fMask = SIF_POS;
	SetScrollInfo (hwnd, SB_HORZ, &si, TRUE);
	GetScrollInfo (hwnd, SB_HORZ, &si);
         
	if (si.nPos != xPos)
	{
		ScrollWindow(hwnd, xChar * (xPos - si.nPos), 0, NULL, NULL);
	}
}

void ScrollbarManager::OnVerticalScroll(HWND hwnd,  WPARAM wParam, LPARAM lParam)
{
	SCROLLINFO si;
	si.cbSize = sizeof (si);
        si.fMask  = SIF_ALL;
        GetScrollInfo (hwnd, SB_VERT, &si);

    yPos = si.nPos;
    switch (LOWORD (wParam))
    {
		case SB_TOP:
			si.nPos = si.nMin;
			break;
              
		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;
              
		case SB_LINEUP:
			si.nPos -= 1;
			break;
              
		case SB_LINEDOWN:
			si.nPos += 1;
			break;
              
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
              
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
              
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
              
		default:
			break; 
    }

    si.fMask = SIF_POS;
    SetScrollInfo (hwnd, SB_VERT, &si, TRUE);
    GetScrollInfo (hwnd, SB_VERT, &si);

    if (si.nPos != yPos)
    {                    
        ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
    }
}