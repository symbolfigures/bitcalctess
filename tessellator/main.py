from src import calc, tessellate
import base64
import json
import io


def encode(image):
	buffer = io.BytesIO()
	image.save(buffer, format='GIF')
	img_data = buffer.getvalue()
	img_base64 = base64.b64encode(img_data).decode('utf-8')
	return img_base64


def handler(event, context):

	query_params = event.get('queryStringParameters', {})
	bs = query_params.get('bitstring', '')
	#bs = event.get('bitstring', '')
	
	bs = calc.root(bs)
	n = len(bs)
	s = 1.5
	repeats = int(25*s) // n + 1
	long_n = n * repeats
	long_bs = bs * repeats
	
	image = tessellate.create_image(long_bs, s)
	encoded_image = encode(image)

	response = {
		'statusCode': 200,
		'headers': {
			'Access-Control-Allow-Origin': '*',
			'Content-Type': 'image/gif'
		},
		'isBase64Encoded': True,
		'body': encoded_image
	}
	
	return response
