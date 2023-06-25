使用SDL2实现的扫雷游戏

[制作实况录播](https://www.bilibili.com/video/BV1zF411d7y5/?share_source=copy_web&vd_source=e1b8baee842192a0e6b2b7d9ef8e10ef)

## 操控

鼠标左键翻开格子，右键插旗子。

显示出失败/胜利图标后再次点击鼠标可以重开一局。

## 截图

![snapshot](./snapshot/snapshot.png)

## 编译

Windows下使用CMake可以进行编译：

```bash
cmake -S . -B cmake-build -DSDL2_ROOT=<your SDL path>
cmake --build cmake-build
```
