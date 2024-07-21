#! /usr/bin/python

# Imported Standard Modules
import sys
from PIL import Image

def decoder(max, digits):
	filename = 'bcd.coe'
	bcd = open(filename, 'w')
	bcd.write('; Binary to BCD lookup table\n')
	bcd.write('; .COE file with bin coefficients\n')
	bcd.write('memory_initialization_radix = 2;\n')
	bcd.write('memory_initialization_vector =\n')
	
	for number in range(0, max+1):
		temp_num = number
		bcd_final = ''
		print('number {0}'.format(number))
		for _ in range(0, digits):
			digit = int(temp_num % 10)
			temp_num = int(temp_num / 10)
			bcd_final = ('{:0=4b}{}'.format(digit, bcd_final))
			print('digit {0} is {0:0=4b}'.format(digit))
		bcd.write('{},\n'.format(bcd_final))

	bcd.write(';')
	bcd.close()
	print('Xilinx Coefficients File: {} DONE'.format(filename))

if __name__ == '__main__':
	decoder(int(sys.argv[1]), int(sys.argv[2]))