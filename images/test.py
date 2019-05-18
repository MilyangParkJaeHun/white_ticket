from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw 

inp = raw_input()
img = Image.open("1.png")
draw = ImageDraw.Draw(img)
# font = ImageFont.truetype(<font-file>, <font-size>)
#font = ImageFont.truetype("Arial.ttf",90)
# draw.text((x, y),"Sample Text",(r,g,b))
draw.text((100, 100),inp,(0,0,0))
img.show()
img.save('2.png')
