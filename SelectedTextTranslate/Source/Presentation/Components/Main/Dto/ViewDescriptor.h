#pragma once
#include "Presentation\Framework\Dto\WindowDescriptor.h"

class ViewDescriptor
{
private:
    WindowDescriptor windowDescriptor;
    bool isResizeable;

public:
    ViewDescriptor();
    ViewDescriptor(WindowDescriptor windowDescriptor, bool isResizeable);

    WindowDescriptor GetWindowDescriptor() const;
    void SetWindowDescriptor(WindowDescriptor windowDescriptor);

    bool IsResizeable() const;
};
