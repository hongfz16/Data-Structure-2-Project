#DeepHash Based Image Retrieval

##Introduction

本项目通过深度哈希值的方法来实现快速图片检索，大致流程为

- 准备一个训练图片集和一个测试图片集，为每个图片集准备一个存有图片名和图片类别的txt文件
- 运行``mytrain.py``文件进行模型训练，训练前应修改``MyDatabase.py``中的图片集路径，使得程序能根据该路径和txt文件中的文件名找到图片，训练结束后程序将输出一个二进制模型文件，得到模型后便可进行查询等操作
- 进行查询前需运行``database.py``文件将查询图片集和数据库图片集输入模型，得到二进制哈希串和特征数据；输入前也需准备图片名列表，并修改``MyDataset.py``中的图片集路径
- 得到二进制哈希串和特征数据后可运行``query.py``或``demo.py``进行查询和展示，运行前需先设置上一步得到的哈希串文件和特征文件的路径

## Environment

> Pytorch 0.4.0
>
> CUDA 9.2
>
> Torchvision 0.2.1

##MyDataset.py

定义了数据集类，该类在其他文件中被调用，用于将图片转换为数据集。使用时需将该文件中``root``路径设为被转换的图片集路径。

## net.py

定义了``AlexNextPlusLatent``网络，继承于``alexnet``，用于模型训练。

## mytrain.py

用于进行模型训练，使用方法为：

```shell
python mytrain.py
```

使用时需设置包含训练图片集的图片名列表的txt文件路径和包含测试图片集的图片名列表的txt文件路径，其中包含图片列表的txt文件包含若干行，每行包括一张图片的图片名和该图片的类别，两者用空格隔开，如：

> n01613177_1751.JPEG n01613177
> n01613177_307.JPEG n01613177
>
> ……

这些图片的根目录应与``net.py``文件中的``root``路径一致。

最后输出为一个二进制文件，默认保存在``./model/``文件夹下，文件名为该模型的测试准确度。

## mAP.py

用于计算模型的均值平均精度（mean Average Precision），使用时需设置测试的图片名列表，方法与``mytrain.py``一致。

在``binary_output``函数中需设置模型路径，该路径为``mytrain.py``输出的路径。

最后输出为

## database.py

用于获取图片检索所需的二进制哈希串和二进制特征文件。

在``load_data``函数中需设置所需建立的数据集的图片集的图片名列表，方法与之前一直。

在``getDatabase``函数中需设置模型路径，方法与``mAP.py``中一致。

最后输出为三个二进制文件``binary``，``label``，``feature``，分别表示该数据集中每张图对应的二进制哈希串、类别和特征。

## search.py

用于试验不同汉明距离对搜索效果的影响。

使用时需设置查询数据集和数据库数据集的二进制哈希串文件和类别文件路径，这几个二进制文件由``database.py``对不同的图片集进行处理而得。

## query.py

用于在数据库中对给定的查询数据集进行查询。

使用时需在``readImgNames``函数中设置包含数据库图片的文件名列表的txt文件路径和包含查询图片的文件名列表的txt文件路径。

在``main``函数中需设置查询数据集和数据库数据集的二进制哈希串文件和类别文件路径，这几个二进制文件由``database.py``对不同的图片集进行处理而得。

结果将输出到由用户在``resultfd``变量中指定的某个txt文件。

## demo.py

用于展示查询结果。

使用方法与``query.py``大致相同，且需在``main``函数中设置数据库图片的路径和查询图片的路径。