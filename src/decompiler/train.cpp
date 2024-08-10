#include "model.h"

void main() {
	int epochs = 5;
	batch_size = 1000;

	Model model(1,1);
	model.train(epoch, batch_size, X_train, y_train);
}
