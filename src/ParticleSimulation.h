#pragma once

#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <vector>
#include <memory>

enum class MaterialType {
    EMPTY = 0,
    SAND = 1,
    WATER = 2,
    FIRE = 3,
    SNOW = 4,
    WATER_SPRAY = 5
};

struct Particle {
    MaterialType type = MaterialType::EMPTY;
    int hue = 0;
    int velocity = 1;
    
    Particle() = default;
    Particle(MaterialType t, int h, int v = 1) : type(t), hue(h), velocity(v) {}
    
    bool isEmpty() const { return type == MaterialType::EMPTY; }
};

struct Color {
    float r, g, b;
    Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f) 
        : r(red), g(green), b(blue) {}
};

class ParticleSimulation {
private:
    static const int WIDTH = 1000;
    static const int HEIGHT = 700;
    static const int CELL_SIZE = 4;
    static const int SPAWN_MATRIX_SIZE = 5;
    static const int SPAWN_PROBABILITY = 75;
    
    int cols;
    int rows;
    int currentHue;
    MaterialType currentMaterial;
    
    std::vector<std::vector<Particle>> grid;
    std::vector<std::vector<Particle>> nextGrid;
    
    // Helper methods
    void initializeGrid();
    bool isValidPosition(int col, int row) const;
    bool isWithinBounds(int col) const;
    Color getMaterialColor(const Particle& particle) const;
    void clearGrid();
    void swapGrids();
    
    // Physics update methods
    void updatePowder();
    void updateFluid();
    void updateFire();
    void updateWaterSpray();
    
    // Rendering methods
    void renderGrid();
    void renderParticle(int col, int row, const Color& color);
    
    // Particle spawning
    void spawnParticles(int mouseCol, int mouseRow);
    
public:
    ParticleSimulation();
    ~ParticleSimulation() = default;
    
    bool initialize();
    void update();
    void render();
    void cleanup();
    
    // Input handling
    void handleMouseDrag(double xpos, double ypos);
    void setMaterial(MaterialType material);
    
    // Getters
    int getWidth() const { return WIDTH; }
    int getHeight() const { return HEIGHT; }
    MaterialType getCurrentMaterial() const { return currentMaterial; }
};
