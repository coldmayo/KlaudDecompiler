import numpy as np
import os
from tqdm import tqdm

def gen_bin(code, num):
    f = open("ccode/"+str(num)+".c", "w+")
    f.write(code)
    f.close()

    os.system("cd ccode ; gcc -o " + str(num) + " "+str(num)+".c")

def main():

    ops = [
        " + ",
        " - ",
        " / ",
        " * "
    ]
    
    minLines = 0

    for i in tqdm(range(5)):
        print(i)
        add = np.random.randint(0, 4)
        numLines = minLines+add

        code = ""
        code += "int main() {\n"

        if numLines > 0:
            for j in range(numLines):
                if j == 0:
                    code += "\tint i;\n "
                else:
                    oneLine = "\ti = "
                    oneLine += str(np.random.randint(0, 10))
                    oneLine += np.random.choice(ops)
                    oneLine += str(np.random.randint(0, 10))
                    code += oneLine + ";\n "
        if numLines > 1:
            code += "\treturn i; \n}"
        else:
            code += "\treturn 0; \n}"

        gen_bin(code, i)

if __name__ == "__main__":
    main()
