#version 330 // 프레그먼트 셰이더는 폴리곤에 기반한 픽셀 안에서서 작동함

in vec2 fragmentPosition; // 버텍스 셰이더에서 가져온 프래그먼트 위치
in vec4 fragmentColor; // 버텍스 셰이더에서 가져온 프래그먼트 색
in vec2 fragmentUV; // 버텍스 셰이더에서 가져온 UV 값

out vec4 outColor; // 최종 색 출력물

uniform float time; // 유니폼 형식의 시각 가져옴 (유니폼 = 읽기전용 전역상수)
uniform sampler2D mySampler;

void main()
{
	// 텍스쳐 정렬
	vec4 textureColor = texture(mySampler, fragmentUV);
	
	// 일렁이는 체크무늬 색 효과 (테스트용)
	outColor = vec4(
	fragmentColor.r * (sin((fragmentPosition.x / 4.0) + (time * 8.0)) + 1.5), 
	fragmentColor.g * (sin((fragmentPosition.y / 4.0) + (time * 8.0)) + 1.5), 
	fragmentColor.b * (sin(((fragmentPosition.x + 14) / 4.0) + sin(fragmentPosition.y / 4.0) + (time * 8.0)) + 1.5), 
	fragmentColor.a) * textureColor;
	
	// 효과 없음 (기본 테스트용)
	//outColor = textureColor * fragmentColor;
}