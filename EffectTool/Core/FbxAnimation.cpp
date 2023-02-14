#include "FbxObject.h"

namespace DXCore
{
	void FbxObject::ParseAnimation()
	{
		FbxArray<FbxString*> AnimStackNameArray;
		m_pFbxScene->FillAnimStackNameArray(AnimStackNameArray);
		int iAnimStackCount = AnimStackNameArray.GetCount();
		for (int iIndex = 0; iIndex < iAnimStackCount; iIndex++)
		{
			ParseAnimationStack(AnimStackNameArray.GetAt(iIndex));
		}
	}

	void FbxObject::ParseAnimationStack(FbxString* szData)
	{
		m_pFbxScene->GetAnimationEvaluator()->Reset();
		// Frame, Tick
		// 1 Frame = 160 Tick
		// FrameSpeed = 1 Sec Per 30 Frame
		// 1 SecTick = 30 (FrameSpeed) * 160 Tick(= 1 Frame) = 4800 Tick
		// fFrameTime = 0 ~ N Frame (N * 160 Tick / 4800 SecTick) = ( ex. 3.3333..Sec (N == 100) )
		FbxTakeInfo* pTakeInfo = m_pFbxScene->GetTakeInfo(*szData);
		FbxTime FrameTime;
		FrameTime.SetTime(0, 0, 0, 1, 0, m_pFbxScene->GetGlobalSettings().GetTimeMode());
		float fFrameTime = FrameTime.GetSecondDouble();
		float fFrameStep = 1;
		m_fSamplingTime = fFrameTime * fFrameStep;

		if (pTakeInfo != nullptr)
		{
			m_fStartTime = static_cast<float>(pTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble());
			m_fEndTime = static_cast<float>(pTakeInfo->mLocalTimeSpan.GetStop().GetSecondDouble());
		}
		else
		{
			FbxTimeSpan timeSpan;
			m_pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(timeSpan);
			m_fStartTime = static_cast<float>(timeSpan.GetStart().GetSecondDouble());
			m_fEndTime = static_cast<float>(timeSpan.GetStop().GetSecondDouble());
		}
	}

	void FbxObject::ParseAnimationNode()
	{
		// �ִϸ��̼� ������ ����
		FbxAnimEvaluator* pAnimation = m_pFbxScene->GetAnimationEvaluator();

		float fCurrentTime = m_fStartTime;
		while (fCurrentTime < m_fEndTime)
		{
			FbxTime time;
			time.SetSecondDouble(fCurrentTime);
			for (int iMesh = 0; iMesh < m_pFbxMeshList.size(); iMesh++)
			{
				Mesh* pMesh = m_pFbxMeshList[iMesh];
				FbxAMatrix matGlobal = pAnimation->GetNodeGlobalTransform(pMesh->m_pFbxNode, time);
				TMatrix matGlobalDX = DxConvertMatrix(ConvertAMatrix(matGlobal));
				pMesh->m_AnimationTrack.push_back(matGlobalDX);
			}
			fCurrentTime += m_fSamplingTime;
		}
	}

	bool FbxObject::ParseMeshSkinning(FbxMesh* pFbxMesh, Mesh* pMesh, SkinData* pSkinData)
	{
		// ��Ų ������̾� ������ �޾��� ��� iNumDeformer �� ���� ��������.
		int iNumDeformer = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
		if (iNumDeformer == 0)
		{
			return false;
		}

		// ������ ���� �������� ����
		int iNumVertexCount = pFbxMesh->GetControlPointsCount();
		// ���� ���� �������� ���� ��ŭ ����ġ ���� �Ҵ�
		pSkinData->m_VertexList.resize(iNumVertexCount);

		for (int iDeformer = 0; iDeformer < iNumDeformer; iDeformer++)
		{
			FbxDeformer* pFbxDeformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
			FbxSkin* pSkin = (FbxSkin*)pFbxDeformer;
			int iNumCluster = pSkin->GetClusterCount();
			pMesh->m_matBindPoseList.resize(iNumCluster);
			
			// ��Ű�� ������Ʈ ��忡 ������ ��ġ�� ����� iNumCluster �ȿ� �ִ�.
			for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
			{
				FbxCluster* pCluster = pSkin->GetCluster(iCluster);
				// ��忡 ������ ��ġ�� ����� iClusterSize ������ ������ ��ģ��.
				int iNumVertex = pCluster->GetControlPointIndicesCount();

				FbxAMatrix matXBindPose, matInitPosition;
				pCluster->GetTransformLinkMatrix(matXBindPose);
				pCluster->GetTransformMatrix(matInitPosition);
				FbxAMatrix matBoneBindPose = matInitPosition.Inverse() * matXBindPose;
				TMatrix matBindPose = DxConvertMatrix(ConvertAMatrix(matBoneBindPose));
				D3DXMatrixInverse(&matBindPose, NULL, &matBindPose);
				pMesh->m_matBindPoseList[iCluster] = matBindPose;

				FbxNode* pLinkNode = pCluster->GetLink();
				pMesh->m_pFbxNodeList.push_back(pLinkNode);
				// ������ ��������Ʈ
				int* iIndex = pCluster->GetControlPointIndices();
				// ����ġ ����Ʈ
				double* pWeight = pCluster->GetControlPointWeights();
				for (int index = 0; index < iNumVertex; index++)
				{
					// iIndex[index] ������
					// iMatrixIndex ��İ� pWeight[index] = 1.0f ����ġ�� ������ ��ģ��.
					pSkinData->m_VertexList[iIndex[index]].m_IndexList.push_back(iCluster);
					pSkinData->m_VertexList[iIndex[index]].m_WeightList.push_back(pWeight[index]);
				}
			}
		}
		return true;
	}
}