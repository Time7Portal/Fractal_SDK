#pragma once
#include <fstream>
#include <vector>
#include "FractalDebug.h"


namespace Fractal
{
	class FileIOManager
	{
	public:
		FileIOManager(); // 클래스 시작시 호출되는 함수 | Function called when the class start
		~FileIOManager(); // 클래스 종료시 호출되는 함수 | Function called when the class end

		static bool ReadFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer); // 파일 경로를 받아서 버퍼에 저장하는 함수 | Read file and Return to buffer

	private:
	};
}
