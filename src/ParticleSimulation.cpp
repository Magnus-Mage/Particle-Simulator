#include "ParticleSimulation.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

ParticleSimulation::ParticleSimulation() 
    : cols(WIDTH / CELL_SIZE)
    , rows(HEIGHT / CELL_SIZE)
    , currentHue(250)
    , currentMaterial(MaterialType::WATER_SPRAY)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    initializeGrid();
}

bool ParticleSimulation::initialize() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    return true;
}

void ParticleSimulation::initializeGrid() {
    grid.resize(cols, std::vector<Particle>(rows));
    nextGrid.resize(cols, std::vector<Particle>(rows));
    
    // Initialize all particles as empty
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            grid[i][j] = Particle();
            nextGrid[i][j] = Particle();
        }
    }
}

bool ParticleSimulation::isValidPosition(int col, int row) const {
    return col >= 0 && col < cols && row >= 0 && row < rows;
}

bool ParticleSimulation::isWithinBounds(int col) const {
    return col >= 0 && col < cols;
}

Color ParticleSimulation::getMaterialColor(const Particle& particle) const {
    switch (particle.type) {
        case MaterialType::SAND:
            return Color(static_cast<float>(particle.hue) / 20.0f, 0.7f, 0.1f);
        case MaterialType::SNOW:
            return Color(static_cast<float>(particle.hue) / 360.0f, 1.0f, 1.0f);
        case MaterialType::WATER:
        case MaterialType::WATER_SPRAY:
            return Color(0.0f, 0.5f, 0.5f);
        case MaterialType::FIRE:
            return Color(static_cast<float>(particle.hue), 0.5f, 0.1f);
        default:
            return Color(0.0f, 0.0f, 0.0f);
    }
}

void ParticleSimulation::clearGrid() {
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            nextGrid[i][j] = Particle();
        }
    }
}

void ParticleSimulation::swapGrids() {
    std::swap(grid, nextGrid);
}

void ParticleSimulation::update() {
    clearGrid();
    
    switch (currentMaterial) {
        case MaterialType::SAND:
        case MaterialType::SNOW:
            updatePowder();
            break;
        case MaterialType::WATER:
            updateFluid();
            break;
        case MaterialType::FIRE:
            updateFire();
            break;
        case MaterialType::WATER_SPRAY:
            updateWaterSpray();
            break;
        default:
            break;
    }
    
    swapGrids();
}

void ParticleSimulation::updatePowder() {
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            const Particle& particle = grid[i][j];
            
            if (particle.isEmpty()) continue;
            
            int newRow = j - particle.velocity; // Move downwards
            
            // Try to move down
            if (isValidPosition(i, newRow) && grid[i][newRow].isEmpty()) {
                nextGrid[i][newRow] = particle;
            }
            // Try diagonal left
            else if (isValidPosition(i - 1, newRow) && grid[i - 1][newRow].isEmpty()) {
                nextGrid[i - 1][newRow] = particle;
            }
            // Try diagonal right
            else if (isValidPosition(i + 1, newRow) && grid[i + 1][newRow].isEmpty()) {
                nextGrid[i + 1][newRow] = particle;
            }
            // Stay in place
            else {
                nextGrid[i][j] = particle;
            }
        }
    }
}

void ParticleSimulation::updateFluid() {
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            const Particle& particle = grid[i][j];
            
            if (particle.isEmpty()) continue;
            
            int newRow = j - particle.velocity; // Move downwards
            bool moved = false;
            
            // Try to move down
            if (isValidPosition(i, newRow) && grid[i][newRow].isEmpty()) {
                nextGrid[i][newRow] = particle;
                moved = true;
            }
            // Try diagonal movements
            else if (isValidPosition(i - 1, newRow) && grid[i - 1][newRow].isEmpty()) {
                nextGrid[i - 1][newRow] = particle;
                moved = true;
            }
            else if (isValidPosition(i + 1, newRow) && grid[i + 1][newRow].isEmpty()) {
                nextGrid[i + 1][newRow] = particle;
                moved = true;
            }
            
            if (!moved) {
                // Try horizontal movement for fluid behavior
                int lateralMove = (rand() % 3) - 1; // -1, 0, or 1
                int newCol = i + lateralMove;
                
                if (isValidPosition(newCol, j) && grid[newCol][j].isEmpty()) {
                    nextGrid[newCol][j] = particle;
                } else {
                    nextGrid[i][j] = particle;
                }
            }
        }
    }
}

