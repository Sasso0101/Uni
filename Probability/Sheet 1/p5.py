def main():
    all_combinations = range(100, 1000)
    positives = 0
    for i in all_combinations:
        i1 = (int)(i / 100 % 10)
        i2 = (int)(i / 10 % 10)
        i3 = (int)(i % 10)
        if (i1 < i2 < i3 or i1 > i2 > i3): 
            print('i %d i1 %d i2 %d i3 %d ok' % (i, i1, i2, i3))
            positives += 1
        # else: print('i %d i1 %d i2 %d i3 %d' % (i, i1, i2, i3))
    print('Positives: %d' % positives)

if __name__ == '__main__':
    main()