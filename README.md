# WaterShader
#OpenGL#Shader#GPU Shader#Graphics#Water Surface Simulation

<a href="http://kon9383.godohosting.com/content/award/award_02.php" target="_blank">홈페이지 바로가기</a>

본 프로젝트는 게임을 비롯한 많은 콘텐츠에서 활용하기 위한 GPU기반 사실적 물 애니메이션 기법을 제안하여 구현하였다. 물 표면은 반사 및 굴절과 같은 물리적 현상이 일어나며, 시점에 따른 반사와 굴절의 정도가 자동적으로 조절된다. 제안된 알고리즘은 윈도우 기반위에서 OpenGL API와 GLSL Shader 언어를 통해 구현되었으며, 구현 결과, 물결의 자연스러운 움직임이 확인 되었고, 30 프레임 이상의 속도로 렌더링 되었음이 확인 되었다.

[SKILL]
1) Reflection Texture(반사)는 물 표면의 윗부분을 절단(Clipping)하고 180도로 회전을 시켜 반사 이미지를 얻는다.
2) Refraction Texture(굴절)는 물 표면의 아랫부분을 절단(Clipping)하여 2개의 이미지를 얻는다.
3) Reflection(반사) & Refraction(굴절) 이미지를 하나의 프레임버퍼오브젝트에 컬러정보와 뎁스정보를 하나의 텍스쳐로 저장한다.
4) FBO(FrameBufferObject)에 DU/DV Map을 이용하여 텍스쳐 좌표를 변형하여 물결에 모양을 얻는다.
5) 텍스쳐 좌표를 변형 후 X축, Y축에 Speed값을 어플리케이션에서 연산 후 GPU쉐이더에 전달하여 물결의 흐름을 구현한다.
6) 수식을 톻아여 카메라의 시점 방향에 따라 물 표면에 반사/굴절이미지가 달라지므로 Normal벡터와 시점 벡터에 대한 값(굴절의 정도)으로 프레넬을 구현함.
7) 마지막으로 믈 표면의 색상을 계산한다. 물의 고유의 색상, 깊이에 따른 색상 변화, 반사광(퐁광원모델)을 수식을 통해 색상을 결정.

[ELEMENT]<br>
최종 결과는 Visual Studio 2013을 기준으로 개발하였으며, 물 표면의 자연스러운 반사/굴절의 모습을 나타내며, 
애니메이션을 통해 자연스러운 물결 흐름을 나타낸다. 렌더리은 30프레임 이상의 실시간 속도를 출력하고 있으며 
프레넬계산에 따라 시점방향이 달라짐에 따라 반사와 굴절의 정도가 바뀌는 것을 볼 수 있다.
프로젝트는 단순히 텍스처의 좌표를 이동함으로서 물결을 표현하였으므로, 강체와의 자연스러운 상호작용은 되지 않는다. 
SPH 혹은 위치기반 다이나믹스와 같은 사실적인 유체 애니메이션기법을 본 논문에서 제안한 렌더링 기법과 결합한다면 좀 더 효과적인 물 표면이 되리라고 생각된다.

[ACHIEVEMENT]
<ul>
<li> 2017. 04. 28~29에 개최한 제주 [정보처리학회]에서 학부생논문경진대회 부분에서 [동상]을 수상함. </li>
<li> 2017. 12. 10 정보처리학회논문지 KTSDE – 12월 호 Real-Time Water Surface Simulation on GPU 게제함. </li>
</ul>

★ 개발언어/엔진
<br>Visual C++/ OpenGL

★ 개발인원
<br>2명

★ 개발기간
<br>2016.01 – 2016.12 / 2개월

★ 개발내용
<ul>
<li> 절단 플레인을 이용한 투사/반사 텍스터 구현</li>
<li> 텍스처 변형 / 프레넬 계산 구현</li>
<li> 퐁 광원 모델을 통한 반사광 구현</li>
<li> 물리적 법칙을 활용한 반사 / 투사정도 조절</li>
<li> 물 표면 애니메이션 구현</li>
<li> WaterShader plane 전체 개발</li>
</ul>
