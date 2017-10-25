#pragma once
#include "Presentation\Framework\Dto\LayoutDescriptor.h"

class ViewDescriptor
{
private:
    LayoutDescriptor layoutDescriptor;
    bool isResizeable;

public:
    ViewDescriptor();
    ViewDescriptor(LayoutDescriptor layoutDescriptor, bool isResizeable);

    LayoutDescriptor GetLayoutDescriptor() const;
    void SetLayoutDescriptor(LayoutDescriptor layoutDescriptor);

    bool IsResizeable() const;
};
