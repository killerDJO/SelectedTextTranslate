#include "View\Content\Settings\SettingsWindow.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogWindow.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogOverlayWindow.h"
#include "View\Framework\Dto\Rendering\RenderResult.h"

SettingsWindow::SettingsWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->settingsState = SettingsState();
    this->hotkeySettingsWindow = nullptr;

    this->saveButton = nullptr;
    this->cancelButton = nullptr;
    this->resetButton = nullptr;
}

void SettingsWindow::Initialize()
{
    ContentWindow::Initialize();
}

void SettingsWindow::SetModel(Settings model)
{
    ModelHolder<Settings>::SetModel(model);
    globalModel = model;
}

Size SettingsWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);
    DestroyChildWindows();

    RenderPosition renderPosition = RenderPosition(paddingX, paddingY);

    renderPosition = CreateSettingsGroups(RenderDescriptor(renderer, renderPosition));

    renderPosition = renderPosition.MoveY(lineHeight / 2).SetX(paddingX);

    CreateControls(RenderDescriptor(renderer, renderPosition));

    renderer->IncreaseWidth(paddingX);
    renderer->IncreaseHeight(paddingY);

    return renderer->GetScaledSize();
}

RenderResult SettingsWindow::CreateSettingsGroups(RenderDescriptor renderDescriptor)
{
    return CreateHotkeySettingsGroup(renderDescriptor);
}

RenderResult SettingsWindow::CreateHotkeySettingsGroup(RenderDescriptor renderDescriptor)
{
    hotkeySettingsWindow = new HotkeySettingsWindow(context, this);
    RenderPosition renderPosition = InitializeSettingsGroup(renderDescriptor, hotkeySettingsWindow, settingsState.GetHotkeySettingsGroupState(), model.GetHotkeySettings());

    hotkeySettingsWindow->OnSettingsChanged.Subscribe([this](HotkeySettings settings) -> void
    {
        model.SetHotkeySettings(settings);
        SetButtonsState();
        SetSettingsState();
    });

    hotkeySettingsWindow->OnSettingsToggled.Subscribe([this]() -> void
    {
        settingsState.SetHotkeySettingsGroupState(ToggleSettingsGroupState(settingsState.GetHotkeySettingsGroupState()));
        OnSettingsStateChanged.Notify();
    });

    return RenderResult(renderPosition);
}

template<typename TModel>
RenderResult SettingsWindow::InitializeSettingsGroup(RenderDescriptor renderDescriptor, SettingsGroupWindow* settingsGroup, SettingsGroupState state, TModel model)
{
    settingsGroup->SetDimensions(renderDescriptor.GetRenderPosition().GetPosition(), 250);
    settingsGroup->SetState(state);

    AddChildWindow(settingsGroup);
    dynamic_cast<ModelHolder<TModel>*>(settingsGroup)->SetModel(model);

    settingsGroup->Render();

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(settingsGroup);

    return RenderResult(settingsGroup->GetBoundingRect(true));
}

void SettingsWindow::CreateControls(RenderDescriptor renderDescriptor)
{
    RenderPosition renderPosition = CreateSaveButtonControl(renderDescriptor);

    cancelButton = CreateTextButtonControl(RenderDescriptor(renderDescriptor.GetRenderer(), renderPosition.MoveX(7)), L"Cancel", bind(&SettingsWindow::UpdateSettings, this, globalModel));

    wstring resetButtonText = L"Reset";
    int resetButtonTextWidth = context->GetScaleProvider()->Downscale(context->GetRenderingContext()->GetTextSize(resetButtonText, fontSmallUnderscored).Width);
    int resetButtonPositionX = hotkeySettingsWindow->GetBoundingRect(true).GetRight() - resetButtonTextWidth;

    ConfirmDialogWindow* confirmResetDialogWindow = new ConfirmDialogWindow(context, this);
    confirmResetDialogWindow->SetDescriptor(WindowDescriptor::CreateFixedWindowDescriptor(Point(0, 0), GetAvailableClientSize(true)));
    confirmResetDialogWindow->SetTitle(L"Confirm settings reset?");
    confirmResetDialogWindow->MakeHidden();
    AddChildWindow(confirmResetDialogWindow);
    confirmResetDialogWindow->OnConfirm.Subscribe(bind(&SettingsWindow::UpdateSettings, this, Settings()));

    resetButton = CreateTextButtonControl(RenderDescriptor(renderDescriptor.GetRenderer(), Point(resetButtonPositionX, renderPosition.GetY())), resetButtonText, [this, confirmResetDialogWindow]() -> void
    {
        confirmResetDialogWindow->Render();
        confirmResetDialogWindow->Show();
    });

    SetButtonsState();
}

RenderResult SettingsWindow::CreateSaveButtonControl(RenderDescriptor renderDescriptor)
{
    int saveButtonHeight = 21;
    saveButton = new HoverFlatButtonWindow(context, this);
    saveButton->SetDimensions(renderDescriptor.GetRenderPosition().GetPosition(), Size(50, saveButtonHeight));
    saveButton->SetText(L"Save");
    saveButton->OnClick.Subscribe([this]() -> void
    {
        this->OnSaveSettings.Notify(model);
    });
    AddChildWindow(saveButton);

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(saveButton);

    return RenderResult(Point(
        saveButton->GetSize(true).Width + saveButton->GetPosition(true).X,
        renderDescriptor.GetRenderPosition().GetY() + saveButton->GetTextBaseline()));
}

HoverTextButtonWindow* SettingsWindow::CreateTextButtonControl(RenderDescriptor renderDescriptor, wstring text, function<void()> clickCallback)
{
    HoverTextButtonWindow* button = new HoverTextButtonWindow(context, this);
    button->SetFont(fontSmallUnderscored);

    int fontAscent = renderDescriptor.GetRenderer()->GetFontAscent(button->GetFont());
    button->SetPosition(renderDescriptor.GetRenderPosition().MoveY(-fontAscent).GetPosition());
    button->SetText(text);
    button->OnClick.Subscribe(clickCallback);
    AddChildWindow(button);

    renderDescriptor.GetRenderer()->UpdateRenderedContentSize(button);

    return button;
}

void SettingsWindow::UpdateSettings(Settings settings)
{
    model = settings;
    hotkeySettingsWindow->SetModel(model.GetHotkeySettings());
    hotkeySettingsWindow->Render(false);
    SetButtonsState();
    SetSettingsState();
}

void SettingsWindow::SetButtonsState() const
{
    if (model.EqualTo(globalModel))
    {
        saveButton->Disable();
        cancelButton->Disable();
    }
    else
    {
        cancelButton->Enable();
        saveButton->Enable();
    }

    if (model.EqualTo(Settings()))
    {
        resetButton->Disable();
    }
    else
    {
        resetButton->Enable();
    }
}

void SettingsWindow::SetSettingsState() const
{
    if (!globalModel.GetHotkeySettings().EqualTo(hotkeySettingsWindow->GetModel()))
    {
        hotkeySettingsWindow->SetModifiedState();
    }
    else
    {
        hotkeySettingsWindow->SetNotModifiedState();
    }
}

SettingsGroupState SettingsWindow::ToggleSettingsGroupState(SettingsGroupState settingsGroupState) const
{
    return settingsGroupState == SettingsGroupState::Expanded
        ? SettingsGroupState::Collapsed
        : SettingsGroupState::Expanded;
}

SettingsWindow::~SettingsWindow()
{
}
