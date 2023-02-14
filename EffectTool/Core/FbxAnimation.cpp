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
		// 애니메이션 데이터 저장
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
		// 스킨 모디파이어 영향을 받았을 경우 iNumDeformer 의 수가 정해진다.
		int iNumDeformer = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
		if (iNumDeformer == 0)
		{
			return false;
		}

		// 영향을 받은 제어점의 개수
		int iNumVertexCount = pFbxMesh->GetControlPointsCount();
		// 영향 받은 제어점의 개수 만큼 가중치 개수 할당
		pSkinData->m_VertexList.resize(iNumVertexCount);

		for (int iDeformer = 0; iDeformer < iNumDeformer; iDeformer++)
		{
			FbxDeformer* pFbxDeformer = pFbxMesh->GetDeformer(iDeformer, FbxDeformer::eSkin);
			FbxSkin* pSkin = (FbxSkin*)pFbxDeformer;
			int iNumCluster = pSkin->GetClusterCount();
			pMesh->m_matBindPoseList.resize(iNumCluster);
			
			// 스키닝 오브젝트 노드에 영향을 미치는 행렬이 iNumCluster 안에 있다.
			for (int iCluster = 0; iCluster < iNumCluster; iCluster++)
			{
				FbxCluster* pCluster = pSkin->GetCluster(iCluster);
				// 노드에 영향을 미치는 행렬이 iClusterSize 정점에 영향을 미친다.
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
				// 제어점 정점리스트
				int* iIndex = pCluster->GetControlPointIndices();
				// 가중치 리스트
				double* pWeight = pCluster->GetControlPointWeights();
				for (int index = 0; index < iNumVertex; index++)
				{
					// iIndex[index] 정점은
					// iMatrixIndex 행렬과 pWeight[index] = 1.0f 가중치로 영향을 미친다.
					pSkinData->m_VertexList[iIndex[index]].m_IndexList.push_back(iCluster);
					pSkinData->m_VertexList[iIndex[index]].m_WeightList.push_back(pWeight[index]);
				}
			}
		}
		return true;
	}
}