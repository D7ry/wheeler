#pragma once
class Texts
{
public:
	enum class TextType
	{
		AlchemyDynamicIDConsumptionWarning,
		NoWheelPresent,
		Total
	};

	static void LoadTranslations();
	static const char* GetText(TextType a_textType);

private:
#define MAP_ENTRY(textTypeName) \
	{                              \
		TextType::textTypeName, #textTypeName\
	}

	static inline std::unordered_map<TextType, std::string> _textData = {
		MAP_ENTRY(AlchemyDynamicIDConsumptionWarning),
		MAP_ENTRY(NoWheelPresent)
	};
};
