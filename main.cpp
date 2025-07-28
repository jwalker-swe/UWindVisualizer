#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <algorithm>

// Define constants
const int SIM_SCALE = 2;

const int WIDTH = 100 * SIM_SCALE;
const int HEIGHT = 30 * SIM_SCALE;
const float WIND_SPEED = 5.0f;
const float dt = 0.3f;

// Define structs to represent simulation objects
struct Point {
    float x, y;
};

struct Particle {
    float x, y;
    float vx, vy;
    int life = 0;
    int maxLife = 300;
    std::vector<Point> trail;
};

struct Building {
    int x, y, w, h;
};

struct Vec2 {
    float vx, vy;
};

// Create Color Helper Function
std::string getColor(float speed) {
    if (speed > 4.0f) return "\033[31m";
    if (speed > 2.0f) return "\033[33m";
    if (speed > 1.0f) return "\033[36m";
    return "\033[34m";
}

// Spawn particle function
void spawnParticles(std::vector<Particle>& particles, int count = 5) {
    for (int i = 0; i < count; ++i) {
        Particle p;
        p.x = 0.0f;
        p.y = static_cast<float>(rand() % HEIGHT);
        p.vx = WIND_SPEED;
        p.vy = 0.0f;
        p.life = 0;
        p.maxLife = 500 + rand() % 100;
        particles.push_back(p);
    }
}

// Define Vortex and Recirculation Wake Function
void applyWakeEffect(const Building& b, std::vector<std::vector<Vec2>>& flowField) {
    float buildingCenterY = b.y + b.h / 2.0f;
    for (int y = b.y; y < b.y + b.h; ++y) {
        for (int x = b.x + b.w; x < std::min(WIDTH, b.x + b.w + 50 * SIM_SCALE); ++x) {
            float relX = x - (b.x + b.w);
            float relY = y - buildingCenterY;

            // Karman vortex street effect
            float vortexStrength = 10.0f * std::exp(-relX / (20.0f * SIM_SCALE));
            float swirl = vortexStrength * std::sin(relX / (5.0f * SIM_SCALE)) * (relY > 0 ? 1 : -1);

            if (x < b.x + b.w + 10 * SIM_SCALE) { // Recirculation zone
                flowField[y][x].vx *= 0.1f;
                flowField[y][x].vy += swirl * 0.5f;
            } else { // Vortex street
                flowField[y][x].vx *= 0.7f;
                flowField[y][x].vy += swirl;
            }
        }
    }
}

// Define particle update function
void updateParticles(std::vector<Particle>& particles, const std::vector<std::vector<Vec2>>& flowField, const std::vector<Building>& buildings) {

    std::vector<Particle> newParticles;
    const int MAX_TRAIL_LENGTH = 5;

    for (auto& p : particles) {
        ++p.life;

        p.trail.push_back({p.x, p.y});
        if (p.trail.size() > MAX_TRAIL_LENGTH) {
            p.trail.erase(p.trail.begin());
        }

        int gx = static_cast<int>(p.x);
        int gy = static_cast<int>(p.y);

        if (gx >= 0  && gx < WIDTH && gy >= 0 && gy < HEIGHT) {
            Vec2 wind = flowField[gy][gx];
            p.vx = wind.vx + ((rand() % 100 - 50) / 300.0f);
            p.vy = wind.vy + ((rand() % 100 - 50) / 500.0f);
        }

        float nextX = p.x + p.vx * dt;
        float nextY = p.y + p.vy * dt;

        bool collided = false;
        for (const auto& b : buildings) {
            if (nextX >= b.x && nextX < b.x + b.w && nextY >= b.y && nextY < b.y + b.h) {
                collided = true;
                break;
            }
        }

        if (collided || p.life > p.maxLife || nextX < 0 || nextX >= WIDTH || nextY < 0 || nextY >= HEIGHT) {
            continue;
        }


        p.x = nextX;
        p.y = nextY;

        newParticles.push_back(p);


        // p.x = std::min(std::max(p.x, 0.0f), static_cast<float>(WIDTH - 1));
        // p.y = std::min(std::max(p.y, 0.0f), static_cast<float>(HEIGHT - 1));
    }

    particles = std::move(newParticles);
}

