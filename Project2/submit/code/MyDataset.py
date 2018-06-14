# class MyDataset similar to class Dataset in torch

from torchvision import transforms
from torch.utils.data import Dataset
from PIL import Image

def default_loader(path):
    return Image.open(path).convert('RGB')

class MyDataset(Dataset):
    def __init__(self, txt, imagepath, transform = None, target_transform = None,
                 loader = default_loader):
        fh = open(txt, 'r')
        imgs = []
        labeldic = dict()  # a dictionary, label name -> label index
        lastlabel = ''
        labelnum = 0
        for line in fh:
            line = line.strip('\n')
            line = line.rstrip()
            words = line.split()
            if len(words) > 1:
                if words[1] != lastlabel:   # a new label
                    lastlabel = words[1]
                    labelnum += 1
                    labeldic[lastlabel] = labelnum
            imgs.append((words[0], labelnum))
        self.imgs = imgs
        self.transform = transform
        self.target_transform = target_transform
        self.loader = loader
        self.labeldic = labeldic
        self.imagepath=imagepath

    def __getitem__(self, index):
        fn, label = self.imgs[index]
        img = self.loader(self.imagepath+fn)
        if self.transform is not None:
            img = self.transform(img)
        return img, label

    def __len__(self):
        return len(self.imgs)
