#! /usr/bin/python

# Imported Standard Modules
import sys
from PIL import Image

BITS_PER_LINE = 3

def convert(image_name):
	"""
		This converts the given image into a Xilinx Coefficients (.coe) file.
		Pass it the name of the image including the file suffix.
		The file must reside in the directory from which this function is called
		or provide the absolute path. 
	"""
	# Open image
	img = Image.open(image_name)
	# Verify that the image is in the 'RGB' mode, every pixel is described by 
	# three bytes
	if img.mode != 'RGB':
		img = img.convert('RGB')

	# Store width and height of image
	width 	= img.size[0]
	height	= img.size[1]

	# Create a .coe file and open it.
	# Write the header to the file, where lines that start with ';' 
	# are commented
	filetype = image_name[image_name.find('.'):]
	filename = image_name.replace(filetype,'.coe')
	imgcoe = open(filename, 'w')
	imgcoe.write(';	VGA Memory Map\n')
	imgcoe.write('; .COE file with hex coefficients\n')
	imgcoe.write('; Height: {0}, Width: {1}\n'.format(height,width))
	imgcoe.write('memory_initialization_radix = 2;\n')
	imgcoe.write('memory_initialization_vector =\n')
	
	# Iterate through every pixel, retain the 3 least significant bits for the
	# red and green bytes and the 2 least significant bits for the blue byte. 
	# These are then combined into one byte and their hex equivalent is written
	# to the .coe file
	cnt = 0
	for r in range(0, height):
		for c in range(0, width):
			# Check for IndexError, usually occurs if the script is trying to 
			# access an element that does not exist
			try:
				R,G,B = img.getpixel((c,r))
			except IndexError:
				print('Index Error Occurred At:')
				print('c: {}, r:{}'.format(c,r))
				sys.exit()
			# convert the value (0-255) to a binary string by cutting off the 
			# '0b' part and left filling zeros until the string represents 8 bits
			# then slice off the bits of interest with [5:] for red and green
			# or [6:] for blue
			Rb = bin(R)[2:].zfill(8)[:4]
			Gb = bin(G)[2:].zfill(8)[:4]
			Bb = bin(B)[2:].zfill(8)[:4]
			
			# Check for Value Error, happened when the case of the pixel being 
			# zero was not handled properly	
			try:
				imgcoe.write('{0}{1}{2}'.format(Rb, Gb, Bb))
			except ValueError:
				print('Value Error Occurred At:')
				print('Contents of line: {0}{1}{2} at r:{1} c:{2}'.format(Rb, Gb, Bb, r, c))
				print('R:{0} G:{1} B{2}'.format(R, G, B))
				print('Rb:{0} Gb:{1} Bb:{2}'.format(Rb, Gb, Bb))
				sys.exit()
			if (cnt + 1) % BITS_PER_LINE == 0:
				imgcoe.write(',\n')
			cnt += 1
	imgcoe.write(';')
	imgcoe.close()
	print('Xilinx Coefficients File:{} DONE'.format(filename))
	print('Converted from {} to .coe'.format(filetype))
	print('Size: h:{} pixels w:{} pixels'.format(height, width))
	# upside-down floor division to avoid floating point error
	print('COE file is {} bits wide and {} bits deep'.format(BITS_PER_LINE, -(cnt//-BITS_PER_LINE)))

if __name__ == '__main__':
	convert(sys.argv[1])