
#include "grey_balance.h"

LPWSTR ConvertString(const std::string instr)
{
	int bufferlen = MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0);
	if (bufferlen == 0) return 0;
	LPWSTR widestr = new WCHAR[bufferlen + 1];
	MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen);
	widestr[bufferlen] = 0;
	return widestr;
}

std::string convLPCWSTRtoString(LPCWSTR wString)
{
	std::wstring tempWstring(wString);
	std::string tempString(tempWstring.begin(), tempWstring.end());
	return tempString;
}

namespace ImageDecoder {

	int red_balance(std::string filename)
	{
		std::ofstream fout;
		fout.open("Image.txt");
		sf::Image img;
		img.loadFromFile(filename);
		const int height = img.getSize().y;
		const int width = img.getSize().x;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				double x = 1 - (img.getPixel(j, i).r) / 255.0;
				fout << round(x * 1000) / 1000 << " ";
			}
			fout << '\n';
		}
		fout.close();
		return height * width;
	}

	void lib_maker(std::string folder_name, std::string object_type)
	{
		WIN32_FIND_DATAW wfd;
		HANDLE const hFind = FindFirstFileW(ConvertString(folder_name + "\\*"), &wfd);
		setlocale(LC_ALL, "");
		if (INVALID_HANDLE_VALUE != hFind)
		{
			std::ofstream fout;
			fout.open("folder.txt");
			do
			{
				std::string filename = folder_name + "\\" + convLPCWSTRtoString(&wfd.cFileName[0]);
				sf::Image img;
				img.loadFromFile(filename);
				const int height = img.getSize().y;
				const int width = img.getSize().x;
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						double x = 1 - (img.getPixel(j, i).r) / 255.0;
						fout << round(x * 1000) / 1000 << " ";
					}
					fout << '\n';
				}
				fout << object_type << '\n';
			} while (NULL != FindNextFileW(hFind, &wfd));
			fout.close();
			FindClose(hFind);
		}
		system("pause");
	}

}
