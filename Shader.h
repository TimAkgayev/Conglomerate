#pragma once
#include <d3d11.h>
#include <vector>
#include <string>

class IKongShader
{
public:
	virtual void UpdateAndSetShader(ID3D11DeviceContext*) const = 0;

protected:

	void compileAndCreateShader(ID3D11Device* device, std::wstring filename, std::string entrypoint, ID3D11VertexShader** outShader, ID3DBlob** outBlob);
	void compileAndCreateShader(ID3D11Device* device, std::wstring filename, std::string entrypoint, ID3D11PixelShader** outShader, ID3DBlob** outBlob);
	void compileAndCreateShader(ID3D11Device* device, std::wstring filename, std::string entrypoint, ID3D11GeometryShader** outShader, ID3DBlob** outBlob);
};



