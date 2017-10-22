#pragma once
#include "Presentation\Framework\Dto\LayoutDescriptor.h"
#include "Presentation\Framework\Dto\Positioning\Rect.h"

class IComponent
{
public:
    virtual ~IComponent() = default;
    virtual void SetDescriptor(LayoutDescriptor descriptor) = 0;

    virtual Rect GetBoundingRect() const = 0;
    virtual void Render(bool preserveScrolls = false) const = 0;

    virtual void Initialize() = 0;
    virtual void InitializeAndRender(bool preserveScrolls = false) = 0;

    virtual void MakeVisible() const = 0;
    virtual void MakeHidden() const = 0;
    virtual void Resize() const = 0;
};