#pragma once
#include "View\Framework\Dto\WindowDescriptor.h"

class ViewDescriptor
{
private:
    WindowDescriptor windowDescriptor;
    bool isResizeable;

public:
    ViewDescriptor();
    ViewDescriptor(WindowDescriptor windowDescriptor, bool isResizeable);
    ~ViewDescriptor();

    WindowDescriptor GetWindowDescriptor() const;
    void SetWindowDescriptor(WindowDescriptor windowDescriptor);

    bool IsResizeable() const;
};
