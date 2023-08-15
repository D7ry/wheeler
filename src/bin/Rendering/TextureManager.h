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
		warhammer_two_handed,
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
		armor_heavy_shield,
		armor_light_shield,
		armor_light_chest,
		armor_heavy_chest,
		armor_light_arm,
		armor_heavy_arm,
		armor_light_foot,
		armor_heavy_foot,
		armor_light_head,
		armor_heavy_head,
		armor_clothing_head,
		armor_clothing_chest,
		armor_clothing_foot,
		armor_clothing_arm,
		armor_necklace,
		armor_circlet,
		armor_ring,
		armor_default,
		scroll,
		arrow,
		hand_to_hand,
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
		armor_rating,
		weapon_damage,
		total
	};
	static Image GetIconImage(icon_image_type a_imageType, RE::TESForm* a_form);
	
private:
	static inline std::map<uint32_t, Image> icon_struct;

	static inline std::map<RE::FormID, Image> icon_struct_formID;
	static inline std::map<std::string, Image> icon_struct_keyword;

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
					//logger::trace("loading texture {}, type: {}, width: {}, height: {}"sv,
					//	entry.path().filename().string().c_str(),
					//	entry.path().filename().extension().string().c_str(),
					//	a_struct[index].width,
					//	a_struct[index].height);
				} else {
					logger::error("failed to load texture {}"sv, entry.path().filename().string().c_str());
				}

				//a_struct[index].width = static_cast<int32_t>(a_struct[index].width * get_resolution_scale_width());
				//a_struct[index].height = static_cast<int32_t>(a_struct[index].height * get_resolution_scale_height());
			}
		}
	}

	static void load_custom_icon_images();

	static inline std::string icon_directory = R"(.\Data\SKSE\Plugins\Wheeler\resources\icons)";
	static inline std::string img_directory = R"(.\Data\SKSE\Plugins\Wheeler\resources\img)";
	static inline std::string icon_custom_directory = R"(.\Data\SKSE\Plugins\Wheeler\resources\icons_custom)";

	

	inline static std::map<std::string, icon_image_type> icon_type_name_map = { { R"(potion_health.svg)",
																					icon_image_type::potion_health },
		{ R"(potion_default.svg)", icon_image_type::potion_default },
		{ R"(sword_one_handed.svg)", icon_image_type::sword_one_handed },
		{ R"(axe_one_handed.svg)", icon_image_type::axe_one_handed },
		{ R"(mace.svg)", icon_image_type::mace },
		{ R"(dagger.svg)", icon_image_type::dagger },
		{ R"(sword_two_handed.svg)", icon_image_type::sword_two_handed },
		{ R"(axe_two_handed.svg)", icon_image_type::axe_two_handed },
		{ R"(warhammer_two_handed.svg)", icon_image_type::warhammer_two_handed },
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
		// armors
		// armor with ratings
		{ R"(armor_light_chest.svg)", icon_image_type::armor_light_chest },
		{ R"(armor_heavy_chest.svg)", icon_image_type::armor_heavy_chest },
		{ R"(armor_light_arm.svg)", icon_image_type::armor_light_arm },
		{ R"(armor_heavy_arm.svg)", icon_image_type::armor_heavy_arm },
		{ R"(armor_light_foot.svg)", icon_image_type::armor_light_foot },
		{ R"(armor_heavy_foot.svg)", icon_image_type::armor_heavy_foot },
		{ R"(armor_light_head.svg)", icon_image_type::armor_light_head },
		{ R"(armor_heavy_head.svg)", icon_image_type::armor_heavy_head },
		// shields
		{ R"(armor_heavy_shield.svg)", icon_image_type::armor_heavy_shield },
		{ R"(armor_light_shield.svg)", icon_image_type::armor_light_shield },

		// clothing
		{ R"(armor_clothing_head.svg)", icon_image_type::armor_clothing_head },
		{ R"(armor_clothing_chest.svg)", icon_image_type::armor_clothing_chest },
		{ R"(armor_clothing_arm.svg)", icon_image_type::armor_clothing_arm },
		{ R"(armor_clothing_foot.svg)", icon_image_type::armor_clothing_foot },

		// jewelry
		{ R"(armor_circlet.svg)", icon_image_type::armor_circlet },
		{ R"(armor_necklace.svg)", icon_image_type::armor_necklace },
		{ R"(armor_ring.svg)", icon_image_type::armor_ring },
		{ R"(armor_default.svg)", icon_image_type::armor_default },
		{ R"(scroll.svg)", icon_image_type::scroll },
		{ R"(arrow.svg)", icon_image_type::arrow },
		{ R"(hand_to_hand.svg)", icon_image_type::hand_to_hand },
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
		{ R"(mask.svg)", icon_image_type::mask },
		
		// Item Stats
		{ R"(armor_rating.svg)", icon_image_type::armor_rating },
		{ R"(weapon_damage.svg)", icon_image_type::weapon_damage }
	
	};
};

