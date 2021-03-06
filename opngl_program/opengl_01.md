### 你好，三角形

可以编程的着色器：顶点着色器，片段着色器

必须要有的着色器：顶点着色器，片段着色器

**注意代码的顺序，先处理shader，再处理定点对象VBO和顶点数组对象VAO， 而且都应该写在while循环外面**

**图形渲染管线**可以被划分为两个主要部分：第一部分把你的3D坐标转换为2D坐标，第二部分是把2D坐标转变为实际的有颜色的像素。*2D坐标和像素也是不同的，2D坐标精确表示一个点在2D空间中的位置，而2D像素是这个点的近似值，2D像素受到你的屏幕/窗口分辨率的限制*。（***就像是画直线的那个算法一样，拟合的***）

![img](https://learnopengl-cn.github.io/img/01/04/pipeline.png)

最初输入的是3d的位置坐标（说到坐标就会对应到坐标系，坐标系就会对应到空间，不同的坐标系不能等同）经过一系列的步骤，每一个的步骤都会产生输出，输出作为下一个处理步骤的输入，最终得到像素。

##### 数据装配的步骤：

**顶点着色器(Vertex Shader)**，它把一个单独的顶点作为输入。顶点着色器主要的目的是把3D坐标转为另一种3D坐标（标准化设备坐标），同时顶点着色器允许我们对顶点属性进行一些基本处理。

**图元装配(Primitive Assembly)**阶段将顶点着色器输出的所有顶点作为输入（如果是GL_POINTS，那么就是一个顶点），并所有的点装配成指定图元的形状；

**光栅化阶段(Rasterization Stage)**这里它会把图元映射为最终屏幕上相应的像素，生成供片段着色器(Fragment Shader)使用的片段(Fragment)。在片段着色器运行之前会执行**裁切(Clipping)**。裁切会丢弃超出你的视图以外的所有像素，用来提升执行效率。

**片段着色器**的主要目的是计算一个像素的最终颜色，这也是所有OpenGL高级效果产生的地方。通常，片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。

**Alpha测试和混合(Blending)阶段**这个阶段检测片段的对应的深度（和模板(Stencil)）值（后面会讲），用它们来判断这个像素是其它物体的前面还是后面，决定是否应该丢弃。这个阶段也会检查alpha值（alpha值定义了一个物体的透明度）并对物体进行混合(Blend)。所以，即使在片段着色器中计算出来了一个像素输出的颜色，在渲染多个三角形的时候最后的像素颜色也可能完全不同。

**标准化设备坐标(Normalized Device Coordinates)范围内的坐标才会最终呈现在屏幕上（在这个范围以外的坐标都不会显示），这个范围是[-1, 1]，包含三个方向的数值，x， y，z（depth），由于OpenGL是在3D空间中工作的，而我们渲染的是一个2D三角形，我们将它顶点的z坐标设置为0.0。这样子的话三角形每一点的深度(Depth)都是一样的，从而使它看上去像是2D的。**

坐标系的变化过程(粗略待详细)：

顶点着色器->(标准化设备坐标)->过视口变换到(屏幕空间坐标)->变换成片段到片段着色器



-----------------------------------------------------------------------------------------------------------------------------------------------------

##### 顶点着色器处理步骤：

~~~cpp
//3D坐标的定义
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

//注意，这里的单位是float而不是直接定义的一个点，这个和gl的处理方式有关，看后面
~~~

1. 把顶点数据发送到Gpu，绑定到顶点缓冲对象

*我们通过**顶点缓冲对象(Vertex Buffer Objects, VBO)**管理这个内存（**注意：他不是单独的点的对象，他管理的是顶点的一个集合**），它会在GPU内存（通常被称为显存）中储存大量顶点。使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。从CPU把数据发送到显卡相对较慢，所以只要可能我们都要尝试尽量一次性发送尽可能多的数据。当数据发送至显卡的内存中后，顶点着色器几乎能立即访问顶点，这是个非常快的过程。*

具体的代码做法参照learnOpengl

~~~cpp
// 我感觉opengl里面所有的对象都是这样子声明的，先声明一个对象的id，然后通过glGenBuffer生成VBO对象第一个参数是生成的对象的数量，第二个数用来存缓冲对象名称的数组，这里生成的仅仅是对象名称而已，还不知道是什么对象，需要通过其他步骤指定
unsigned int VBO;
glGenBuffers(1, &VBO);

// 指定之前生成的对象是顶点缓冲对象
glBindBuffer(GL_ARRAY_BUFFER, VBO);  

//把之前的顶点数据传到顶点对象，最后一个参数是表示GL_STATIC_DRAW ：数据不会或几乎不会改变。GL_DYNAMIC_DRAW：数据会被改变很多。GL_STREAM_DRAW ：数据每次绘制时都会改变。
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
~~~

2. 顶点着色器（通过GLSL编程）处理传进来的顶点数据

**顶点着色器：**定义这样的顶点数据**vertices**以后，我们会把它作为输入发送给图形渲染管线的第一个处理阶段：顶点着色器。它会在GPU上创建内存用于储存我们的顶点数据，还要配置OpenGL如何解释这些内存，并且指定其如何发送给显卡。接下来顶点着色器接着会处理我们在内存中指定数量的顶点。

~~~glsl
#version 330 core //当前的opengl版本
layout (location = 0) in vec3 aPos; //这一句不怎么懂，后面回顾

void main()
{
    //当前这个顶点着色器可能是我们能想到的最简单的顶点着色器了，因为我们对输入数据什么都没有处理就把它传到着色器的输出了。在真实的程序里输入数据通常都不是标准化设备坐标，所以我们首先必须先把它们转换至OpenGL的可视区域内。
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
~~~

3. 编译着色器(为了能够让opneGL利用他)

~~~cpp
// 创建着色器对象,这里并不知道着色器的类型
unsigned int vertexShader;
vertexShader = glCreateShader(GL_VERTEX_SHADER);

// 指定着色器的类型，vertexShaderSource就是glsl的代码
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
// 编译着色器
glCompileShader(vertexShader);
~~~



-----------------------------------------------------------------------------------------------------------------------------------------------------

##### 片段着色器处理步骤：

片段着色器所做的是计算像素最后的颜色输出。

大致步骤同上面的顶点着色器：

唯一的不同是这里的glsl用的是out关键字(输出变量)，顶点着色器用的是in关键字（在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)）

~~~glsl
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
~~~





~~~cpp
//创建着色器对象
unsigned int fragmentShader;
fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

//指定着色器类型
glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//编译着色器
glCompileShader(fragmentShader);
~~~



-----------------------------------------------------------------------------------------------------------------------------------------------------

**处理所有的着色器**

着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。如果要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，然后在渲染对象的时候激活这个着色器程序。已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。

当链接着色器至一个程序的时候，它会把每个着色器的输出链接到下个着色器的输入。当输出和输入不匹配的时候，你会得到一个连接错误。



##### 步骤：

**创建一个着色器程序对象**

~~~cpp
unsigned int shaderProgram;
shaderProgram = glCreateProgram();
~~~

**附加之前创建的顶点着色器和片段着色器到着色器程序对象上面**

~~~cpp
//附加shader到着色器对象（整合）上面
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
//链接着色器
glLinkProgram(shaderProgram);
~~~

**激活刚刚创建的着色器对象**

~~~cpp
glUseProgram(shaderProgram);
~~~

**删除之前创建的顶点着色器和片段着色器对象**

​	**我的理解是**，一个着色器对象对应管理的是gpu中的一段内存， 而gpu是流水线处理的这里需要手动的释放掉

~~~cpp
// 删除顶点着色器对象
glDeleteShader(vertexShader);
// 删除片段着色器对象
glDeleteShader(fragmentShader);
~~~

-----------------------------------------------------------------------------------------------------------------------------------------------------

##### 链接顶点属性

顶点着色器允许我们指定任何以顶点属性为形式的输入。这使其具有很强的灵活性的同时，它还的确意味着我们必须手动指定输入数据的哪一个部分对应顶点着色器的哪一个顶点属性。所以，我们必须在渲染前指定OpenGL该如何解释顶点数据。



​	**我的理解：**

​	之前声明的是**vertices**是**float**数组，而不像是在游戏引擎里面的点的集合，所以需要告诉顶点属性到哪里算是一个点，这里是3个浮点数是一个点，而且这里没有间隙，紧密排列

~~~cpp
// 告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
// 第五个参数是偏移量，最后一个也是偏移量表示数据在缓冲中起始位置的偏移量
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// 
glEnableVertexAttribArray(0);
~~~



~~~cpp
// 0. 复制顶点数组到缓冲中供OpenGL使用
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 1. 设置顶点属性指针
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
// 2. 当我们渲染一个物体时要使用着色器程序
glUseProgram(shaderProgram);
// 3. 绘制物体
someOpenGLFunctionThatDrawsOurTriangle();
~~~





