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

def precision(db_binary, db_label, qry_binary, qry_label):
    db_binary = db_binary.cpu().numpy()
    db_binary = np.asarray(db_binary, np.int32)
    db_label = db_label.cpu().numpy()
    qry_binary = qry_binary.cpu().numpy()
    qry_binary = np.asarray(qry_binary, np.int32)
    qry_label = qry_label.cpu().numpy()
    query_times = qry_binary.shape[0]
    dbset_len = db_binary.shape[0]
    AP = np.zeros(query_times)
    Ns = np.arange(1, dbset_len + 1)
    total_time_start = time.time()
    for i in range(query_times):
        print('Query ', i+1)
        query_label = qry_label[i]
        query_binary = qry_binary[i,:]
        query_result = np.count_nonzero(query_binary != db_binary, axis=1)  # hamming distance
        sort_indices = np.argsort(query_result)  # sort by hamming distance
        print(query_label)
        print(sort_indices[:10])
        buffer_yes= np.equal(query_label, db_label[sort_indices]).astype(int)
        P = np.cumsum(buffer_yes) / Ns
        AP[i] = np.sum(P * buffer_yes) /sum(buffer_yes)
    map = np.mean(AP)
    print(map)
    print('total query time = ', time.time() - total_time_start)


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
    db_binary = torch.load('./result/db_binary')
    db_label = torch.load('./result/db_label')
    query_binary = torch.load('./result/query_binary')
    query_label = torch.load('./result/query_label')

    qry_binary = query_binary.cpu().numpy()
    qry_binary = np.asarray(query_binary, np.int32)
    qry_label = query_label.cpu().numpy()

    precision(db_binary, db_label, query_binary, query_label) # evaluate by mAP

    # show images
    num=888
    ans=single_query(qry_binary[num],qry_label[num],db_binary,db_label)
    query_fn,label=queryset.imgs[num]
    img=Image.open('./data/image/'+query_fn).convert('RGB')
    plt.subplot(4,4,1)
    plt.imshow(img)  # print query image
    for i in range(len(ans)):
        fn,label=dbset.imgs[ans[i]]
        img=Image.open('./data/image/'+fn).convert('RGB')
        plt.subplot(4,4,2+i)
        plt.imshow(img)  # print retrieved images
    plt.show()
