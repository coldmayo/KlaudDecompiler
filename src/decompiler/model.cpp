#include <cmath>
#include <vector>

#include "utils.h"
#include "model.h"

using namespace std;

Model :: Model(int input_size, int output_size) {
	inp = input_size;
	out = output_size;
	hid = 20;
}

vector<float> Model :: he_nit(int n_in, int n_out) {

	vector<float> w(n_in*n_out);

	float stddev = sqrt(2/n_in);

	for (int i = 0; i < n_in*n_out; i++) {
		w[i] = randint(n_in, n_out) * stddev;
	}

	return w;
}

vector<float> Model :: zeros(int len) {
	return vector<float>(len, 0.0f);
}

vector<float> Model :: forward(vector<float>X) {
	vector<float>z1 = dot(X, wei_1) + b_1;
	vector<float>a1 = ReLU(z1, len(z1));

	vector<float>z2 = dot(a1, wei_2) + b_2;
	vector<float>a2 = ReLU(z2, len(z2));

	vector<float>z3 = dot(a2, wei_3) + b_3;
	vector<float>a3 = softmax(z3, len(z3));

	return a3;
}

void Model :: backward(vector<float>& X, int alpha) {
	vector<float>z1 = dot(X, wei_1) + b_1;
	vector<float>a1 = ReLU(z1, len(z1));

	vector<float>z2 = dot(a1, wei_2) + b_2;
	vector<float>a2 = ReLU(z2, len(z2));

	vector<float>z3 = dot(a2, wei_3) + b_3;
	vector<float>a3 = softmax(z3, len(z3));

	vector<float>d3 = subtract(a3, y);
	vector<float>d2 = elementwiseMultiply(dot(d3, transpose(wei_3)), dReLU(a2));
	vector<float>d1 = elementwiseMultiply(dot(d2, transpose(wei_2)), dReLU(a1));

	vector<float>dw3 = dot(transpose(a2), d3);
	vector<float>dw2 = dot(transpose(a1), d2);
	vector<float>dw1 = dot(transpose(X), d1);

	wei_1 = subtract(wei_1, scalarMultiply(dw1, alpha));
	wei_2 = subtract(wei_2, scalarMultiply(dw2, alpha));
	wei_3 = subtract(wei_3, scalarMultiply(dw3, alpha));

	b_1 = subtract(b_1, scalarMultiply(sum(d1), alpha));
	b_2 = subtract(b_2, scalarMultiply(sum(d2), alpha));
	b_3 = subtract(b_3, scalarMultiply(sum(d3), alpha));

}

float loss(vector<float>& predicted, vector<float>& actual) {
    float loss = 0.0f;
    for (size_t i = 0; i < actual.size(); ++i) {
        loss -= actual[i] * log(predicted[i] + 1e-9f);
    }
    return loss;
}

vector<float> Model :: predict(vector<float>& X) {
	return forward(X);
}

void Model :: train(int epoch, int batch_size, vector<float>& X_train, vector<float>& y_train) {
	wei_1 = he_init(inp, hid);
	wei_2 = he_init(hid, hid);
	wei_3 = he_init(hid, out);

	b_1 = zeros(inp);
	b_2 = zeros(hid);
	b_3 = zeros(out);

	int samples = X_train.size();
	int batches = samples/batch_size;

	for (int i = 0; i < epoch; i++) {
		cout << "Epoch: " << i << endl;
		out = forward(X_train);
		cout << loss(out, y) << endl;
		backward(X_train);
	}
}
