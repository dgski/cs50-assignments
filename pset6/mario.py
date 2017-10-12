def printline(size, row):
    out_o = size - row

    print(" " * out_o, end = "")
    print("X" * row, end = "")
    print("  ", end = "")
    print("X" * row, end = "")
    print(" " * out_o, end = "")
    print()


while True:
    size = input("Height:")

    if size.isdigit() and (int(size) < 24) and (int(size) > 0):
        break

size = int(size)

for i in range(1,size + 1):
    printline(size, i)





