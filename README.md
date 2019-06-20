## 示例：Windows 异步读取文件

在Windows环境下，采用异步的方式读取文件的一个例子。

base库——基础库，封装了线程模型，MessageLoop。实现了命令模式，C++闭包

file_fetcher——封装了异步读文件的实现逻辑

## 编译环境
VS2008 下通过

## 编译步骤
需要包含工程的 WTL

Tools->Options->Projects and Solutions->VC++ Directories

选择 Include files

添加 WTL 目录
