#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "Services\Dictionary\Dto\LogRecord.h"
#include "Utilities\Subscribeable.h"

class DictionaryWindow : public ContentWindow
{
private:
    vector<LogRecord> dictionaryRecords;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    DictionaryWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow);
    ~DictionaryWindow() override;

    Subscribeable<int> OnShowTranslation;

    void SetModel(vector<LogRecord> dictionaryRecords);
};