#pragma once

#include <d3d11.h>

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <dxgi.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
// stole from Lama's tiny hud, plus some stuff I added

namespace Drawer
{
	void draw_text(float a_x,
		float a_y,
		const char* a_text,
		ImU32 a_color,
		float a_font_size,
		DrawArgs a_drawArgs);
	
	void draw_text_block(float a_x,
		float a_y,
		std::string& a_text,
		ImU32 a_color,
		float a_font_size,
		float a_line_spacing,
		float a_line_length,
		DrawArgs a_drawArgs);
	
	void draw_texture(ID3D11ShaderResourceView* a_texture,
		ImVec2 a_center,
		float a_offset_x, 
		float a_offset_y,
		ImVec2 a_size,
		ImU32 a_color,
		DrawArgs a_drawArgs);
	void draw_arc(
		ImVec2 center,
		float radius_min, float radius_max,
		float inner_ang_min, float inner_ang_max,
		float outer_ang_min, float outer_ang_max,
		ImU32 color, uint32_t segments, DrawArgs a_drawArgs);
	void draw_arc_gradient(ImVec2 center, float radius_min, float radius_max, 
		float inner_ang_min, float inner_ang_max, 
		float outer_ang_min, float outer_ang_max, 
		ImU32 color_start, ImU32 color_end, uint32_t segments, DrawArgs a_drawArgs);

	void draw_circle_filled(const ImVec2& center, float radius, ImU32 color, int segments, DrawArgs a_drawArgs);

	void draw_triangle_filled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, DrawArgs a_drawArgs);
};
