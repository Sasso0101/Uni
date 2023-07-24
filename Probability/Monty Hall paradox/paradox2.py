from random import randrange

totalwins = 0
total = 0

for i in range(1000):
    win = randrange(3)
    pick = randrange(3)

    remove = randrange(3)
    while remove == win or remove == pick:
        remove = randrange(3)

    doors = [0,1,2]
    doors.remove(remove)
    pick = randrange(2)
    pick = doors[pick]

    total += 1
    if win == pick:
        totalwins += 1

print("Total wins: " + str(totalwins))
print("Total games: " + str(total))
print("Win percentage: " + str(totalwins/total))