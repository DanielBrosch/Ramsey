max_val = None

print("This program tests if the maximum partition used is not above 20")

with open("F_edges20partition__n25_unlabeled.txt") as f:
    for line in f:
        numbers = list(map(int, line.split()))
        if len(numbers) > 1:  # make sure there's something after the first number
            candidate = max(numbers[1:])
            if max_val is None or candidate > max_val:
                max_val = candidate

print("Maximum value:", max_val)
print("If less than 20, we are good")
