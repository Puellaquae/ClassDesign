#include "�ļ�����.hpp"

namespace �ļ����� {
	std::wifstream �ļ���ʽ�Զ��жϲ���(const std::string& �ļ�·��)
	{
		std::string encoding;
		{
			std::ifstream encodingTest(�ļ�·��, std::ios::binary);
			unsigned char byte;
			encodingTest.read(reinterpret_cast<char*>(&byte), sizeof(byte));
			int bom = byte << 8;
			encodingTest.read(reinterpret_cast<char*>(&byte), sizeof(byte));
			bom |= byte;
			switch (bom)
			{
			case 0xfffe:  //65534
				encoding = ".UTF16LE";
				break;
			case 0xfeff://65279
				encoding = ".UTF16BE";
				break;
			case 0xefbb://61371
				encoding = ".UTF8";
				break;
			default:
				encoding = "";
			}
		}
		std::wifstream ifs(�ļ�·��);
		ifs.imbue(std::locale(encoding));
		return ifs;
	}
}