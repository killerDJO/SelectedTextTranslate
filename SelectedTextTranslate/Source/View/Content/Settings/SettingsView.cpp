#include "View\Content\Settings\SettingsView.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "View\Framework\Rendering\Dto\RenderResult.h"
#include "View\Content\Settings\Hotkeys\HotkeySettingsComponent.h"
#include "View\Framework\MessageBus.h"

SettingsView::SettingsView(ViewContext* context, View* parentView, ModelHolder<Settings>* modelHolder)
    : ComponentView(context, parentView, modelHolder)
{
    this->hotkeySettingsComponent = nullptr;

    this->saveButton = nullptr;
    this->cancelButton = nullptr;
    this->resetButton = nullptr;
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
    hotkeySettingsComponent = new HotkeySettingsComponent(context, this, this);
    RenderPosition renderPosition = InitializeSettingsGroup(renderDescriptor, hotkeySettingsComponent);

    return RenderResult(renderPosition);
}

void SettingsView::CreateControls(RenderDescriptor renderDescriptor)
{
    RenderPosition renderPosition = CreateSaveButtonControl(renderDescriptor);

    cancelButton = CreateTextButtonControl(
        RenderDescriptor(renderDescriptor.GetRenderer(), renderPosition.MoveX(7, scaleProvider)),
        L"Cancel",
        bind(&SettingsView::CancelSettingsChanges, this));

    wstring resetButtonText = L"Reset";
    int resetButtonTextWidth = context->Get<RenderingProvider>()->GetTextSize(resetButtonText, fontSmallUnderscored).GetWidth();

    //TODO: make settings group agnostic
    int resetButtonPositionX = hotkeySettingsComponent->GetBoundingRect().GetRight() - resetButtonTextWidth;

    resetButton = CreateTextButtonControl(RenderDescriptor(renderDescriptor.GetRenderer(), Point(resetButtonPositionX, renderPosition.GetY())), resetButtonText, [this]() -> void
    {
        OnResetSettings.Notify();
    });

    SetButtonsState();
}

RenderResult SettingsView::CreateSaveButtonControl(RenderDescriptor renderDescriptor)
{
    saveButton = new HoverFlatButtonWindow(context, this);
    saveButton->SetPosition(renderDescriptor.GetRenderPosition().GetPosition());
    saveButton->SetText(L"Save");
    saveButton->OnClick.Subscribe([this]() -> void
    {
        this->OnSaveSettings.Notify(GetCurrentSettings());
    });
    saveButton->InitializeAndRender();

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(saveButton);

    return RenderResult(Point(
        saveButton->GetBoundingRect().GetRight(),
        saveButton->GetPosition().GetY() + saveButton->GetTextBaseline()));
}

HoverTextButtonWindow* SettingsView::CreateTextButtonControl(RenderDescriptor renderDescriptor, wstring text, function<void()> clickCallback)
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

void SettingsView::CancelSettingsChanges() const
{
    hotkeySettingsComponent->ResetCurrentSettings();
    hotkeySettingsComponent->Render(false);
    SetButtonsState();
}

void SettingsView::SetButtonsState() const
{
    bool areSettingsModified = !hotkeySettingsComponent->HasChanges();
    bool areSettingsValid = hotkeySettingsComponent->IsValid();

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

    bool isResetPossible = GetCurrentSettings().EqualTo(Settings());
    if (isResetPossible)
    {
        resetButton->Disable();
    }
    else
    {
        resetButton->Enable();
    }
}

Settings SettingsView::GetCurrentSettings() const
{
    return Settings(hotkeySettingsComponent->GetCurrentSettings());
}

HotkeySettings SettingsView::GetModel()
{
    return modelHolder->GetModel().GetHotkeySettings();
}
