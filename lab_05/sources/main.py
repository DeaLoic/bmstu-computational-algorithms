from math import pi, sin, cos, exp
from numpy.polynomial.legendre import leggauss
from numpy import arange
import matplotlib.pyplot as plt

def func_form(param):
    subfunc = lambda x, y: 2 * cos(x) / (1 - (sin(x) ** 2) * (cos(y) ** 2))
    func =    lambda x, y: (4 / pi) * (1 - exp(-param * subfunc(x, y))) * cos(x) * sin(x)
    return func

def pin_x(func, x):
    return lambda y: func(x, y)

# limits = ([a, b], [c, d])
def two_integrate(N, M, limits, func):
    first = lambda x: simpson(pin_x(func, x), limits[0][0], limits[0][1], N)
    return gauss_quadrature(first, limits[1][0], limits[1][1], M)

def simpson(func, a, b, nodes_count):

    h = (b - a) / (nodes_count - 1)
    x = a
    res = 0

    for i in range((nodes_count - 1) // 2):
        res += func(x) + 4 * func(x + h) + func(x + 2 * h)
        x += 2 * h

    return (h / 3) * res


def gauss_x_transform(t, a, b):
    return (b + a) / 2 + (b - a) / 2 * t


def gauss_quadrature(func, a, b, nodes_count):
    args, coeffs = leggauss(nodes_count)  # Computes the sample points and weights for Gauss-Legendre quadrature
    res = 0

    for i in range(nodes_count):
        res += (b - a) / 2 * coeffs[i] * func(gauss_x_transform(args[i], a, b))

    return res


def form_graph(func, ar_params, label):
    X = list()
    Y = list()
    for t in arange(ar_params[0], ar_params[1] + ar_params[2], ar_params[2]):
        X.append(t)
        Y.append(func(t))
    plt.plot(X, Y, label=label)

if (__name__ == "__main__"):
    end = '1'
    while (end != '0'):
        N, M = list(map(int, input("Input N and M: ").strip().split()))
        print(N, M)

        limits = [[0, pi / 2], [0, pi / 2]]
        tau = float(input("Enter tau: "))

        func_res = lambda tau: two_integrate(N, M, limits, func_form(tau))

        print("Calculated value: ", two_integrate(N, M, limits, func_form(tau)))
        print()
        form_graph(func_res, [0.05, 10, 0.05], "N = " + str(N) + ", M = " + str(M))
        end = input()
    plt.legend()
    plt.ylabel("Result")
    plt.xlabel("Tau")
    plt.show()

    