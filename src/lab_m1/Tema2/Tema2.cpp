#include "lab_m1/Tema2/Tema2.h"
#include "RenderPlayer.h"
#include "lab_m1/Tema1/transform2D.h"

#include <vector>
#include <string>
#include <iostream>
#include <set>

#define TOTAL_NR_OF_ENEMIES 7

using namespace std;
using namespace tema2;
using namespace implementedCamera;

tema2::Tema2::Tema2() {
}

tema2::Tema2::~Tema2() {
}

void tema2::Tema2::Init() {
    // set seed for maze
    srand((unsigned int) time(NULL));

    // time to find the exit
    timer = 60;

    timedef = 0;

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    // map initialization
    map = new Map();

    glm::vec3 playerPos = glm::vec3(0);

    for (auto room : map->getRooms()) {
        if (room->type == 1) {
            fullRooms.push_back(room);
        }

        if (room->type == 2) {
            playerPos = room->position;
            playerPos.y -= 1.5f;
            player = new Character(playerPos);
        }

        if (room->type == 0) {
            emptyRooms.push_back(room);
        }
    }

    // camera init
    isThirdPerson = true;
    camera = new implementedCamera::Camera();
    camera->Set(glm::vec3(playerPos.x - 10, playerPos.y + 6, playerPos.z), playerPos, glm::vec3(0, 1, 0));

    initEnemies();

    // init enemy shaders
    {
        Shader* shader = new Shader("EnemyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    logicSpace.x = 0;
    logicSpace.y = 0;
    logicSpace.width = 7;
    logicSpace.height = 7;

    // wireframe mesh
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
        CreateMesh("wireframe", vertices, indices);
        meshes["wireframe"]->SetDrawMode(GL_LINE_LOOP);
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

glm::mat3 Tema2::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace) {
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

void Tema2::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear) {
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    
    hudCamera = GetSceneCamera();
    hudCamera->SetPosition(glm::vec3(0, 0, 25));
    hudCamera->SetRotation(glm::vec3(0, 0, 0));
    hudCamera->Update();
    hudCamera->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
}

void Tema2::FrameStart() {
    glClearColor(0.75f, 0.75f, 0.75f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds) {
    timedef += deltaTimeSeconds;
    timer -= deltaTimeSeconds;

    // Player Model
    RenderPlayer(isThirdPerson);

    // Bullet process
    WorkOnBullets(deltaTimeSeconds);

    if (player->won()) {
        cout << "GG\n";
        Exit();
    }

    if (player->getLives() <= 0 || timer < 0) {
        cout << "Game over!\n";
        Exit();
    }

    // delete enemies that are dead
    vector<int> toDeleteEnemies;
    int index = 0;

    for (auto enemy : enemies) {
        if (enemy->getIsHit() == 1) {
            enemy->decHitTime(deltaTimeSeconds);
        }

        if (enemy->getIsHit() == 0) {
            enemy->hitsPlayer(player);
        }

        if (enemy->getHitTime() < 0) {
            enemy->kill();
        }

        if (!enemy->isAlive()) {
            toDeleteEnemies.push_back(index);
        }

        index++;
    }

    for (auto er : toDeleteEnemies) {
        enemies.erase(enemies.begin() + er);
    }

    // Render Map
    for (auto room : fullRooms) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(room->position.x, room->position.y, room->position.z);
        modelMatrix *= transform3D::Scale(20, 10, 20);
        RenderMesh(meshes["box"], modelMatrix, glm::vec3(0.737f, 0.29f, 0.235f));
    }

    // Render plane
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(100, 0, 100);
        modelMatrix *= transform3D::Scale(4.4f, 1, 4.4f);
        RenderMesh(meshes["plane"], modelMatrix, glm::vec3(0.494f, 0.509f, 0.455f));
    }

    // Render bullets
    for (auto bullet : bullets) {
        glm::vec3 pos = bullet->getPosition();
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(pos.x, pos.y, pos.z);
        RenderMesh(meshes["sphere"], modelMatrix, glm::vec3(0.956f, 0.768f, 0.361f));
    }

    // Render enemies
    for (auto enemy : enemies) {
        glm::vec3 pos = enemy->getPosition();
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(pos.x, pos.y, pos.z);
        modelMatrix *= transform3D::Scale(6, 6, 6);
        RenderEnemy(meshes["sphere"], shaders["EnemyShader"], modelMatrix, glm::vec3(0.188f, 0.145f, 0.274f), enemy->getIsHit(), timedef);
        
        if (enemy->getIsHit() == 0) {
            enemy->move(deltaTimeSeconds);
        }
    }

    // --------------------------------- HUD THINGS --------------------------- //

    // resolution
    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(4 * resolution.x / 5, 0, resolution.x / 5, resolution.y / 5);
    SetViewportArea(viewSpace, glm::vec3(0.15f), true);
    glm::mat3 visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    // healthbar wireframe
    for (int i = 0; i < 3; i++) {
        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix * transform2D::Translate(-1 + i * 3.5f, 1.0);
        modelMatrix *= transform2D::Scale(2.5f, 1);
        RenderMesh2D(meshes["wireframe"], modelMatrix, glm::vec3(1, 0, 0));
    }

    // healthbar box
    for (int i = 0; i < player->getLives(); i++) {
        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix * transform2D::Translate(-2.22f + i * 3.5f, 0.5);
        modelMatrix *= transform2D::Scale(2.5f, 1);
        RenderMesh2D(meshes["healthBar"], modelMatrix, glm::vec3(1, 0, 0));
    }

    // time wireframe
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * transform2D::Translate(8.5, 3.5);
    modelMatrix *= transform2D::Scale(1, 6);
    RenderMesh2D(meshes["wireframe"], modelMatrix, glm::vec3(1, 1, 1));

    // time box
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix * transform2D::Translate(8, 0.5);
    modelMatrix *= transform2D::Scale(1, 6 * timer / 60);
    RenderMesh2D(meshes["healthBar"], modelMatrix, glm::vec3(1, 1, 1));
}

void Tema2::FrameEnd() {
}

void Tema2::RenderMesh(Mesh* mesh, const glm::mat4& modelMatrix, const glm::vec3& color) {
    Shader* shader = shaders.at("Color");

    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified color and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform3f(shader->GetUniformLocation("color"), color.r, color.g, color.b);

    mesh->Render();
}

void Tema2::RenderEnemy(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color, int isHit, float timedef) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int loc1 = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc1, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int loc2 = glGetUniformLocation(shader->program, "View");
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(loc2, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int loc3 = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc3, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int loc_color = glGetUniformLocation(shader->program, "obj_color");
    glUniform3fv(loc_color, 1, glm::value_ptr(color));

    int loc_isHit = glGetUniformLocation(shader->program, "is_hit");
    glUniform1i(loc_isHit, isHit);

    GLint loc_time = glGetUniformLocation(shader->program, "time");
    glUniform1f(loc_time, (GLfloat) (.005 * timedef));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    if (window->KeyHold(GLFW_KEY_W) && !player->hitsWallForward(fullRooms, deltaTime)) {
        player->moveForward(deltaTime);
        camera->MoveForward(player->getSpeed() * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S) && !player->hitsWallForward(fullRooms, -deltaTime)) {
        player->moveForward(-deltaTime);
        camera->MoveForward(-player->getSpeed() * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D) && !player->hitsWallRight(fullRooms, deltaTime)) {
        player->moveRight(deltaTime);
        camera->TranslateRight(player->getSpeed() * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_A) && !player->hitsWallRight(fullRooms, -deltaTime)) {
        player->moveRight(-deltaTime);
        camera->TranslateRight(-player->getSpeed() * deltaTime);
    }

    /*if (window->KeyHold(GLFW_KEY_E)) {
        camera->TranslateUpward(player->getSpeed() * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        camera->TranslateUpward(-player->getSpeed() * deltaTime);
    }*/
}

void Tema2::OnKeyPress(int key, int mods) {
    if (key == GLFW_KEY_LEFT_CONTROL) {
        if (isThirdPerson) {
            auto ppos = player->getPosition();
            camera->Set(glm::vec3(ppos.x, ppos.y + 2, ppos.z), glm::vec3(ppos.x + 2, ppos.y + 2, ppos.z), glm::vec3(0, 1, 0));
            camera->RotateFirstPerson_OY(player->getAngleOY());
            player->setAngleOX(0);
        }
        else {
            auto ppos = player->getPosition();
            camera->Set(glm::vec3(ppos.x - 10, ppos.y + 6, ppos.z), ppos, glm::vec3(0, 1, 0));
            camera->RotateThirdPerson_OY(player->getAngleOY());
        }

        isThirdPerson = !isThirdPerson;
    }
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;
        player->modAngleOY(-deltaX * sensivityOY);
        player->modAngleOX(-deltaY * sensivityOX);

        if (isThirdPerson) {
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
        }
        else {
            camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
            camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
        }
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_2  && !isThirdPerson &&  player->getTTF() <= 0) {
        bullets.push_back(new BulletT2(player));
        player->setTTF(player->getFireRate());
    }
}

void Tema2::WorkOnBullets(float deltaTimeSeconds) {
    vector<int> toEraseBullets;
    int index = 0;

    for (auto bullet : bullets) {
        if (bullet->exists()) {
            bullet->step(deltaTimeSeconds, fullRooms, player, enemies);
        }
        else {
            toEraseBullets.push_back(index);
        }

        index++;
    }

    for (auto er : toEraseBullets) {
        bullets.erase(bullets.begin() + er);
    }

    if (player->getTTF() > 0) {
        player->modTTF(-deltaTimeSeconds);
    }
}

void Tema2::initEnemies() {
    set<int> indices;

    while (indices.size() < TOTAL_NR_OF_ENEMIES) {
        indices.insert(rand() % emptyRooms.size());
    }

    for (auto index : indices) {
        enemies.push_back(new EnemyT2(emptyRooms.at(index)));
    }
}

void Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices) {
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
