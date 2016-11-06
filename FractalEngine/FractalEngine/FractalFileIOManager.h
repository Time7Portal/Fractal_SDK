#pragma once
#include <fstream>
#include <vector>
#include "FractalDebug.h"


namespace Fractal
{
	class FileIOManager
	{
	public:
		FileIOManager(); // Ŭ���� ���۽� ȣ��Ǵ� �Լ� | Function called when the class start
		~FileIOManager(); // Ŭ���� ����� ȣ��Ǵ� �Լ� | Function called when the class end

		static bool ReadFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer); // ���� ��θ� �޾Ƽ� ���ۿ� �����ϴ� �Լ� | Read file and Return to buffer

	private:
	};
}
