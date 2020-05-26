
#include "network.h"
using namespace std;

namespace NeuralNetwork {

	//sigmoid function as activation of neuron
	inline double sigmoid(double x)
	{
		return (1 / (1 + pow(2.71828, -x)));
	}

	inline void neuron::activation()
	{
		value = sigmoid(value);
	}

	perceptron::perceptron(bool study, int n, int* p, int test_count, vector<string> out_set, string filename, string libname)
	{
		for (auto a : out_set)
		{
			output_set.push_back(a);
		}
		have_to_study = study;
		lib_adress = libname;
		count_study = test_count;
		input = new double[p[0]];
		//if we want to train the network, we will create an new one with random weights
		if (have_to_study)
		{
			srand(time(0));
			layers = n;
			neurons = new neuron * [n];
			weights = new double** [n - 1];
			size = new int[n];
			for (int i = 0; i < n; i++) {
				size[i] = p[i];
				neurons[i] = new neuron[p[i]];
				if (i < n - 1) {
					weights[i] = new double* [p[i]];
					for (int j = 0; j < p[i]; j++) {
						weights[i][j] = new double[p[i + 1]];
						for (int k = 0; k < p[i + 1]; k++) {
							weights[i][j][k] = ((rand() % 100)) * 0.01 / size[i];
						}
					}
				}
			}
		}
		//else we'll build a network with weights from file
		else
		{
			ifstream fin;
			fin.open(filename);
			srand(time(0));
			layers = n;
			neurons = new neuron * [n];
			weights = new double** [n - 1];
			size = new int[n];
			for (int i = 0; i < n; i++) {
				size[i] = p[i];
				neurons[i] = new neuron[p[i]];
				if (i < n - 1) {
					weights[i] = new double* [p[i]];
					for (int j = 0; j < p[i]; j++) {
						weights[i][j] = new double[p[i + 1]];
						for (int k = 0; k < p[i + 1]; k++) {
							fin >> weights[i][j][k];
						}
					}
				}
			}
		}
	}

	//the derivative of sigmoid function
	double perceptron::def_sigmoid(double x)
	{
		double i = sigmoid(x);
		return i * (1.0 - i);
	}

	//training
	void perceptron::study()
	{
		srand(time(0));
		ifstream fin;
		double lr = 0.5;
		string rresult;
		double result;
		double ra = 0;
		int maxra = 0;
		int maxraepoch = 0;
		const int n = count_study;

		data_one* data = new data_one[n];

		if (have_to_study) {
			fin.open(lib_adress);
			for (int i = 0; i < n; i++)
			{
				//we read the whole library
				double temp;
				for (int j = 0; j < size[0]; j++)
				{
					fin >> temp;
					data[i].info.push_back(temp);
				}
				fin >> data[i].rresult;
			}
			//while network can't solve the whole library, it will training
			for (int e = 0; ra / n * 100 < 100; e++)
			{
				ra = 0;
				for (int i = 0; i < n; i++) {

					for (int j = 0; j < size[0]; j++)
					{
						input[j] = data[i].info[j];
					}
					rresult = data[i].rresult;
					set_input(input);

					//getting a result
					result = ForwardFeed();

					if (output_set[result] == rresult)
					{
						ra++;
					}
					else
					{
						int i = 0;
						while (i < output_set.size())
						{
							if (rresult == output_set[i]) break;
							i++;
						}
						//for incorrect result we will calculate errors
						BackPropagation(i, lr);
					}
				}
				cout << "Right answers: " << ra / n * 100 << "% \t Max RA: " << double(maxra) / n * 100 << "(epoch " << maxraepoch << " )" << endl;
				if (ra > maxra)
				{
					maxra = ra;
					maxraepoch = e;
				}
				if (maxraepoch < e - 250) maxra = 0;

			}
			if (SaveWeights()) cout << "Weights saved!" << endl;
		}
		fin.close();
	}

	//creation of input layer
	void perceptron::set_input(double* p)
	{
		for (int i = 0; i < size[0]; i++)
		{
			neurons[0][i].value = p[i];
		}
	}

