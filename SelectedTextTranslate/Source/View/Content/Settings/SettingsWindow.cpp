#include "View\Content\Settings\SettingsWindow.h"
#include "View\Controls\Buttons\HoverFlatButtonWindow.h"
#include "View\Controls\Buttons\HoverTextButtonWindow.h"

SettingsWindow::SettingsWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    settingsGroupsState[0] = true;
    settingsGroupsState[1] = true;

    this->fontSmallUnderscored = context->GetRenderingContext()->CreateCustomFont(FontSizes::Small, false, true);
    this->hotkeySettingsWindow = nullptr;
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

    hotkeySettingsWindow = new HotkeySettingsWindow(context, this);
    curY = InitializeSettingsGroup(renderer, hotkeySettingsWindow, curY, 0, model.GetHotkeySettings());
    hotkeySettingsWindow->OnSettingsChanged.Subscribe([this](HotkeySettings settings) -> void
    {
        this->model.SetHotkeySettings(settings);
        this->SetSaveButtonState();
    });

    curY += lineHeight / 2;

    CreateControls(renderer, curY);

    renderer->IncreaseWidth(paddingX);
    renderer->IncreaseHeight(paddingY);

    return renderer->GetScaledSize();
}

template<typename TModel>
int SettingsWindow::InitializeSettingsGroup(Renderer* renderer, SettingsGroupWindow* settingsGroup, int curY, int index, TModel model)
{
    settingsGroup->SetDimensions(Point(paddingX, curY), 250);
    settingsGroup->SetState(settingsGroupsState[index]);
    settingsGroup->OnSettingsToggled.Subscribe([index, this]() -> void
    {
        settingsGroupsState[index] ^= true;
        this->OnSettingsStateChanged.Notify();
    });

    AddChildWindow(settingsGroup);
    dynamic_cast<ModelHolder<TModel>*>(settingsGroup)->SetModel(model);

    settingsGroup->Render();

    renderer->UpdateSize(Size(
        settingsGroup->GetSize().Width + settingsGroup->GetPosition().X,
        settingsGroup->GetSize().Height + settingsGroup->GetPosition().Y));

    return curY + settingsGroup->GetSize(true).Height;
}

void SettingsWindow::CreateControls(Renderer* renderer, int curY)
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
    saveButton->Disable();

    renderer->UpdateSize(Size(
        saveButton->GetSize().Width + saveButton->GetPosition().X,
        saveButton->GetSize().Height + saveButton->GetPosition().Y));

    int curX = saveButton->GetSize(true).Width + saveButton->GetPosition(true).X;

    HoverTextButtonWindow* resetButton = new HoverTextButtonWindow(context, this);
    resetButton->SetFont(fontSmallUnderscored);

    int fontAscent = renderer->GetFontAscent(resetButton->GetFont());
    resetButton->SetPosition(Point(curX + 7, curY + saveButton->GetTextBaseline() - fontAscent));
    resetButton->SetText(L"Reset to default");
    resetButton->OnClick.Subscribe([this]() -> void
    {
        //this->OnSaveSettings.Notify(model);
    });
    AddChildWindow(resetButton);

    renderer->UpdateSize(Size(
        resetButton->GetSize().Width + resetButton->GetPosition().X,
        resetButton->GetSize().Height + resetButton->GetPosition().Y));
}

void SettingsWindow::SetSaveButtonState() const
{
    if (model.EqualTo(globalModel))
    {
        saveButton->Disable();
    }
    else
    {
        saveButton->Enable();
    }
}

SettingsWindow::~SettingsWindow()
{
}
