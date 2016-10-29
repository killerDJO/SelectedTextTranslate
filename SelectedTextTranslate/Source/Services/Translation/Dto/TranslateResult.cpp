#include "Services\Translation\Dto\TranslateResult.h"
#include "Utilities\StringUtilities.h"

TranslateResult::TranslateResult()
{
    IsEmptyResult = false;
}

bool TranslateResult::IsInputCorrected() const
{
    wstring trimmedOrigin = StringUtilities::Trim(Sentence.Origin);
    wstring trimmedInput = StringUtilities::Trim(Sentence.Input);
    return trimmedOrigin.compare(trimmedInput) != 0;
}