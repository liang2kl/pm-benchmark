from datetime import datetime
import matplotlib.pyplot as plt
import csv, regex, sys
from pathlib import Path
from hurry.filesize import size

RESULT_DIRECOTRY = Path("./result")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Invalid arguments")
        exit(1)
    
    node = sys.argv[1]

    paths = RESULT_DIRECOTRY.rglob(f"result-node{node}*.csv")
    paths = sorted(paths, key=lambda p: str(p))
    all_data = {"read": [], "write": []}

    for path in paths:
        datas = {"device": "", "data": []}
        
        with open(path) as file:
            reader = csv.reader(file)
            access_type = ""
            pattern = ""
            
            for row in reader:
                access_type = row[1]
                datas["device"] = row[0]
                datas["pattern"] = row[2]
                datas["data"].append({
                    "block_size": size(int(row[3])),
                    "bandwidth": float(row[5]) / 1024 ** 3
                })

            all_data[access_type].append(datas)
    
    for access_type in all_data:
        datass = all_data[access_type]
        if not datass:
            continue
        fig, ax = plt.subplots()

        plt.title(f'{access_type} bandwidth on node {node}')
        plt.ylabel(f'bandwidth (GB/s)')
        plt.xlabel(f'access size')

        y = [data["block_size"] for data in datass[0]["data"]]

        for datas in datass:
            ax.plot(y, [data["bandwidth"] for data in datas["data"]],
                    label=f'{datas["device"]} {datas["pattern"]}')
        
        x_labels = ax.get_xticklabels()
        [l.set_visible(False) for (i, l) in enumerate(x_labels) if i % 2 != 0 and i != len(x_labels) - 1]

        plt.legend()
        plt.grid()
        
        plt.savefig("./plots/" + f"node{node}-" + access_type + ".png")