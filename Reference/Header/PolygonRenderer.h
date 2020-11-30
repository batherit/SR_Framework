#ifndef PolygonRenderer_h__
#define PolygonRenderer_h__

#include "Engine_Define.h"
#include "BaseRenderer.h"

BEGIN(Engine)
class CVIBuffer;
class CTexture;
class ENGINE_DLL CPolygonRenderer : public CBaseRenderer
{
private:
	explicit CPolygonRenderer(void);
	explicit CPolygonRenderer(const CPolygonRenderer& rhs);
	virtual ~CPolygonRenderer(void);

public:
	// CComponent��(��) ���� ��ӵ�
	virtual HRESULT Ready(void) override;
	virtual int Update(const _float & _fDeltaTime) override;
	virtual void Render() override;

	static CPolygonRenderer* Create();
	virtual CComponent*	Clone();

	static const _tchar* GetComponentTag() { return L"PolygonRenderer"; }
	static const Engine::COMPONENTID GetComponentID() { return Engine::ID_STATIC; }

	void SetRenderInfo(RENDERID _eRenderID, CVIBuffer* _pBuffer, CTexture* _pTexture = nullptr);
	void SetTextureIndex(const _uint& _uiTextureIndex) { m_uiTextureIndex = _uiTextureIndex; }

private:
	virtual void Free(void);

protected:
	CVIBuffer* m_pBuffer = nullptr;
	CTexture* m_pTexture = nullptr;
	_uint m_uiTextureIndex = 0;
	D3DMATERIAL9 m_stMtrl;
};

END
#endif // Renderer_h__