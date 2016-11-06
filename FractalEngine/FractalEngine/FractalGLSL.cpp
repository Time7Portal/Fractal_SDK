#include "FractalGLSL.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::GLSLProgram::GLSLProgram()
{
	_programID = 0;
	_vertexShaderID = 0;
	_fragmentShaderID = 0;
	_numAttribute = 0;
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::GLSLProgram::~GLSLProgram()
{
}


/* - 유니폼 위치를 얻는 함수 | Get uniform location (Uniform name) - */
GLint Fractal::GLSLProgram::GetUniformLocation(const std::string& uniformName)
{
	GLint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX)
	{
		Fractal::Debug::GetInstance()->ConsoleError(uniformName + " - Unable to get GLSL uniforms!");
	}
	return location;
}


/* - GLSL 셰이더에 어트리뷰트를 추가하는 함수 (속성 이름) | Add attribute in GLSL shader (Uniform name) - */
void Fractal::GLSLProgram::AddAttribute(const std::string &attributeName)
{
	glBindAttribLocation(_programID, _numAttribute, attributeName.c_str()); // 어트리뷰트 위치 지정 | Specify attribute location
	_numAttribute++; // 어트리뷰트 개수 카운트 증가
}


/* - GLSL 셰이더를 컴파일 하기 전 체크하는 함수 (버텍스 셰이더 파일경로, 프레그먼트 셰이더 파일 경로) | Check the GLSL shader file (Vertex shader file path, Fragment Shader file path) - */
void Fractal::GLSLProgram::CompileShader(const std::string vertexShaderFilePath, const std::string fragmentShaderFilepath)
{
	// GL 프로그램 개체 생성하고 ID 반환 받기 | If shaders are successfully compiled then link them together into a program and get a program object
	_programID = glCreateProgram();

	// 버텍스 셰이더 개체 생성 후 ID 반환 받기 | Create the vertex shader object, and store its ID
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0)
	{
		Fractal::Debug::GetInstance()->ConsoleError("Vertex shader is not created!");
	}

	// 프레그먼트 셰이더 개체 생성 후 ID 반환 받기 | Create the fragment shader object, and store its ID
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0)
	{
		Fractal::Debug::GetInstance()->ConsoleError("Fragment shader is not created!");
	}

	// 셰이더 컴파일 | Compile each shader
	CompileShaderCore(vertexShaderFilePath, _vertexShaderID); // 버텍스 셰이더 컴파일 하기
	CompileShaderCore(fragmentShaderFilepath, _fragmentShaderID); // 프레그먼트 셰이더 컴파일 하기
}


/* - GLSL 셰이더를 실제로 컴파일 하는 함수 (파일 경로, 셰이더 ID) | Compile the GLSL Shader file really (GLSL file path, Shader ID) - */
void Fractal::GLSLProgram::CompileShaderCore(const std::string &filePath, GLuint shaderID)
{
	// 파일 열기 | Open the file
	std::ifstream vertexFile(filePath);
	if (vertexFile.fail())
	{
		Fractal::Debug::GetInstance()->ConsoleError(filePath + " - Unable to read file from this location!");
	}

	// 텍스트 파일 내용 | File contents stores all the text in the file
	std::string fileContents = "";

	// 텍스트 라인 수 | Line is used to grab each line of the file
	std::string line;

	// 코드를 한줄씩 나누기 | Get all the lines in the file and add it to the contents
	while (std::getline(vertexFile, line))
	{
		fileContents += line + "\n";
	}

	// 파일 닫기 | Close the file
	vertexFile.close();

	// 텍스트 파일 내용의 string 형식을 char* 로 변환한 상수 포인터 | Get a pointer to our file contents c string
	const char* contentsPtr = fileContents.c_str();

	// 셰이더 소스 gl로 넘기기 | Tell opengl that we want to use fileContents as the contents of the shader file
	glShaderSource(shaderID, 1, &contentsPtr, nullptr);

	// 셰이더 컴파일 | Compile the shader
	glCompileShader(shaderID);

	// 오류 검사 | check for errors
	GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success); // 셰이더 컴파일 상황 받기

	if (success == GL_FALSE)
	{
		GLint maxLength = 0; // 널 값을 포함한 최대 길이 | The maxLength includes the NULL character
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength); // 셰이더 컴파일 상황 최대 글자수 받기

		// 셰이더 컴파일 중 오류 발생시 로그 내용 저장
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		// 셰이더 제거하여 VRAM 누수 방지 | Don't leak the shader
		glDeleteShader(shaderID);

		// 오류 발생시 내용 출력
		std::cout << filePath << std::endl;
		std::cout << &errorLog[0] << std::endl;
		Fractal::Debug::GetInstance()->ConsoleError("The shader failed to compile!");
	}
}


/* - GLSL 셰이더 파일을 링킹하는 함수 | Link the GLSL files - */
void Fractal::GLSLProgram::LinkShader()
{
	// 컴파일 완료된 셰이더 링킹 시작 | Attach our shaders to our program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);
	glLinkProgram(_programID); // 프로그램 링크 | Link our program

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0; // 널 값을 포함한 최대 길이 | The maxLength includes the NULL character
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		// 셰이더 컴파일 중 오류 발생시 로그 내용 저장
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		// GLSL 셰이더 프로그램 제거 | We don't need this program anymore
		glDeleteProgram(_programID);

		// 셰이더 제거하여 VRAM 누수 방지 | Don't leak the shader
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		// 오류 발생시 내용 출력
		std::cout << &errorLog[0] << std::endl;
		Fractal::Debug::GetInstance()->ConsoleError("Unable to link compiled shaders!");
	}

	// 더이상 셰이더가 필요 없으므로 속성을 전부 분리합니다 | Always detach shaders after a successful link
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}


/* - GLSL 셰이더 사용 | Activate the GLSL shader - */
void Fractal::GLSLProgram::Use()
{
	// shaderID 등록하기 | Bind the shaderID
	glUseProgram(_programID);

	// 버텍스 어트리뷰트 활성화 | Activate the vertex shader
	for (int i = 0; i < _numAttribute; i++)
	{
		glEnableVertexAttribArray(i);
	}
}


/* - GLSL 셰이더 사용안함 | Deactivate the GLSL shader - */
void Fractal::GLSLProgram::Unuse()
{
	// shaderID 비우기 | Unbind the shaderID
	glUseProgram(0);

	// 버텍스 어트리뷰트 비활성화 | Deactivate the vertex shader
	for (int i = 0; i < _numAttribute; i++)
	{
		glDisableVertexAttribArray(i);
	}
}
