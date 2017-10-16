#pragma once
#include "View\Framework\View\Component.h"
#include "Services\Dictionary\Dto\DictionaryRecord.h"
#include "View\Framework\ModelHolder.h"
#include "Services\Dictionary\DictionaryService.h"
#include "View\Content\Dictionary\DictionaryView.h"

class DictionaryComponent : public Component<DictionaryView>, public ModelHolder<vector<DictionaryRecord>>
{
private:
    DictionaryService* dictionaryService;

    void ProcessShowTranslation(int index);

public:
    DictionaryComponent(ViewContext* context, View* parentView);

    vector<DictionaryRecord> GetModel() override;

    Subscribeable<wstring> OnShowTranslation;
};