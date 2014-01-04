#pragma once
class ScrollbarManager
{
public: 
	static double xChar;       // horizontal scrolling unit 
	static double yChar;       // vertical scrolling unit 

	static int xPos;        // current horizontal scrolling position 
	static int yPos;        // current vertical scrolling position

	void InitializeScrollbars(HWND hwnd, int windowWidth, int windowHeight, int horizontalChars, int verticalChars);
	void OnHorizontalScroll(HWND hwnd,  WPARAM wParam, LPARAM lParam);
	void OnVerticalScroll(HWND hwnd,  WPARAM wParam, LPARAM lParam);
};