// Helper function to get arrow character from velocity
char getArrowChar(float vx, float vy) {
    float angle = atan2(vy, vx) * 180.0 / M_PI;
    if (angle < 0) angle += 360;

    if (angle >= 337.5 || angle < 22.5) return '>';
    if (angle >= 22.5 && angle < 67.5) return '/';
    if (angle >= 67.5 && angle < 112.5) return '^';
    if (angle >= 112.5 && angle < 157.5) return '\\';
    if (angle >= 157.5 && angle < 202.5) return '<';
    if (angle >= 202.5 && angle < 247.5) return '/';
    if (angle >= 247.5 && angle < 292.5) return 'v';
    if (angle >= 292.5 && angle < 337.5) return '\\';
    return '*';
}

// Define Draw function
void drawBuildingsAndParticlesWithColor(const std::vector<Building>& buildings, const std::vector<Particle>& particles) {
    // Create a grid for characters and another for colors
    std::vector<std::vector<char>> charGrid(HEIGHT, std::vector<char>(WIDTH, ' '));
    std::vector<std::vector<std::string>> colorGrid(HEIGHT, std::vector<std::string>(WIDTH, "\033[0m"));

    // Draw buildings
    for (const auto& b : buildings) {
        for (int y = b.y; y < b.y + b.h; ++y) {
            for (int x = b.x; x < b.x + b.w; ++x) {
                if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                    charGrid[y][x] = '#';
                }
            }
        }
    }

    // Draw trails
    for (const auto& p : particles) {
        for (size_t i = 0; i < p.trail.size(); ++i) {
            int tx = static_cast<int>(p.trail[i].x);
            int ty = static_cast<int>(p.trail[i].y);
            if (tx >= 0 && tx < WIDTH && ty >= 0 && ty < HEIGHT) {
                int grayLevel = 232 + (i * (255 - 232) / p.trail.size());
                charGrid[ty][tx] = '.';
                colorGrid[ty][tx] = "\033[38;5;" + std::to_string(grayLevel) + "m";
            }
        }
    }

    // Draw particles as arrows
    for (const auto& p : particles) {
        int px = static_cast<int>(p.x);
        int py = static_cast<int>(p.y);
        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
            float speed = std::sqrt(p.vx * p.vx + p.vy * p.vy);
            charGrid[py][px] = getArrowChar(p.vx, p.vy);
            colorGrid[py][px] = getColor(speed);
        }
    }

    // Build the final buffer
    std::string buffer = "\033[H";
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            buffer += colorGrid[y][x] + charGrid[y][x];
        }
        buffer += "\n";
    }

    std::cout << buffer << "\033[0m";
    std::cout.flush();
}

// Main function
int main() {
    srand(time(0));
    std::vector<std::vector<Vec2>> flowField(HEIGHT, std::vector<Vec2>(WIDTH, {WIND_SPEED, 0.0f}));

    std::vector<Building> buildings;
    int numBuildings = 5;
    int minSpacing = 5 * SIM_SCALE;
    int currentX = 15 * SIM_SCALE;
    int margin = 2 * SIM_SCALE;

    for (int i = 0; i < numBuildings; ++i) {
        if (currentX > WIDTH - 20 * SIM_SCALE) break;
        int w = (rand() % 5 + 3) * SIM_SCALE;
        int h = (rand() % 10 + 5) * SIM_SCALE;
        int y = margin + (rand() % (HEIGHT - h - 2 * margin));
        buildings.push_back({currentX, y, w, h});
        currentX += w + minSpacing + (rand() % 5) * SIM_SCALE;
    }

    for (const auto& b : buildings)
        applyWakeEffect(b, flowField);

    std::vector<Particle> particles;
    for (int i = 0; i < 200; ++i) {
        particles.push_back({
            static_cast<float>(rand() % 10),
            static_cast<float>(rand() % HEIGHT),
            WIND_SPEED,
            0.0f
        });
    }

    const std::chrono::milliseconds frameDuration(1000 / 24);

    while (true) {
        auto startTime = std::chrono::high_resolution_clock::now();

        spawnParticles(particles, 5500);
        updateParticles(particles, flowField, buildings);
        drawBuildingsAndParticlesWithColor(buildings, particles);

        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        auto sleepTime = frameDuration - elapsedTime;

        if (sleepTime > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(sleepTime);
        }
    }

    return 0;
}