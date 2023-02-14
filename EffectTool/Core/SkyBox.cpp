#include "SkyBox.h"

bool SkyBox::SetTextureArray()
{
	T_STR skyTextures[]
	{
		L"../data/Texture/Sky/front.bmp",
		L"../data/Texture/Sky/back.bmp",
		L"../data/Texture/Sky/right.bmp",
		L"../data/Texture/Sky/left.bmp",
		L"../data/Texture/Sky/top.bmp",
		L"../data/Texture/Sky/bottom.bmp"
	};

	m_pTexture = new Texture();
	for (int index = 0; index < _countof(skyTextures); index++)
	{
		m_pTexture->LoadWIC(skyTextures[index]);
	}

	return true;
}

bool SkyBox::Render(ID3D11DeviceContext* pContext)
{
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->PSSetShader(m_pMainPS, NULL, 0);
	pContext->IASetInputLayout(m_pVertexLayout);
	
	UINT stride = sizeof(PNCT_VERTEX);
	UINT offset = 0;

	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);

	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	pContext->PSSetShaderResources(0, m_pTexture->m_pTextureSRV.size(), &m_pTexture->m_pTextureSRV.at(0));

	pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}

SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}
