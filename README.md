# Effect Tool
## ๐๋ชฉ์ฐจ
- ๐ [๊ฐ์](#-๊ฐ์)
- ๐ [๋ชฉํ](#-๋ชฉํ)
- ๐  [๊ธฐ์  ๋ฐ ๋๊ตฌ](#-๊ธฐ์ -๋ฐ-๋๊ตฌ)
- ๐งโ๐ป [๊ฐ๋ฐ ๋ฌธ์](#-๊ฐ๋ฐ-๋ฌธ์)
- ๐บ [์์ฐ ์์](#-์์ฐ-์์)
- ๐ผ๏ธ [์คํฌ๋ฆฐ์ท](#%EF%B8%8F-์คํฌ๋ฆฐ์ท)
- โ ๏ธ [์ฃผ์์ฌํญ](#%EF%B8%8F-์ฃผ์์ฌํญ)

## ๐ ๊ฐ์
<img src="https://user-images.githubusercontent.com/16281218/218702104-19e47381-6c84-476e-9662-d233968656a7.png" width="800" height="600"/>    

> - ํ๋ก์ ํธ: DirectX 11๊ณผ MFC๋ฅผ ํ์ฉํ ์ดํํธ ํด ์ ์      
> - ๊ธฐํ ๋ฐ ์ ์: ์ค๋ฏผ๊ท   
> - ๋ถ๋ฅ: ๊ฐ์ธ ํ๋ก์ ํธ   
> - ์ ์ ๊ธฐ๊ฐ: 25์ผ    
> - ํด ์๊ฐ:   
> &emsp;DirectX๋ฅผ ์ด์ฉํ์ฌ ๊ฒ์์ ์ ์ํ  ๋, ๊ฒ์ ํ๋ก์ ํธ์์ ์ง์  ์ดํํธ๋ฅผ ์์ฑํ๋ ๊ฒ์ด ์๋ ๊ฒ์์์ ์ฌ์ฉ๋๋        
> &emsp;์ดํํธ ๋ฆฌ์์ค๋ฅผ ๊ธฐํ์ ๋ง์ถฐ์ ์ ์ํ๊ธฐ ์ํ์ฌ DirectX์ MFC๋ฅผ ์ด์ฉํ์ฌ ์ดํํธ ํด์ ์ ์ํ๋ค.    
> &emsp;์ฌ์ฉ์๊ฐ ์ดํํธ ๊ฐ์ฒด๋ฅผ ์ฝ๊ฒ ์ ์ํ๊ธฐ ์ํด MFC๋ฅผ ์ด์ฉํ์ฌ ํ์คํธ ์๋ ฅ ๋ฐ์ค์ ๋ฒํผ ๋ฑ์ UI๋ฅผ ์ ๊ณตํ๋ค.    
> &emsp;์ฌ์ฉ์์๊ฒ ์ดํํธ ๋ฐ์ดํฐ(์์น, ๋ฐฉํฅ, ์๋ ๋ฑ)๋ฅผ ์๋ ฅํ๋๋ก ํ์ฌ ๋ ์ฝ๊ณ  ๊ฐํธํ๊ฒ ์ดํํธ๋ฅผ ์ ์ํ  ์ ์๋๋ก ํ๋ค.       

## ๐ ๋ชฉํ      
- DirectX๋ฅผ ํ์ฉํ์ฌ ๊ฒ์ ์์ง(๋ผ์ด๋ธ๋ฌ๋ฆฌ)์ ์ ์ํ๊ณ  ์์ง ๊ฐ๋ฐ์ ํตํด 3D ์ํ ๊ฐ๋๊ณผ ์ปดํจํฐ ๊ทธ๋ํฝ์ค๋ฅผ ํ์ตํ๋ค.    
- DirectX 3D ๊ฒ์ ํ๋ก์ ํธ๋ฅผ ์งํํ๊ธฐ ์ด์ ์, ๊ฒ์์์ ์ฌ์ฉํ  ์ดํํธ ๊ฐ์ฒด๋ค์ ์ง์  ๋ง๋ค๊ธฐ ์ํด ๊ฐ๋ฐํ ๊ฒ์ ์์ง๊ณผ MFC๋ฅผ ์ด์ฉํ์ฌ ์ดํํธ ํด์ ์ ์ํ๋ค.    
- ํด๋น ํด์ ์ฌ์ฉํ๋ ๋ค๋ฅธ ์ฌ์ฉ์๋ค์ด ์ดํํธ๋ฅผ ์ ์ํ๊ธฐ ํธ๋ฆฌํ๋๋ก ํด ๊ธฐ๋ฅ์ ์ต์ํํ์ฌ UI๋ฅผ ๋ฐฐ์นํ์๋ค.    
- ์ดํํธ๋ฅผ ์ ์ํ๊ธฐ ์ํ ๊ธฐ๋ณธ์ ์ธ ๊ธฐ๋ฅ(์์ฑ, ์ถ๊ฐ, ์ญ์ , ์์ , ์ ์ฅ, ๋ถ๋ฌ์ค๊ธฐ ๋ฑ)์ ๊ตฌํํ์ฌ ์ฌ์ฉ์๊ฐ ์ํ๋ ์ดํํธ๋ฅผ ์์ฑํ  ์ ์๋๋ก ํ๋ค.    


## ๐  ๊ธฐ์  ๋ฐ ๋๊ตฌ   
&emsp;<img src="https://img.shields.io/badge/C-A8B9CC?style=flat-square&&logo=c&logoColor=white"/> 
<img src="https://img.shields.io/badge/C++-00599C?style=flat-square&&logo=cplusplus&logoColor=white"/> 
<img src="https://img.shields.io/badge/Window API-0078D6?style=flat-square&&logo=Windows&logoColor=white"/> 
<img src="https://img.shields.io/badge/-MFC-red?style=flat-square&&logo=Microsoft&logoColor=white"/> 
<img src="https://img.shields.io/badge/-DirectX-black?style=flat-square&&logo=Microsoft&logoColor=white"/> 
<img src="https://img.shields.io/badge/Visual Studio-5C2D91?style=flat-square&&logo=Visual Studio&logoColor=white"/>    

## ๐งโ๐ป ๊ฐ๋ฐ ๋ฌธ์
&emsp;[๊ฐ๋ฐ ๋ฌธ์ ํ์ธํ๊ธฐ](https://github.com/MingyuOh/EffectTool/tree/main/Document)
 
 
## ๐บ ์์ฐ ์์
&emsp;[![Video Label](https://user-images.githubusercontent.com/16281218/218701661-20602342-d82e-4e7d-b7f8-a64dbdc82337.png)](https://youtu.be/GfTfRzlZWFo)        
&emsp;**๐ผ ์ด๋ฏธ์ง๋ฅผ ํด๋ฆญํด์ฃผ์ธ์.**    

## ๐ผ๏ธ ์คํฌ๋ฆฐ์ท
&emsp;<img src="https://user-images.githubusercontent.com/16281218/219298699-3464111c-6e0f-4b87-be59-5b4dea4e8ded.png" width="800" height="600"/>    
&emsp;<img src="https://user-images.githubusercontent.com/16281218/219298705-148e1ff5-06a5-4d52-acf4-fb0508df5b2f.png" width="800" height="600"/>    
&emsp;<img src="https://user-images.githubusercontent.com/16281218/219298707-e4101072-13cb-4be4-89d6-8e30712da347.png" width="800" height="600"/>    
&emsp;<img src="https://user-images.githubusercontent.com/16281218/219298711-9d356e36-a485-482b-a2b0-a0b94c4d8feb.png" width="800" height="600"/>    
&emsp;<img src="https://user-images.githubusercontent.com/16281218/219298715-85c20e3a-4ba1-407d-bb3e-dccf18801d89.png" width="800" height="600"/>    

## โ ๏ธ ์ฃผ์์ฌํญ    
ํ๋ก์ ํธ ์คํ์ ์ํ์๋ฉด 
**[libfbxsdk-md.lib, libfbxsdk-mt.lib] ํ์ผ**์ 
**lib/fbx/x64/debug ํด๋**์ ์ถ๊ฐํด์ฃผ์ธ์.
