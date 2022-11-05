def read_table_weight(filename):
    table_weight = []

    with open(filename, 'r') as f:
        for line in f:
            table_weight.append(list(map(float, line.strip().split())))
    
    return table_weight