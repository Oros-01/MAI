import numpy as np
from sklearn.base import RegressorMixin

class MeanRegressor(RegressorMixin):
    def __init__(self):
        self.mean_ = None



    def fit(self, X=None, y=None):
        self.mean_ = float(np.mean(y))
        return self

    

    def predict(self, X=None):
        if X is None:
            return np.array([self.mean_])

        n_samples = np.asarray(X).shape[0]
        
        return np.full(n_samples, self.mean_)