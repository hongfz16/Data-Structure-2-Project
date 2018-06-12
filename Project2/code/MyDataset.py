from torchvision import transforms
from torch.utils.data import Dataset
from PIL import Image
root = 'H:\python\cvpr\pytorch_deephash\data\image\\'

def default_loader(path):
    # print(root + path)
    return Image.open(root + path).convert('RGB')
class MyDataset(Dataset):
    def __init__(self, txt, transform = None, target_transform = None,
                 loader = default_loader):
        fh = open(txt, 'r')
        imgs = []
        labeldic = dict()
        lastlabel = ''
        labelnum = 0
        for line in fh:
            line = line.strip('\n')
            line = line.rstrip()
            words = line.split()
            # imgs.append((words[0], words[1]))
            # print(words[0], words[1])
            if len(words) > 1:
                if words[1] != lastlabel:
                    lastlabel = words[1]
                    labelnum += 1
                    labeldic[lastlabel] = labelnum
                imgs.append((words[0], labelnum))
        self.imgs = imgs
        self.transform = transform
        self.target_transform = target_transform
        self.loader = loader
        self.labeldic = labeldic

    def __getitem__(self, index):
        fn, label = self.imgs[index]
        img = self.loader(fn)
        # print(img)
        if self.transform is not None:
            img = self.transform(img)
        return img, label

    def __len__(self):
        return len(self.imgs)

# trainset = MyDataset(txt = root + 'list.txt', transform = transforms.ToTensor())