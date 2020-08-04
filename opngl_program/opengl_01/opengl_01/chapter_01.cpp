#include <glad\glad.h> 
#include <GLFW\glfw3.h>

#include <iostream>
// �ص�����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// �����ӿڴ�С
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	// ������esc��ť�����رհ�ť
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// mac����
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//�������ڶ��󣨰���������ص����ݣ�
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// ��֪glfwΪ��ǰ����Ϊ�̵߳���������
	glfwMakeContextCurrent(window);

	// GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ���ڴ�С�����仯��ʱ���ӿڴ�СҲ�����仯����Ⱦ��С��
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ����
		processInput(window);
		
		// ��Ⱦָ��
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	// ���������ʹ�ã�һ��������״̬һ����ʹ��״̬


		// ��鲢�����¼�����������
		glfwPollEvents();
		glfwSwapBuffers(window);	// ˫����
	}

	// �ͷ�֮ǰ�õ�����Դ
	glfwTerminate();
	return 0;
}