#include "ParticleSimulation.h"
#include <iostream>
#include <memory>

class Application {
private:
    GLFWwindow* window;
    std::unique_ptr<ParticleSimulation> simulation;
    
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            app->simulation->handleMouseDrag(xpos, ypos);
        }
    }
    
    void handleInput() {
        // Material selection keys
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            simulation->setMaterial(MaterialType::SAND);
        }
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            simulation->setMaterial(MaterialType::WATER);
        }
        else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            simulation->setMaterial(MaterialType::FIRE);
        }
        else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
            simulation->setMaterial(MaterialType::SNOW);
        }
        else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            simulation->setMaterial(MaterialType::WATER_SPRAY);
        }
        
        // Exit on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
    
public:
    Application() : window(nullptr) {}
    
    ~Application() {
        cleanup();
    }
    
    bool initialize() {
        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        
        // Create simulation
        simulation = std::make_unique<ParticleSimulation>();
        
        // Create window
        window = glfwCreateWindow(
            simulation->getWidth(), 
            simulation->getHeight(), 
            "Particle Physics Simulation", 
            nullptr, 
            nullptr
        );
        
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }
        
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, this);
        
        // Set callbacks
        glfwSetCursorPosCallback(window, mouseCallback);
        
        // Initialize simulation
        if (!simulation->initialize()) {
            std::cerr << "Failed to initialize particle simulation" << std::endl;
            return false;
        }
        
        // Enable VSync
        glfwSwapInterval(1);
        
        std::cout << "Particle Physics Simulation Controls:" << std::endl;
        std::cout << "S - Sand particles" << std::endl;
        std::cout << "W - Water particles" << std::endl;
        std::cout << "F - Fire particles" << std::endl;
        std::cout << "N - Snow particles" << std::endl;
        std::cout << "O - Water spray particles" << std::endl;
        std::cout << "Mouse - Hold left button and drag to create particles" << std::endl;
        std::cout << "ESC - Exit" << std::endl;
        
        return true;
    }
    
    void run() {
        while (!glfwWindowShouldClose(window)) {
            handleInput();
            
            simulation->update();
            simulation->render();
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    
    void cleanup() {
        if (simulation) {
            simulation->cleanup();
            simulation.reset();
        }
        
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        
        glfwTerminate();
    }
};

int main() {
    Application app;
    
    if (!app.initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }
    
    app.run();
    
    return 0;
}
