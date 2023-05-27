#pragma once
#include <d3d11.h>
// stole from lama's tinyhud

#include "imgui.h"

static float get_resolution_scale_width()
{
	return ImGui::GetIO().DisplaySize.x / 1920.f;
}
static float get_resolution_scale_height()
{
	return ImGui::GetIO().DisplaySize.y / 1080.f;
}

class Texture
{
public:
	static inline ID3D11Device* device_ = nullptr; // Should only be set once by Renderer

	/// <summary>
	/// Load all images.
	/// </summary>
	static void Init();

	struct Image
	{
		ID3D11ShaderResourceView* texture = nullptr;
		int32_t width = 0; // native width
		int32_t height = 0; // native height
	};

	enum class image_type
	{
		hud,
		round,
		key,
		total
	};

	enum class icon_image_type
	{
		potion_health,
		potion_default,
		sword_one_handed,
		axe_one_handed,
		mace,
		dagger,
		sword_two_handed,
		axe_two_handed,
		staff,
		bow,
		crossbow,
		spell_default,
		destruction,
		shout,
		power,
		food,
		shield,
		icon_default,
		destruction_fire,
		destruction_frost,
		destruction_shock,
		restoration,
		poison_default,
		armor_light,
		armor_heavy,
		armor_clothing,
		scroll,
		arrow,
		hand_to_hand,
		katana,
		halberd,
		whip,
		claw,
		rapier,
		quarter_staff,
		pike,
		potion_stamina,
		potion_magicka,
		potion_fire_resist,
		potion_shock_resist,
		potion_frost_resist,
		potion_magic_resist,
		alteration,
		conjuration,
		illusion,
		torch,
		lantern,
		mask,
		total
	};
	static Image& GetIconImage(icon_image_type a_imageType);

private:
	static inline std::map<uint32_t, Image> icon_struct;

	static bool load_texture_from_file(const char* filename,
		ID3D11ShaderResourceView** out_srv,
		int& out_width,
		int& out_height);
	template <typename T>
	static void load_images(std::map<std::string, T>& a_map,
		std::map<uint32_t, Image>& a_struct,
		std::string& file_path)
	{
		for (const auto& entry : std::filesystem::directory_iterator(file_path)) {
			if (a_map.contains(entry.path().filename().string())) {
				if (entry.path().filename().extension() != ".svg") {
					logger::warn("file {}, does not match supported extension '.svg'"sv,
						entry.path().filename().string().c_str());
					continue;
				}
				const auto index = static_cast<int32_t>(a_map[entry.path().filename().string()]);
				if (load_texture_from_file(entry.path().string().c_str(),
						&a_struct[index].texture,
						a_struct[index].width,
						a_struct[index].height)) {
					logger::trace("loading texture {}, type: {}, width: {}, height: {}"sv,
						entry.path().filename().string().c_str(),
						entry.path().filename().extension().string().c_str(),
						a_struct[index].width,
						a_struct[index].height);
				} else {
					logger::error("failed to load texture {}"sv, entry.path().filename().string().c_str());
				}

				//a_struct[index].width = static_cast<int32_t>(a_struct[index].width * get_resolution_scale_width());
				//a_struct[index].height = static_cast<int32_t>(a_struct[index].height * get_resolution_scale_height());
			}
		}
	}

	static inline std::string icon_directory = R"(.\Data\SKSE\Plugins\resources\icons)";
	static inline std::string img_directory = R"(.\Data\SKSE\Plugins\resources\img)";

	

	inline static std::map<std::string, icon_image_type> icon_type_name_map = { { R"(potion_health.svg)",
																					icon_image_type::potion_health },
		{ R"(potion_default.svg)", icon_image_type::potion_default },
		{ R"(sword_one_handed.svg)", icon_image_type::sword_one_handed },
		{ R"(axe_one_handed.svg)", icon_image_type::axe_one_handed },
		{ R"(mace.svg)", icon_image_type::mace },
		{ R"(dagger.svg)", icon_image_type::dagger },
		{ R"(sword_two_handed.svg)", icon_image_type::sword_two_handed },
		{ R"(axe_two_handed.svg)", icon_image_type::axe_two_handed },
		{ R"(staff.svg)", icon_image_type::staff },
		{ R"(bow.svg)", icon_image_type::bow },
		{ R"(crossbow.svg)", icon_image_type::crossbow },
		{ R"(spell_default.svg)", icon_image_type::spell_default },
		{ R"(destruction.svg)", icon_image_type::destruction },
		{ R"(shout.svg)", icon_image_type::shout },
		{ R"(power.svg)", icon_image_type::power },
		{ R"(food.svg)", icon_image_type::food },
		{ R"(shield.svg)", icon_image_type::shield },
		{ R"(icon_default.svg)", icon_image_type::icon_default },
		{ R"(destruction_fire.svg)", icon_image_type::destruction_fire },
		{ R"(destruction_frost.svg)", icon_image_type::destruction_frost },
		{ R"(destruction_shock.svg)", icon_image_type::destruction_shock },
		{ R"(restoration.svg)", icon_image_type::restoration },
		{ R"(poison_default.svg)", icon_image_type::poison_default },
		{ R"(armor_light.svg)", icon_image_type::armor_light },
		{ R"(armor_heavy.svg)", icon_image_type::armor_heavy },
		{ R"(armor_clothing.svg)", icon_image_type::armor_clothing },
		{ R"(scroll.svg)", icon_image_type::scroll },
		{ R"(arrow.svg)", icon_image_type::arrow },
		{ R"(hand_to_hand.svg)", icon_image_type::hand_to_hand },
		{ R"(katana.svg)", icon_image_type::katana },
		{ R"(halberd.svg)", icon_image_type::halberd },
		{ R"(whip.svg)", icon_image_type::whip },
		{ R"(claw.svg)", icon_image_type::claw },
		{ R"(rapier.svg)", icon_image_type::rapier },
		{ R"(quarter_staff.svg)", icon_image_type::quarter_staff },
		{ R"(pike.svg)", icon_image_type::pike },
		{ R"(potion_stamina.svg)", icon_image_type::potion_stamina },
		{ R"(potion_magicka.svg)", icon_image_type::potion_magicka },
		{ R"(potion_fire_resist.svg)", icon_image_type::potion_fire_resist },
		{ R"(potion_shock_resist.svg)", icon_image_type::potion_shock_resist },
		{ R"(potion_frost_resist.svg)", icon_image_type::potion_frost_resist },
		{ R"(potion_magic_resist.svg)", icon_image_type::potion_magic_resist },
		{ R"(alteration.svg)", icon_image_type::alteration },
		{ R"(conjuration.svg)", icon_image_type::conjuration },
		{ R"(illusion.svg)", icon_image_type::illusion },
		{ R"(torch.svg)", icon_image_type::torch },
		{ R"(lantern.svg)", icon_image_type::lantern },
		{ R"(mask.svg)", icon_image_type::mask } };
};
