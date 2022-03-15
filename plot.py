import matplotlib.pyplot as plt
import csv, os, math, regex
from pathlib import Path
from hurry.filesize import size

RESULT_DIRECOTRY = Path("./result")

def get_node(file_name):
    return regex.search(r"result-node([0-9])", file_name)

def convert_size(size_bytes):
   if size_bytes == 0:
       return "0B"
   size_name = ("B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB")
   i = int(math.floor(math.log(size_bytes, 1024)))
   p = math.pow(1024, i)
   s = round(size_bytes / p, 2)
   return "%s %s" % (s, size_name[i])

if __name__ == "__main__":
    paths = RESULT_DIRECOTRY.rglob("*.csv")
    all_data = {"read": [], "write": []}

    for path in paths:
        datas = {"device": "", "data": []}
        with open(path) as file:
            reader = csv.reader(file)
            access_type = ""
            for row in reader:
                access_type = row[1]
                datas["device"] = row[0]
                datas["data"].append({
                    "block_size": size(int(row[3])),
                    "bandwidth": float(row[5]) / 1024 ** 3
                })

            all_data[access_type].append(datas)
    
    for access_type in all_data:
        datass = all_data[access_type]
        fig, ax = plt.subplots()

        plt.title(f'sequential {access_type}'
            f' on node {get_node(str(path)).group(1)}')
        plt.ylabel(f'bandwidth (GB/s)')
        plt.xlabel(f'access size')

        y = [data["block_size"] for data in datass[0]["data"]]

        for datas in datass:
            ax.plot(y, [data["bandwidth"] for data in datas["data"]],
                    label=datas["device"])
        plt.legend()
        plt.grid()
        plt.savefig("./plots/" + access_type + ".png")