#pragma once
#include "Presentation\Framework\Dto\LayoutDescriptor.h"

class ViewDescriptor
{
private:
    LayoutDescriptor windowDescriptor;
    bool isResizeable;

public:
    ViewDescriptor();
    ViewDescriptor(LayoutDescriptor windowDescriptor, bool isResizeable);

    LayoutDescriptor GetWindowDescriptor() const;
    void SetWindowDescriptor(LayoutDescriptor windowDescriptor);

    bool IsResizeable() const;
};