void ParticleSimulation::updateFire() {
    // Spawn new fire particles at the bottom occasionally
    for (int i = 0; i < cols; ++i) {
        if (rand() % 100 < 30) {
            nextGrid[i][0] = Particle(MaterialType::FIRE, 255, 1);
        }
    }
    
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            const Particle& particle = grid[i][j];
            
            if (particle.isEmpty()) continue;
            
            // Fire rises (opposite of gravity)
            int newRow = j + particle.velocity; // Move upwards
            
            // Try to move up
            if (isValidPosition(i, newRow) && grid[i][newRow].isEmpty()) {
                nextGrid[i][newRow] = particle;
            }
            // Try diagonal movements
            else if (isValidPosition(i - 1, newRow) && grid[i - 1][newRow].isEmpty()) {
                nextGrid[i - 1][newRow] = particle;
            }
            else if (isValidPosition(i + 1, newRow) && grid[i + 1][newRow].isEmpty()) {
                nextGrid[i + 1][newRow] = particle;
            }
            else {
                // Add some lateral movement for fire
                int lateralMove = (rand() % 3) - 1;
                int newCol = i + lateralMove;
                
                if (isValidPosition(newCol, newRow) && grid[newCol][newRow].isEmpty()) {
                    nextGrid[newCol][newRow] = particle;
                } else {
                    // Fire dissipates over time
                    if (rand() % 100 < 95) { // 95% chance to keep existing
                        nextGrid[i][j] = particle;
                    }
                }
            }
        }
    }
}

void ParticleSimulation::updateWaterSpray() {
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            const Particle& particle = grid[i][j];
            
            if (particle.isEmpty()) continue;
            
            int newRow = j - particle.velocity; // Move downwards
            
            // Try to move down first
            if (isValidPosition(i, newRow) && grid[i][newRow].isEmpty()) {
                nextGrid[i][newRow] = particle;
            }
            else {
                // Add lateral spray movement
                int lateralMove = (rand() % 3) - 1;
                int newCol = i + lateralMove;
                
                if (isValidPosition(newCol, newRow) && grid[newCol][newRow].isEmpty()) {
                    nextGrid[newCol][newRow] = particle;
                }
                else if (isValidPosition(newCol, j) && grid[newCol][j].isEmpty()) {
                    nextGrid[newCol][j] = particle;
                }
                else {
                    nextGrid[i][j] = particle;
                }
            }
        }
    }
}

void ParticleSimulation::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderGrid();
}

void ParticleSimulation::renderGrid() {
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            const Particle& particle = grid[i][j];
            
            if (!particle.isEmpty()) {
                Color color = getMaterialColor(particle);
                renderParticle(i, j, color);
            }
        }
    }
}

void ParticleSimulation::renderParticle(int col, int row, const Color& color) {
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    glVertex2f(col * CELL_SIZE, row * CELL_SIZE);
    glVertex2f((col + 1) * CELL_SIZE, row * CELL_SIZE);
    glVertex2f((col + 1) * CELL_SIZE, (row + 1) * CELL_SIZE);
    glVertex2f(col * CELL_SIZE, (row + 1) * CELL_SIZE);
    glEnd();
}

void ParticleSimulation::handleMouseDrag(double xpos, double ypos) {
    int mouseCol = static_cast<int>(xpos / CELL_SIZE);
    int mouseRow = static_cast<int>((HEIGHT - ypos) / CELL_SIZE); // Flip ypos
    
    spawnParticles(mouseCol, mouseRow);
}

void ParticleSimulation::spawnParticles(int mouseCol, int mouseRow) {
    int extent = SPAWN_MATRIX_SIZE / 2;
    
    for (int i = -extent; i <= extent; ++i) {
        for (int j = -extent; j <= extent; ++j) {
            if (rand() % 100 < SPAWN_PROBABILITY) {
                int col = mouseCol + i;
                int row = mouseRow + j;
                
                if (isValidPosition(col, row)) {
                    grid[col][row] = Particle(currentMaterial, currentHue, 1);
                }
            }
        }
    }
    
    // Change hue over time for visual variety
    currentHue += 10;
    if (currentHue > 360) {
        currentHue = 1;
    }
}

void ParticleSimulation::setMaterial(MaterialType material) {
    currentMaterial = material;
}

void ParticleSimulation::cleanup() {
    // Clean up resources if needed
}
