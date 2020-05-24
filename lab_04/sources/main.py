import table
import matplotlib.pyplot as plt
import numpy as np

FILE_TABLE = 'table.txt'

def main():
    n = int(input("Введите степень полинома n: ")) + 1
    table_weight = table.read_table_weight(FILE_TABLE)
    print_table(table_weight)
    least_square_coeff = least_square(table_weight, n)

    print("Coeffs:")
    print(least_square_coeff)
    output_graphic(least_square_coeff, table_weight)

    return

def func(x):
    return x ** (1/2)

def approxim_func(coeffs, x):
    result = coeffs[0]
    for i in range(1, len(coeffs)):
        result += coeffs[i] * x
        x *= x
    return result

def least_square(table_weight, n):
    lenght = len(table_weight)

    matrix = []
    for i in range (n):
        current_row = []
        for m in range(n):
            current_sum = 0
            for j in range (lenght):
                current_sum += table_weight[j][2] * (table_weight[j][0] ** (i + m))
            current_row.append(current_sum)

        current_right = 0
        for j in range(lenght):
            current_right += table_weight[j][2] * table_weight[j][1] * (table_weight[j][0] ** i)
        current_row.append(current_right)
        matrix.append(current_row)
    
    return gauss(matrix)

def gauss(matrix):
    n = len(matrix)

    for i in range(0, n):
        # Search for maximum in this column
        maxEl = abs(matrix[i][i])
        maxRow = i
        for k in range(i + 1, n):
            if abs(matrix[k][i]) > maxEl:
                maxEl = abs(matrix[k][i])
                maxRow = k

        # Swap maximum row with current row
        for k in range(i, n + 1):
            tmp = matrix[maxRow][k]
            matrix[maxRow][k] = matrix[i][k]
            matrix[i][k] = tmp

        # Make all rows below this one 0 in current column
        for k in range(i + 1, n):
            c = -matrix[k][i] / matrix[i][i]
            for j in range(i, n + 1):
                if i == j:
                    matrix[k][j] = 0
                else:
                    matrix[k][j] += c * matrix[i][j]

    # Solve equation Ax=b for an upper triangular matrix matrix
    x = [0 for i in range(n)]
    for i in range(n - 1, -1, -1):
        x[i] = matrix[i][n] / matrix[i][i]
        for k in range(i - 1, -1, -1):
            matrix[k][n] -= matrix[k][i] * x[i]
    return x

def output_graphic(coeffs, table_weight):
    t = np.arange(-1.0 + min(table_weight, key=lambda x: x[0])[0],
                  5.0 + max(table_weight, key=lambda x: x[0])[0], 0.02)
    plt.figure(1)
    plt.ylabel("y")
    plt.xlabel("x")
    plt.plot(t, f(t, coeffs), 'k')
    for i in range(len(table_weight)):
        plt.plot(table_weight[i][0], table_weight[i][1], 'ro')
    plt.show()

def print_table(table_weight):
    length = len(table_weight)
    print("x      y      weight")
    for i in range(length):
        print("%.4f %.4f %.4f" % (table_weight[i][0], table_weight[i][1], table_weight[i][2]))
    print()

def f(x_arr, coeff):
    res = np.zeros(len(x_arr))
    for i in range(len(coeff)):
        res += coeff[i]*(x_arr**i)
    return res  
    
main()

'''
 n = len(matrix)
    # приводим к треугольному виду
    for k in range(n):
        for i in range(k + 1, n):
            coeff = -(matrix[i][k] / matrix[k][k])
            for j in range(k, n + 1):
                matrix[i][j] += coeff * matrix[k][j]

    # Solve equation Ax=b for an upper triangular matrix matrix
    a = [0 for i in range(n)]
    for i in range(n - 1, -1, -1):
        for j in range(n - 1, i, -1):
            matrix[i][n] -= a[j] * matrix[i][j]
        a[i] = matrix[i][n] / matrix[i][i]
    return a
    '''