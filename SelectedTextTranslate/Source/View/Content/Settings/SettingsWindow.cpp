#include "View\Content\Settings\SettingsWindow.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "View\Controls\Dialogs\Confirm\ConfirmDialogWindow.h"

SettingsWindow::SettingsWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->fontSmallUnderscored = context->GetRenderingContext()->CreateCustomFont(FontSizes::Small, false, true);
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

    int curY = paddingY;

    curY = CreateSettingsGroups(renderer, curY);

    curY += lineHeight / 2;

    CreateControls(renderer, curY, paddingX);

    ConfirmDialogWindow* dialog = new ConfirmDialogWindow(context, this);
    dialog->SetDescriptor(WindowDescriptor::CreateFixedWindowDescriptor(Point(0, 0), GetSize(true)));
    AddChildWindow(dialog);

    renderer->IncreaseWidth(paddingX);
    renderer->IncreaseHeight(paddingY);

    return renderer->GetScaledSize();
}

int SettingsWindow::CreateSettingsGroups(Renderer* renderer, int curY)
{
    return CreateHotkeySettingsGroup(renderer, curY);
}

int SettingsWindow::CreateHotkeySettingsGroup(Renderer* renderer, int curY)
{
    hotkeySettingsWindow = new HotkeySettingsWindow(context, this);
    curY = InitializeSettingsGroup(renderer, hotkeySettingsWindow, curY, settingsState.GetHotkeySettingsGroupState(), model.GetHotkeySettings());

    hotkeySettingsWindow->OnSettingsChanged.Subscribe([this](HotkeySettings settings) -> void
    {
        if (!globalModel.GetHotkeySettings().EqualTo(settings))
        {
            hotkeySettingsWindow->SetModifiedState();
        }
        else
        {
            hotkeySettingsWindow->SetNotModifiedState();
        }

        model.SetHotkeySettings(settings);
        SetButtonsState();
    });

    hotkeySettingsWindow->OnSettingsToggled.Subscribe([this]() -> void
    {
        settingsState.SetHotkeySettingsGroupState(ToggleSettingsGroupState(settingsState.GetHotkeySettingsGroupState()));
        OnSettingsStateChanged.Notify();
    });

    return curY;
}

template<typename TModel>
int SettingsWindow::InitializeSettingsGroup(Renderer* renderer, SettingsGroupWindow* settingsGroup, int curY, SettingsGroupState state, TModel model)
{
    settingsGroup->SetDimensions(Point(paddingX, curY), 250);
    settingsGroup->SetState(state);

    AddChildWindow(settingsGroup);
    dynamic_cast<ModelHolder<TModel>*>(settingsGroup)->SetModel(model);

    settingsGroup->Render();

    renderer->UpdateRenderedContentSize(settingsGroup);

    return curY + settingsGroup->GetSize(true).Height;
}

void SettingsWindow::CreateControls(Renderer* renderer, int curY, int curX)
{
    Point saveButtonPosition = CreateSaveButtonControl(renderer, curY, curX);

    cancelButton = CreateTextButtonControl(renderer, Point(saveButtonPosition.X + 7, saveButtonPosition.Y), L"Cancel", bind(&SettingsWindow::UpdateSettings, this, globalModel));

    wstring resetButtonText = L"Reset";
    int resetButtonTextWidth = context->GetScaleProvider()->Downscale(context->GetRenderingContext()->GetTextSize(resetButtonText, fontSmallUnderscored).Width);
    int resetButtonPositionX = hotkeySettingsWindow->GetBoundingRect(true).GetRight() - resetButtonTextWidth;

    resetButton = CreateTextButtonControl(renderer, Point(resetButtonPositionX, saveButtonPosition.Y), resetButtonText, bind(&SettingsWindow::UpdateSettings, this, Settings()));

    SetButtonsState();
}

Point SettingsWindow::CreateSaveButtonControl(Renderer* renderer, int curY, int curX)
{
    int saveButtonHeight = 21;
    saveButton = new HoverFlatButtonWindow(context, this);
    saveButton->SetDimensions(Point(paddingX, curY), Size(50, saveButtonHeight));
    saveButton->SetText(L"Save");
    saveButton->OnClick.Subscribe([this]() -> void
    {
        this->OnSaveSettings.Notify(model);
    });
    AddChildWindow(saveButton);

    renderer->UpdateRenderedContentSize(saveButton);

    return Point(
        saveButton->GetSize(true).Width + saveButton->GetPosition(true).X,
        curY + saveButton->GetTextBaseline());
}

HoverTextButtonWindow* SettingsWindow::CreateTextButtonControl(Renderer* renderer, Point position, wstring text, function<void()> clickCallback)
{
    HoverTextButtonWindow* button = new HoverTextButtonWindow(context, this);
    button->SetFont(fontSmallUnderscored);

    int fontAscent = renderer->GetFontAscent(button->GetFont());
    button->SetPosition(Point(position.X, position.Y - fontAscent));
    button->SetText(text);
    button->OnClick.Subscribe(clickCallback);
    AddChildWindow(button);

    renderer->UpdateRenderedContentSize(button);

    return button;
}

void SettingsWindow::UpdateSettings(Settings settings)
{
    model = settings;
    hotkeySettingsWindow->SetModel(model.GetHotkeySettings());
    hotkeySettingsWindow->Render(false);
    SetButtonsState();
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

SettingsGroupState SettingsWindow::ToggleSettingsGroupState(SettingsGroupState settingsGroupState) const
{
    return settingsGroupState == SettingsGroupState::Expanded
        ? SettingsGroupState::Collapsed
        : SettingsGroupState::Expanded;
}

SettingsWindow::~SettingsWindow()
{
    AssertCriticalWinApiResult(DeleteObject(fontSmallUnderscored));
}
