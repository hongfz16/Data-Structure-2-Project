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
    dbset = MyDataset(txt='./data/image/list.txt', transform=transform_train)
    dbloader = torch.utils.data.DataLoader(dbset, batch_size=100,
                                              shuffle=False, num_workers=2)

    return dbloader

def getDatabase(dataloader):
    net = AlexNetPlusLatent(args.bits)
    net.load_state_dict(torch.load('./model/82.9'))
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

if __name__ == '__main__':

        dbloader = load_data()
        db_binary, db_label, db_feature = getDatabase(dbloader)
        if not os.path.isdir('database'):
            os.mkdir('database')
        torch.save(db_binary, './database/db_binary')
        torch.save(db_label, './database/db_label')
        torch.save(db_feature, './database/db_feature')
