#include "Drawer.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

void Drawer::draw_text(float a_x, float a_y, float a_offset_x, float a_offset_y, const char* a_text, UINT32 a_alpha, UINT32 a_red, UINT32 a_green, UINT32 a_blue, float a_font_size, bool a_center_text, bool a_deduct_text_x, bool a_deduct_text_y, bool a_add_text_x, bool a_add_text_y)
{
	//it should center the text, it kind of does
	auto text_x = 0.f;
	auto text_y = 0.f;

	if (!a_text || !*a_text || a_alpha == 0) {
		return;
	}

	const ImU32 color = IM_COL32(a_red, a_green, a_blue, a_alpha);

	const ImVec2 text_size = ImGui::CalcTextSize(a_text);
	if (a_center_text) {
		text_x = -text_size.x * 0.5f;
		text_y = -text_size.y * 0.5f;
	}
	if (a_deduct_text_x) {
		text_x = text_x - text_size.x;
	}
	if (a_deduct_text_y) {
		text_y = text_y - text_size.y;
	}
	if (a_add_text_x) {
		text_x = text_x + text_size.x;
	}
	if (a_add_text_y) {
		text_y = text_y + text_size.y;
	}

	const auto position =
		ImVec2(a_x + a_offset_x + text_x, a_y + a_offset_y + text_y);

	auto* font = ImGui::GetDefaultFont(); // TODO: add custom font support
	if (!font) {
		font = ImGui::GetDefaultFont();
	}

	ImGui::GetWindowDrawList()->AddText(font, a_font_size, position, color, a_text, nullptr, 0.0f, nullptr);
}

void Drawer::draw_texture(ID3D11ShaderResourceView* a_texture, ImVec2 a_center, float a_offset_x, float a_offset_y, ImVec2 a_size, float a_angle, ImU32 a_color)
{
	a_center = ImVec2(a_center.x + a_offset_x, a_center.y + a_offset_y);
	const float cos_a = cosf(a_angle);
	const float sin_a = sinf(a_angle);
	const ImVec2 pos[4] = { a_center + ImRotate(ImVec2(-a_size.x * 0.5f, -a_size.y * 0.5f), cos_a, sin_a),
		a_center + ImRotate(ImVec2(+a_size.x * 0.5f, -a_size.y * 0.5f), cos_a, sin_a),
		a_center + ImRotate(ImVec2(+a_size.x * 0.5f, +a_size.y * 0.5f), cos_a, sin_a),
		a_center + ImRotate(ImVec2(-a_size.x * 0.5f, +a_size.y * 0.5f), cos_a, sin_a)

	};
	ImVec2 uvs[4] = { ImVec2(0.0f, 0.0f), ImVec2(1.0f, 0.0f), ImVec2(1.0f, 1.0f), ImVec2(0.0f, 1.0f) };

	ImGui::GetWindowDrawList()
		->AddImageQuad(a_texture, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], a_color);
}
