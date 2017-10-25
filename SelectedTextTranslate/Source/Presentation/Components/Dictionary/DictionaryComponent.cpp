#include "DictionaryComponent.h"
#include "Presentation\Components\Dictionary\DictionaryView.h"
#include "BusinessLogic\Dictionary\DictionaryService.h"

DictionaryComponent::DictionaryComponent(CommonContext* context, View* parentView)
    : Component(context, new DictionaryView(context, parentView, this))
{
    dictionaryService = context->Get<DictionaryService>();
    CurrentView->OnShowTranslation.Subscribe(bind(&DictionaryComponent::ProcessShowTranslation, this, placeholders::_1));
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