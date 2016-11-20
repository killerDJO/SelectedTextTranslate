#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "Services\Dictionary\Dto\DictionaryRecord.h"
#include "Utilities\Subscribeable.h"

class DictionaryWindow : public ContentWindow
{
private:
    vector<DictionaryRecord> dictionaryRecords;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    DictionaryWindow(WindowContext* context, Window* parentWindow);
    ~DictionaryWindow() override;

    Subscribeable<int> OnShowTranslation;

    void SetModel(vector<DictionaryRecord> dictionaryRecords);
};