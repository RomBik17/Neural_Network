
#include <functional>
#include <fstream>
#include <random>
#include <vector>
#include <time.h>
#include <Windows.h>
#include <iostream>
using namespace std;

namespace NeuralNetwork {

	inline double sigmoid(double x);

	struct data_one {
		vector<double> info;
		char rresult;
	};

	struct neuron {
		double value;
		double error;
		inline void activation();
	};

	class perceptron {
	private:
		string lib_adress;
		int layers;
		vector<string> output_set;
		neuron** neurons;
		double*** weights;
		int* size;
		double* input;
		bool have_to_study;
		int count_study;
		int threadsNum = 1;
		double def_sigmoid(double x);
		void study();
		void set_input(double* p);
		void LayersCleaner(int LayerNum, int begin, int end);
		void ForwardFeeder(int LayerNum, int begin, int end);
		double ForwardFeed();
		void ErrorCounter(int LayerNumber, int start, int stop, double rresult, double lr);
		void BackPropagation(double rresult, double lr);
		bool SaveWeights();
	public:
		perceptron(bool study, int n, int* p, int test_count, vector<string> out_set, string filename = "weights.txt", string libname = "lib.txt");
		void work(string filename);
	};

}
