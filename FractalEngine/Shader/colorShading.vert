#version 330 // 버텍스 셰이더는 각 정점에서 작동함

// vbo 로부터 아래 2가지 데이터를 받아옴
in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

//프레그먼트 셰이더에 넘길 값들
out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 camMatrix;

void main()
{
	gl_Position.xy = (camMatrix * vec4(vertexPosition, 0.0, 1.0)).xy; // 버텍스 포지션을 GL포지션으로 사용
	
	gl_Position.z = 0.0; // 깊이 값은 2D니 0으로 설정
	gl_Position.w = 1.0; // 좌표 표준화 (각 축의 최대값 설정, 기본값은 1.0)
	
	fragmentPosition = vertexPosition; // 정점 위치를 프레그먼트 위치로 사용
	fragmentColor = vertexColor; // 정점 색을 프레그먼트 색으로 사용
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y); // 정점 UV를 프레그먼트 UV로 사용 (그리고 UV 좌표계 뒤집기)
}