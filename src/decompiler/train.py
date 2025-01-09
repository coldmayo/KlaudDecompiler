import numpy as np
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
from rnn import *
import pandas as pd
import sys
from keras.utils import to_categorical
from keras.preprocessing.sequence import pad_sequences

def main(argv, argc):
    epochs = 3
    
    if argc == 2:
        epochs = int(argv[1])

    df = pd.read_csv("data.csv")

    X = []
    Y = []
    x = 0
    for i in df.columns.tolist():
        if x % 2 == 0:
            X.append(df[i].tolist())
        else:
            Y.append(df[i].tolist())
        x += 1
    model = RNN(len(X[0]), 10, len(X[0]))
    model.train(X, Y, epochs)

if __name__ == '__main__':
    main(sys.argv, len(sys.argv))
