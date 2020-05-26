
#include "grey_balance.h"
#include <iomanip>
#include <chrono>
#include "network.h"

void make_lib()
{
	string photo_name = "C:\\Users\\rrafa\\Desktop\\eagles";
	ImageDecoder::lib_maker(photo_name, "eagle");
}

void base_test()
{
	string photo_name = "C:\\Users\\rrafa\\Desktop\\image_test\\D.png";
	ImageDecoder::grey_balance(photo_name);
	cout << "Image has been decomposed" << endl;
	ifstream fin;
	fin.open("output_set.txt");
	int out_neurons;
	fin >> out_neurons;
	vector<char> output_set;
	for (int i = 0; i < out_neurons; i++)
	{
		char local;
		fin >> local;
		output_set.push_back(local);
	}
	int size[4] = { 4096, 256, 64, out_neurons };
	typedef chrono::seconds s;
	auto start = chrono::high_resolution_clock::now();
	NeuralNetwork::perceptron nn(false, 4, size, 83, output_set);
	nn.work("Image.txt");
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = end - start;
	cout << duration.count();
}

int main()
{
	ios_base::sync_with_stdio(false);
	base_test();
	return 0;
}

