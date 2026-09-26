import numpy as np
import pandas as pd
from sklearn.base import ClassifierMixin


class RubricCityMedianClassifier(ClassifierMixin):
    def fit(self, X, y):
        
        self.medians_ = pd.DataFrame({
            'rubric': X['modified_rubrics'],
            'city': X['city'],
            'y': y
        }).groupby(['rubric', 'city'])['y'].median()
        self.global_median_ = y.median()
        return self

    def predict(self, X):



        keys = list(zip(X['modified_rubrics'], X['city']))
        preds = self.medians_.reindex(keys).fillna(self.global_median_).values
        return preds
