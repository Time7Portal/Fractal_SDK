#version 330 // ���ؽ� ���̴��� �� �������� �۵���

// vbo �κ��� �Ʒ� 2���� �����͸� �޾ƿ�
in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

//�����׸�Ʈ ���̴��� �ѱ� ����
out vec2 fragmentPosition;
out vec4 fragmentColor;
out vec2 fragmentUV;

uniform mat4 camMatrix;

void main()
{
	gl_Position.xy = (camMatrix * vec4(vertexPosition, 0.0, 1.0)).xy; // ���ؽ� �������� GL���������� ���
	
	gl_Position.z = 0.0; // ���� ���� 2D�� 0���� ����
	gl_Position.w = 1.0; // ��ǥ ǥ��ȭ (�� ���� �ִ밪 ����, �⺻���� 1.0)
	
	fragmentPosition = vertexPosition; // ���� ��ġ�� �����׸�Ʈ ��ġ�� ���
	fragmentColor = vertexColor; // ���� ���� �����׸�Ʈ ������ ���
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y); // ���� UV�� �����׸�Ʈ UV�� ��� (�׸��� UV ��ǥ�� ������)
}