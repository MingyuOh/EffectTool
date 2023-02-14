//#include "Octree.h"
//
////--------------------------------------------------------------------------------------
//// Constructor & Destructor
////--------------------------------------------------------------------------------------
//Octree::Octree()
//{
//	m_pRootNode = nullptr;
//	m_iMaxDepth = OCTREE_MIN_DEPTH;
//	m_iNodeCount = 0;
//}
//
//Octree::~Octree()
//{
//
//}
//
////--------------------------------------------------------------------------------------
//// Init
////--------------------------------------------------------------------------------------
//void Octree::Init(int iMaxDepth)
//{
//	if (iMaxDepth <= OCTREE_MIN_DEPTH)
//	{
//		m_iMaxDepth = iMaxDepth;
//	}
//}
//
////--------------------------------------------------------------------------------------
//// Create index buffer
////--------------------------------------------------------------------------------------
//bool Octree::CreateIndexBuffer(Node* pNode)
//{
//	HRESULT hr = S_OK;
//
//	D3D11_BUFFER_DESC ibDesc;
//	ibDesc.ByteWidth = sizeof(DWORD) * pNode->GetIndexBuffer().size();
//	ibDesc.Usage = D3D11_USAGE_DEFAULT;
//	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	ibDesc.CPUAccessFlags = 0;
//	ibDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA ibInitData;
//	ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
//	ibInitData.pSysMem = &pNode->GetIndexBuffer().at(0);
//
//	//m_pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, pNode->m_pIndexBuffer.GetAddressOf());
//	return hr;
//}
//
////--------------------------------------------------------------------------------------
//// Build
////--------------------------------------------------------------------------------------
//bool Octree::Create(HeightMap* pHeightMap)
//{
//	m_pHeightMap = pHeightMap;
//
//	TVector3 vMin(-(m_pHeightMap->m_iNumRows / 2.0f), 0, -(pHeightMap->m_iNumCols / 2.0f));
//	TVector3 vMax(m_pHeightMap->m_iNumRows / 2.0f, m_pHeightMap->m_fMaxHeight, pHeightMap->m_iNumCols / 2.0f);
//
//	m_pRootNode = CreateNode(nullptr, vMin, vMax);
//	m_pRootNode->SetIndex(m_iNodeCount);
//	m_iNodeCount++;
//
//	if (CreateOctree(m_pRootNode) == true)
//	{
//		return true;
//	}
//	return false;
//}
//
////--------------------------------------------------------------------------------------
//// Create node
////--------------------------------------------------------------------------------------
//Node* Octree::CreateNode(Node* pParent, TVector3 vMin, TVector3 vMax)
//{
//	Node* pNode = new Node();
//	pNode->GetChildList().reserve(OCTREE);
//	pNode->GetCornerList().reserve(OCTREE_MIN_DEPTH);
//
//	pNode->SetMaxPosition(vMax);
//	pNode->SetMinPosition(vMin);
//
//	TVector3 vCenter;
//	vCenter.x = (vMax.x - ((vMax.x - vMin.x) / 2));
//	vCenter.y = (vMax.y - ((vMax.y - vMin.y) / 2));
//	vCenter.z = (vMax.z - ((vMax.z - vMin.z) / 2));
//	pNode->SetCenterPosition(vCenter);
//
//	// Up rect
//	pNode->SetCorner(TVector3(vMin.x, vMax.y, vMax.z));
//	pNode->SetCorner(TVector3(vMax.x, vMax.y, vMax.z));
//	pNode->SetCorner(TVector3(vMax.x, vMax.y, vMin.z));
//	pNode->SetCorner(TVector3(vMin.x, vMax.y, vMin.z));
//
//	// Middle rect
//	pNode->SetCorner(TVector3(vMin.x, vCenter.y, vMax.z));
//	pNode->SetCorner(TVector3(vMax.x, vCenter.y, vMax.z));
//	pNode->SetCorner(TVector3(vMax.x, vCenter.y, vMin.z));
//	pNode->SetCorner(TVector3(vMin.x, vCenter.y, vMin.z));
//
//	// Down rect
//	pNode->SetCorner(TVector3(vMin.x, vMin.y, vMax.z));
//	pNode->SetCorner(TVector3(vMax.x, vMin.y, vMax.z));
//	pNode->SetCorner(TVector3(vMax.x, vMin.y, vMin.z));
//	pNode->SetCorner(TVector3(vMin.x, vMin.y, vMin.z));
//
//	// Vertical rect
//	pNode->SetCorner(TVector3(vCenter.x, vMax.y, vMin.z));
//	pNode->SetCorner(TVector3(vCenter.x, vMax.y, vMax.z));
//	pNode->SetCorner(TVector3(vCenter.x, vMin.y, vMax.z));
//	pNode->SetCorner(TVector3(vCenter.x, vMin.y, vMin.z));
//
//	// Horizental rect
//	pNode->SetCorner(TVector3(vMin.x, vMax.y, vCenter.z));
//	pNode->SetCorner(TVector3(vMax.x, vMax.y, vCenter.z));
//	pNode->SetCorner(TVector3(vMax.x, vMin.y, vCenter.z));
//	pNode->SetCorner(TVector3(vMin.x, vMin.y, vCenter.z));
//
//
//	if (pParent != nullptr)
//	{
//		if (pNode->GetDepth() < m_iMaxDepth)
//		{
//			pNode->SetDepth(pParent->GetDepth() + 1);
//		}
//	}
//
//	return pNode;
//}
//
//
////--------------------------------------------------------------------------------------
//// Create octree
////--------------------------------------------------------------------------------------
//bool Octree::CreateOctree(Node* pNode)
//{
//	if (DivideTree(pNode) == true)
//	{
//		for (int index = 0; index < pNode->GetChildList().size(); index++)
//		{
//			CreateOctree(pNode->GetChild(index));
//		}
//		return true;
//	}
//	return false;
//}
//
////--------------------------------------------------------------------------------------
//// Divide space
////--------------------------------------------------------------------------------------
//bool Octree::DivideTree(Node* pNode)
//{
//	if (pNode == nullptr || pNode->GetDepth() >= m_iMaxDepth)
//	{
//		pNode->SetIsLeaf(true);
//		m_pLeafNodeList.push_back(pNode);
//		return false;
//	}
//
//
//	// ========================================================
//	// ================== Above the center ====================
//	// ======================== Cube ==========================
//	// ========================================================
//	// == Left: Min X ~ Center X, Right: Cnter X ~ Max X     ==
//	// == Top: Center Y ~ Max Y, Bottom: Min Y ~ Center Y    ==
//	// == Inner: Center Z ~ Max Z, Outer: Min Z ~ Center Z   ==
//	// ========================================================
//	// ======================= Start ==========================
//	// ========================================================
//
//	// Left, top, and inner cubes of the octree
//	pNode->SetChild(CreateNode(pNode,
//		TVector3(pNode->GetMinPosition().x,
//			pNode->GetCenterPosition().y,
//			pNode->GetCenterPosition().z),
//		TVector3(pNode->GetCenterPosition().x,
//			pNode->GetMaxPosition().y,
//			pNode->GetMaxPosition().z)
//	));
//	pNode->GetChild(0)->SetIndex(m_iNodeCount++);
//
//	// Right, top, and inner cubes of the octree
//	pNode->SetChild(CreateNode(pNode,
//		pNode->GetCenterPosition(),
//		pNode->GetMaxPosition()
//	));
//	pNode->GetChild(1)->SetIndex(m_iNodeCount++);
//
//	// Left, top, and outer cubes of the octree
//	pNode->SetChild(CreateNode(pNode,
//		TVector3(pNode->GetMinPosition().x,
//			pNode->GetCenterPosition().y,
//			pNode->GetMinPosition().z),
//		TVector3(pNode->GetCenterPosition().x,
//			pNode->GetMaxPosition().y,
//			pNode->GetCenterPosition().z)
//	));
//	pNode->GetChild(2)->SetIndex(m_iNodeCount++);
//
//	// Right, top, and outer cubes of the octree
//	pNode->SetChild(CreateNode(pNode,
//		TVector3(pNode->GetCenterPosition().x,
//			pNode->GetCenterPosition().y,
//			pNode->GetMinPosition().z),
//		TVector3(pNode->GetMaxPosition().x,
//			pNode->GetMaxPosition().y,
//			pNode->GetCenterPosition().z)
//	));
//	pNode->GetChild(3)->SetIndex(m_iNodeCount++);
//
//	// ========================================================
//	// ======================== End ===========================
//	// ========================================================
//
//
//	// ========================================================
//	// ================== Below the center ====================
//	// ======================== Cube ==========================
//	// ========================================================
//	// ====================== Start ===========================
//	// ========================================================
//
//	// Left, bottom, and inner cubes of the octree
//	pNode->SetChild(CreateNode(pNode,
//		TVector3(pNode->GetMinPosition().x,
//			pNode->GetMinPosition().y,
//			pNode->GetCenterPosition().z),
//		TVector3(pNode->GetCenterPosition().x,
//			pNode->GetCenterPosition().y,
//			pNode->GetMaxPosition().z)
//	));
//	pNode->GetChild(4)->SetIndex(m_iNodeCount++);
//
//	// Right, bottom, and inner cubes of the octree
//	pNode->SetChild(CreateNode(pNode,
//		TVector3(pNode->GetCenterPosition().x,
//			pNode->GetMinPosition().y,
//			pNode->GetCenterPosition().z),
//		TVector3(pNode->GetMaxPosition().x,
//			pNode->GetCenterPosition().y,
//			pNode->GetMaxPosition().z)
//	));
//	pNode->GetChild(5)->SetIndex(m_iNodeCount++);
//
//	// Left, bottom, and outer cubes of the octree
//	pNode->SetChild(CreateNode(pNode,
//		pNode->GetMinPosition(),
//		pNode->GetCenterPosition()
//	));
//	pNode->GetChild(6)->SetIndex(m_iNodeCount++);
//
//	// Right, bottom, and outer cubes of the octree
//	pNode->SetChild(CreateNode(pNode,
//		TVector3(pNode->GetCenterPosition().x,
//			pNode->GetMinPosition().y,
//			pNode->GetMinPosition().z),
//		TVector3(pNode->GetMaxPosition().x,
//			pNode->GetCenterPosition().y,
//			pNode->GetCenterPosition().z)
//	));
//	pNode->GetChild(7)->SetIndex(m_iNodeCount++);
//
//	// ========================================================
//	// ======================== End ===========================
//	// ========================================================
//	return true;
//}
//
////--------------------------------------------------------------------------------------
//// Delete an object from the list of objects in a node
////--------------------------------------------------------------------------------------
//bool Octree::DeleteObject(Node* pNode, Object* object)
//{
//	if (pNode->isExistObjectInNode(object) == true)
//	{
//		pNode->DeleteObjectInNode(object);
//		return true;
//	}
//	return false;
//}
//
////--------------------------------------------------------------------------------------
//// Find node
////--------------------------------------------------------------------------------------
//Node* Octree::FindNodeInOctree(TVector3 vPos)
//{
//	if (m_pRootNode == nullptr)
//	{
//		return nullptr;
//	}
//
//	Node* pNode = m_pRootNode;
//	while (pNode != nullptr && pNode->GetIsLeaf() == false)
//	{
//		for (int index = 0; index < OCTREE; index++)
//		{
//			if (pNode->GetChild(index) != nullptr
//				&& pNode->GetChild(index)->EnableExistObjectInNode(vPos) == true)
//			{
//				m_Queue.push(pNode->GetChild(index));
//				break;
//			}
//		}
//		if (m_Queue.empty() == true)
//		{
//			break;
//		}
//		pNode = m_Queue.front();
//		m_Queue.pop();
//	}
//
//	if (pNode == m_pRootNode)
//	{
//		pNode = nullptr;
//	}
//
//	return pNode;
//}
//
////--------------------------------------------------------------------------------------
//// Add object in octree
////--------------------------------------------------------------------------------------
//bool Octree::AddInOctree(Node* pNode, Object* object)
//{
//	if (pNode != nullptr)
//	{
//		pNode->AddObjectInNode(object);
//		return true;
//	}
//
//	return false;
//}
//
////--------------------------------------------------------------------------------------
//// Update
////--------------------------------------------------------------------------------------
//bool Octree::UpdateOctree(Object* object, TVector3 vPos)
//{
//	Node<T>* pNode = FindNodeInOctree(vPos);
//	if (pNode != nullptr)
//	{
//		DeleteObject(pNode, object);
//		AddInOctree(pNode, object);
//		return true;
//	}
//	return false;
//}
//
////--------------------------------------------------------------------------------------
//// Update index list
////--------------------------------------------------------------------------------------
//bool Octree::UpdateIndexList(Node* pNode)
//{
//	m_pHeightMap->m_iNumCols;
//
//	int iIndex = 0;
//	// Up rect
//
//	// Middle rect
//
//	// Down rect
//
//	// Vertical rect
//
//	// Horizental rect
//	// Up rect
//	//pNode->m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 1; m_IndexList[iIndex++] = 2;
//	//m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 2; m_IndexList[iIndex++] = 3;
//
//	//// Middle rect
//	//m_IndexList[iIndex++] = 4; m_IndexList[iIndex++] = 5; m_IndexList[iIndex++] = 6;
//	//m_IndexList[iIndex++] = 4; m_IndexList[iIndex++] = 6; m_IndexList[iIndex++] = 7;
//
//	//// Down rect
//	//m_IndexList[iIndex++] = 8; m_IndexList[iIndex++] = 9; m_IndexList[iIndex++] = 10;
//	//m_IndexList[iIndex++] = 8; m_IndexList[iIndex++] = 10; m_IndexList[iIndex++] = 11;
//
//	//// Vertical rect
//	//m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 13; m_IndexList[iIndex++] = 14;
//	//m_IndexList[iIndex++] = 12; m_IndexList[iIndex++] = 14; m_IndexList[iIndex++] = 15;
//
//	//// Horizental rect
//	//m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 17; m_IndexList[iIndex++] = 18;
//	//m_IndexList[iIndex++] = 16; m_IndexList[iIndex++] = 18; m_IndexList[iIndex++] = 19;
//
//	//// ¾Æ·¡
//	//m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 21; m_IndexList[iIndex++] = 22;
//	//m_IndexList[iIndex++] = 20; m_IndexList[iIndex++] = 22; m_IndexList[iIndex++] = 23;
//	//pNode->m_IndexList.resize(pNode->get * 6 * 2 * 3);
//	return true;
//}
//
////--------------------------------------------------------------------------------------
//// Render
////--------------------------------------------------------------------------------------
//bool Octree::Render(ID3D11DeviceContext* pContext)
//{
//
//	return true;
//}
//
////--------------------------------------------------------------------------------------
//// Release
////--------------------------------------------------------------------------------------
//bool Octree::Release()
//{
//	delete m_pRootNode;
//	return true;
//}
//
////--------------------------------------------------------------------------------------
//// Get Function
////--------------------------------------------------------------------------------------
//Node* Octree::GetRootNode()
//{
//	return m_pRootNode;
//}
//
//int Octree::GetNodeCountInOctree()
//{
//	return m_iNodeCount;
//}