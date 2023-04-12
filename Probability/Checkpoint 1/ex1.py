import random

die = [1,2,3,4,5,6]
totals = [0,0,0,0]

def is5or6(throw):
    if throw == 5 or throw == 6: return True
    return False

def percentage(elements, total):
    return round(elements/total*100, 2)

for trie in range(1, 10000):
    throws = random.choices(die, k=3)
    throws5or6 = 0
    for throw in throws:
        if is5or6(throw): throws5or6 += 1
    totals[throws5or6] += 1

total = totals[0]+totals[1]+totals[2]+totals[3]
print("Games with exactly 0 5 or 6: " + str(percentage(totals[0], total)))
print("Games with exactly 1 5 or 6: " + str(percentage(totals[1], total)))
print("Games with exactly 2 5 or 6: " + str(percentage(totals[2], total)))
print("Games with exactly 3 5 or 6: " + str(percentage(totals[3], total)))