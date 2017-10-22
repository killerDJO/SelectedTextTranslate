#pragma once
#include "View\Framework\Dto\WindowDescriptor.h"
#include "View\Framework\Dto\Positioning\Rect.h"

class IComponent
{
public:
    virtual ~IComponent() = default;
    virtual void SetDescriptor(WindowDescriptor descriptor) = 0;

    virtual Rect GetBoundingRect() const = 0;
    virtual void Render(bool preserveScrolls = false) const = 0;

    virtual void Initialize() = 0;
    virtual void InitializeAndRender(bool preserveScrolls = false) = 0;

    virtual void MakeVisible() const = 0;
    virtual void MakeHidden() const = 0;
    virtual void Resize() const = 0;
};