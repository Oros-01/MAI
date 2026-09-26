import numpy as np

def construct_matrix(first_array, second_array):
    return np.vstack([first_array, second_array]).T

a1 = (1,2,6,6)
a2 = (9,56,4,3)

print (construct_matrix(a1,a2))