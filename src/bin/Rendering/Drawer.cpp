#include "Drawer.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "bin/Utilities/Utils.h"
void Drawer::draw_text(float a_x,
		float a_y,
		const char* a_text,
		ImU32 a_color,
		float a_font_size,
		bool add_shadow, float alphaMult, float rotation, float scale)
{
	auto* font = ImGui::GetDefaultFont();  // TODO: add custom font support

	//it should center the text, it kind of does
	auto text_x = 0.f;
	auto text_y = 0.f;

	if (!a_text || !*a_text) {
		return;
	}

	Utils::Color::MultAlpha(a_color, alphaMult);

	float currFontSize = ImGui::GetFontSize();

	ImVec2 text_size = ImGui::CalcTextSize(a_text);
	text_size.x *= a_font_size / currFontSize;
	text_size.y *= a_font_size / currFontSize;
	
	text_x = -text_size.x * 0.5f;
	text_y = -text_size.y * 0.5f;
	

	const auto position =
		ImVec2(a_x + text_x, a_y + text_y);

	auto drawList = ImGui::GetWindowDrawList();
	if (add_shadow) {
		ImVec2 shadowPos1(position);
		ImVec2 shadowPos2(position);
		ImVec2 shadowPos3(position);
		ImVec2 shadowPos4(position);
		auto offset = a_font_size * 0.08f;
		shadowPos1.x -= offset;
		shadowPos1.y -= offset;
		//drawList->AddText(font, a_font_size, shadowPos1, 0xFF000000, a_text, nullptr, 0.0f, nullptr);
		shadowPos2.x -= offset;
		shadowPos2.y += offset;
		//drawList->AddText(font, a_font_size, shadowPos2, 0xFF000000, a_text, nullptr, 0.0f, nullptr);
		shadowPos3.x += offset;
		shadowPos3.y -= offset;
		//drawList->AddText(font, a_font_size, shadowPos3, 0xFF000000, a_text, nullptr, 0.0f, nullptr);
		shadowPos4.x += offset;
		shadowPos4.y += offset;
		drawList->AddText(font, a_font_size, shadowPos4, 0xFF000000, a_text, nullptr, 0.0f, nullptr);
	}
	
	drawList->AddText(font, a_font_size, position, a_color, a_text, nullptr, 0.0f, nullptr);
}


void Drawer::draw_texture(ID3D11ShaderResourceView* a_texture, ImVec2 a_center, float a_offset_x, float a_offset_y, ImVec2 a_size, float a_angle, float alphaMult, ImU32 a_color)
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

	Utils::Color::MultAlpha(a_color, alphaMult);
	ImGui::GetWindowDrawList()
		->AddImageQuad(a_texture, pos[0], pos[1], pos[2], pos[3], uvs[0], uvs[1], uvs[2], uvs[3], a_color);
}

void Drawer::draw_arc(ImVec2 center, float radius_min, float radius_max, float inner_ang_min, float inner_ang_max, float outer_ang_min, float outer_ang_max, ImU32 a_color, uint32_t segments, float alphaMult)
{
	auto drawList = ImGui::GetWindowDrawList();

	const float fAngleStepInner = (inner_ang_max - inner_ang_min) / segments;
	const float fAngleStepOuter = (outer_ang_max - outer_ang_min) / segments;

	const ImVec2& vTexUvWhitePixel = ImGui::GetDrawListSharedData()->TexUvWhitePixel;
	// draw an arc for the current item
	ImColor color = a_color;
	Utils::Color::MultAlpha(a_color, alphaMult);

	drawList->PrimReserve(segments * 6, (segments + 1) * 2);
	for (int iSeg = 0; iSeg <= segments; ++iSeg) {
		float fCosInner = cosf(inner_ang_min + fAngleStepInner * iSeg);
		float fSinInner = sinf(inner_ang_min + fAngleStepInner * iSeg);
		float fCosOuter = cosf(outer_ang_min + fAngleStepOuter * iSeg);
		float fSinOuter = sinf(outer_ang_min + fAngleStepOuter * iSeg);

		if (iSeg < segments) {
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 0);
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 2);
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 1);
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 3);
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 2);
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 1);
		}

		drawList->PrimWriteVtx(ImVec2(center.x + fCosInner * radius_min, center.y + fSinInner * radius_min), vTexUvWhitePixel, color);
		drawList->PrimWriteVtx(ImVec2(center.x + fCosOuter * radius_max, center.y + fSinOuter * radius_max), vTexUvWhitePixel, color);
	}
}

static inline uint32_t bigLerp(uint32_t a, uint32_t b, double t)
{
	return (a + (b - a) * t);
}


void Drawer::draw_arc_gradient(ImVec2 center, float radius_min, float radius_max, float inner_ang_min, float inner_ang_max, float outer_ang_min, float outer_ang_max, ImU32 a_color_start, ImU32 a_color_end, uint32_t segments, float alphaMult)
{
	const float fAngleStepInner = (inner_ang_max - inner_ang_min) / segments;
	const float fAngleStepOuter = (outer_ang_max - outer_ang_min) / segments;

	const ImVec2& vTexUvWhitePixel = ImGui::GetDrawListSharedData()->TexUvWhitePixel;
	// draw an arc for the current item
	auto drawList = ImGui::GetWindowDrawList();
	drawList->PrimReserve(segments * 6, (segments + 1) * 2);
	for (int iSeg = 0; iSeg <= segments; ++iSeg) {
		float fCosInner = cosf(inner_ang_min + fAngleStepInner * iSeg);
		float fSinInner = sinf(inner_ang_min + fAngleStepInner * iSeg);
		float fCosOuter = cosf(outer_ang_min + fAngleStepOuter * iSeg);
		float fSinOuter = sinf(outer_ang_min + fAngleStepOuter * iSeg);

		if (iSeg < segments) {
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 0);
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 2);
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 1);
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 3);
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 2);
			drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 1);
		}
		
		ImU32 color_start = a_color_start, color_end = a_color_end;
		Utils::Color::MultAlpha(color_start, alphaMult);
		Utils::Color::MultAlpha(color_end, alphaMult);

		// Interpolate between the start and end colors for inner vertex
		ImU32 colorInner = bigLerp(color_start, color_end, 0);
		drawList->PrimWriteVtx(ImVec2(center.x + fCosInner * radius_min, center.y + fSinInner * radius_min), vTexUvWhitePixel, colorInner);

		// Interpolate between the start and end colors for outer vertex
		ImU32 colorOuter = bigLerp(color_start, color_end, 1);
		drawList->PrimWriteVtx(ImVec2(center.x + fCosOuter * radius_max, center.y + fSinOuter * radius_max), vTexUvWhitePixel, colorOuter);
	}
}

void Drawer::draw_circle_filled(const ImVec2& center, float radius, ImU32 color, int segments, float alphaMult)
{
	Utils::Color::MultAlpha(color, alphaMult);
	ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, color, segments);
}

void Drawer::draw_triangle_filled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float alphaMult)
{
	Utils::Color::MultAlpha(col, alphaMult);
	ImGui::GetWindowDrawList()->AddTriangleFilled(p1, p2, p3, col);
}

