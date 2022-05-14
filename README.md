
**通告：本项目已发布可运行程序，包含依赖库，有兴趣的兄弟可以下载试用 https://github.com/dengqizhou30/AIAssistOpenCV/releases ；**<br><br>


**游戏助手已发布三个项目，区别如下：**<br>
AIAssist 是c#版的游戏游戏助手，已不再更新，感兴趣的可以参考 https://github.com/dengqizhou30/AIAssist <br>
AIAssistC 是c++版本的AIAssist，原因是c#的版本无法实现显卡CUDA加速等底层优化，这个版本也不再更新，感兴趣的可以参考 https://github.com/dengqizhou30/AIAssistC <br>
AIAssistOpenCV 是c++重构简化版本，在AIAssistC基础上做了重构简化，主要考虑方便使用，拷贝一个opencv dll就可以跑起来了。<br>


**重构简化内容：** <br>
1、简化AI推理依赖的库，删除tensorflow库，只依赖opencv dnn；<br>
2、编译opencv，调整参数针对AMD CPU进行优化； opencv本地编译的时候，会自动检测硬件是否支持指令集优化,应该有一定性能优化；<br>
3、编译opencv，尝试启用opencl，用于opencv dnn推理加速；适用于intel显卡，实测n卡启用opencl性能比cpu还低；<br>
4、编译opencv，尝试启用cuda，用于opencv dnn推理加速。以前反复尝试都失败了，这次用opencv新版本再试试；又花了一些时间验证cuda推理加速，还是失败，intel的opencv dnn和N卡的对接很不顺畅，避免在这个技术方向上尝试，非常浪费时间；<br>
5、使用DXGI Desktop Duplication API优化截屏性能；验证发现目前不能指定截取区域，只能截取全屏，性能不稳定，大部分时候比GDI函数BitBlt截取部分区域还慢。<br>

**推理加速方向：** <br>
1、使用CPU推理，配置多核CPU，能上两块物理CPU更佳，可以使用一块CPU专用于推理；<br>
2、使用GPU推理，优先选择intel gpu，opencv支持得更好，能上一块专用得intel显卡用于推理最好；<br>
<br>


**一、项目说明：**<br>
AIAssistC是一个AI游戏助手，使用OpenCv、DNN、tensorflow c api、cppflow、ssd_mobilenet/efficientdet、hidriver、MFC等技术，截取游戏屏幕，使用AI模型进行对象识别，并实现自动瞄准/自动开枪等鼠标操作，提升玩家的游戏体验。<br>
<br>

**二、工程说明：**<br>
AIAssist：mfc前端UI子工程；<br>
AIAssistLib：AI助手静态库子工程；<br>
Data：存放模型文件及工具文件的子工程；<br>
DXGICaptureSample：DXGI Desktop Duplication API截屏测试子工程；<br>
OpencvTest：openc功能验证测试子工程；<br>
<br>

**三、主要的运行库：**<br>
1、intel贡献的大神级图像处理框架OpenCv：<br>
https://opencv.org/ <br>

OpenCV DNN 模块目前支持多种AI对象检测模型推理运行：<br>
https://github.com/opencv/opencv/tree/master/samples/dnn <br>
https://github.com/opencv/opencv/wiki/TensorFlow-Object-Detection-API <br>
https://github.com/openvinotoolkit/open_model_zoo <br>

2、谷歌tensoflow的对象检测模型efficientdet-lite0、ssd_mobilenet_v3： <br>
实测gpu加速效果，efficientdet-lite模型比ssd_mobilenet好很多 <br>
https://hub.tensorflow.google.cn/tensorflow/efficientnet/lite0/classification/2 <br>
https://github.com/tensorflow/models <br>
<br>

**四、使用注意：**<br>
1、使用技巧：目前基于AI图像检测，只做到了人员识别，无法区分敌我。为避免游戏中自动追踪并射击队友的尴尬，参考如下使用技巧。习惯这些操作方式后，这个工具使用体验相对好一些。:<br>
a、按其他数字键停止自动追踪：目前在工具中加入了控制逻辑，只有切换到主副武器时才会运行自动追踪和自动射击，切换到其他武器时不会自动追踪。
所以在捡枪或者跟队友跑时，切换到手枪或其他数字键，停止自动追踪。对敌时再切换到主副武器（数字键1或2），自动追踪才生效。<br>
b、避免追踪游戏者自己：目前无法区分识别出来的图像是自己还是其他人，钢枪或者搜索时切换为T第一人称视图，避免检测到游戏者自己。<br>

2、使用windows hook及鼠标键盘api实现了鼠标键盘操作模拟，HIDDriver驱动程序不再是必须项。在绝地求生、逆战、穿越火线三个游戏上测试，可以正常工作。推测现在的游戏安全检测侧重游戏内部的行为数据检测，游戏外部环境中windwos hook等通用操作机制不再管控<br>

3、HIDDriver鼠标键盘模拟驱动已移除，这个驱动没有微软颁发的正式证书，只能在win 10测试模式下执行。如果要尝试，参考项目说明： <br>
https://github.com/dengqizhou30/HIDDriver <br>
<br>

**五、游戏截图：**<br>
穿越火线游戏截图：<br>
![blockchain](https://github.com/dengqizhou30/AIAssistOpenCV/blob/master/Data/img/chuanyuehuoxian21.png)</br>
![blockchain](https://github.com/dengqizhou30/AIAssistOpenCV/blob/master/Data/img/chuanyuehuoxian2.png)</br>
