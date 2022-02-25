#include "lab_m1/Tema1/Tema1.h"

#define BORDER_COLOR glm::vec3(0, 0, 0.6f)
#define BODY_COLOR glm::vec3(1, 0.6f, 0.6f)
#define BAG_COLOR glm::vec3(0.4f, 0.5f, 0.7f)
#define HANDS_COLOR glm::vec3(1, 0.69f, 0.4f)
#define RED_COLOR glm::vec3(1, 0, 0)
#define DARKRED_COLOR glm::vec3(0.4f, 0, 0)
#define BLACK_COLOR glm::vec3(0, 0, 0)
#define ENEMY_COLOR glm::vec3(0.29f, 0.6f, 0)
#define WHITE_COLOR glm::vec3(1, 1, 1)
#define PINK_COLOR glm::vec3(1, 0.8f, 1)
#define TIME_TO_SPAWN 2

using namespace std;
using namespace tema1;

Tema1::Tema1() {
    srand((int) time(0));
    modelMatrix = glm::mat3(1);
    visMatrix = glm::mat3(1);
    scaleXBorder = 60;
    scaleYBorder = 40;
    TTSE = 0;
    minSpawnDistance = 10;
    xMinGlobal = -scaleXBorder / 2;
    yMinGlobal = -scaleYBorder / 2;
    xMaxGlobal = scaleXBorder / 2;
    yMaxGlobal = scaleYBorder / 2;
}

Tema1::~Tema1() {
}

