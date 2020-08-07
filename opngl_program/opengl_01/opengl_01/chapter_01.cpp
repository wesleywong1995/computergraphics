#include <glad\glad.h> 
#include <GLFW\glfw3.h>

#include <iostream>
// 回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 设置视口大小
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	// 检索到esc按钮输入后关闭按钮
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// mac必须
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//创建窗口对象（包含窗口相关的数据）
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 告知glfw为当前窗口为线程的主上下文
	glfwMakeContextCurrent(window);

	// GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 窗口大小发生变化的时候，视口大小也发生变化（渲染大小）
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 输入
		processInput(window);
		
		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	// 这两句搭配使用，一个是设置状态一个是使用状态


		// 检查并调用事件，交换缓冲
		glfwPollEvents();
		glfwSwapBuffers(window);	// 双缓冲
	}

	// 释放之前用到的资源
	glfwTerminate();
	return 0;
}