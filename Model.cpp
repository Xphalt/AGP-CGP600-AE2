#include "Model.h"

Model::~Model()
{
	if (m_pObject != nullptr) { delete m_pObject; m_pObject = nullptr; }
}

HRESULT Model::LoadObjModel(char* _filename, ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11ShaderResourceView* _texture)
{
	m_pDevice = _device;
	m_pDeviceContext = _deviceContext;
	m_pTexture = _texture;

	m_pObject = new ObjFileModel(_filename, m_pDevice, m_pDeviceContext);

	if (m_pObject->filename == "FILE NOT LOADED") { return S_FALSE; }

	Vertex vertex[] =
	{
		Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f),
		Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 0.0f),
		Vertex( 0.5f,  0.5f, -0.5f, 1.0f, 0.0f),
		Vertex( 0.5f, -0.5f, -0.5f, 1.0f, 1.0f),
		Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 1.0f),
		Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 0.0f),
		Vertex( 0.5f,  0.5f,  0.5f, 1.0f, 0.0f), 
		Vertex( 0.5f, -0.5f,  0.5f, 1.0f, 1.0f), 
	};

	HRESULT hr;
	
	hr = m_CB_VS_vertexShader.Initialize(m_pDevice, m_pDeviceContext);
	if (FAILED(hr)) { return hr; }

	return S_OK;
}

void Model::Draw(XMMATRIX* _view, XMMATRIX* _projection)
{
	XMMATRIX world;
	world = XMMatrixIdentity();
	world *= XMMatrixScaling(1, 1, 1);
	world *= XMMatrixRotationX(XMConvertToRadians(m_xAngle));
	world *= XMMatrixRotationY(XMConvertToRadians(m_yAngle));
	world *= XMMatrixRotationZ(XMConvertToRadians(m_zAngle));
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	m_pDeviceContext->VSSetShader(Renderer::GetInstance().GetShaders()->GetVertexShader(), NULL, 0);
	m_pDeviceContext->PSSetShader(Renderer::GetInstance().GetShaders()->GetPixelShader(), NULL, 0);

	m_CB_VS_vertexShader.data.WorldViewProjection = world * *(_view) * *(_projection);

	m_pDeviceContext->PSSetSamplers(0, 1, Renderer::GetInstance().GetSamplerState());
	m_pDeviceContext->IASetInputLayout(Renderer::GetInstance().GetShaders()->GetInputLayout());
	Renderer::GetInstance().GetDeviceContext()->VSSetConstantBuffers(0, 1, m_CB_VS_vertexShader.GetBufferAddress());
	m_pDeviceContext->UpdateSubresource(m_CB_VS_vertexShader.GetBuffer(), 0, 0, &m_CB_VS_vertexShader.data, 0, 0);
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	m_pObject->Draw();
}

void Model::AddTexture(ID3D11ShaderResourceView* _texture)
{
	m_pTexture = _texture;
}

