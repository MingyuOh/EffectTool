# Effect Tool
## 📙목차
- 📝 [개요](#-개요)
- 🏆 [목표](#-목표)
- 🛠 [기술 및 도구](#-기술-및-도구)
- 🧑‍💻 [개발 문서](#-개발-문서)
- 📺 [시연 영상](#-시연-영상)
- 🖼️ [스크린샷](#%EF%B8%8F-스크린샷)
- ⚠️ [주의사항](#%EF%B8%8F-주의사항)

## 📝 개요
![EffectTool_Thumbnail](https://user-images.githubusercontent.com/16281218/218702104-19e47381-6c84-476e-9662-d233968656a7.png)    
> - 프로젝트: DirectX 11과 MFC를 활용한 이펙트 툴 제작      
> - 기획 및 제작: 오민규   
> - 분류: 개인 프로젝트   
> - 제작 기간: 25일    
> - 게임 소개:   
> &emsp;DirectX를 이용하여 게임을 제작할 때, 게임 프로젝트에서 직접 이펙트를 생성하는 것이 아닌 게임에서 사용되는        
> &emsp;이펙트 리소스를 기획에 맞춰서 제작하기 위하여 DirectX와 MFC를 이용하여 이펙트 툴을 제작한다.    
> &emsp;사용자가 이펙트 개체를 쉽게 제작하기 위해 MFC를 이용하여 텍스트 입력 박스와 버튼 등의 UI를 제공한다.    
> &emsp;사용자에게 이펙트 데이터(위치, 방향, 속도 등)를 입력하도록 하여 더 쉽고 간편하게 이펙트를 제작할 수 있도록 한다.       

## 🏆 목표      
- DirectX를 활용하여 게임 엔진(라이브러리)을 제작하고 엔진 개발을 통해 3D 수학 개념과 컴퓨터 그래픽스를 학습한다.    
- DirectX 3D 게임 프로젝트를 진행하기 이전에, 게임에서 사용할 이펙트 개체들을 직접 만들기 위해 개발한 게임 엔진과 MFC를 이용하여 이펙트 툴을 제작한다.    
- 해당 툴을 사용하는 다른 사용자들이 이펙트를 제작하기 편리하도록 툴 기능을 최소화하여 UI를 배치하였다.    
- 이펙트를 제작하기 위한 기본적인 기능(생성, 추가, 삭제, 수정, 저장, 불러오기 등)을 구현하여 사용자가 원하는 이펙트를 생성할 수 있도록 한다.    


## 🛠 기술 및 도구   
&emsp;<img src="https://img.shields.io/badge/C-A8B9CC?style=flat-square&&logo=c&logoColor=white"/> 
<img src="https://img.shields.io/badge/C++-00599C?style=flat-square&&logo=cplusplus&logoColor=white"/> 
<img src="https://img.shields.io/badge/Window API-0078D6?style=flat-square&&logo=Windows&logoColor=white"/> 
<img src="https://img.shields.io/badge/-MFC-red?style=flat-square&&logo=Microsoft&logoColor=white"/> 
<img src="https://img.shields.io/badge/-DirectX-black?style=flat-square&&logo=Microsoft&logoColor=white"/> 
<img src="https://img.shields.io/badge/Visual Studio-5C2D91?style=flat-square&&logo=Visual Studio&logoColor=white"/>    

## 🧑‍💻 개발 문서
&emsp;[개발 문서 확인하기](https://github.com/MingyuOh/EffectTool/tree/main/Document)
 
 
## 📺 시연 영상
&emsp;[![Video Label](https://user-images.githubusercontent.com/16281218/218701661-20602342-d82e-4e7d-b7f8-a64dbdc82337.png)](https://youtu.be/GfTfRzlZWFo)        
&emsp;**🔼 이미지를 클릭해주세요.**    

## 🖼️ 스크린샷
&emsp;![EffectTool1](https://user-images.githubusercontent.com/16281218/219298699-3464111c-6e0f-4b87-be59-5b4dea4e8ded.png){: width="300" height="300"}    
&emsp;![EffectTool2](https://user-images.githubusercontent.com/16281218/219298705-148e1ff5-06a5-4d52-acf4-fb0508df5b2f.png){: width="300" height="300"}    
&emsp;![EffectTool3](https://user-images.githubusercontent.com/16281218/219298707-e4101072-13cb-4be4-89d6-8e30712da347.png){: width="300" height="300"}    
&emsp;![EffectTool4](https://user-images.githubusercontent.com/16281218/219298711-9d356e36-a485-482b-a2b0-a0b94c4d8feb.png){: width="300" height="300"}    
&emsp;![EffectTool5](https://user-images.githubusercontent.com/16281218/219298715-85c20e3a-4ba1-407d-bb3e-dccf18801d89.png){: width="300" height="300"}    

## ⚠️ 주의사항    
프로젝트 실행을 원하시면 
**[libfbxsdk-md.lib, libfbxsdk-mt.lib] 파일**을 
**lib/fbx/x64/debug 폴더**에 추가해주세요.
