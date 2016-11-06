#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include "FractalDebug.h"


namespace Fractal
{
	class GLSLProgram
	{
	public:
		GLSLProgram(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~GLSLProgram(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		GLint GetUniformLocation(const std::string& uniformName); // ������ ��ġ�� ��� �Լ� (�Ӽ� �̸�) | Get uniform location (Uniform name)

		void AddAttribute(const std::string &attributeName); // GLSL ���̴��� ��Ʈ����Ʈ�� �߰��ϴ� �Լ� (�Ӽ� �̸�) | Add attribute in GLSL shader (Uniform name)
		void CompileShader(const std::string vertexShaderFilePath, const std::string fragmentShaderFilepath); // GLSL ���̴��� ������ �ϱ� �� üũ�ϴ� �Լ� (���ؽ� ���̴� ���ϰ��, �����׸�Ʈ ���̴� ���� ���) | Check the GLSL shader file (Vertex shader file path, Fragment Shader file path)
		void LinkShader(); // GLSL ���̴� ������ ��ŷ�ϴ� �Լ� | Link the GLSL files
		
		void Use(); // GLSL ���̴� ��� | Activate the GLSL shader
		void Unuse(); // GLSL ���̴� ������ | Deactivate the GLSL shader

	private:
		int _numAttribute; // ��Ʈ����Ʈ ���� | Number of attributes

		void CompileShaderCore(const std::string &filePath, GLuint shaderID); // GLSL ���̴��� ������ ������ �ϴ� �Լ� (���� ���, ���̴� ID) | Compile the GLSL Shader file really (GLSL file path, Shader ID)

		GLuint _programID; // ���̴� ���α׷� ���̵�
		GLuint _vertexShaderID; // ���ؽ� ���̴� ���̵�
		GLuint _fragmentShaderID; // �����׸�Ʈ ���̴� ���̵�
	};
}
