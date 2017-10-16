//#pragma once
//
//class Component
//{
//protected:
//    View* parentWindow;
//    bool isLayered;
//
//    Point GetInitialViewOffset() override;
//    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;
//
//public:
//    ChildView(ViewContext* context, View* parentWindow);
//
//    void Initialize() override;
//
//    void EnableLayeredMode();
//
//    Subscribeable<> OnRequestRender;
//};