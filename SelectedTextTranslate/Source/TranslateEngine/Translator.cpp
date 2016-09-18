#include "TranslateEngine\Translator.h"

using namespace web;

Translator::Translator(Logger* logger, RequestProvider* requestProvider, TranslatePageParser* translatePageParser)
{
    this->requestProvider = requestProvider;
    this->translatePageParser = translatePageParser;
    this->logger = logger;
}

TranslateResult Translator::TranslateSentence(wstring sentence)
{
    //wstring translatorResponse = L"[[[\"размер\",\"size\",,,2],[,,\"razmer\",\"sīz\"]],[[\"имя существительное\",[\"размер\",\"формат\",\"объем\",\"величина\",\"сечение\",\"габарит\",\"калибр\",\"номер\",\"клей\",\"шлихта\",\"мерка\",\"кегель\",\"проклейка\",\"класс крупности\",\"клейстер\",\"шлихт\",\"номер обуви\"],[[\"размер\",[\"size\",\"rate\",\"scale\",\"extent\",\"dimension\",\"proportions\"],,0.47236654],[\"формат\",[\"format\",\"size\",\"form\"],,0.011825466],[\"объем\",[\"volume\",\"capacity\",\"size\",\"bulk\",\"space\",\"content\"],,0.0077553294],[\"величина\",[\"value\",\"size\",\"quantity\",\"magnitude\",\"dimension\",\"greatness\"],,0.0074001872],[\"сечение\",[\"section\",\"cut\",\"size\"],,0.00028693638],[\"габарит\",[\"size\",\"overall dimensions\"],,0.00019415087],[\"калибр\",[\"caliber\",\"gauge\",\"gage\",\"size\",\"pass\",\"bore\"],,0.00016349142],[\"номер\",[\"room\",\"number\",\"item\",\"size\",\"gauge\",\"gage\"],,0.00011775846],[\"клей\",[\"glue\",\"adhesive\",\"paste\",\"cement\",\"size\",\"gum\"],,3.8230573e-05],[\"шлихта\",[\"size\"],,2.1445408e-05],[\"мерка\",[\"measure\",\"yardstick\",\"size\"],,9.8184237e-06],[\"кегель\",[\"size\"],,7.1833119e-06],[\"проклейка\",[\"size\"],,5.1739457e-06],[\"класс крупности\",[\"size\"],,3.0895039e-06],[\"клейстер\",[\"paste\",\"flour paste\",\"size\"],,1.0348542e-06],[\"шлихт\",[\"size\"],,1.0348542e-06],[\"номер обуви\",[\"size\"],,1.0348542e-06]],\"size\",1],[\"глагол\",[\"калибровать\",\"шлихтовать\",\"разделять по величине\",\"сортировать по размеру\",\"сортировать по величине\",\"определять величину\",\"классифицировать\",\"изготовлять по размеру\",\"доводить\",\"проклеивать\"],[[\"калибровать\",[\"calibrate\",\"gauge\",\"size\",\"gage\",\"standardize\",\"graduate\"],,7.1833119e-06],[\"шлихтовать\",[\"size\"],,1.5294154e-06],[\"разделять по величине\",[\"size\"],,1.0348542e-06],[\"сортировать по размеру\",[\"size\"],,1.0348542e-06],[\"сортировать по величине\",[\"size\"],,1.0348542e-06],[\"определять величину\",[\"size\"],,1.0348542e-06],[\"классифицировать\",[\"classify\",\"categorize\",\"sort\",\"class\",\"rank\",\"size\"],,1.0348542e-06],[\"изготовлять по размеру\",[\"size\"],,1.0348542e-06],[\"доводить\",[\"bring\",\"carry\",\"drive\",\"lead to\",\"lap\",\"size\"],,1.0348542e-06],[\"проклеивать\",[\"size\"],,1.0348542e-06]],\"size\",2]],\"en\",,,[[\"size\",32000,[[\"размер\",1000,true,false]],[[0,4]],\"size\",0,0]],0.92248064,,[[\"en\"],,[0.92248064],[\"en\"]],,,[[\"имя существительное\",[[[\"dimensions\",\"measurements\",\"proportions\",\"magnitude\",\"largeness\",\"bigness\",\"area\",\"expanse\",\"breadth\",\"width\",\"length\",\"height\",\"depth\",\"immensity\",\"hugeness\",\"vastness\"],\"m_en_us1291061.002\"],[[\"sizing\"],\"\"]],\"size\"],[\"глагол\",[[[\"sort\",\"categorize\",\"classify\"],\"m_en_us1291061.005\"]],\"size\"]],[[\"имя существительное\",[[\"the relative extent of something; a thing's overall dimensions or magnitude; how big something is.\",\"m_en_us1291061.001\",\"the schools varied in size\"],[\"each of the classes, typically numbered, into which garments or other articles are divided according to how large they are.\",\"m_en_us1291061.003\",\"I can never find anything in my size\"],[\"a gelatinous solution used in gilding paper, stiffening textiles, and preparing plastered walls for decoration.\",\"m_en_us1291062.001\"]],\"size\"],[\"глагол\",[[\"alter or sort in terms of size or according to size.\",\"m_en_us1291061.005\",\"some drills are sized in millimeters\"],[\"treat with size to glaze or stiffen.\",\"m_en_us1291062.002\",\"Be sure to prime or size any newly applied drywall compound before painting or papering.\"]],\"size\"],[\"имя прилагательное\",[[\"having a specified size; sized.\",\"m_en_us1291061.008\",\"marble-size chunks of hail\"]],\"size\"]],[[[\"They can range in \u003cb\u003esize\u003c/b\u003e from the huge Wellingtonia tree to ground cover woodland plants.\",,,,3,\"m_en_us1291061.001\"],[\"The collection ranges in \u003cb\u003esize\u003c/b\u003e from the miniature to those over five feet in height.\",,,,3,\"m_en_us1291061.001\"],[\"they can make them to \u003cb\u003esize\u003c/b\u003e\",,,,3,\"neid_18967\"],[\"she was wearing a \u003cb\u003esize\u003c/b\u003e 20\",,,,3,\"neid_18971\"],[\"She was barefoot, and wearing only a flowered dress which was a \u003cb\u003esize\u003c/b\u003e too big for her skinny frame.\",,,,3,\"m_en_us1291061.003\"],[\"The houses vary in \u003cb\u003esize\u003c/b\u003e , with a choice of one to four bedrooms, sleeping up to eight.\",,,,3,\"m_en_us1291061.001\"],[\"he bought a different \u003cb\u003esize\u003c/b\u003e\",,,,3,\"neid_18969\"],[\"You can also look at your current shoes, but if you feel a bit discomfort, it's ok to get a bigger \u003cb\u003esize\u003c/b\u003e .\",,,,3,\"m_en_us1291061.003\"],[\"Scans of patients' brains showed that the injured area almost doubled in \u003cb\u003esize\u003c/b\u003e following treatment.\",,,,3,\"m_en_us1291061.001\"],[\"the business increased in \u003cb\u003esize\u003c/b\u003e\",,,,3,\"neid_18967\"],[\"After 2012, the Olympic stadium would be reduced in \u003cb\u003esize\u003c/b\u003e to become a new home for athletics.\",,,,3,\"m_en_us1291061.001\"],[\"Even within the main cultivated species, yams vary to a remarkable extent in \u003cb\u003esize\u003c/b\u003e , shape, and colour.\",,,,3,\"m_en_us1291061.001\"],[\"The councils vary in size from seven to fifteen members depending on the \u003cb\u003esize\u003c/b\u003e of the settlement they manage.\",,,,3,\"m_en_us1291061.001\"],[\"The \u003cb\u003esize\u003c/b\u003e of the pelvic area varies in cows and those with small pelvic areas are more prone to difficulty.\",,,,3,\"m_en_us1291061.001\"],[\"I had ticked off ten shops already that we had been to and didn't even offer anything in my dress \u003cb\u003esize\u003c/b\u003e .\",,,,3,\"m_en_us1291061.003\"],[\"she was a \u003cb\u003esize\u003c/b\u003e 14\",,,,3,\"neid_18970\"],[\"While presidents often talk of reducing the \u003cb\u003esize\u003c/b\u003e of their White House staff, none actually do.\",,,,3,\"m_en_us1291061.001\"],[\"I looked over, and for the first time, I noticed that my belly had drastically reduced in \u003cb\u003esize\u003c/b\u003e .\",,,,3,\"m_en_us1291061.001\"],[\"The sac is often covered by a thin layer of skin and can range in \u003cb\u003esize\u003c/b\u003e from the dimensions of a grape to those of a grapefruit.\",,,,3,\"m_en_us1291061.001\"],[\"The \u003cb\u003esize\u003c/b\u003e of the house excluding underground parking is about 4,100 square feet.\",,,,3,\"m_en_us1291061.001\"],[\"what \u003cb\u003esize\u003c/b\u003e is it?\",,,,3,\"neid_18967\"],[\"The \u003cb\u003esize\u003c/b\u003e of the house wasn't really big, but what made it look big was the garden beyond.\",,,,3,\"m_en_us1291061.001\"],[\"All apartments will be for sale and will range in \u003cb\u003esize\u003c/b\u003e from 750 to 1500 square feet.\",,,,3,\"m_en_us1291061.001\"],[\"he was amazed at the \u003cb\u003esize\u003c/b\u003e of the hall\",,,,3,\"neid_18968\"],[\"Territories vary greatly in size, the largest one more than five times the \u003cb\u003esize\u003c/b\u003e of the smallest one.\",,,,3,\"m_en_us1291061.001\"],[\"The craftspeople construct everything in small scale and the houses range in \u003cb\u003esize\u003c/b\u003e up to palaces which can cost as much as a full-sized house.\",,,,3,\"m_en_us1291061.001\"],[\"I would have given anything to make the perfect dress in a \u003cb\u003esize\u003c/b\u003e 18 appear for her to stop the tears.\",,,,3,\"m_en_us1291061.003\"],[\"We are retaining the play area although this will be slightly reduced in \u003cb\u003esize\u003c/b\u003e .\",,,,3,\"m_en_us1291061.001\"],[\"I suggest you also \u003cb\u003esize\u003c/b\u003e the walls before doing this, and as your decorator is a traditionalist I am sure he will agree.\",,,,3,\"m_en_us1291062.002\"],[\"The judges were more interested in \u003cb\u003esize\u003c/b\u003e than overall balance, proportion and symmetry.\",,,,3,\"m_en_us1291061.001\"]]],[[\"font size\",\"file size\",\"actual size\",\"shoe size\",\"size up\",\"grain size\",\"full size\",\"paper size\",\"lot size\",\"small size\"]]]";
    logger->Log(L"Start translating sentence '" + sentence + L"'.");

    wstring hash = GetHash(sentence);
    wstring translateURL = L"https://translate.google.com/translate_a/single?client=t&sl=en&tl=ru&hl=ru&dt=at&dt=bd&dt=ex&dt=ld&dt=md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&source=bh&ssel=0&tsel=0&kc=1&tco=2&tk=" 
        + hash 
        + L"&q=" + requestProvider->EscapeText(sentence);

    wstring translatorResponse = requestProvider->GetStringResponse(translateURL);

    TranslateResult result;
    
    try 
    {
        result = ParseJSONResponse(translatorResponse);
        result.Sentence.Input = StringUtilities::CopyWideChar(sentence);
    }
    catch (json::json_exception exception)
    {
        wstring errorMessage = L"Error parsing json response. Exception: " + StringUtilities::GetUtf16String(exception.what()) + L".";
        
        logger->Log(errorMessage);
        result.SetError(errorMessage);
    }

    logger->Log(L"End translating sentence.");

    return result;
}

