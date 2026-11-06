import enum
import typing as tp

import numpy as np
from scipy.stats import mode


class NodeType(enum.Enum):
    REGULAR = 1
    TERMINAL = 2


def gini(y: np.ndarray) -> float:
    _, counts = np.unique(y, return_counts=True)
    probabilities = counts / len(y)
    gini = 1.0 - np.sum(probabilities ** 2)
    return gini


def weighted_impurity(y_left: np.ndarray, y_right: np.ndarray) -> \
        tp.Tuple[float, float, float]:
    n_left = len(y_left)
    n_right = len(y_right)
    left_impurity = gini(y_left)
    right_impurity = gini(y_right)
    weighted_impurity = (n_left * left_impurity + n_right * right_impurity) / (n_left + n_right)
    return weighted_impurity, left_impurity, right_impurity


def create_split(feature_values: np.ndarray, threshold: float) -> tp.Tuple[np.ndarray, np.ndarray]:
    left_idx = np.where(feature_values <= threshold)[0]
    right_idx = np.where(feature_values > threshold)[0]
    return left_idx, right_idx


class MyDecisionTreeNode:
    def __init__(
            self,
            meta,
            depth,
            node_type: NodeType = NodeType.REGULAR,
            predicted_class: tp.Optional[tp.Union[int, str]] = None,
            left_subtree: tp.Optional['MyDecisionTreeNode'] = None,
            right_subtree: tp.Optional['MyDecisionTreeNode'] = None,
            feature_id: int = None,
            threshold: float = None,
            impurity: float = np.inf
    ):
        self._node_type = node_type
        self._meta = meta
        self._depth = depth
        self._predicted_class = predicted_class
        self._class_proba = None
        self._left_subtree = left_subtree
        self._right_subtree = right_subtree
        self._feature_id = feature_id
        self._threshold = threshold
        self._impurity = impurity

    def _best_split(self, X: np.ndarray, y: np.ndarray):
        lowest_impurity = np.inf
        best_feature_id = None
        best_threshold = None
        lowest_left_child_impurity, lowest_right_child_impurity = None, None
        features = self._meta.rng.permutation(X.shape[1])
        for feature in features:
            current_feature_values = X[:, feature]
            thresholds = np.unique(current_feature_values)
            for threshold in thresholds:
                left_idx, right_idx = create_split(current_feature_values, threshold)
                if len(left_idx) == 0 or len(right_idx) == 0:
                    continue
                current_weighted_impurity, current_left_impurity, current_right_impurity = \
                    weighted_impurity(y[left_idx], y[right_idx])
                if current_weighted_impurity < lowest_impurity:
                    lowest_impurity = current_weighted_impurity
                    best_feature_id = feature
                    best_threshold = threshold
                    lowest_left_child_impurity = current_left_impurity
                    lowest_right_child_impurity = current_right_impurity

        return best_feature_id, best_threshold, lowest_left_child_impurity, lowest_right_child_impurity

    def fit(self, X: np.ndarray, y: np.ndarray):
        if (
                self._depth >= self._meta.max_depth
                or len(y) < self._meta.min_samples_split
                or len(np.unique(y)) == 1
        ):
            self._node_type = NodeType.TERMINAL
            _classes, _counts = np.unique(y, return_counts=True)
            self._predicted_class = self._meta.rng.choice(_classes[_counts == _counts.max()])
            self._class_proba = np.zeros(self._meta._n_classes)
            classes, counts = np.unique(y, return_counts=True)
            self._class_proba[classes.astype(int)] = counts / counts.sum()
            return self

        self._feature_id, self._threshold, left_imp, right_imp = self._best_split(X, y)
        left_idx, right_idx = create_split(X[:, self._feature_id], self._threshold)
        self._left_subtree = MyDecisionTreeNode(
            meta=self._meta,
            depth=self._depth + 1,
            impurity=left_imp
        ).fit(X[left_idx], y[left_idx])
        self._right_subtree = MyDecisionTreeNode(
            meta=self._meta,
            depth=self._depth + 1,
            impurity=right_imp
        ).fit(X[right_idx], y[right_idx])
        return self

    def predict(self, x: np.ndarray):
        if self._node_type is NodeType.TERMINAL:
            return self._predicted_class
        if x[self._feature_id] <= self._threshold:
            return self._left_subtree.predict(x)
        return self._right_subtree.predict(x)

    def predict_proba(self, x: np.ndarray):
        if self._node_type is NodeType.TERMINAL:
            return self._class_proba
        if x[self._feature_id] <= self._threshold:
            return self._left_subtree.predict_proba(x)
        return self._right_subtree.predict_proba(x)


class MyDecisionTreeClassifier:
    def __init__(
            self,
            max_depth: tp.Optional[int] = None,
            min_samples_split: tp.Optional[int] = 2,
            seed: int = 0
    ):
        self.root = MyDecisionTreeNode(self, 1)
        self._is_trained = False
        self.max_depth = max_depth or np.inf
        self.min_samples_split = min_samples_split or 2
        self.rng = np.random.default_rng(seed)
        self._n_classes = 0

    def fit(self, X: np.ndarray, y: np.ndarray):
        self._n_classes = int(np.max(y)) + 1
        self.root = MyDecisionTreeNode(self, 1)
        self.root.fit(X, y)
        self._is_trained = True
        return self

    def predict(self, X: np.ndarray) -> np.ndarray:
        if not self._is_trained:
            raise RuntimeError('predict call on untrained model')
        X = np.asarray(X)
        if X.ndim == 1:
            X = X.reshape(1, -1)
        return np.array([self.root.predict(x) for x in X])

    def predict_proba(self, X: np.ndarray) -> np.ndarray:
        if not self._is_trained:
            raise RuntimeError('predict call on untrained model')
        X = np.asarray(X)
        if X.ndim == 1:
            X = X.reshape(1, -1)
        return np.array([self.root.predict_proba(x) for x in X])


class MyRandomForestClassifier:
    def __init__(
            self,
            n_estimators=10,
            max_depth=None,
            min_samples_split=2,
            seed=0
    ):
        self.n_estimators = n_estimators
        self.max_depth = max_depth
        self.min_samples_split = min_samples_split
        self.rng = np.random.default_rng(seed)
        self.estimators = [
            MyDecisionTreeClassifier(
                max_depth=max_depth,
                min_samples_split=min_samples_split,
                seed=int(self.rng.integers(0, 2**31 - 1))
            )
            for _ in range(n_estimators)
        ]
        self._n_classes = 0
        self._is_trained = False

    def _bootstrap_sample(self, X, y):
        n_samples = X.shape[0]
        indices = self.rng.integers(0, n_samples, size=n_samples)
        return X[indices], y[indices]

    def fit(self, X, y):
        self._n_classes = int(np.max(y)) + 1
        for estimator in self.estimators:
            X_boot, y_boot = self._bootstrap_sample(X, y)
            estimator.fit(X_boot, y_boot)
        self._is_trained = True
        return self

    def predict_proba(self, X):
        if not self._is_trained:
            raise RuntimeError('predict call on untrained model')
        X = np.asarray(X)
        if X.ndim == 1:
            X = X.reshape(1, -1)
        probas = np.zeros((X.shape[0], self._n_classes))
        for estimator in self.estimators:
            probas += estimator.predict_proba(X)
        probas /= len(self.estimators)
        return probas

    def predict(self, X):
        probas = self.predict_proba(X)
        return np.argmax(probas, axis=1)