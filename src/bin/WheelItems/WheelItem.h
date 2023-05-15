#pragma once
class ImVec2;
class WheelItem
{
public:
	virtual void Draw(ImVec2 a_center, bool a_hovered);
	virtual void Activate(uint32_t a_keyID);
	static ID3D11ShaderResourceView* GetTexture();

private:
};
