# imageAutoClass 对压缩文件的自动分类
部分构建自zlib库。
## 编译

```shell
  g++ -o main.exe main.cpp ioapi.c unzip.c libz.a libz.dll.a
```

## 使用方法
对压缩文件内文件名为`***_***_**.***`格式的文件进行自动分类。将压缩包拖到软件图标上运行即可。
