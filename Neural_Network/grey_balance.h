
#include <SFML/Graphics.hpp>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <atlstr.h>

namespace ImageDecoder {

	//decoding image to numbers by calculating the saturation of red in each pixel
	int red_balance(std::string filename);

	//creating a library from folder of photos, that represent the same object
	void lib_maker(std::string folder_name, std::string object_type);

}
