#include "View\Content\Settings\SettingsView.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsComponent.h"
#include "View\Framework\MessageBus.h"

SettingsView::SettingsView(ViewContext* context, View* parentView, ModelHolder<SettingsViewModel*>* modelHolder)
    : ComponentView(context, parentView, modelHolder)
{
    this->saveButton = nullptr;
    this->cancelButton = nullptr;
    this->resetButton = nullptr;

    this->viewName = L"SettingsWindow";
}

Size SettingsView::RenderContent(Renderer* renderer)
{
    DestroyChildViews();

    RenderPosition renderPosition = RenderPosition(paddingX, paddingY);

    renderPosition = CreateSettingsGroups(RenderDescriptor(renderer, renderPosition));

    renderPosition = renderPosition.MoveY(lineHeight / 2).SetX(paddingX);

    CreateControls(RenderDescriptor(renderer, renderPosition));

    SetButtonsState();

    return renderer->GetSize();
}

RenderResult SettingsView::CreateSettingsGroups(RenderDescriptor renderDescriptor)
{
    return CreateHotkeySettingsGroup(renderDescriptor);
}

RenderResult SettingsView::CreateHotkeySettingsGroup(RenderDescriptor renderDescriptor)
{
    HotkeySettingsComponent* hotkeySettingsComponent = new HotkeySettingsComponent(context, this, modelHolder->GetModel()->GetHotkeySettingsViewModel());
    RenderPosition renderPosition = InitializeSettingsGroup(renderDescriptor, hotkeySettingsComponent);

    return RenderResult(renderPosition);
}

void SettingsView::CreateControls(RenderDescriptor renderDescriptor)
{
    RenderPosition renderPosition = CreateSaveButtonControl(renderDescriptor);

    cancelButton = CreateTextButtonControl(
        RenderDescriptor(renderDescriptor.GetRenderer(), renderPosition.MoveX(7, scaleProvider)),
        L"Cancel",
        &OnCancelChanges);

    wstring resetButtonText = L"Reset";
    int resetButtonTextWidth = context->Get<RenderingProvider>()->GetTextSize(resetButtonText, fontSmallUnderscored).GetWidth();

    int maxRightPosition = 0;
    for(size_t i = 0; i < settingsGroups.size(); ++i)
    {
        maxRightPosition = max(maxRightPosition, settingsGroups[i]->GetBoundingRect().GetRight());
    }
    int resetButtonPositionX = maxRightPosition - resetButtonTextWidth;

    resetButton = CreateTextButtonControl(RenderDescriptor(renderDescriptor.GetRenderer(), Point(resetButtonPositionX, renderPosition.GetY())), resetButtonText, &OnResetSettings);

    SetButtonsState();
}

RenderResult SettingsView::CreateSaveButtonControl(RenderDescriptor renderDescriptor)
{
    saveButton = new HoverFlatButtonWindow(context, this);
    saveButton->SetPosition(renderDescriptor.GetRenderPosition().GetPosition());
    saveButton->SetText(L"Save");
    saveButton->OnClick.Subscribe(&OnSaveSettings);
    saveButton->InitializeAndRender();

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(saveButton);

    return RenderResult(Point(
        saveButton->GetBoundingRect().GetRight(),
        saveButton->GetPosition().GetY() + saveButton->GetTextBaseline()));
}

HoverTextButtonWindow* SettingsView::CreateTextButtonControl(RenderDescriptor renderDescriptor, wstring text, Subscribeable<>* clickCallback)
{
    HoverTextButtonWindow* button = new HoverTextButtonWindow(context, this);
    button->SetFont(fontSmallUnderscored);

    button->SetPosition(renderDescriptor.GetRenderPosition().MoveY(-button->GetFont()->GetAscent()).GetPosition());
    button->SetText(text);
    button->OnClick.Subscribe(clickCallback);
    button->InitializeAndRender();

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(button);

    return button;
}

void SettingsView::SetButtonsState() const
{
    bool areSettingsModified = modelHolder->GetModel()->HasChanges();
    bool areSettingsValid = modelHolder->GetModel()->IsValid();

    if (areSettingsModified)
    {
        cancelButton->Enable();
    }
    else
    {
        cancelButton->Disable();
    }

    if (areSettingsModified && areSettingsValid)
    {
        saveButton->Enable();
    }
    else
    {
        saveButton->Disable();
    }

    if (!modelHolder->GetModel()->AreDefaultSettings())
    {
        resetButton->Enable();
    }
    else
    {
        resetButton->Disable();
    }
}