from cs50 import get_float

# Get user input
while True:
    dollars = get_float("Change owed: ")
    if dollars > 0:
        cents = dollars * 100
        break

coins = 0
while cents > 0:
    # Subtract max # of quarters
    while cents - 25 >= 0:
        cents -= 25
        coins += 1
    # Subtract max # of dimes
    while cents - 10 >= 0:
        cents -= 10
        coins += 1
    # Subtract max # of nickels
    while cents - 5 >= 0:
        cents -= 5
        coins += 1
    # Subtract max # of pennies
    while cents - 1 >= 0:
        cents -= 1
        coins += 1

print(coins)