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
parser.add_argument('--bits', type=int, default=48, metavar='bts',
                    help='binary bits')
args = parser.parse_args()


def EuclideanDistance(vec1,vec2):
    dist = np.sqrt(np.sum(np.square(vec1 - vec2)))
    return dist


def takeSecond(a):
    return a[1]


# test precision of a query
def precision(db_binary, db_label, db_feature, qry_binary, qry_label, qry_feature, HAMMINGDIS, QUERYNUM):
    db_binary = db_binary.cpu().numpy()
    db_binary = np.asarray(db_binary, np.int32)
    db_label = db_label.cpu().numpy()
    db_feature = db_feature.cpu().numpy()
    qry_binary = qry_binary.cpu().numpy()
    qry_binary = np.asarray(qry_binary, np.int32)
    qry_label = qry_label.cpu().numpy()
    qry_feature = qry_feature.cpu().numpy()
    query_times = qry_binary.shape[0]
    toptenacc=0  # accuracy of top 10 images
    candinum=0  # size of candidate set
    total_time_start = time.time()
    for i in range(query_times):
        query_label = qry_label[i]
        query_binary = qry_binary[i,:]
        query_result = np.count_nonzero(query_binary != db_binary, axis=1)    #don't need to divide binary length
        sort_indices = np.argsort(query_result)
        candidate = []
        for j in sort_indices:
            if(query_result[j] < HAMMINGDIS):
                candidate.append((j,EuclideanDistance(db_feature[j],qry_feature[i])))
            else:
                break
        candidate.sort(key=takeSecond)
        num=min(QUERYNUM,len(candidate))
        candinum+=len(candidate)
        if num==0:
            continue
        correct=0
        for i in range(num):
            candi_label=db_label[candidate[i][0]]
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
        db_binary = torch.load('./database/db_binary')
        db_label = torch.load('./database/db_label')
        db_feature = torch.load('./database/db_feature')
        query_binary = torch.load('./database/query_binary')
        query_label = torch.load('./database/query_label')
        query_feature = torch.load('./database/query_feature')

        # try different Hamming Distance threshold for candidate set
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 1, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 2, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 3, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 4, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 5, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 6, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 7, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 8, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 9, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 10, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 11, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 12, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 13, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 14, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 15, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 16, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 17, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 18, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 19, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 20, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 21, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 22, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 23, 10)
        precision(db_binary, db_label, db_feature, query_binary, query_label, query_feature, 24, 10)
