# DeepHash Based Image Retrieval

## Introduction

本项目通过深度哈希值的方法来实现快速图片检索，大致流程为

- 准备一个训练图片集和一个测试图片集，为每个图片集准备一个存有图片名和图片类别的txt文件
- 运行``mytrain.py``文件进行模型训练，训练前应修改``MyDatabase.py``中的图片集路径，使得程序能根据该路径和txt文件中的文件名找到图片，训练结束后程序将输出一个二进制模型文件，得到模型后便可进行查询等操作
- 进行查询前需运行``database.py``文件将查询图片集和数据库图片集输入模型，得到二进制哈希串和特征数据；输入前也需准备图片名列表，并修改``MyDataset.py``中的图片集路径
- 得到二进制哈希串和特征数据后可运行``query.py``或``demo.py``进行查询和展示，运行前需先设置上一步得到的哈希串文件和特征文件的路径
- 下面将会简述如何使用我们的代码，并且详述每个文件的作用

## Environment

> Pytorch 0.4.0
>
> CUDA 9.2
>
> Torchvision 0.2.1

## 使用方法

### 图片预处理

- 请将所有数据库图片置于`./data/image`文件夹下，并且在同目录下给出三个文件`list.txt`,`trainlist.txt`和`testlist.txt`，第一个文件内存放所有图片名，后两个文件内分别存放训练与测试图片文件名，以及每一个图片对应的类别，具体格式参见`mytrain.py`的详细使用方法；
- 请将查询请求图片置于`./data/query`文件夹下，并且在同目录下给出文件`list.txt`，文件内存放所有图片的文件名；

### 训练模型

直接在命令行中使用命令`python mytrain.py`，训练好的模型将会存储于`./model/`文件夹下，并且以当前准确率为文件名，注意下面部分源文件运行之前请先将对应文件中的`modelname`赋值为训练好模型的路径；

### 查询前预处理

运行`python database.py`，将会在`./database`文件夹下生成处理好的图片特征、哈希值、类别；

### 测试mAP

运行`python mAP.py`；

### 搜索合适的Hamming距离阈值

运行`python search.py`，根据输出可以选取一个最佳的阈值；

### 进行查询

运行`python query.py`，将会在`./result/`文件夹下给出查询结果文件

### Demo

运行`python demo.py`，将会显示某一次查询的图片，以及被查询出来的前十张图片

### 64位版本

请修改命令行参数`bits`为64，按照上述过程重新运行一遍

## 对应源文件介绍

### MyDataset.py

定义了数据集类，该类在其他文件中被调用，用于将图片转换为数据集。

### net.py

定义了``AlexNextPlusLatent``网络，继承于``alexnet``，用于模型训练。

### mytrain.py

用于进行模型训练，使用方法为：

```shell
python mytrain.py
```

使用时需设置包含训练图片集的图片名列表的txt文件路径和包含测试图片集的图片名列表的txt文件路径，其中包含图片列表的txt文件包含若干行，每行包括一张图片的图片名和该图片的类别，两者用空格隔开，如：

> n01613177_1751.JPEG n01613177
>
> n01613177_307.JPEG n01613177
>
> ……

最后输出为一个二进制文件，默认保存在``./model/``文件夹下，文件名为该模型的测试准确度。

### mAP.py

用于计算模型的均值平均精度（mean Average Precision)

### database.py

用于获取图片检索所需的二进制哈希串和二进制特征文件。

需要制定`modelpath`为训练好的模型路径

最后输出为三个二进制文件``binary``，``label``，``feature``，分别表示该数据集中每张图对应的二进制哈希串、类别和特征。

### search.py

用于试验不同汉明距离对搜索效果的影响。

### query.py

用于在数据库中对给定的查询数据集进行查询。

结果将输出到由用户在``resultfd``变量中指定的某个txt文件。

### demo.py

用于展示查询结果。

如果需要制定某个特定的图片进行查询，需要将`main`函数中的`num`变量赋值为某一制定序号

## Reference
[flyingpot/pytorch_deephash](https://github.com/flyingpot/pytorch_deephash)