import os
import pandas as pd
from tqdm import tqdm

toks = {
    "main()": 300,
    "int": 301,
    "return": 302,
    " + ": 303,
    " - ": 304,
    " * ": 305,
    " / ": 306,
    " = ": 307
}

def token_c(lenth, fileNum):
    f = open("ccode/"+str(fileNum)+".c", "r")
    code = f.read()

    seps = code.split()

    tokens = []

    for i in seps:
        if i in toks:
            tokens.append(toks[i])
        else:
            if i[-1] == ';':
                tokens.append(ord(i[:-1]))
            else:
                tokens.append(ord(i))

    tokens += (lenth - len(tokens)) * [0]

    return tokens

def main():

    data = {}

    for i in tqdm(range(int(len(os.listdir("ccode"))/2))):

        bytess = []

        with open("ccode/"+str(i), "rb") as file:
            while True:
                byte = file.read(1)
                if not byte:
                    break
                bytess.append(int(byte.hex(), 16))

        tokens = token_c(len(bytess), i)

        if len(tokens) == len(bytess):
            data["X_"+str(i)] = bytess
            data["Y_"+str(i)] = tokens

    df = pd.DataFrame(data)
    df.to_csv('data.csv', index=False)

if __name__ == "__main__":
    main()
