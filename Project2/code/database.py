# save the binary hash code and feature gained from net

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

modelpath='./model/86.7'

alldatalist=['./data/image/list.txt','./data/query/list.txt','./data/image/trainlist.txt','./data/image/testlist.txt']
imagepath=['./data/image/','./data/query/','./data/image/','./data/image/']
dataname=['db','query','train','test']


parser = argparse.ArgumentParser(description='Deep Hashing evaluate mAP')
parser.add_argument('--pretrained', type=int, default=0, metavar='pretrained_model',
                    help='loading pretrained model(default = None)')
parser.add_argument('--bits', type=int, default=48, metavar='bts',
                    help='binary bits')
args = parser.parse_args()

def load_data(datalist,_imagepath):
    transform_train = transforms.Compose(
        [transforms.Resize(227),
         transforms.CenterCrop(227),
         transforms.ToTensor(),
         transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010))])
    dbset = MyDataset(txt=datalist, imagepath=_imagepath, transform=transform_train)
    dbloader = torch.utils.data.DataLoader(dbset, batch_size=1,
                                              shuffle=False, num_workers=2)
    return dbloader

def getDatabase(dataloader):
    net = AlexNetPlusLatent(args.bits)
    net.load_state_dict(torch.load(modelpath,map_location='cpu')) # load trained model
    use_cuda = torch.cuda.is_available()
    if use_cuda:
        net.cuda()
        full_batch_hash = torch.cuda.FloatTensor()
        full_batch_label = torch.cuda.LongTensor()
        full_batch_feature = torch.cuda.FloatTensor()
    else:
        full_batch_hash = torch.FloatTensor()
        full_batch_label = torch.LongTensor()
        full_batch_feature = torch.FloatTensor()
    net.eval()
    for batch_idx, (inputs, targets) in enumerate(dataloader):
        if use_cuda:
            inputs, targets = inputs.cuda(), targets.cuda()
        inputs, targets = Variable(inputs, volatile=True), Variable(targets)
        hash,result,feature = net(inputs) # hash code, output of the net and feature
        full_batch_hash = torch.cat((full_batch_hash, hash.data), 0)
        full_batch_label = torch.cat((full_batch_label, targets.data), 0)
        full_batch_feature = torch.cat((full_batch_feature, feature.data), 0)
    return torch.round(full_batch_hash), full_batch_label, full_batch_feature  # round to get hash code with 0 and 1

if __name__ == '__main__':

    for i in range(len(alldatalist)):
        dbloader = load_data(alldatalist[i],imagepath[i])
        db_binary, db_label, db_feature = getDatabase(dbloader)
        if not os.path.isdir('database'):
            os.mkdir('database')
        torch.save(db_binary, './database/'+dataname[i]+'_binary')
        torch.save(db_label, './database/'+dataname[i]+'_label')
        torch.save(db_feature, './database/'+dataname[i]+'_feature')
