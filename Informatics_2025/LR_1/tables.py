import pandas as pd

df = pd.read_csv("results.csv", sep=";")
df_sorted = df.sort_values(["DataType", "Algorithm", "Size"])

with open("graphs/tables.typ", "w", encoding="utf-8") as f:
    for dtype in ["random", "sorted", "reversed"]:
        type_names = {
            "random": "Случайные данные",
            "sorted": "Отсортированные данные",
            "reversed": "Обратный порядок"
        }
        f.write(f"=== {type_names[dtype]}\n\n")
        f.write("#table(\n")
        f.write("  columns: (auto, auto, auto, auto, auto),\n")
        f.write("  [*n*], [*Алгоритм*], [*Сравнения*], [*Обмены*], [*Время (мкс)*],\n")
        subset = df_sorted[df_sorted["DataType"] == dtype]
        for _, row in subset.iterrows():
            f.write(f'  [{int(row["Size"])}], [{row["Algorithm"]}], [{int(row["Comparisons"])}], [{int(row["Swaps"])}], [{int(row["Time_us"])}],\n')
        f.write(")\n\n")

print("Таблицы сохранены в graphs/tables.typ")