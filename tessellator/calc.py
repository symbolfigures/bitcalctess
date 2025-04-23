from sympy import divisors

def bs_equals(bs1, bs2):
	"""are bitstrings equal"""
	return bs1 == bs2

def bl_equals(bs1, bs2):
	"""are bitloops equal"""
	return len(bs1) == len(bs2) and bs2 in bs1 + bs1

def dec_to_bs(n, dec):
	"""convert decimal to binary, preserve string length"""
	b = (bin(dec))[2:]
	return (n - len(b)) * '0' + b

def bs_to_dec(bs):
	"""convert binary to decimal"""
	bs = bs[bs.find('1'):]
	return int(str(bs), 2)

def parity(bs):
	"""sum of 1s in the bitstring is even (true/false)"""
	return bs.count('1') % 2 == 0

def inverse(bs):
	"""switch 1s for 0s and vice versa"""
	i = ''
	for b in bs:
		i += str(int(b) ^ 1)
	return i

def is_inverse(bs):
	"""does the rotation class of the bitloop
	contain inverse pairs of bitstrings"""
	return bl_equals(bs, inverse(bs))

def reverse(bs):
	"""reverse order of bits"""
	return bs[::-1]

def is_reverse(bs):
	"""is reverse the same as non-reverse"""
	return bs == reverse(bs)

def is_symmetric(bs):
	"""does the rotation class of the bitloop
	contain reverse pairs of bitstrings"""
	for i in rotations(bs):
		if bs == reverse(i):
			return True
	return False

def rotate(bs, t):
	"""rotate order of bits by t bits"""
	t %= len(bs)
	r = bs[t:] + bs[:t]
	return r

def link(bs):
	"""return the link of a given bitloop"""
	l = ''
	for i in range(len(bs)):
		l += str(int(bs[i]) ^ int(bs[(i +1) % len(bs)]))
	return min_val_rot(l)

def sublink(bs):
	"""return the sublink of a given bitloop
	or an empty string if the bitloop has no sublink"""
	sl = ''
	if parity(bs):
		sl += '0'
		for i in range(len(bs) - 1):
			sl += str(int(sl[i]) ^ int(bs[i]))
	return sl if sl == '' else min_val_rot(sl)

def pattern(bs):
	"""return the pattern of a given bitloop"""
	n = len(bs)
	divs = divisors(n)
	for div in divs:
		p = bs[:div]
		if p * (n // div) == bs:
			return p

def rotations(bs):
	"""return all unique rotations of a bitstring
	that is, the rotation class of the respective bitloop"""
	rots = []
	for i in range(len(pattern(bs))):
		rots.append(rotate(bs, i))
	return rots

def min_val_rot(bs):
	"""minimum value rotation (canonical form)"""
	rots = [bs_to_dec(r) for r in rotations(bs)]
	mn = dec_to_bs(len(bs), min(rots))
	return (len(bs) - len(mn)) * '0' + mn

def root(bs):
	"""the root of the tree that a bitloop is in"""
	cycle = [bs]
	lbs = link(bs)
	while not lbs in cycle:
		cycle.append(lbs)
		lbs = link(lbs)
	return min_val_rot(lbs)

def is_root(bs):
	"""is the bitloop a tree root"""
	return bs == root(bs)

def num_roots(rt):
	"""number of roots in a cycle"""
	l = link(rt)
	i = 1
	while not bl_equals(rt, l):
		i += 1
		l = link(l)
	return i

def roots(rt, num_roots):
	"""list of roots in a chain
	input a root and num_roots(root)"""
	rts = []
	for i in range(num_roots):
		rts.append(rt)
	rt = link(rt)
	return rts

def min_val_root(roots):
	"""minimum value root given the roots in a chain"""
	for r in roots:
		r = bs_to_dec(r)
	return min(roots)

def sublink_and_root(root):
	"""the sublink of a root that is also a root"""
	s = sublink(root)
	return s if is_root(s) else inverse(s)

def num_lvl_tree(bs):
	"""number of levels in a tree"""
	l = 1
	bs = inverse(root(bs))
	while sublink(bs) != '':
		l += 1
		bs = sublink(bs)
	return l

def structure(bs):
	"""the structure of a tree"""
	s = '2'
	bs = inverse(root(bs))
	while sublink(bs) != '':
		bs = sublink(bs)
		s += '1' if is_inverse(bs) else '2'
	s += '0'
	return s

def num_bl_tree(bs):
	"""number of bitloops in a tree"""
	s = structure(bs)
	if len(s) == 2: return 1
	num_bl_lv = num_bl_tr = 1
	for lv in range(2, len(s)):
		num_bl_lv *= int(s[lv - 1])
		num_bl_tr += num_bl_lv
	return num_bl_tr

def num_bs_tree(bs):
	"""number of bitstrings in a tree"""
	s = structure(bs)
	bs = inverse(root(bs))
	num_bs_tr = len(pattern(bs))
	if len(s) == 2: return num_bs_tr
	num_bl_lv = 1
	for lv in range(2, len(s)):
		num_bl_lv *= int(s[lv - 1])
		bs = sublink(bs)
		num_bs_tr += len(pattern(bs)) * num_bl_lv
	return num_bs_tr

def num_bl_chain(bs):
	"""number of bitloops in a chain"""
	return num_roots(bs) + num_roots(bs) * num_bl_tree(bs)

def num_bs_chain(bs):
	"""number of bitstrings in a chain"""
	bs = root(bs)
	num_bs_cyc = len(pattern(bs)) * num_roots(bs)
	return num_bs_cyc + num_roots(bs) * num_bs_tree(bs)

def tree_nodes(root):
	"""returns the set of all bitloops in a tree"""
	t_ns = [root]
	sl_off_cycle = inverse(sublink_and_root(root))
	make_tree(sl_off_cycle, t_ns)
	return t_ns

def make_tree(t_root, t_ns):
	"""helper function for tree_nodes"""
	t_ns.append(t_root)
	sl = sublink(t_root)
	if sl == '':
		return
	inv = inverse(sl)
	make_tree(sl, t_ns)
	if not bl_equals(sl, inv):
		make_tree(inv, t_ns)

