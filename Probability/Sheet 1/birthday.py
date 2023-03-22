n = int(input("num people in room: "))
sum = 1
for i in range(1, n):
    sum *= (365-i)/365
print("Prob at least 2 share same birthday: " + str((1-sum)*100))