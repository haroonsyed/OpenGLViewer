#include "inputController.h"
#include <iostream>

InputController::InputController(GLFWwindow* window) {
    this->window = window;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void InputController::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    double timeSinceLastInput = glfwGetTime() - timeAtLastInput;

    if (timeSinceLastInput > 1.0/60) { // 60 Hz polling rate

        // Reset time since last input
        timeAtLastInput = glfwGetTime();


        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            viewTransform = glm::scale(viewTransform, glm::vec3(1.02,1.02,1.02)); // ZOOM IN
        }
        else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            viewTransform = glm::scale(viewTransform, glm::vec3(0.98, 0.98, 0.98)); // ZOOM IN
        }

        //Mouse input for rotation...

    }

}

char InputController::getPressedKey()
{
    return pressedKey;
}

glm::mat4 InputController::getViewTransform() {
    return viewTransform;
}

