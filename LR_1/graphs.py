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











from graphviz import Digraph

# ===== Блок-схема 1: классический пузырёк =====
# ===== Блок-схема 1: классический пузырёк =====
g1 = Digraph("classic_bubble", format="png")
g1.attr(rankdir="TB", splines="ortho")
g1.attr("node", fontname="Arial", fontsize="11")

g1.node("start",    "Nachalo",                   shape="ellipse")
g1.node("i_init",   "i = 0",                     shape="box")
g1.node("i_check",  "i < size - 1?",             shape="diamond")
g1.node("j_init",   "j = 0",                     shape="box")
g1.node("j_check",  "j < size - 1?",             shape="diamond")
g1.node("comps",    "comps++",                   shape="box")
g1.node("cmp_check","arr[j] > arr[j+1]?",        shape="diamond")
g1.node("swap",     "swap(arr[j], arr[j+1])\nswaps++", shape="box")
g1.node("j_inc",    "j++",                       shape="box")
g1.node("i_inc",    "i++",                       shape="box")
g1.node("end",      "Konec",                     shape="ellipse")

g1.edge("start",     "i_init")
g1.edge("i_init",    "i_check")
g1.edge("i_check",   "j_init",   label="da")
g1.edge("i_check",   "end",      label="net")
g1.edge("j_init",    "j_check")
g1.edge("j_check",   "comps",    label="da")
g1.edge("j_check",   "i_inc",    label="net")
g1.edge("comps",     "cmp_check")
g1.edge("cmp_check", "swap",     label="da")
g1.edge("cmp_check", "j_inc",    label="net")
g1.edge("swap",      "j_inc")
g1.edge("j_inc",     "j_check")
g1.edge("i_inc",     "i_check")

g1.render("graphs/flowchart_classic", cleanup=True)
print("Blok-shema classic gotova")

# ===== Блок-схема 2: пузырёк с флагом =====
g2 = Digraph("flag_bubble", format="png")
g2.attr(rankdir="TB", splines="ortho")
g2.attr("node", fontname="Arial", fontsize="11")

g2.node("start",     "Nachalo",                      shape="ellipse")
g2.node("i_init",    "i = 0",                        shape="box")
g2.node("i_check",   "i < size - 1?",                shape="diamond")
g2.node("flag_init", "swapped = false",              shape="box")
g2.node("j_init",    "j = 0",                        shape="box")
g2.node("j_check",   "j < size - 1 - i?",            shape="diamond")
g2.node("comps",     "comps++",                      shape="box")
g2.node("cmp_check", "arr[j] > arr[j+1]?",           shape="diamond")
g2.node("swap",      "swap(arr[j], arr[j+1])\nswaps++\nswapped = true", shape="box")
g2.node("j_inc",     "j++",                          shape="box")
g2.node("flag_check","swapped == false?",            shape="diamond")
g2.node("i_inc",     "i++",                          shape="box")
g2.node("end",       "Konec",                        shape="ellipse")

g2.edge("start",      "i_init")
g2.edge("i_init",     "i_check")
g2.edge("i_check",    "flag_init",  label="da")
g2.edge("i_check",    "end",        label="net")
g2.edge("flag_init",  "j_init")
g2.edge("j_init",     "j_check")
g2.edge("j_check",    "comps",      label="da")
g2.edge("j_check",    "flag_check", label="net")
g2.edge("comps",      "cmp_check")
g2.edge("cmp_check",  "swap",       label="da")
g2.edge("cmp_check",  "j_inc",      label="net")
g2.edge("swap",       "j_inc")
g2.edge("j_inc",      "j_check")
g2.edge("flag_check", "end",        label="da")
g2.edge("flag_check", "i_inc",      label="net")
g2.edge("i_inc",      "i_check")

g2.render("graphs/flowchart_flag", cleanup=True)
print("Blok-shema flag gotova")