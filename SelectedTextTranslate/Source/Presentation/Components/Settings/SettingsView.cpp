#include "Presentation\Components\Settings\SettingsView.h"
#include "Presentation\Controls\Buttons\HoverFlatButtonControl.h"
#include "Presentation\Controls\Buttons\HoverTextButtonControl.h"
#include "Presentation\Framework\Rendering\Dto\RenderResult.h"
#include "Presentation\Components\Settings\Hotkeys\HotkeySettingsComponent.h"
#include "Presentation\MessageBus.h"

SettingsView::SettingsView(CommonContext* context, View* parentView, ModelHolder<SettingsViewModel*>* modelHolder)
    : ComponentView(context, parentView, modelHolder)
{
    this->saveButton = nullptr;
    this->cancelButton = nullptr;
    this->resetButton = nullptr;

    this->Name = L"SettingsWindow";
}

Size SettingsView::RenderContent(Renderer* renderer)
{
    DestroyChildViews();

    RenderPosition renderPosition = RenderPosition(PaddingX, PaddingY);

    renderPosition = CreateSettingsGroups(RenderDescriptor(renderer, renderPosition));

    renderPosition = renderPosition.MoveY(LineHeight / 2).SetX(PaddingX);

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
    HotkeySettingsComponent* hotkeySettingsComponent = new HotkeySettingsComponent(Context->GetServiceRegistry(), this, GetModel()->GetHotkeySettingsViewModel());
    RenderPosition renderPosition = AddSettingsGroup(renderDescriptor, hotkeySettingsComponent);

    return RenderResult(renderPosition);
}

RenderResult SettingsView::InitializeSettingsGroup(RenderDescriptor renderDescriptor, IComponent* settingsGroup)
{
    settingsGroups.push_back(settingsGroup);

    settingsGroup->SetLayout(LayoutDescriptor::CreateLayoutDescriptor(
        renderDescriptor.GetRenderPosition().GetPosition(),
        Size(ScaleProvider->Scale(257), 0),
        OverflowModes::Fixed,
        OverflowModes::Stretch));

    settingsGroup->InitializeAndRender();

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(settingsGroup);

    return RenderResult(settingsGroup->GetBoundingRect());
}

void SettingsView::CreateControls(RenderDescriptor renderDescriptor)
{
    RenderPosition renderPosition = CreateSaveButtonControl(renderDescriptor);

    cancelButton = CreateTextButtonControl(
        RenderDescriptor(renderDescriptor.GetRenderer(), renderPosition.MoveX(7, ScaleProvider)),
        L"Cancel",
        &OnCancelChanges);

    wstring resetButtonText = L"Reset";
    int resetButtonTextWidth = RenderingProvider->GetTextSize(resetButtonText, FontSmallUnderscored).GetWidth();

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
    saveButton = new HoverFlatButtonControl(Context, this);
    saveButton->SetPosition(renderDescriptor.GetRenderPosition().GetPosition());
    saveButton->SetText(L"Save");
    saveButton->OnClick.Subscribe(&OnSaveSettings);
    saveButton->InitializeAndRender();

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(saveButton);

    return RenderResult(Point(
        saveButton->GetBoundingRect().GetRight(),
        saveButton->GetPosition().GetY() + saveButton->GetTextBaseline()));
}

HoverTextButtonControl* SettingsView::CreateTextButtonControl(RenderDescriptor renderDescriptor, wstring text, Subscribeable<>* clickCallback)
{
    HoverTextButtonControl* button = new HoverTextButtonControl(Context, this);
    button->SetFont(FontSmallUnderscored);

    button->SetPosition(renderDescriptor.GetRenderPosition().MoveY(-button->GetFont()->GetAscent()).GetPosition());
    button->SetText(text);
    button->OnClick.Subscribe(clickCallback);
    button->InitializeAndRender();

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(button);

    return button;
}

void SettingsView::SetButtonsState() const
{
    bool areSettingsModified = GetModel()->HasChanges();
    bool areSettingsValid = GetModel()->IsValid();

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

    if (!GetModel()->AreDefaultSettings())
    {
        resetButton->Enable();
    }
    else
    {
        resetButton->Disable();
    }
}