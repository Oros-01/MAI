import numpy as np
import pandas as pd
from sklearn.base import RegressorMixin



class CityMeanRegressor(RegressorMixin):
    def fit(self, X, y):
        self.means_ = pd.DataFrame({
            'city': X['city'],
            'y': y
        }).groupby('city')['y'].mean()
        
        self.global_mean_ = y.mean()
        return self

    def predict(self, X):

        preds = X['city'].map(self.means_)
        preds = preds.fillna(self.global_mean_)
        
        return preds.values