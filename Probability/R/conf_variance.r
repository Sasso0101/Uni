x <- c(2.85, 2.18, 2.46, 4.58, 5.88, 7.51, 4.56, 2.96, 1.25, 2.43, 1.95)
conf <- 0.99
bottom <- var(x)*(length(x)-1)/qchisq(1-(1-conf)/2, df=length(x)-1)
top <- var(x)*(length(x)-1)/qchisq((1-conf)/2, df=length(x)-1)
print(bottom)
print(top)