	//clearing of old values
	void perceptron::LayersCleaner(int LayerNum, int begin, int end)
	{
		for (int i = begin; i < end; i++)
		{
			neurons[LayerNum][i].value = 0;
		}
	}

	//calcalation for whole layer
	void perceptron::ForwardFeeder(int LayerNum, int begin, int end)
	{
		for (int j = begin; j < end; j++)
		{
			for (int k = 0; k < size[LayerNum - 1]; k++)
			{
				neurons[LayerNum][j].value += neurons[LayerNum - 1][k].value * weights[LayerNum - 1][k][j];
			}
			neurons[LayerNum][j].activation();
		}
	}

	//getting a result
	double perceptron::ForwardFeed()
	{
		for (int i = 1; i < layers; i++)
		{
			LayersCleaner(i, 0, size[i]);
			ForwardFeeder(i, 0, size[i]);
		}
		double max = 0;
		double prediction = 0;
		for (int i = 0; i < size[layers - 1]; i++)
		{
			if (neurons[layers - 1][i].value > max)
			{
				max = neurons[layers - 1][i].value;
				prediction = i;
			}
		}
		return prediction;
	}

	void perceptron::ErrorCounter(int LayerNumber, int start, int stop, double rresult, double lr)
	{
		if (LayerNumber == layers - 1) {
			for (int j = start; j < stop; j++) {
				if (j != int(rresult)) {
					neurons[LayerNumber][j].error = -pow((neurons[LayerNumber][j].value), 2);
				}
				else {
					neurons[LayerNumber][j].error = pow(1.0 - neurons[LayerNumber][j].value, 2);
				}
			}
		}
		else {
			for (int j = start; j < stop; j++) {
				double error = 0.0;
				for (int k = 0; k < size[LayerNumber + 1]; k++) {
					error += neurons[LayerNumber + 1][k].error * weights[LayerNumber][j][k];
				}
				neurons[LayerNumber][j].error = error;
			}
		}
	}

	void perceptron::BackPropagation(double rresult, double lr)
	{
		for (int i = layers - 1; i > 0; i--)
		{
			ErrorCounter(i, 0, size[i], rresult, lr);
		}
		function<void(int, int, int, double)> wupdate = [&](int begin, int end, int i, double lr) {
			for (int j = 0; j < size[i]; j++)
			{
				for (int k = 0; k < size[i + 1]; k++)
				{
					weights[i][j][k] += lr * neurons[i + 1][k].error * def_sigmoid(neurons[i + 1][k].value) * neurons[i][j].value;
				}
			}
		};
		for (int i = 0; i < layers - 1; i++)
		{
			wupdate(0, size[i], i, lr);
		}
	}

	bool perceptron::SaveWeights()
	{
		ofstream fout;
		fout.open("weights.txt");
		for (int i = 0; i < layers; i++)
		{
			if (i < layers - 1)
			{
				for (int j = 0; j < size[i]; j++)
				{
					for (int k = 0; k < size[i + 1]; k++)
					{
						fout << weights[i][j][k] << " ";
					}
				}
			}
		}
		fout.close();
		return true;
	}

	void perceptron::work(string filename)
	{
		study();
		ifstream fin;
		double result;
		cout << "Start test:(1/0) ";
		bool to_start_test = 1;
		string right_res;
		if (to_start_test) {
			fin.open(filename);
			for (int i = 0; i < size[0]; i++)
				fin >> input[i];
			set_input(input);
			result = ForwardFeed();
			cout << "I think it is: " << output_set[result] << "\n\n";
			cout << "What is the real result?...  ";
			cin >> right_res;
			if (right_res != output_set[result]) {
				int i = 0;
				while (i < output_set.size())
				{
					if (right_res == output_set[i]) break;
					i++;
				}
				cout << "Ok, I am repairing system!";
				BackPropagation(i, 0.15);
				SaveWeights();
			}
		}
	}

}
