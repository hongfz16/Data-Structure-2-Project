# our net based on AlexNet

import torch.nn as nn
from torchvision import models

alexnet_model = models.alexnet(pretrained=True)


class AlexNetPlusLatent(nn.Module):
    def __init__(self, bits):
        super(AlexNetPlusLatent, self).__init__()
        self.bits = bits
        self.features = nn.Sequential(*list(alexnet_model.features.children()))
        self.remain = nn.Sequential(*list(alexnet_model.classifier.children())[:-1])
        self.Linear1 = nn.Linear(4096, self.bits)  # to extract feature
        self.sigmoid = nn.Sigmoid()  # to gain value between 0 and 1 and round to binary hash code
        self.Linear2 = nn.Linear(self.bits, 10)  # output layer

    def forward(self, x):
        x = self.features(x)
        x = x.view(x.size(0), 256 * 6 * 6)
        x = self.remain(x)
        feature = self.Linear1(x)  # feature for distance measure in fine retrieval
        hash = self.sigmoid(feature)   # hash code for coarse retrieval
        result = self.Linear2(hash)
        return hash, result, feature
