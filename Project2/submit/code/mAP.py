#evaluate the precision of trained net using train and test set

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

root = './data/image/'

parser = argparse.ArgumentParser(description='Deep Hashing evaluate mAP')
parser.add_argument('--pretrained', type=int, default=0, metavar='pretrained_model',
                    help='loading pretrained model(default = None)')
parser.add_argument('--bits', type=int, default=48, metavar='bts',
                    help='binary bits')
args = parser.parse_args()

def precision(trn_binary, trn_label, tst_binary, tst_label):
    query_times = tst_binary.shape[0]
    trainset_len = trn_binary.shape[0]
    AP = np.zeros(query_times)
    Ns = np.arange(1, trainset_len + 1)
    total_time_start = time.time()
    for i in range(query_times):
        print('Query ', i+1)
        query_label = tst_label[i]
        query_binary = tst_binary[i,:]
        query_result = np.count_nonzero(query_binary != trn_binary, axis=1)  #hamming distance
        sort_indices = np.argsort(query_result)  #sort by hamming distance
        buffer_yes= np.equal(query_label, trn_label[sort_indices]).astype(int)
        P = np.cumsum(buffer_yes) / Ns
        AP[i] = np.sum(P * buffer_yes) /sum(buffer_yes)
    map = np.mean(AP)
    print(map)
    print('total query time = ', time.time() - total_time_start)

if __name__ == '__main__':
    if os.path.exists('./database/train_binary') and os.path.exists('./database/train_label') and \
        os.path.exists('./database/test_binary') and os.path.exists('./database/test_label'):
        train_binary = torch.load('./database/train_binary')
        train_label = torch.load('./database/train_label')
        test_binary = torch.load('./database/test_binary')
        test_label = torch.load('./database/test_label')

        tst_binary = test_binary.cpu().numpy()
        tst_binary = np.asarray(test_binary, np.int32)
        tst_label = test_label.cpu().numpy()

        trn_binary = train_binary.cpu().numpy()
        trn_binary = np.asarray(train_binary, np.int32)
        trn_label = train_label.cpu().numpy()

        precision(trn_binary, trn_label, tst_binary, tst_label) #evaluate by mAP
    else:
        print("Please first run database.py to prepare the data")
