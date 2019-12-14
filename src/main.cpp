#include <iostream>

#include <glad/glad.h> // Must include before GLFW
#include <GLFW/glfw3.h>

using namespace std;

int main(void) {
	
	if(!glfwInit()) {
		cout << "ERROR: GLFW Initialization failed, aborting." << endl;
		return -1;
	}
	
	cout << "Success! GLFW initialized." << endl;
	
	// Terminate to free memory and resources
	glfwTerminate();
	
	return 0;
}
