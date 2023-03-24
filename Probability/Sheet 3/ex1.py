import random

secondIsGold = 0
total = 0
for i in range(5000000):
    pouche = random.randint(1, 3)
    if pouche == 1:
        secondIsGold += 1
    
    total += 1

print("Second is gold: " + str(secondIsGold) + "/" + str(total))