//const XMVECTOR& Model::GetPositionVector() const
//{
//	return m_positionVector;
//}
//
//const XMFLOAT3& Model::GetPositionFloat3() const
//{
//	return m_position;
//}
//
//const XMVECTOR& Model::GetRotationVector() const
//{
//	return m_rotationVector;
//}
//
//const XMFLOAT3& Model::GetRotationFloat3() const
//{
//	return m_rotation;
//}
//
//void Model::SetPosition(const XMVECTOR& _pos)
//{
//	XMStoreFloat3(&m_position, _pos);
//	m_positionVector = _pos;
//	UpdateWorldMatrix();
//}
//
//void Model::SetPosition(const XMFLOAT3& _pos)
//{
//	m_position = _pos;
//	m_positionVector = XMLoadFloat3(&m_position);
//	UpdateWorldMatrix();
//}
//
//void Model::SetPosition(float _xPos, float _yPos, float _zPos)
//{
//	m_position = XMFLOAT3(_xPos, _yPos, _zPos);
//	m_positionVector = XMLoadFloat3(&m_position);
//	UpdateWorldMatrix();
//}
//
//void Model::AdjustPosition(const XMVECTOR& _position)
//{
//	m_positionVector += _position;
//	XMStoreFloat3(&m_position, m_positionVector);
//	UpdateWorldMatrix();
//}
//
//void Model::AdjustPosition(const XMFLOAT3& _position)
//{
//	m_position.x += _position.y;
//	m_position.y += _position.y;
//	m_position.z += _position.z;
//	m_positionVector = XMLoadFloat3(&m_position);
//	UpdateWorldMatrix();
//}
//
//void Model::AdjustPosition(float _xPos, float _yPos, float _zPos)
//{
//	m_position.x += _xPos;
//	m_position.y += _yPos;
//	m_position.z += _zPos;
//	m_positionVector = XMLoadFloat3(&m_position);
//	UpdateWorldMatrix();
//}
//
//void Model::SetRotation(const XMVECTOR& _rotation)
//{
//	m_rotationVector = _rotation;
//	XMStoreFloat3(&m_rotation, _rotation);
//	UpdateWorldMatrix();
//}
//
//void Model::SetRotation(const XMFLOAT3& _rotation)
//{
//	m_rotation = _rotation;
//	m_rotationVector = XMLoadFloat3(&_rotation);
//	UpdateWorldMatrix();
//}
//
//void Model::SetRotation(float x, float y, float z)
//{
//	m_rotation = XMFLOAT3(x, y, z);
//	m_rotationVector = XMLoadFloat3(&m_rotation);
//	UpdateWorldMatrix();
//}
//
//void Model::AdjustRotation(const XMVECTOR& _rotation)
//{
//	m_rotationVector += _rotation;
//	XMStoreFloat3(&m_rotation, m_rotationVector);
//	UpdateWorldMatrix();
//}
//
//void Model::AdjustRotation(const XMFLOAT3& _rotation)
//{
//	m_rotation.x += _rotation.x;
//	m_rotation.y += _rotation.y;
//	m_rotation.z += _rotation.z;
//	m_rotation = XMLoadFloat3(&rot);
//	UpdateWorldMatrix();
//}
//
//void Model::AdjustRotation(float x, float y, float z)
//{
//	m_rotation.x += x;
//	m_rotation.y += y;
//	m_rotation.z += z;
//	m_rotation = XMLoadFloat3(&rot);
//	UpdateWorldMatrix();
//}
//
//void Model::SetLookAtPos(XMFLOAT3 lookAtPos)
//{
//	//Verify that look at m_posis not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
//	if (lookAtPos.x == pos.x && lookAtPos.y == pos.y && lookAtPos.z == pos.z)
//		return;
//
//	lookAtPos.x = pos.x - lookAtPos.x;
//	lookAtPos.y = pos.y - lookAtPos.y;
//	lookAtPos.z = pos.z - lookAtPos.z;
//
//	float pitch = 0.0f;
//	if (lookAtPos.y != 0.0f)
//	{
//		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
//		pitch = atan(lookAtPos.y / distance);
//	}
//
//	float yaw = 0.0f;
//	if (lookAtPos.x != 0.0f)
//	{
//		yaw = atan(lookAtPos.x / lookAtPos.z);
//	}
//	if (lookAtPos.z > 0)
//		yaw += XM_PI;
//
//	SetRotation(pitch, yaw, 0.0f);
//}
//
//const XMVECTOR& Model::GetForwardVector()
//{
//	return vec_forward;
//}
//
//const XMVECTOR& Model::GetRightVector()
//{
//	return vec_right;
//}
//
//const XMVECTOR& Model::GetBackwardVector()
//{
//	return vec_backward;
//}
//
//const XMVECTOR& Model::GetLeftVector()
//{
//	return vec_left;
//}
//
//void Model::UpdateWorldMatrix()
//{
//}
