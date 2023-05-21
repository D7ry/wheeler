#pragma once

#include <d3d11.h>

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <dxgi.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
// stole from Lama's tiny hud

namespace Drawer
{
	void draw_text(float a_x,
		float a_y,
		float a_offset_x,
		float a_offset_y,
		const char* a_text,
		UINT32 a_alpha,
		UINT32 a_red,
		UINT32 a_green,
		UINT32 a_blue,
		float a_font_size = 20.f,
		bool add_shadow = true,
		bool a_center_text = true,
		bool a_deduct_text_x = false,
		bool a_deduct_text_y = false,
		bool a_add_text_x = false,
		bool a_add_text_y = false);
	void draw_texture(ID3D11ShaderResourceView* a_texture,
		ImVec2 a_center,
		float a_offset_x, 
		float a_offset_y,
		ImVec2 a_size,
		float a_angle,
		ImU32 a_color = IM_COL32_WHITE);
	void draw_arc(
		ImVec2 center,
		float radius_min, float radius_max,
		float inner_ang_min, float inner_ang_max,
		float outer_ang_min, float outer_ang_max,
		ImU32 color, uint32_t segments);
	void draw_arc_gradient(ImVec2 center, float radius_min, float radius_max, float inner_ang_min, float inner_ang_max, float outer_ang_min, float outer_ang_max, ImU32 color_start, ImU32 color_end, uint32_t segments);

	void draw_circle_filled(const ImVec2& center, float radius, ImU32 color, int segments = 0);

	void draw_triangle_filled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col);

	void set_alpha_mult(float a_in);
};
