```shell
#gcc g++ make
sudo apt install build-essential
```

wsl开发配置
由于wsl没有图像界面，需要用显示window上
window上下载VcXsrv
之后win上打开服务器
设置环境变量
```shell
vim ~/.bashrc

#添加一行
export DISPLAY=:0.0
#或者直接命令行
export DISPLAY=:0.0
```
```shell
#安装x11-apps测试是否正常
sudo apt install x11-apps mesa-utils
glxgears
```
配置OPenGL版本为3.3以上
```shell
#查看版本
glxinfo | grep "OpenGL version"
#由于wsl的OpenGL版本过低，我们可以不用本地OpenGL
#设置环境变量
export MESA_GL_VERSION_OVERRIDE=3.3 
#不启用本地OpenGL
unset LIBGL_ALWAYS_INDIRECT
#最后别忘了把VcXsrv设置为不启用native(本地)OpenGL
```