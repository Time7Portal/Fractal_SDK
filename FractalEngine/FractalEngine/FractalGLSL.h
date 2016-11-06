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
		GLSLProgram(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~GLSLProgram(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		GLint GetUniformLocation(const std::string& uniformName); // 유니폼 위치를 얻는 함수 (속성 이름) | Get uniform location (Uniform name)

		void AddAttribute(const std::string &attributeName); // GLSL 셰이더에 어트리뷰트를 추가하는 함수 (속성 이름) | Add attribute in GLSL shader (Uniform name)
		void CompileShader(const std::string vertexShaderFilePath, const std::string fragmentShaderFilepath); // GLSL 셰이더를 컴파일 하기 전 체크하는 함수 (버텍스 셰이더 파일경로, 프레그먼트 셰이더 파일 경로) | Check the GLSL shader file (Vertex shader file path, Fragment Shader file path)
		void LinkShader(); // GLSL 셰이더 파일을 링킹하는 함수 | Link the GLSL files
		
		void Use(); // GLSL 셰이더 사용 | Activate the GLSL shader
		void Unuse(); // GLSL 셰이더 사용안함 | Deactivate the GLSL shader

	private:
		int _numAttribute; // 어트리뷰트 개수 | Number of attributes

		void CompileShaderCore(const std::string &filePath, GLuint shaderID); // GLSL 셰이더를 실제로 컴파일 하는 함수 (파일 경로, 셰이더 ID) | Compile the GLSL Shader file really (GLSL file path, Shader ID)

		GLuint _programID; // 셰이더 프로그램 아이디
		GLuint _vertexShaderID; // 버텍스 셰이더 아이디
		GLuint _fragmentShaderID; // 프레그먼트 셰이더 아이디
	};
}
