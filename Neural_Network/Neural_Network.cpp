
//inluding of grey_balance.h must be on the top of header, cause SFML need to be heighter then standart libs
#include "grey_balance.h"
#include <iomanip>
#include <chrono>
//including our network class
#include "network.h"

void make_lib()
{
	string folder_name = "C:\\Users\\rrafa\\Desktop\\eagles";
	//creating a library from folder of photos, that represent the same object
	ImageDecoder::lib_maker(folder_name, "eagle");
}

void base_test()
{
	string photo_name = "C:\\Users\\rrafa\\Desktop\\image_test\\D.png";
	//decoding image to numbers by calculating the saturation of red in each pixel
	ImageDecoder::red_balance(photo_name);
	cout << "Image has been decomposed" << endl;
	ifstream fin;
	//loading the set of results
	fin.open("output_set.txt");
	int out_neurons;
	fin >> out_neurons;
	vector<string> output_set;
	for (int i = 0; i < out_neurons; i++)
	{
		string local;
		fin >> local;
		output_set.push_back(local);
	}
	//this array consists from nubers, that corresponds to quantity of neurons for each layer in network
	int size[4] = { 4096, 256, 64, out_neurons };
	typedef chrono::seconds s;
	auto start = chrono::high_resolution_clock::now();
	/*
	in constructor of network we have 7 arguments:
		1. true, if we have to train our network
		2. quantity of layers
		3. previous array
		4. exapmles' quantity for training
		5. results' set
		6. name of file, which consist weights for pretrained neural network
		7. name of library for train
	*/
	NeuralNetwork::perceptron nn(false, 4, size, 83, output_set);
	//analysis of image
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

