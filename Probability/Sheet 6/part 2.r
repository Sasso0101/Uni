p <- 0
for (i in (9:12)) {
    temp <- 0
    for (x in max(1, i-8):min(8, i-1)) {
        temp <- temp + 1/8*1/8
    }
    temp1 <- 0
    for (j in (13-i):4) {
        temp1 <- temp1 + 1/4
    }
    p <- p + temp*temp1
}
print(p)