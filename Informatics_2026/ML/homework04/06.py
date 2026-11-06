import sklearn
import sklearn.model_selection
import numpy as np


class ExperimentHandler:
    """This class perfoms experiments with given model, measures metrics and logs everything for thorough comparison"""
    stacking_prediction_filename = 'cv_stacking_prediction.npy'
    test_stacking_prediction_filename = 'test_stacking_prediction.npy'


def score_cv(self, estimator, metrics, run):
    """
    computes scores on cross-validation
    :param estimator: estimator to fit
    :param metrics: metrics to compute
    :param run: run to log to
    :return: None
    """
    cross_val_results = sklearn.model_selection.cross_validate(
        estimator,
        self.X_train,
        self.y_train,
        cv=self._cv_iterable,
        scoring=metrics,
        n_jobs=self._n_jobs,
    )
    for key, value in cross_val_results.items():
        if key.startswith('test_'):
            metric_name = key.split('_', maxsplit=1)[1]
            mean_score = np.mean(value)
            std_score = np.std(value)
            run.log_values(
                {
                    metric_name + '_mean': mean_score,
                    metric_name + '_std': std_score
                }
            )


def generate_stacking_predictions(self, estimator, run):
    """
    generates predictions over cross-validation folds, then saves them as artifacts
    returns fitted estimator for convinience and les train overhead
    :param estimator: estimator to use
    :param run: run to log to
    :return: estimator fitted on train, stacking cross-val predictions, stacking test predictions
    """
    if hasattr(estimator, "predict_proba"):
        method = "predict_proba"
    elif hasattr(estimator, "decision_function"):
        method = "decision_function"
    else:
        method = "predict"
    cross_val_stacking_prediction = sklearn.model_selection.cross_val_predict(
        estimator,
        self.X_train,
        self.y_train,
        cv=self._cv_iterable,
        method=method,
        n_jobs=self._n_jobs,
    )
    run.log_artifact(ExperimentHandler.stacking_prediction_filename,
                     lambda file: np.save(file, cross_val_stacking_prediction))
    estimator.fit(self.X_train, self.y_train)
    test_stacking_prediction = getattr(estimator, method)(self.X_test)
    run.log_artifact(ExperimentHandler.test_stacking_prediction_filename,
                     lambda file: np.save(file, test_stacking_prediction))
    return estimator, cross_val_stacking_prediction, test_stacking_prediction