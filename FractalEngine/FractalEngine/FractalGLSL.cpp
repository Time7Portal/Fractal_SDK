#include "FractalGLSL.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::GLSLProgram::GLSLProgram()
{
	_programID = 0;
	_vertexShaderID = 0;
	_fragmentShaderID = 0;
	_numAttribute = 0;
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::GLSLProgram::~GLSLProgram()
{
}


/* - ������ ��ġ�� ��� �Լ� | Get uniform location (Uniform name) - */
GLint Fractal::GLSLProgram::GetUniformLocation(const std::string& uniformName)
{
	GLint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX)
	{
		Fractal::Debug::GetInstance()->ConsoleError(uniformName + " - Unable to get GLSL uniforms!");
	}
	return location;
}


/* - GLSL ���̴��� ��Ʈ����Ʈ�� �߰��ϴ� �Լ� (�Ӽ� �̸�) | Add attribute in GLSL shader (Uniform name) - */
void Fractal::GLSLProgram::AddAttribute(const std::string &attributeName)
{
	glBindAttribLocation(_programID, _numAttribute, attributeName.c_str()); // ��Ʈ����Ʈ ��ġ ���� | Specify attribute location
	_numAttribute++; // ��Ʈ����Ʈ ���� ī��Ʈ ����
}


/* - GLSL ���̴��� ������ �ϱ� �� üũ�ϴ� �Լ� (���ؽ� ���̴� ���ϰ��, �����׸�Ʈ ���̴� ���� ���) | Check the GLSL shader file (Vertex shader file path, Fragment Shader file path) - */
void Fractal::GLSLProgram::CompileShader(const std::string vertexShaderFilePath, const std::string fragmentShaderFilepath)
{
	// GL ���α׷� ��ü �����ϰ� ID ��ȯ �ޱ� | If shaders are successfully compiled then link them together into a program and get a program object
	_programID = glCreateProgram();

	// ���ؽ� ���̴� ��ü ���� �� ID ��ȯ �ޱ� | Create the vertex shader object, and store its ID
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0)
	{
		Fractal::Debug::GetInstance()->ConsoleError("Vertex shader is not created!");
	}

	// �����׸�Ʈ ���̴� ��ü ���� �� ID ��ȯ �ޱ� | Create the fragment shader object, and store its ID
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0)
	{
		Fractal::Debug::GetInstance()->ConsoleError("Fragment shader is not created!");
	}

	// ���̴� ������ | Compile each shader
	CompileShaderCore(vertexShaderFilePath, _vertexShaderID); // ���ؽ� ���̴� ������ �ϱ�
	CompileShaderCore(fragmentShaderFilepath, _fragmentShaderID); // �����׸�Ʈ ���̴� ������ �ϱ�
}


/* - GLSL ���̴��� ������ ������ �ϴ� �Լ� (���� ���, ���̴� ID) | Compile the GLSL Shader file really (GLSL file path, Shader ID) - */
void Fractal::GLSLProgram::CompileShaderCore(const std::string &filePath, GLuint shaderID)
{
	// ���� ���� | Open the file
	std::ifstream vertexFile(filePath);
	if (vertexFile.fail())
	{
		Fractal::Debug::GetInstance()->ConsoleError(filePath + " - Unable to read file from this location!");
	}

	// �ؽ�Ʈ ���� ���� | File contents stores all the text in the file
	std::string fileContents = "";

	// �ؽ�Ʈ ���� �� | Line is used to grab each line of the file
	std::string line;

	// �ڵ带 ���پ� ������ | Get all the lines in the file and add it to the contents
	while (std::getline(vertexFile, line))
	{
		fileContents += line + "\n";
	}

	// ���� �ݱ� | Close the file
	vertexFile.close();

	// �ؽ�Ʈ ���� ������ string ������ char* �� ��ȯ�� ��� ������ | Get a pointer to our file contents c string
	const char* contentsPtr = fileContents.c_str();

	// ���̴� �ҽ� gl�� �ѱ�� | Tell opengl that we want to use fileContents as the contents of the shader file
	glShaderSource(shaderID, 1, &contentsPtr, nullptr);

	// ���̴� ������ | Compile the shader
	glCompileShader(shaderID);

	// ���� �˻� | check for errors
	GLint success = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success); // ���̴� ������ ��Ȳ �ޱ�

	if (success == GL_FALSE)
	{
		GLint maxLength = 0; // �� ���� ������ �ִ� ���� | The maxLength includes the NULL character
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength); // ���̴� ������ ��Ȳ �ִ� ���ڼ� �ޱ�

		// ���̴� ������ �� ���� �߻��� �α� ���� ����
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		// ���̴� �����Ͽ� VRAM ���� ���� | Don't leak the shader
		glDeleteShader(shaderID);

		// ���� �߻��� ���� ���
		std::cout << filePath << std::endl;
		std::cout << &errorLog[0] << std::endl;
		Fractal::Debug::GetInstance()->ConsoleError("The shader failed to compile!");
	}
}


/* - GLSL ���̴� ������ ��ŷ�ϴ� �Լ� | Link the GLSL files - */
void Fractal::GLSLProgram::LinkShader()
{
	// ������ �Ϸ�� ���̴� ��ŷ ���� | Attach our shaders to our program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);
	glLinkProgram(_programID); // ���α׷� ��ũ | Link our program

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0; // �� ���� ������ �ִ� ���� | The maxLength includes the NULL character
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		// ���̴� ������ �� ���� �߻��� �α� ���� ����
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		// GLSL ���̴� ���α׷� ���� | We don't need this program anymore
		glDeleteProgram(_programID);

		// ���̴� �����Ͽ� VRAM ���� ���� | Don't leak the shader
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		// ���� �߻��� ���� ���
		std::cout << &errorLog[0] << std::endl;
		Fractal::Debug::GetInstance()->ConsoleError("Unable to link compiled shaders!");
	}

	// ���̻� ���̴��� �ʿ� �����Ƿ� �Ӽ��� ���� �и��մϴ� | Always detach shaders after a successful link
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}


/* - GLSL ���̴� ��� | Activate the GLSL shader - */
void Fractal::GLSLProgram::Use()
{
	// shaderID ����ϱ� | Bind the shaderID
	glUseProgram(_programID);

	// ���ؽ� ��Ʈ����Ʈ Ȱ��ȭ | Activate the vertex shader
	for (int i = 0; i < _numAttribute; i++)
	{
		glEnableVertexAttribArray(i);
	}
}


/* - GLSL ���̴� ������ | Deactivate the GLSL shader - */
void Fractal::GLSLProgram::Unuse()
{
	// shaderID ���� | Unbind the shaderID
	glUseProgram(0);

	// ���ؽ� ��Ʈ����Ʈ ��Ȱ��ȭ | Deactivate the vertex shader
	for (int i = 0; i < _numAttribute; i++)
	{
		glDisableVertexAttribArray(i);
	}
}
