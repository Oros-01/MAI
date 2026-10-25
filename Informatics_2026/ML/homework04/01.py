import numpy as np

def gini(y: np.ndarray) -> float:
    """
    Computes Gini index for given set of labels
    :param y: labels
    :return: Gini impurity
    """

    _, counts = np.unique(y, return_counts=True)

    probabilities = counts / len(y)

    gini_index = 1.0 - np.sum(probabilities ** 2)


    
    return gini_index