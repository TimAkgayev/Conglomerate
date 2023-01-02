#include "EffectLibrary.h"
#include "ShaderLibrary.h"

namespace KongEffectLibrary
{

	BasicEffect::BasicEffect(ID3D11Device* device, const Kong::Camera* camera)
	{
		BasicEffect_vs* vs = new BasicEffect_vs(device, camera);
		BasicEffect_ps* ps = new BasicEffect_ps(device);
		addShader(vs);
		addShader(ps);

	}

}