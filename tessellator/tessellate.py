from PIL import Image, ImageDraw, ImageFont

def create_image(long_bs):

	width, height = 1000, 1000
	image = Image.new('RGB', (width, height), color='#000000')
	draw = ImageDraw.Draw(image)
	font = ImageFont.truetype('american-typewriter.ttf', 24)

	global long_n
	long_n = len(long_bs)

	# lines
	long_link = long_bs
	shift = 0
	for y in range(26):
		long_ar = [int(i) for i in long_link]
		for x in range(long_n):

			# bit coordinates
			l0 = ((x + shift) % long_n, y)
			l2 = ((x + shift + 1) % long_n, y)
			u1 = ((x + shift + 0.5) % long_n, y + 1)
			u3 = ((x + shift + 1.5) % long_n, y + 1)
			# pixel coordinates
			L0 = transform(l0[0], l0[1])
			L2 = transform(l2[0], l2[1])
			U1 = transform(u1[0], u1[1])
			U3 = transform(u3[0], u3[1])

			if long_ar[x] == 0:
				if long_ar[(x + 1) % long_n] == 1:
					if long_ar[(x + 2) % long_n] == 0:
						# half-triangle visible on the left
						if u1[0] > l2[0] < u3[0]:
							U1 = (L2[0] - 20, U1[1])
						# half-triangle visible on the right
						if u1[0] < l2[0] > u3[0]:
							U3 = (L2[0] + 20, U3[1])
						# triangle
						draw.polygon((L2, U1, U3), fill='#8800ff')
						draw.line((L2, U1, U3, L2), fill='#8800ff', width=5)
					else:
						# wraparound
						if l2[0] < u1[0]:
							U1 = (L2[0] - 20, U1[1])
						# backward diagonal line
						draw.line((L2, U1), fill='#8800ff', width=5)
			else:
				if long_ar[(x + 1) % long_n] == 0:
					# wraparound
					if l0[0] > u1[0]:
						L0 = (U1[0] - 20, L0[1])
					# forward diagonal line
					draw.line((L0, U1), fill='#8800ff', width=5)
				else:
					# wraparound
					if l0[0] > l2[0]:
						L0 = (L2[0] - 40, L0[1])
					# horizontal line
					draw.line((L0, L2), fill='#8800ff', width=5)

		shift = shift + 0.5
		long_link = link(long_link)

	# numbers
	long_link = long_bs
	shift = 0
	for y in range(26):
		long_ar = [int(i) for i in long_link]
		for x in range(long_n):
			x_y = transform((x + shift) % long_n, y)
			bit = str(long_ar[x])
			color = '#00ff00' if bit == '0' else '#ff8800'
			draw.text(x_y, bit, fill=color, anchor='mm', font=font)
		shift = shift + 0.5
		long_link = link(long_link)

	return image

	
def transform(x, y):
	x = 40 * x
	y = 1000 - 40 * y
	return (x, y)


def link(bs):
	# first bit shifts 0.5 bits to the right
	l = ''
	for i in range(len(bs)):
		l += str(int(bs[i]) ^ int(bs[(i +1) % len(bs)]))
	return l


	