#pragma once

#include <d3d11.h>

#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <dxgi.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"


namespace Drawer
{
	void draw_text(float a_x,
		float a_y,
		float a_offset_x,
		float a_offset_y,
		float a_offset_extra_x,
		float a_offset_extra_y,
		const char* a_text,
		UINT32 a_alpha,
		UINT32 a_red,
		UINT32 a_green,
		UINT32 a_blue,
		float a_font_size = 20.f,
		bool a_center_text = true,
		bool a_deduct_text_x = false,
		bool a_deduct_text_y = false,
		bool a_add_text_x = false,
		bool a_add_text_y = false);
	void draw_element(ID3D11ShaderResourceView* a_texture,
		ImVec2 a_center,
		ImVec2 a_size,
		float a_angle,
		ImU32 a_color = IM_COL32_WHITE);
};
