#include <stdlib.h>

using namespace std;

class Model {
	public:
		Model(int input_size, int output_size);
		std::vector<std::vector<float>> he_init(int n_in, int n_out);
    	std::vector<float> zeros(int len);
    	std::vector<float> forward(const std::vector<float>& X);
    	void backward(vector<float>& X, vector<float>& y, int alpha);
    	float loss(vector<float>& predicted, vector<float>& actual);
    	void train(vector<float>& X_train, vector<float>& y_train, int epoch, int batch_size, int alpha);
	private:
    	int inp, hid, out;
		vector<float> wei_1, b_1, wei_2, b_2, wei_3, b_3;
};
