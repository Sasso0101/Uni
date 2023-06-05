won <- 0
is20 <- 0
for (p1 in 1:90) {
    for (p2 in 1:90) {
        if (p1 == p2) next
        if ((p1 %% 2) == 0 && (p2 %% 10) == 0) {
            won <- won + 1
            if ((p2 %% 20) == 0) is20 <- is20 + 1
        }
        else if ((p1 %% 2) == 1 && (p2 %% 20) == 0) {
            won <- won + 1
            is20 <- is20 + 1
        }
    }
}
cat("won: ", won)
cat("is 20: ", is20)
