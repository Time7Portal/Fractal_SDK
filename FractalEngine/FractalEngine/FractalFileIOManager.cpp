#include "FractalFileIOManager.h"


/* - Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start - */
Fractal::FileIOManager::FileIOManager()
{
}


/* - Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end - */
Fractal::FileIOManager::~FileIOManager()
{
}


/* - ���� ��θ� �޾Ƽ� ���ۿ� �����ϴ� �Լ� | Read file and Return to buffer - */
bool Fractal::FileIOManager::ReadFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
{
	// �о���� ���� �غ�
	std::ifstream file(filePath, std::ios::binary);

	// ���� �б⿡ ������ ���
	if (file.fail())
	{
		Fractal::Debug::GetInstance()->ConsoleError(filePath + " - Unable to read file from this location!");
		return false;
	}

	// ������ �� ��ġ�� Ŀ�� �̵� | seek to the end
	file.seekg(0, std::ios::end);

	// ���� ������ ���� | Get the file size
	int fileSize = (int)file.tellg();

	// ������ ���� ��ġ�� Ŀ�� �̵� | seek to the beginning
	file.seekg(0, std::ios::beg);

	// ��� ����Ʈ ũ�⸸ŭ ���� ũ�� ���� | Reduce the file size by header bytes that might be present
	fileSize -= (int)file.tellg();

	// ���� ������ ���� | Resize buffer
	buffer.resize(fileSize);

	// ������ �о���̰� ���� | File read & close
	file.read((char *)&(buffer[0]), fileSize);
	file.close();

	return true;
}
