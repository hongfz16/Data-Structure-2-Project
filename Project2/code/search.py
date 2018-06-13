# study the relationship of Hamming Dis threshold, time and precision

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

parser = argparse.ArgumentParser(description='Deep Hashing evaluate mAP')
parser.add_argument('--pretrained', type=int, default=0, metavar='pretrained_model',
                    help='loading pretrained model(default = None)')
parser.add_argument('--bits', type=int, default=64, metavar='bts',
                    help='binary bits')
args = parser.parse_args()


def EuclideanDistance(vec1,vec2):
    dist = np.sqrt(np.sum(np.square(vec1 - vec2)))
    return dist


def takeSecond(a):
    return a[1]


# test precision of a query
def precision(trn_binary, trn_label, trn_feature, tst_binary, tst_label, tst_feature, HAMMINGDIS, QUERYNUM):
    trn_binary = trn_binary.cpu().numpy()
    trn_binary = np.asarray(trn_binary, np.int32)
    trn_label = trn_label.cpu().numpy()
    trn_feature = trn_feature.cpu().numpy()
    tst_binary = tst_binary.cpu().numpy()
    tst_binary = np.asarray(tst_binary, np.int32)
    tst_label = tst_label.cpu().numpy()
    tst_feature = tst_feature.cpu().numpy()
    query_times = tst_binary.shape[0]
    toptenacc=0  # accuracy of top 10 images
    candinum=0  # size of candidate set
    total_time_start = time.time()
    for i in range(query_times):
        query_label = tst_label[i]
        query_binary = tst_binary[i,:]
        query_result = np.count_nonzero(query_binary != trn_binary, axis=1)    #don't need to divide binary length
        sort_indices = np.argsort(query_result)
        candidate = []
        for j in sort_indices:
            if(query_result[j] < HAMMINGDIS):
                candidate.append((j,EuclideanDistance(trn_feature[j],tst_feature[i])))
            else:
                break
        candidate.sort(key=takeSecond)
        num=min(QUERYNUM,len(candidate))
        candinum+=len(candidate)
        if num==0:
            continue
        correct=0
        for i in range(num):
            candi_label=trn_label[candidate[i][0]]
            if candi_label==query_label:
                correct+=1
        toptenacc+=correct/num
    toptenacc/=query_times
    candinum/=query_times
    print('Hamming Dist: ',HAMMINGDIS)
    print('Average candidate num: ',candinum)
    print('Average top ten acc: ', toptenacc)
    print('total query time = ', time.time() - total_time_start)


if __name__ == '__main__':
        train_binary = torch.load('./database/train_binary')
        train_label = torch.load('./database/train_label')
        train_feature = torch.load('./database/train_feature')
        test_binary = torch.load('./database/test_binary')
        test_label = torch.load('./database/test_label')
        test_feature = torch.load('./database/test_feature')

        # try different Hamming Distance threshold for candidate set
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 1, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 2, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 3, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 4, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 5, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 6, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 7, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 8, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 9, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 10, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 11, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 12, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 13, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 14, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 15, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 16, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 17, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 18, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 19, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 20, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 21, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 22, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 23, 10)
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature, 24, 10)
