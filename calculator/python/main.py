from src import calc
import json


def get_item(json_file, n):
	with open(json_file, 'r') as file:
		data = json.load(file)
	for item in data:
		if int(item['n']['N']) == n:
			return {
				'bitloops': int(item['bitloops']['N']),
				'bitstrings': int(item['bitstrings']['N']),
				'sym_chains': int(item['sym_chains']['N']),
				'ls_sym_chains': item['ls_sym_chains']['S'],
				'asym_chains': int(item['asym_chains']['N']),
				'ls_asym_chains': item['ls_asym_chains']['S'],
				'graphs': int(item['graphs']['N']),
				'ls_graphs': item['ls_graphs']['S'],
			}
	return {
		'bitloops': 'no data',
		'bitstrings': 'no data',
		'sym_chains': 'no data',
		'ls_sym_chains': 'no data',
		'asym_chains': 'no data',
		'ls_asym_chains': 'no data',
		'graphs': 'no data',
		'ls_graphs': 'no data',
	}


def handler(event, context):
	query_params = event.get('queryStringParameters', {})
	bs = query_params.get('bitstring', '')
	#bs = event.get('bitstring', '')
	#bs = input('enter a bitstring: ')
	
	bs_num_bits = len(bs)
	bs_parity = 'Even' if calc.parity(bs) else 'Odd'
	bs_inverse = calc.inverse(bs)
	bs_reverse = calc.reverse(bs)
	bs_pattern = calc.pattern(bs)
	
	bl_symmetry = 'Symmetric' if calc.is_symmetric(bs) else 'Asymmetric'
	bl_link = calc.link(bs)
	bl_sublink = calc.sublink(bs)
	if bl_sublink == '':
		bl_sublink = 'None'
	bl_mvrot = calc.min_val_rot(bs)
	
	tr_root = calc.root(bs)
	tr_structure = calc.structure(bs)
	tr_num_levels = calc.num_lvl_tree(bs)
	tr_num_bl = calc.num_bl_tree(bs)
	tr_num_bs = calc.num_bs_tree(bs)
	tr_ls_bl = calc.tree_nodes(tr_root)
	
	ch_num_trees = calc.num_roots(tr_root)
	ch_ls_roots = calc.roots(tr_root, ch_num_trees)
	ch_mvroo = calc.min_val_root(ch_ls_roots)
	
	json_file = 'data/powerset.json'
	item = get_item(json_file, bs_num_bits)
	
	gr_num_bs = item['bitstrings']
	gr_num_bl = item['bitloops']
	gr_num_sym_chains = item['sym_chains']
	gr_ls_sym_chains = item['ls_sym_chains']
	gr_num_asym_chains = item['asym_chains']
	gr_ls_asym_chains = item['ls_asym_chains']
	if gr_ls_asym_chains == '':
		gr_ls_asym_chains = 'None'
	gr_num_graphs = item['graphs']
	gr_ls_graphs = item['ls_graphs']
	
	return {
		'statusCode': 200,
		'headers': {
			'Access-Control-Allow-Origin': '*'
		},
		'body': json.dumps({
			'bs_num_bits': bs_num_bits,
			'bs_parity': bs_parity,
			'bs_inverse': bs_inverse,
			'bs_reverse': bs_reverse,
			'bs_pattern': bs_pattern,
			
			'bl_symmetry': bl_symmetry,
			'bl_link': bl_link,
			'bl_sublink': bl_sublink,
			'bl_mvrot': bl_mvrot,
			
			'tr_root': tr_root,
			'tr_structure': tr_structure,
			'tr_num_levels': tr_num_levels,
			'tr_num_bl': tr_num_bl,
			'tr_num_bs': tr_num_bs,
			'tr_ls_bl': tr_ls_bl,
			
			'ch_num_trees': ch_num_trees,
			'ch_ls_roots': ch_ls_roots,
			'ch_mvroo': ch_mvroo,
			
			'gr_num_bs': gr_num_bs,
			'gr_num_bl': gr_num_bl,
			'gr_num_sym_chains': gr_num_sym_chains,
			'gr_ls_sym_chains': gr_ls_sym_chains,
			'gr_num_asym_chains': gr_num_asym_chains,
			'gr_ls_asym_chains': gr_ls_asym_chains,
			'gr_num_graphs': gr_num_graphs,
			'gr_ls_graphs': gr_ls_graphs
		})
	}