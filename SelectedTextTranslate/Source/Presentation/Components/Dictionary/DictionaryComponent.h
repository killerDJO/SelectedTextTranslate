#pragma once
#include "Presentation\Framework\Components\Component.h"
#include "BusinessLogic\Dictionary\Dto\DictionaryRecord.h"
#include "Presentation\Framework\ModelHolder.h"
#include "BusinessLogic\Dictionary\DictionaryService.h"
#include "Presentation\Components\Dictionary\DictionaryView.h"

class DictionaryComponent : public Component<DictionaryView>, public ModelHolder<vector<DictionaryRecord>>
{
private:
    DictionaryService* dictionaryService;

    void ProcessShowTranslation(int index);

public:
    DictionaryComponent(ServiceRegistry* serviceRegistry, View* parentView);

    vector<DictionaryRecord> GetModel() override;

    Subscribeable<wstring> OnShowTranslation;
};