#include "FractalFileIOManager.h"


/* - 클래스 시작시 호출되는 함수 | Function called when the class start - */
Fractal::FileIOManager::FileIOManager()
{
}


/* - 클래스 종료시 호출되는 함수 | Function called when the class end - */
Fractal::FileIOManager::~FileIOManager()
{
}


/* - 파일 경로를 받아서 버퍼에 저장하는 함수 | Read file and Return to buffer - */
bool Fractal::FileIOManager::ReadFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
{
	// 읽어들일 파일 준비
	std::ifstream file(filePath, std::ios::binary);

	// 파일 읽기에 실패한 경우
	if (file.fail())
	{
		Fractal::Debug::GetInstance()->ConsoleError(filePath + " - Unable to read file from this location!");
		return false;
	}

	// 파일의 끝 위치로 커서 이동 | seek to the end
	file.seekg(0, std::ios::end);

	// 파일 사이즈 저장 | Get the file size
	int fileSize = (int)file.tellg();

	// 파일의 시작 위치로 커서 이동 | seek to the beginning
	file.seekg(0, std::ios::beg);

	// 헤더 바이트 크기만큼 파일 크기 감소 | Reduce the file size by header bytes that might be present
	fileSize -= (int)file.tellg();

	// 버퍼 사이즈 지정 | Resize buffer
	buffer.resize(fileSize);

	// 파일을 읽어들이고 종료 | File read & close
	file.read((char *)&(buffer[0]), fileSize);
	file.close();

	return true;
}
