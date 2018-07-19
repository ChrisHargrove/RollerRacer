#pragma once

class ShadowMapBuffer
{
public:
	ShadowMapBuffer();
	~ShadowMapBuffer();

	void Create(int width, int height);
	void Bind();
	void Clear();
	void Unbind();

	unsigned int GetID() { return _ID; }
	unsigned int GetDepthMapID() { return _DepthMapID; }

private:
	unsigned int _ID;
	unsigned int _DepthMapID;

	int _Width, _Height;
};

