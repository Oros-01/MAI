import numpy as np
from importlib import import_module

# Импортируем всё из 01.py (файл называется 01 — модуль тоже 01)
mod = import_module("01")

# 1) Маленький синтетический датасет: 3 класса, 100 объектов, 4 признака
rng = np.random.default_rng(0)
X = rng.normal(size=(100, 4))
y = rng.integers(0, 3, size=100)

# 2) Обучаем лес из 100 деревьев (как в задании)
forest = mod.MyRandomForestClassifier(n_estimators=100, max_depth=5, seed=42)
forest.fit(X, y)

# 3) Проверяем предсказания
proba = forest.predict_proba(X)
preds = forest.predict(X)

print("Форма вероятностей:", proba.shape)   # ожидаем (100, 3)
print("Форма предсказаний:", preds.shape)   # ожидаем (100,)
print("Сумма вероятностей по строке (должна быть ~1):", proba.sum(axis=1)[:5])
print("Точность на трейне:", (preds == y).mean())