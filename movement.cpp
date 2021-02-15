
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
const char* vertex_shader =
"#version 430 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 texture_coordinates;\n"
"out vec2 new_texture_coordinates;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"new_texture_coordinates = texture_coordinates;\n"
"gl_Position = projection * view * model * vec4(position, 1);\n"
"}\n";
const char* fragment_shader =
"#version 430 core\n"
"out vec4 filled_textured;\n"
"in vec2 new_texture_coordinates;\n"
"uniform sampler2D tex;\n"
"void main()\n"
"{\n"
"filled_textured = texture(tex, new_texture_coordinates);\n"
"}\n";
// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//origin where camera is

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

//facing negative because thats where our box is
//where our camera is will face in look at matrix


glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//which way is our camera up


void framebuffer_size_callback(GLFWwindow* pointer_to_window, int width, int height);
void processInput(GLFWwindow * pointer_to_window);
int main() {
	
	glfwInit();
	GLFWwindow* pointer_to_window = glfwCreateWindow(800, 600, "E", NULL, NULL);
	if (pointer_to_window == NULL)
	{
		cout << "terminate";
	}
	
	glfwMakeContextCurrent(pointer_to_window);
	glewInit();
	glfwSetFramebufferSizeCallback(pointer_to_window, framebuffer_size_callback);
	int vertex = glCreateShader(GL_VERTEX_SHADER);
	int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char* pointer_to_strings[1];
	pointer_to_strings[0] = vertex_shader;
	glShaderSource(vertex, 1, pointer_to_strings, 0);
	pointer_to_strings[0] = fragment_shader;
	glShaderSource(fragment, 1, pointer_to_strings, 0);
	glCompileShader(vertex);
	glCompileShader(fragment);
	int program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	glEnable(GL_DEPTH_TEST);

	float vertices[]{
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * 4, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * 4, (void*)(3 * 4));


	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width,  height, nrchannels;
	unsigned char* data = SOIL_load_image("resources/murkey.jpg", &width, &height, &nrchannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	while(!glfwWindowShouldClose(pointer_to_window))
	{ 
		
	;
	
		// input
		processInput(pointer_to_window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, texture);
		

		// create transformations
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		

		//THIS IS THE MATRIX THAT DEFINES THE CAMERA 
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        //FUNCTION FIRST TAKES IN
		//CAMERAPOS FOR ORIGIN
		//WE NEED THE PLACE WHERE THE CAMERA WILL LOOK SO WE NEED CAMERA FRONT
		// UP DIRECTION WIll NEVER CHANGE
		//OUR CAMERA POSITION WILL CHANGE THOUGH BASED ON CAMERA FRONT
		//OR OUR OWN KEYBOARD
		
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
		
		
		glUseProgram(program);
		unsigned int modelLoc = glGetUniformLocation(program, "model");
		unsigned int viewLoc = glGetUniformLocation(program, "view");
		unsigned int projectionLoc = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE,  glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(VAO);
		glfwPollEvents();
		glfwSwapBuffers(pointer_to_window);
	};
	glfwTerminate();
	return 0;

}
void processInput(GLFWwindow* pointer_to_window)
{
	//closes window
	if (glfwGetKey(pointer_to_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(pointer_to_window, true);

	float cameraSpeed = 0.003; //THIS IS A NICE SPEED

	if (glfwGetKey(pointer_to_window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos = cameraPos + cameraSpeed * cameraFront;
	//Camera Position Plus DIRECTION CAMERA FACES, * CAMERA SPEED
	// will equal the new camera POS vector
	
	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	//this new vec3 = original vec3 + (speed times, the direction vector which faces negative)
	//0.0f               0.0f
	//0.0f  + (0.003 *   0.0f )
	//3.0f              -1.0f 
	if (glfwGetKey(pointer_to_window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos = cameraPos - cameraSpeed * cameraFront;
	//0.0f            0.0f
	//0.0f - (0.003 * 0.0f )
	//0.0f           -1.0f 


	if (glfwGetKey(pointer_to_window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos = cameraPos - glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	//so the cross product of cameraFront and camera up * camera speed
	//a x b
	//   0.0f     0.0      1
	//   0.0f  *  1.0   =  0 the world moves from, or the box   * 0.03 = x
	
	// - 1.0f     0.0      0
	

	//0.0f                       
	//0.0f   -      x        =     new position that world moves to 
	//3 f                        
	if (glfwGetKey(pointer_to_window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos = cameraPos + glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;



	//so the cross product of cameraFront and camera up * camera speed
	//a x b
	//   0.0f     0.0      1
	//   0.0f  *  1.0   =  0 the world moves from, or the box   * 0.03 = x

	// - 1.0f     0.0      0


	//0.0f                       
	//0.0f   +      x        =     new position that world moves to 
	//3 f         
	//this might actually just change all the vectors not just camera position as i am looking at my graph
}
void framebuffer_size_callback(GLFWwindow* pointer_to_window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
