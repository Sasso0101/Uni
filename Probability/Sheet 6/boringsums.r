l <- 13
p <- 0
for (n in 13:20) {
    for (x in max(1, n-12):min(8, n-2)) {
        for (y in max(1, n-4-x):min(8, n-x-1)) {
            p <- p + 1/8*1/8*1/4
        }
    }
}
print(p)