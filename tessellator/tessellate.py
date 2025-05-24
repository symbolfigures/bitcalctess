from PIL import Image, ImageDraw, ImageFont
from src import calc


def create_image(long_bs, s=1):

	width, height = int(1000*s), int(1000*s)
	image = Image.new('RGB', (width, height), color='#d0d0d0')
	draw = ImageDraw.Draw(image)
	font = ImageFont.truetype('data/american-typewriter.ttf', 24)

	global long_n
	long_n = len(long_bs)

	# lines
	long_link = long_bs
	shift = 0
	for y in range(int(26*s)):
		long_ar = [int(i) for i in long_link]
		for x in range(long_n):
			
			l0 = ((x + shift) % long_n, y)
			l2 = ((x + shift + 1) % long_n, y)
			u1 = ((x + shift + 0.5) % long_n, y + 1)
			u3 = ((x + shift + 1.5) % long_n, y + 1)

			L0 = transform(l0[0], l0[1], s)
			L2 = transform(l2[0], l2[1], s)
			U1 = transform(u1[0], u1[1], s)
			U3 = transform(u3[0], u3[1], s)

			if long_ar[x] == 0:
				if long_ar[(x + 1) % long_n] == 1:
					if long_ar[(x + 2) % long_n] == 0:
						if u1[0] > l2[0] < u3[0]: # half-triangle visible on the left
							U1 = (L2[0] - 20, U1[1])
						if u1[0] < l2[0] > u3[0]: # half-triangle visible on the right
							U3 = (L2[0] + 20, U3[1])
						draw.polygon((L2, U1, U3), fill='#404040') # triangle
						draw.line((L2, U1, U3, L2), fill='#404040', width=5) # triangle
					else:
						if l2[0] < u1[0]: # wraparound
							U1 = (L2[0] - 20, U1[1])
						draw.line((L2, U1), fill='#404040', width=5) # backward diagonal line
			else:
				if long_ar[(x + 1) % long_n] == 0:
					if l0[0] > u1[0]: # wraparound
						L0 = (U1[0] - 20, L0[1])
					draw.line((L0, U1), fill='#404040', width=5) # forward diagonal line
				else:
					if l0[0] > l2[0]: # wraparound
						L0 = (L2[0] - 40, L0[1])
					draw.line((L0, L2), fill='#404040', width=5) # horizontal line

		shift = shift + 0.5
		long_link = link(long_link)

	# numbers
	long_link = long_bs
	shift = 0
	for y in range(int(26*s)):
		long_ar = [int(i) for i in long_link]
		for x in range(long_n):
			x_y = transform((x + shift) % long_n, y, s)
			bit = str(long_ar[x])
			color = '#0088ff' if bit == '0' else '#ff8800'
			draw.text(x_y, bit, fill=color, anchor='mm', font=font)
		shift = shift + 0.5
		long_link = link(long_link)

	return image

	
def transform(x, y, s):
	x = 40 * x
	y = int(1000*s) - 40 * y
	return (x, y)


def link(bs):
	"""first bit shifts 0.5 bits to the right"""
	l = ''
	for i in range(len(bs)):
		l += str(int(bs[i]) ^ int(bs[(i +1) % len(bs)]))
	return l

