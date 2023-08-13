#include "Texts.h"

static void loadTextFromIni(CSimpleIniA& a_ini, std::string& r_text)
{
	r_text = std::string(a_ini.GetValue("Texts", r_text.data(), r_text.data()));
}
void Texts::LoadTranslations()
{
	CSimpleIniA ini;
	ini.SetUnicode();
#define TEXTS_PATH "Data\\SKSE\\Plugins\\wheeler\\Texts.ini"
	ini.LoadFile(TEXTS_PATH);
	try {
		for (auto& [textType, text] : _textData) {
			loadTextFromIni(ini, text);
		}
	}
	catch (std::exception e) {
		ERROR("Error loading from Texts.ini: {}", e.what());
	}

}

const char* Texts::GetText(TextType a_textType)
{
	return _textData[a_textType].data();
}
