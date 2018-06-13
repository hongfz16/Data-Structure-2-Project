# image retrieval from database, input binary and feature

import argparse
import numpy as np
import torch
import os

parser = argparse.ArgumentParser(description='Deep Hashing evaluate mAP')
parser.add_argument('--pretrained', type=int, default=0, metavar='pretrained_model',
                    help='loading pretrained model(default = None)')
parser.add_argument('--bits', type=int, default=64, metavar='bts',
                    help='binary bits')
args = parser.parse_args()


# store the image names by index
def readImgNames():
    db_list = open("./list/dblist.txt",'r')
    db_name = []
    for line in db_list:
        line = line.strip('\n')
        line = line.rstrip()
        words = line.split()
        db_name.append(words[0])
    query_list = open("./list/querylist.txt",'r')
    query_name = []
    for line in query_list:
        line = line.strip('\n')
        line = line.rstrip()
        words = line.split()
        query_name.append(words[0])
    return db_name, query_name

db_name, query_name = readImgNames()


def EuclideanDistance(vec1,vec2):
    dist = np.sqrt(np.sum(np.square(vec1 - vec2)))
    return dist


def takeSecond(a):
    return a[1]

resultfd = open("./result/query.txt",'w+')

# image retrieval by binary hash code and feature
def query(trn_binary, trn_feature, tst_binary, tst_feature, HAMMINGDIS, QUERYNUM):
    trn_binary = trn_binary.cpu().numpy()
    trn_binary = np.asarray(trn_binary, np.int32)
    trn_feature = trn_feature.cpu().numpy()
    tst_binary = tst_binary.cpu().numpy()
    tst_binary = np.asarray(tst_binary, np.int32)
    tst_feature = tst_feature.cpu().numpy()
    query_times = tst_binary.shape[0]
    print(query_times)
    for i in range(query_times):
        query_binary = tst_binary[i,:]
        query_result = np.count_nonzero(query_binary != trn_binary, axis=1)
        sort_indices = np.argsort(query_result)
        candidate = []   # candidate set
        for j in sort_indices:
            if(query_result[j] < HAMMINGDIS):
                candidate.append((j,EuclideanDistance(trn_feature[j],tst_feature[i])))
            else:
                break
        candidate.sort(key=takeSecond)
        num=min(QUERYNUM,len(candidate))
        # output the top k=num results for each query
        resultfd.write(query_name[i])
        resultfd.write(':')
        for k in range(num):
            resultfd.write(db_name[candidate[k][0]])
            if not k == num - 1:
                resultfd.write(', ')
        resultfd.write("\n")


if __name__ == '__main__':
        db_binary = torch.load('./database/db_binary')
        db_feature = torch.load('./database/db_feature')
        query_binary = torch.load("./query/query_binary")
        query_feature = torch.load("./query/query_feature")
        query(db_binary,db_feature,query_binary,query_feature,20,10)  # Hamming dis <= 20, retrieve top 10 images
