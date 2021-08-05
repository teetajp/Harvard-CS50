from cs50 import get_int

while True:
    n = get_int("Height: ")
    if n > 0 and n <= 8:
        break

for row in range(n):
    print(" " * (n - row - 1), end="")
    print("#" * (row + 1), end="")
    print("  ", end="")
    print("#" * (row + 1))