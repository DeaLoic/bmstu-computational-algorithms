from math import *

UNDEFINED = 'undefined' 

def right_diff(table, step, index):
    if index < len(table) - 1 and index >= 0:
        return (table[index + 1] - table[index]) / step

    return UNDEFINED

def left_diff(table, step, index):
    if index < len(table) and index >= 1:
        return (table[index] - table[index - 1]) / step

    return UNDEFINED

def center_diff(table, step, index):
    if index >= 1 and index < len(table) - 1:
        return (table[index + 1] - table[index - 1] / 2 / step)
        
    return UNDEFINED

def second_diff(table, step, index):
    if index >= 1 and index < len(table) - 1:
        return (table[index - 1] - 2 * table[index] + table[index + 1]) / pow(step, 2)
    
    return UNDEFINED
        
def second_runge(table, step, index):
    if index >= 2:
        return 2 * left_diff(table, step, index) - ((table[index] - table[index - 2]) / 2 / step)
        
    return UNDEFINED

def align_vars_diff(Ys, Xs, step, index):
    if index <= len(Ys) - 2:
        eta_ksi_diff = (1 / Ys[index + 1] - 1 / Ys[index]) / (1 / Xs[index + 1] - 1 / Xs[index])
        y = Ys[index]
        x = Xs[index]
        return eta_ksi_diff * y * y / x / x

    return UNDEFINED

