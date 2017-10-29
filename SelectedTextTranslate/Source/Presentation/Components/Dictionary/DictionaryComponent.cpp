#include "DictionaryComponent.h"
#include "Presentation\Components\Dictionary\DictionaryView.h"
#include "BusinessLogic\Dictionary\DictionaryService.h"

DictionaryComponent::DictionaryComponent(ServiceRegistry* serviceRegistry, View* parentView)
    : Component(new DictionaryView(serviceRegistry->Get<ViewContext>(), parentView, this, this))
{
    dictionaryService = serviceRegistry->Get<DictionaryService>();
    messageBus = serviceRegistry->Get<MessageBus>();

    CurrentView->OnShowTranslation.Subscribe(bind(&DictionaryComponent::ProcessShowTranslation, this, placeholders::_1));
}

void DictionaryComponent::ProcessShowTranslation(int wordInDictionaryIndex) const
{
    vector<DictionaryRecord> dictionaryRecords = dictionaryService->GetTopRecords(200);
    DictionaryRecord recordToTranslate = dictionaryRecords[wordInDictionaryIndex];
    messageBus->OnTranslateText.Notify(recordToTranslate.GetWord(), false);
}

vector<DictionaryRecord> DictionaryComponent::GetModel()
{
    return dictionaryService->GetTopRecords(200);
}