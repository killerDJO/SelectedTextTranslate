#include "DictionaryComponent.h"
#include "View\Content\Dictionary\DictionaryView.h"
#include "Services\Dictionary\DictionaryService.h"

DictionaryComponent::DictionaryComponent(ViewContext* context, View* parentView, DictionaryService* dictionaryService)
    : Component(new DictionaryView(context, parentView, this))
{
    this->dictionaryService = dictionaryService;
    this->view->OnShowTranslation.Subscribe(bind(&DictionaryComponent::ProcessShowTranslation, this, placeholders::_1));
}

void DictionaryComponent::ProcessShowTranslation(int wordInDictionaryIndex)
{
    vector<DictionaryRecord> dictionaryRecords = dictionaryService->GetTopRecords(200);
    DictionaryRecord recordToTranslate = dictionaryRecords[wordInDictionaryIndex];
    OnShowTranslation.Notify(recordToTranslate.GetWord());
}

vector<DictionaryRecord> DictionaryComponent::GetModel()
{
    return dictionaryService->GetTopRecords(200);
}