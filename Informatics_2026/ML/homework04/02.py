import numpy as np
import typing as tp

def gini(y: np.ndarray) -> float:
    """
    Computes Gini index for given set of labels
    :param y: labels
    :return: Gini impurity
    """
    _, counts = np.unique(y, return_counts=True)
    probs = counts / len(y)
    return 1.0 - np.sum(probs ** 2)





def weighted_impurity(y_left: np.ndarray, y_right: np.ndarray) -> \
        tp.Tuple[float, float, float]:
    """
    Computes weighted impurity by averaging children impurities
    :param y_left: left  partition
    :param y_right: right partition
    :return: averaged impurity, left child impurity, right child impurity
    """
    n_left = len(y_left)
    n_right = len(y_right)
    n_total = n_left + n_right
    
    imp_left = gini(y_left)
    imp_right = gini(y_right)
    
    weighted_imp = (n_left / n_total) * imp_left + (n_right / n_total) * imp_right

    
    return weighted_imp, imp_left, imp_right