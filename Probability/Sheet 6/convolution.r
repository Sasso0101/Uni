#Question 2:
#Have 2 d8 and 1 d4
#What is the probability that the sum of the 3 dice is >=13?

#Add 8 zeros of padding so that the convolution has size 16
d1 <- c(rep(1/8,8), rep(0,8))
d2 <- c(rep(1/8,8), rep(0,8))
#Add 16 zeros of padding so that the convolution has size 20
d3 <- c(rep(1/4,4), rep(0,16))
o1 <- convolve(d1, rev(d2))
o1 <- convolve(c(o1, rep(0,4)), rev(d3))
print(sum(o1[13:20]))