// Grabbed from google minified js code
wstring Translator::GetHash(wstring sentence)
{
    string utf8Sentence = StringUtilities::GetUtf8String(sentence);
    const char* bytes = utf8Sentence.c_str();

    long long tkk1 = translatePageParser->GetTkk1();
    long long tkk2 = translatePageParser->GetTkk2();

    long long a = tkk1;
    long long pow32 = 4294967295;
    long long pow31 = pow32 / 2;

    for (unsigned i = 0; i < strlen(bytes); ++i) {
        a = a + (unsigned char)bytes[i];

        /* ------ */
        a = a + (a << 10) & pow32;
        a = a ^ (unsigned(a) >> 6);
        /* ------ */
    }

    /* ------ */
    a = a + (a << 3) & pow32;
    a = a ^ (unsigned(a) >> 11);
    a = a + (a << 15) & pow32;
    /* ------ */

    a = a ^ tkk2;

    if (a < 0) {
        a = a & pow31 + pow31 + 1;
    }

    a = a % 1000000;

    return to_wstring(a) + L"." + to_wstring(a ^ tkk1);
}

// Relevant response has the following format
//[
//	[
//		["<translation>", "<original text>"],
//		[null, null, "<translit>"]
//	],
//	[
//		[
//			"<part_of_speech>", 
//			[<irrelevant>],
//			[
//				["<variant_1>", ["<reverse_translation_1>", "<reverse_translation_2>", ..], null, <score>],
//				....
//			], 
//			"<base_form>", 
//			<irrelevant>
//		],		
//	]
//]
TranslateResult Translator::ParseJSONResponse(wstring json)
{
    TranslateResult result;

    if (json.empty()){
        wstring errorMessage = L"Error. Unable to parse JSON. JSON value is empty.";
        
        logger->Log(errorMessage);
        result.SetError(errorMessage);

        return result;
    }

    // Normalize json response
    Translator::ReplaceAll(json, L",,", L",null,");
    Translator::ReplaceAll(json, L"[,", L"[null,");

    json::value root = json::value::parse(json);

    if (root.is_null())
    {
        wstring errorMessage = L"Error. Unable to parse JSON. Json value = '" + json + L"'.";

        logger->Log(errorMessage);
        result.SetError(errorMessage);

        return result;
    }

    json::array sentences = root[0][0].as_array();
    if (sentences.size() > 0)
    {
        result.Sentence.Translation = StringUtilities::CopyWideChar(sentences[0].as_string());
        result.Sentence.Origin = StringUtilities::CopyWideChar(sentences[1].as_string());
    }

    if (!root[1].is_array()) {
        return result;
    }

    json::array dict = root[1].as_array();

    for (size_t i = 0; i < dict.size(); ++i)
    {
        TranslateResultDictionary category;

        category.IsExtendedList = false;
        category.PartOfSpeech = StringUtilities::CopyWideChar(dict[i][0].as_string());
        category.BaseForm = StringUtilities::CopyWideChar(dict[i][3].as_string());

        if (!dict[i][2].is_array()) 
        {
            continue;
        }

        json::array entries = dict[i][2].as_array();
        for (size_t j = 0; j < entries.size(); ++j)
        {
            TranslateResultDictionaryEntry entry;
            entry.Word = StringUtilities::CopyWideChar(entries[j][0].as_string());

            entry.Score = entries[j][3].is_double()
                ? entries[j][3].as_double()
                : 0;

            if (!entries[j][1].is_array())
            {
                continue;
            }

            json::array reverseTranslations = entries[j][1].as_array();
            for (size_t k = 0; k < reverseTranslations.size(); ++k)
            {
                entry.ReverseTranslation.push_back(StringUtilities::CopyWideChar(reverseTranslations[k].as_string()));
            }

            category.Entries.push_back(entry);
        }
        result.TranslateCategories.push_back(category);
    }

    return result;
}

void Translator::ReplaceAll(wstring &str, const wstring &search, const wstring &replace)
{
    for (size_t pos = 0;; pos += replace.length() - 1)
    {
        pos = str.find(search, pos);
        if (pos == string::npos) break;

        str.erase(pos, search.length());
        str.insert(pos, replace);
    }
}

Translator::~Translator()
{
}