import pandas as pd
import matplotlib.pyplot as plt
import os

df = pd.read_csv("results.csv", sep=";")
os.makedirs("graphs", exist_ok=True)

data_types = df["DataType"].unique()
algorithms = df["Algorithm"].unique()

# стили для каждого алгоритма — разный цвет, маркер и тип линии
# чтобы совпадающие линии всё равно были различимы
styles = {
    "classic":  {"color": "blue",  "marker": "o", "linestyle": "-"},
    "flag":     {"color": "orange","marker": "s", "linestyle": "--"},
    "boundary": {"color": "green", "marker": "^", "linestyle": ":"},
}

# --- График 1: время работы ---
for dtype in data_types:
    plt.figure(figsize=(10, 6))
    for algo in algorithms:
        subset = df[(df["DataType"] == dtype) & (df["Algorithm"] == algo)]
        plt.plot(subset["Size"], subset["Time_us"],
                 label=algo, **styles[algo])  # ** - распаковывает словарь в аргументы функции
    plt.title(f"Время работы — {dtype}")
    plt.xlabel("Размер массива (n)")
    plt.ylabel("Время (микросекунды)")
    plt.legend()
    plt.grid(True)
    plt.savefig(f"graphs/time_{dtype}.png")
    plt.close()

print("Графики времени сохранены")

# --- График 2: количество сравнений ---
for dtype in data_types:
    plt.figure(figsize=(10, 6))
    for algo in algorithms:
        subset = df[(df["DataType"] == dtype) & (df["Algorithm"] == algo)]
        plt.plot(subset["Size"], subset["Comparisons"],
                 label=algo, **styles[algo])
    plt.title(f"Количество сравнений — {dtype}")
    plt.xlabel("Размер массива (n)")
    plt.ylabel("Сравнения")
    plt.legend()
    plt.grid(True)
    plt.savefig(f"graphs/comparisons_{dtype}.png")
    plt.close()

print("Графики сравнений сохранены")

# --- График 3: количество обменов ---
for dtype in data_types:
    plt.figure(figsize=(10, 6))
    for algo in algorithms:
        subset = df[(df["DataType"] == dtype) & (df["Algorithm"] == algo)]
        plt.plot(subset["Size"], subset["Swaps"],
                 label=algo, **styles[algo])
    plt.title(f"Количество обменов — {dtype}")
    plt.xlabel("Размер массива (n)")
    plt.ylabel("Обмены")
    plt.legend()
    plt.grid(True)
    plt.savefig(f"graphs/swaps_{dtype}.png")
    plt.close()

print("Графики обменов сохранены")