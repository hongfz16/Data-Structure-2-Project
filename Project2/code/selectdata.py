import os
import random

imagelist = open('./data/image/list.txt', 'r')
imgs = []
for line in imagelist:
    line = line.strip('\n')
    line = line.rstrip()
    words = line.split()
    imgs.append((words[0], words[1]))

random.shuffle(imgs)
trainlist = imgs[:4000]
testlist = imgs[4000:5000]

def takesecond(elem):
    return elem[1]

trainlist.sort(key = takesecond)
testlist.sort(key = takesecond)

# print(trainlist)
# print(testlist)

ftrain = open('./data/image/trainlist.txt', 'w')
for elem in trainlist:
    print('%s %s' % (elem[0], elem[1]), file = ftrain)
ftrain.close()

ftest = open('./data/image/testlist.txt', 'w')
for elem in testlist:
    print('%s %s' % (elem[0], elem[1]), file = ftest)
ftest.close()