void Tema1::Init() {
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    player = Player();
    border = Border(scaleXBorder, scaleYBorder);

    logicSpace.width = 25;
    logicSpace.height = 25;
    logicSpace.x = -logicSpace.width / 2;
    logicSpace.y = -logicSpace.height / 2;

    // obstacles init
    obstacles.push_back(Obstacle(5, 4, 6.5, -5));
    obstacles.push_back(Obstacle(17, 4, -4.5, -12));
    obstacles.push_back(Obstacle(6, 7, -22, -4.5));
    obstacles.push_back(Obstacle(14, 3, 14, 12.5));
    obstacles.push_back(Obstacle(3, 11, 22.5, 8.5));
    obstacles.push_back(Obstacle(3, 3, -24.5, 14.5));
    obstacles.push_back(Obstacle(3, 3, -18.5, 14.5));
    obstacles.push_back(Obstacle(3, 3, -12.5, 14.5));

    // circle mesh
    {
        // vertices
        vector<VertexFormat> vertices{
            VertexFormat(glm::vec3(0, 0, 0))
        };

        for (int i = 0; i < circleLines; i++) {
            vertices
                .push_back(VertexFormat(glm::vec3(1 * cos(2 * M_PI / circleLines * i), 1 * sin(2 * M_PI / circleLines * i), 0)));
        }

        // indices
        vector<unsigned int> indices;

        for (int i = 0; i <= circleLines; i++) {
            indices.push_back(i);
        }

        indices.push_back(1);

        // create the mesh
        CreateMesh("circle", vertices, indices);
        meshes["circle"]->SetDrawMode(GL_TRIANGLE_FAN);
    }

    // border mesh
    {
        // vertices
        vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
            VertexFormat(glm::vec3(0.5f, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
            VertexFormat(glm::vec3(0.5f, 0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
            VertexFormat(glm::vec3(-0.5f, 0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
        };

        // indices
        vector<unsigned int> indices = { 0, 1, 2, 3 };
        CreateMesh("border", vertices, indices);
        meshes["border"]->SetDrawMode(GL_LINE_LOOP);
    }

    // obstacle mesh
    {
        // vertices
        vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-0.5f, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
            VertexFormat(glm::vec3(0.5f, -0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
            VertexFormat(glm::vec3(-0.5f, 0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
            VertexFormat(glm::vec3(0.5f, 0.5f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
        };

        // indices
        vector<unsigned int> indices = { 0, 1, 2,
                                         1, 3, 2 };

        CreateMesh("obstacle", vertices, indices);
    }

    // healthbar mesh
    {
        // vertices
        vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.6f)),
        };

        // indices
        vector<unsigned int> indices = { 0, 1, 2,
                                         1, 3, 2 };

        CreateMesh("healthBar", vertices, indices);
    }
}

void Tema1::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices) {
    // Create the VAO and bind it
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO = 0;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);
    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.76f, 0.77f, 0.8f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema1::Update(float deltaTimeSeconds) {
    // sets viewPort
    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // compute visMatrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    if (player.getScore() < player.getMaxScore() && player.getHealth() > 0) {
        // verify player status and bullets status
        WorkOnBullets(bullets, deltaTimeSeconds, obstacles, border, player, enemies);
        player.hitsEnemy(enemies);

        // spawn an enemy if TTSE (time to spawn enemy) is 0
        if (TTSE <= 0) {
            int xSign = ((float)rand() / (float)RAND_MAX > 0.5f) ? 1 : -1;
            int ySign = ((float)rand() / (float)RAND_MAX > 0.5f) ? 1 : -1;
            float xCoord = xSign * (float)rand() / ((float)RAND_MAX / (scaleXBorder / 2 - 1));
            float yCoord = ySign * (float)rand() / ((float)RAND_MAX / (scaleYBorder / 2 - 1));

            // the enemy should not spawn too close to the player
            // if that's the case, change spawn location
            if (xCoord < player.getXCoord() + minSpawnDistance && xCoord > player.getXCoord() - minSpawnDistance &&
                yCoord < player.getYCoord() + minSpawnDistance && yCoord > player.getYCoord() - minSpawnDistance) {

                if (player.getXCoord() - minSpawnDistance < xMinGlobal) {
                    xCoord = player.getXCoord() + minSpawnDistance;
                }
                else if (player.getXCoord() + minSpawnDistance > xMaxGlobal) {
                    xCoord = player.getXCoord() - minSpawnDistance;
                }
                else {
                    xSign = ((float)rand() / (float)RAND_MAX > 0.5f) ? 1 : -1;
                    xCoord = player.getXCoord() + xSign * minSpawnDistance;
                }
            }

            enemies.push_back(new Enemy(xCoord, yCoord));
            TTSE = TIME_TO_SPAWN;
        }

        // decrement TTSE
        TTSE -= deltaTimeSeconds;
    }

    // finally draw the scene
    DrawScene(visMatrix, deltaTimeSeconds);
}

void Tema1::FrameEnd() {
}

void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds) {
    // healthbar wireframe
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(player.getXCoord(), player.getYCoord());
    modelMatrix *= transform2D::Translate(14, 10.5f);
    modelMatrix *= transform2D::Scale(14, 2);
    RenderMesh2D(meshes["border"], modelMatrix, BLACK_COLOR);

    // healthbar
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(player.getXCoord(), player.getYCoord());
    modelMatrix *= transform2D::Translate(7, 9.5f);
    modelMatrix *= transform2D::Scale(14 * player.getHealth() / player.getMaxHealth() , 2);
    RenderMesh2D(meshes["healthBar"], modelMatrix, RED_COLOR);

    // scorebar wireframe
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(player.getXCoord(), player.getYCoord());
    modelMatrix *= transform2D::Translate(20, -2);
    modelMatrix *= transform2D::Scale(2, 20);
    RenderMesh2D(meshes["border"], modelMatrix, PINK_COLOR);

    // scorebar
    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(player.getXCoord(), player.getYCoord());
    modelMatrix *= transform2D::Translate(19, -12);
    modelMatrix *= transform2D::Scale(2, 20 * player.getScore() / player.getMaxScore());
    RenderMesh2D(meshes["healthBar"], modelMatrix, WHITE_COLOR);

    // border
    modelMatrix = visMatrix * transform2D::Translate(0, 0);
    modelMatrix *= transform2D::Scale(scaleXBorder, scaleYBorder);
    RenderMesh2D(meshes["border"], modelMatrix, BORDER_COLOR);

    if (player.getHealth() > 0) {
        // body
        modelMatrix = visMatrix * transform2D::Translate(player.getXCoord(), player.getYCoord());
        modelMatrix *= transform2D::Rotate(player.getAngle());
        RenderMesh2D(meshes["circle"], modelMatrix, BODY_COLOR);

        //right hand
        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(player.getXCoord(), player.getYCoord());
        modelMatrix *= transform2D::Rotate(player.getAngle());
        modelMatrix *= transform2D::Translate(0.8f, -0.75f);
        modelMatrix *= transform2D::Scale(0.4f, 0.4f);
        RenderMesh2D(meshes["circle"], modelMatrix, HANDS_COLOR);

        //left hand
        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(player.getXCoord(), player.getYCoord());
        modelMatrix *= transform2D::Rotate(player.getAngle());
        modelMatrix *= transform2D::Translate(0.8f, 0.75f);
        modelMatrix *= transform2D::Scale(0.4f, 0.4f);
        RenderMesh2D(meshes["circle"], modelMatrix, HANDS_COLOR);

        // bag
        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(player.getXCoord(), player.getYCoord());
        modelMatrix *= transform2D::Rotate(player.getAngle());
        modelMatrix *= transform2D::Translate(-0.55f, 0);
        RenderMesh2D(meshes["circle"], modelMatrix, BAG_COLOR);
    }
    
    // enemies
    for (auto enemy : enemies) {
        if (enemy->exists()) {
            enemy->setAngle(atan2(enemy->getYCoord() - player.getYCoord(), enemy->getXCoord() - player.getXCoord()) - (float) M_PI);
            enemy->modXCoord(cos(enemy->getAngle()) * deltaTimeSeconds * enemy->getSpeed());
            enemy->modYCoord(sin(enemy->getAngle()) * deltaTimeSeconds * enemy->getSpeed());

            // body
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(enemy->getXCoord(), enemy->getYCoord());
            modelMatrix *= transform2D::Rotate(enemy->getAngle());
            modelMatrix *= transform2D::Scale(enemy->getScale(), enemy->getScale());
            RenderMesh2D(meshes["obstacle"], modelMatrix, ENEMY_COLOR);

            // left hand
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(enemy->getXCoord(), enemy->getYCoord());
            modelMatrix *= transform2D::Rotate(enemy->getAngle());
            modelMatrix *= transform2D::Translate(1.25f, 0.7f);
            modelMatrix *= transform2D::Scale(0.5f, 0.5f);
            RenderMesh2D(meshes["obstacle"], modelMatrix, DARKRED_COLOR);

            // right hand
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(enemy->getXCoord(), enemy->getYCoord());
            modelMatrix *= transform2D::Rotate(enemy->getAngle());
            modelMatrix *= transform2D::Translate(1.25f, -0.7f);
            modelMatrix *= transform2D::Scale(0.5f, 0.5f);
            RenderMesh2D(meshes["obstacle"], modelMatrix, DARKRED_COLOR);
        }
    }

    // obstacles
    for (auto obstacle : obstacles) {
        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(obstacle.getXCenter(), obstacle.getYCenter());
        modelMatrix *= transform2D::Scale(obstacle.getScaleX(), obstacle.getScaleY());
        RenderMesh2D(meshes["obstacle"], modelMatrix, BORDER_COLOR);
    }

    // bullets
    for (auto bullet : bullets) {
        if (bullet->exists()) {
            modelMatrix = visMatrix;
            modelMatrix *= transform2D::Translate(bullet->getXCoord(), bullet->getYCoord());
            modelMatrix *= transform2D::Rotate(bullet->getAngle());
            modelMatrix *= transform2D::Scale(bullet->getScale(), bullet->getScale());
            RenderMesh2D(meshes["obstacle"], modelMatrix, BLACK_COLOR);
        }
    }
}

void Tema1::OnInputUpdate(float deltaTime, int mods) {
    if (window->KeyHold(GLFW_KEY_W) && player.canMoveUp(border) &&
        !player.hitsObstacle(obstacles, 0, player.getSpeed() * deltaTime) &&
        player.getHealth() > 0) {
        logicSpace.y += player.getSpeed() * deltaTime;
        player.setYCoord(player.getSpeed() * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S) && player.canMoveDown(border) &&
        !player.hitsObstacle(obstacles, 0, -player.getSpeed() * deltaTime) &&
        player.getHealth() > 0) {
        logicSpace.y -= player.getSpeed() * deltaTime;
        player.setYCoord(-player.getSpeed() * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_A) && player.canMoveLeft(border) &&
        !player.hitsObstacle(obstacles, -player.getSpeed() * deltaTime, 0) &&
        player.getHealth() > 0) {
        logicSpace.x -= player.getSpeed() * deltaTime;
        player.setXCoord(-player.getSpeed() * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D) && player.canMoveRight(border) && 
        !player.hitsObstacle(obstacles, player.getSpeed() * deltaTime, 0) &&
        player.getHealth() > 0) {
        logicSpace.x += player.getSpeed() * deltaTime;
        player.setXCoord(player.getSpeed() * deltaTime);
    }
}

void Tema1::OnKeyPress(int key, int mods) {
    // restart the game by pressing R key
    if (key == GLFW_KEY_R) {
        enemies.clear();
        bullets.clear();
        player.resetHealth();
        player.resetScore();
        player.resetCoords();
        logicSpace.x = -logicSpace.width / 2;
        logicSpace.y = -logicSpace.height / 2;
        TTSE = 0;
        player.setTTF(0);
    }
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    glm::ivec2 res = window->GetResolution();
    player.setAngle(-atan2(mouseY - (float) res.y / 2, mouseX - (float) res.x / 2));
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_2 && player.getTTF() <= 0 &&
        player.getHealth() > 0 && player.getScore() < player.getMaxScore()) {
        bullets.push_back(new Bullet(player));
        player.setTTF(player.getFireRate());
    }
}

void Tema1::WorkOnBullets(vector<Bullet*> &bullets, float deltaTimeSeconds, std::vector<Obstacle> obstacles,
                          Border border, Player &player, std::vector<Enemy*> &enemies) {
    vector<int> toEraseBullets;
    vector<int> toEraseEnemies;
    int index = 0;

    for (auto bullet : bullets) {
        if (bullet->exists()) {
            bullet->step(deltaTimeSeconds, obstacles, border, enemies, player);
        } else {
            toEraseBullets.push_back(index);
        }

        index++;
    }

    for (auto er : toEraseBullets) {
        bullets.erase(bullets.begin() + er);
    }

    index = 0;

    for (auto enemy : enemies) {
        if (!enemy->exists()) {
            toEraseEnemies.push_back(index);
        }

        index++;
    }

    for (auto er : toEraseEnemies) {
        enemies.erase(enemies.begin() + er);
    }

    if (player.getTTF() > 0) {
        player.modTTF(-deltaTimeSeconds);
    }
}
