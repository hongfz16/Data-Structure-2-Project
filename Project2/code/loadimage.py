import torch
import torch.nn as nn
from torchvision import datasets, models, transforms
from torch.autograd import Variable

from PIL import Image
import numpy as np


root = './data/image/'

transform_train = transforms.Compose(
        [transforms.Scale(227),
         transforms.ToTensor(),
         transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010))])

imagelist = open(root+'list.txt','r')
labeldic = dict()
labelnum = 0
lastlabel = ''
for line in imagelist:
    line = line.strip('\n')
    line = line.rstrip()
    words = line.split()
    imagename = words[0]
    if(words[1] != lastlabel):
        labelnum += 1
        lastlabel = words[1]
        labeldic[words[1]] = labelnum
        print('labelnum %d : %s' % (labelnum, words[1]))

im = Image.open(root + 'n01613177_6.JPEG').convert('RGB')
img = transform_train(im)
# print('img = ',img)