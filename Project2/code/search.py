import os
import argparse

import numpy as np
from scipy.spatial.distance import hamming, cdist
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

def load_data():
    transform_train = transforms.Compose(
        [transforms.Resize(227),
         transforms.CenterCrop(227),
         transforms.ToTensor(),
         transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010))])
    transform_test = transforms.Compose(
        [transforms.Resize(227),
         transforms.CenterCrop(227),
         transforms.ToTensor(),
         transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010))])
    trainset = MyDataset(txt='./data/image/trainlist.txt', transform=transform_train)
    trainloader = torch.utils.data.DataLoader(trainset, batch_size=100,
                                              shuffle=False, num_workers=2)

    testset = MyDataset(txt='./data/image/testlist.txt', transform=transform_test)
    testloader = torch.utils.data.DataLoader(testset, batch_size=100,
                                             shuffle=False, num_workers=2)
    return trainloader, testloader

def binary_output(dataloader):
    net = AlexNetPlusLatent(args.bits)
    net.load_state_dict(torch.load('./model/86.7'))
    use_cuda = torch.cuda.is_available()
    if use_cuda:
        net.cuda()
    full_batch_hash = torch.cuda.FloatTensor()
    full_batch_label = torch.cuda.LongTensor()
    full_batch_feature = torch.cuda.FloatTensor()
    net.eval()
    for batch_idx, (inputs, targets) in enumerate(dataloader):
        if use_cuda:
            inputs, targets = inputs.cuda(), targets.cuda()
        inputs, targets = Variable(inputs, volatile=True), Variable(targets)
        hash,result,feature = net(inputs)
        full_batch_hash = torch.cat((full_batch_hash, hash.data), 0)
        full_batch_label = torch.cat((full_batch_label, targets.data), 0)
        full_batch_feature = torch.cat((full_batch_feature, feature.data), 0)
    return torch.round(full_batch_hash), full_batch_label, full_batch_feature

def precision(trn_binary, trn_label, trn_feature, tst_binary, tst_label, tst_feature):
    trn_binary = trn_binary.cpu().numpy()
    trn_binary = np.asarray(trn_binary, np.int32)
    trn_label = trn_label.cpu().numpy()
    trn_feature = trn_feature.cpu().numpy()
    tst_binary = tst_binary.cpu().numpy()
    tst_binary = np.asarray(tst_binary, np.int32)
    tst_label = tst_label.cpu().numpy()
    tst_feature = tst_feature.cpu().numpy()
    query_times = tst_binary.shape[0]
    trainset_len = train_binary.shape[0]
    AP = np.zeros(query_times)
    Ns = np.arange(1, trainset_len + 1)
    total_time_start = time.time()
    for i in range(query_times):
        print('Query ', i+1)
        query_label = tst_label[i]
        query_binary = tst_binary[i,:]
        query_result = np.count_nonzero(query_binary != trn_binary, axis=1)    #don't need to divide binary length
        sort_indices = np.argsort(query_result)
        buffer_yes= np.equal(query_label, trn_label[sort_indices]).astype(int)
        P = np.cumsum(buffer_yes) / Ns
        AP[i] = np.sum(P * buffer_yes) /sum(buffer_yes)
    map = np.mean(AP)
    print(map)
    print('total query time = ', time.time() - total_time_start)


if __name__ == '__main__':
        train_binary = torch.load('./database/db_binary')
        train_label = torch.load('./database/db_label')
        train_feature = torch.load('./database/db_feature')
        test_binary = train_binary
        test_label = train_label
        test_feature = train_feature
        precision(train_binary, train_label, train_feature, test_binary, test_label, test_feature)
