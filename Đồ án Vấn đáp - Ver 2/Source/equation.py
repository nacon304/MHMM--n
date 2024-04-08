import random
import numpy as np
import sympy as sp

def calculate_f(A_hex, X):
    numbers = [int(h, 16) for h in A_hex]
    x = sp.symbols('x')
    
    polynomial = sum(coef * (x ** exp) for exp, coef in enumerate(numbers))
    res = [hex(int(polynomial.subs(x, x_val)))[2:] for x_val in X]
    
    return res

def gen_x(crts):
    X = random.sample(range(1, 1000), len(crts))

    Y_hex = calculate_f(crts, X)
    with open("../Key/res.txt", "w") as file:
        for y in Y_hex:
            file.write(y + "\n")

    X_hex = [hex(x)[2:] for x in X]

    return X_hex