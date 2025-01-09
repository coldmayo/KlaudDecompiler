from tqdm import tqdm
import numpy as np
from Adam import *
import pandas as pd
# this is code for a Recurrent Neural Network

def pad_sequences(sequences, max_len=None, pad_value=0):
    if max_len is None:
        max_len = max(len(seq) for seq in sequences)
    padded = np.full((len(sequences), max_len), pad_value, dtype=np.float32)
    for i, seq in enumerate(sequences):
        padded[i, :len(seq)] = seq
    return padded

def one_hot(y):
    y = pd.Series(y)
    y = pd.get_dummies(y).values.tolist()
    return np.array(y)
import numpy as np

def he_init(n_in, n_out):
    # He initialization for weights
    stdev = np.sqrt(2 / n_in)
    return np.random.randn(n_in, n_out) * stdev

def ReLU(x):
    return np.maximum(0, x)

def softmax(x):
    e_x = np.exp(x - np.max(x, axis=-1, keepdims=True))
    return e_x / np.sum(e_x, axis=-1, keepdims=True)

class RNN:
    def __init__(self, input_sz, hidden_sz, output_sz, w1=None, w2=None, w3=None, b1=None, b3=None):
        # Set the sizes for input, hidden, and output layers
        self.input_sz = input_sz
        self.hidden_sz = hidden_sz
        self.output_sz = output_sz

        # Initialize weights using He initialization
        self.w1 = he_init(input_sz, hidden_sz) if w1 is None else np.array(w1)  # input to hidden weights
        self.w2 = he_init(hidden_sz, hidden_sz) if w2 is None else np.array(w2)  # hidden to hidden weights
        self.w3 = he_init(hidden_sz, output_sz) if w3 is None else np.array(w3)  # hidden to output weights

        # Initialize biases
        self.b1 = np.zeros(hidden_sz) if b1 is None else np.array(b1)  # hidden biases
        self.b3 = np.zeros(output_sz) if b3 is None else np.array(b3)  # output biases

    def forward(self, x):
        seq_len = x.shape[0]
        h = np.zeros((seq_len + 1, self.hidden_sz))
        y_pred = np.zeros((seq_len, self.output_sz))

        for t in range(seq_len):
            
            h[t + 1] = ReLU(np.dot(x[t], self.w1) + np.dot(h[t], self.w2) + self.b1)

            z_out = np.dot(h[t + 1], self.w3) + self.b3
            y_pred[t] = softmax(z_out)
        return y_pred

    def backprop(self, x, y, lr=0.001):
        # Backpropagation through time
        grads = {
            "w1": np.zeros_like(self.w1),
            "w2": np.zeros_like(self.w2),
            "w3": np.zeros_like(self.w3),
            "b1": np.zeros_like(self.b1),
            "b3": np.zeros_like(self.b3)
        }

        del_t = x.shape[0]
        h = np.zeros((del_t + 1, self.hidden_sz))
        del_h_next = np.zeros(self.hidden_sz)

        # Forward pass to compute the hidden states
        for t in range(del_t):
            h[t + 1] = ReLU(np.dot(x[t], self.w1) + np.dot(h[t], self.w2) + self.b1)

        z_out = np.dot(h[-1], self.w3) + self.b3
        y_pred = softmax(z_out)  # Predicted output
        print(x.shape, y.shape)
        print(y_pred.shape, y.shape)
        del_out = y_pred - y  # Derivative of the loss w.r.t the output

        # Compute gradients for w3 and b3
        grads["w3"] = np.outer(h[-1], del_out)
        grads["b3"] = del_out

        # Backpropagate the error through the hidden layers
        print(del_out.shape, self.w3.shape)
        for t in reversed(range(del_t)):
            del_h = np.dot(del_out, self.w3.T) + del_h_next
            del_h *= (h[t + 1] > 0)  # Derivative of ReLU

            print(x[t].shape, del_h.shape, x.shape, y.shape)
            grads["w1"] += np.outer(x[t], del_h)
            grads["w2"] += np.outer(h[t], del_h)
            grads["b1"] += del_h

            del_h_next = np.dot(del_h, self.w2.T)

        # Average gradients over all time steps
        grads["w1"] /= del_t
        grads["w2"] /= del_t
        grads["b1"] /= del_t

        # Update weights and biases using gradient descent
        self.w1 -= lr * grads["w1"]
        self.w2 -= lr * grads["w2"]
        self.w3 -= lr * grads["w3"]
        self.b1 -= lr * grads["b1"]
        self.b3 -= lr * grads["b3"]

    def accuracy(self, y, preds):
        # Compute accuracy
        correct = np.sum(np.argmax(y, axis=1) == np.argmax(preds, axis=1))
        return correct / len(y)

    def predict(self, x):
        # Make predictions using the trained model
        out = self.forward(x)
        return out

    def train(self, x, y, epoch):
        # Training the RNN for the specified number of epochs
        X = np.array(x)
        Y = np.array(y)  # Ensure y is a numpy array
        print("Start Training: ")
        for i in range(epoch):
            self.forward(X)
            self.backprop(X,Y)
            print(f"Epoch {epoch + 1}/{epochs} completed.")
        y_preds = self.predict(X)
        print("Model accuracy: ", self.accuracy(y, y_preds))
        return self.w1, self.w2, self.w3, self.b1, self.b3
