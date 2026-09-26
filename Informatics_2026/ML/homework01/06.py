import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split

from sklearn.base import ClassifierMixin




class MostFrequentClassifier(ClassifierMixin):
    def fit(self, X=None, y=None):
        values, counts = np.unique(y, return_counts=True)
        self.most_frequent_ = values[np.argmax(counts)]
        return self

    def predict(self, X=None):
        if X is None:
            return np.array([self.most_frequent_])
        return np.full(X.shape[0], self.most_frequent_)