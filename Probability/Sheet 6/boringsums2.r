l <- 13
p <- 0
for (n in 2:12) {
    for (x in max(1, n-8):min(8, n-1)) {
        p <- p + 1/8*1/8
    }
}
print(p)