#include "Texture.h"
#define NANOSVG_IMPLEMENTATION
#define NANOSVG_ALL_COLOR_KEYWORDS
#include "include/lib/nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "include/lib/nanosvgrast.h"


void Texture::Init()
{
	//load_images(image_type_name_map, image_struct, img_directory);
	load_images(icon_type_name_map, icon_struct, icon_directory);
	//load_images(key_icon_name_map, key_struct, key_directory);
	//load_images(default_key_icon_name_map, default_key_struct, key_directory);
	//load_images(gamepad_ps_icon_name_map, ps_key_struct, key_directory);
	//load_images(gamepad_xbox_icon_name_map, xbox_key_struct, key_directory);
}
Texture::Image& Texture::GetIconImage(icon_image_type a_imageType)
{
	return icon_struct[static_cast<int32_t>(a_imageType)];
}
bool Texture::load_texture_from_file(const char* filename, ID3D11ShaderResourceView** out_srv, int& out_width, int& out_height)
{
	ASSERT(device_ != nullptr);
	auto* render_manager = RE::BSRenderManager::GetSingleton();
	if (!render_manager) {
		logger::error("Cannot find render manager. Initialization failed."sv);
		return false;
	}

	auto [forwarder, context, unk58, unk60, unk68, swapChain, unk78, unk80, renderView, resourceView] =
		render_manager->GetRuntimeData();

	// Load from disk into a raw RGBA buffer
	auto* svg = nsvgParseFromFile(filename, "px", 96.0f);
	auto* rast = nsvgCreateRasterizer();

	auto image_width = static_cast<int>(svg->width);
	auto image_height = static_cast<int>(svg->height);

	auto image_data = (unsigned char*)malloc(image_width * image_height * 4);
	nsvgRasterize(rast, svg, 0, 0, 1, image_data, image_width, image_height, image_width * 4);
	nsvgDelete(svg);
	nsvgDeleteRasterizer(rast);

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture2D* p_texture = nullptr;
	D3D11_SUBRESOURCE_DATA sub_resource;
	sub_resource.pSysMem = image_data;
	sub_resource.SysMemPitch = desc.Width * 4;
	sub_resource.SysMemSlicePitch = 0;
	device_->CreateTexture2D(&desc, &sub_resource, &p_texture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof srv_desc);
	srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = desc.MipLevels;
	srv_desc.Texture2D.MostDetailedMip = 0;
	forwarder->CreateShaderResourceView(p_texture, &srv_desc, out_srv);
	p_texture->Release();

	free(image_data);

	out_width = image_width;
	out_height = image_height;

	return true;
}
