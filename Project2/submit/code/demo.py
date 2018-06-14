# show result images given a query pic

import os
import argparse

import numpy as np
from net import AlexNetPlusLatent

from timeit import time

import torch
import torch.nn as nn

from torchvision import datasets, models, transforms
from torch.autograd import Variable
import torch.backends.cudnn as cudnn
import torch.optim.lr_scheduler
from MyDataset import MyDataset
from PIL import Image
import matplotlib.pyplot as plt


parser = argparse.ArgumentParser(description='Deep Hashing evaluate mAP')
parser.add_argument('--pretrained', type=int, default=0, metavar='pretrained_model',
                    help='loading pretrained model(default = None)')
parser.add_argument('--bits', type=int, default=48, metavar='bts',
                    help='binary bits')
args = parser.parse_args()

transform_train = transforms.Compose(
    [transforms.Resize(227),
     transforms.CenterCrop(227),
     transforms.ToTensor(),
     transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010))])
queryset = MyDataset(txt='./data/query/list.txt', imagepath='./data/query/', transform=transform_train)
dbset = MyDataset(txt='./data/image/list.txt', imagepath='./data/image/', transform=transform_train)

def single_query(query_binary,label,db_binary,db_label):
    db_binary = db_binary.cpu().numpy()
    db_binary = np.asarray(db_binary, np.int32)
    db_label = db_label.cpu().numpy()
    curr_time=time.time()
    query_result=np.count_nonzero(query_binary!=db_binary,axis=1)
    sort_indices=np.argsort(query_result)
    print(sort_indices[:10])
    return sort_indices[:10]

if __name__ == '__main__':
    db_binary = torch.load('./database/db_binary')
    db_label = torch.load('./database/db_label')
    query_binary = torch.load('./database/query_binary')
    query_label = torch.load('./database/query_label')

    qry_binary = query_binary.cpu().numpy()
    qry_binary = np.asarray(query_binary, np.int32)
    qry_label = query_label.cpu().numpy()

    # show images
    num=0
    ans=single_query(qry_binary[num],qry_label[num],db_binary,db_label)
    query_fn,label=queryset.imgs[num]
    img=Image.open('./data/query/'+query_fn).convert('RGB')
    plt.subplot(4,4,1)
    plt.imshow(img)  # print query image
    for i in range(len(ans)):
        fn,label=dbset.imgs[ans[i]]
        img=Image.open('./data/image/'+fn).convert('RGB')
        plt.subplot(4,4,2+i)
        plt.imshow(img)  # print retrieved images
    plt.show()
