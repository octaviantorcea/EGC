#include "Tema3.h"

using namespace std;
using namespace tema3;

Tema3::Tema3() {
}

Tema3::~Tema3() {
}

void Tema3::Init() {
    // special = new Dancer(glm::vec3(5, 0.5, 5));

    srand(time(NULL));

    lightFromDanceFloor = 1;
    lightFromSpotLight = 1;
    lightFromGlobe = 1;
    deltaTime = 0;

    // wall mesh
    {
        Mesh* mesh = new Mesh("wall");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    //lightCone mesh
    {
        // vertices
        vector<VertexFormat> vertices{
            VertexFormat(glm::vec3(0, 0, 0))
        };

        for (int i = 0; i < CONE_LINES; i++) {
            vertices
                .push_back(VertexFormat(glm::vec3(1 * cos(2 * M_PI / CONE_LINES * i), -1, 1 * sin(2 * M_PI / CONE_LINES * i))));
        }

        // indices
        vector<unsigned int> indices;

        for (int i = 0; i <= CONE_LINES; i++) {
            indices.push_back(i);
        }

        indices.push_back(1);

        // create the mesh
        meshes["cone"] = new Mesh("cone");
        meshes["cone"]->InitFromData(vertices, indices);
        meshes["cone"]->SetDrawMode(GL_TRIANGLE_FAN);
    }

    // sphere mesh
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // danceFloor shader
    {
        Shader* shader = new Shader("FloorShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FloorVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FloorFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // wall shader
    {
        Shader* shader = new Shader("WallShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "WallVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "WallFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // dancer shader
    {
        Shader* shader = new Shader("DancerShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "DancerVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "DancerFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // cone shader
    {
        Shader* shader = new Shader("ConeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "ConeVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "ConeFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // ceil shader
    {
        Shader* shader = new Shader("CeilShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "CeilVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "CeilFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // globe shader
    {
        Shader* shader = new Shader("GlobeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "GlobeVertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "GlobeFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // init danceFloor
    danceFloor = new FloorTile*[NR_TILES * NR_TILES + 1];
    danceFloor[NR_TILES * NR_TILES] = new FloorTile();

    for (int i = 0; i < NR_TILES; i++) {
        for (int j = 0; j < NR_TILES; j++) {
            danceFloor[i * NR_TILES + j] = new FloorTile(glm::vec3(i, 0, j));
        }
    }

    // init walls
    walls = new Wall*[NR_TILES * 3];

    // left walls
    for (int i = 0; i < NR_TILES; i++) {
        walls[i] = new Wall(glm::vec3(i * WALL_WIDTH, WALL_HEIGHT / 2, -FLOORTILE_RADIUS), i, danceFloor);
    }

    // back walls
    for (int i = 0; i < NR_TILES; i++) {
        walls[i + NR_TILES] = new Wall(glm::vec3(NR_TILES * WALL_WIDTH - FLOORTILE_RADIUS, WALL_HEIGHT / 2, i * WALL_WIDTH), i + NR_TILES, danceFloor);
    }

    // right walls
    for (int i = 0; i < NR_TILES; i++) {
        walls[i + 2 * NR_TILES] = new Wall(glm::vec3(i * WALL_WIDTH, WALL_HEIGHT / 2, NR_TILES * WALL_WIDTH - FLOORTILE_RADIUS), i + 2 * NR_TILES, danceFloor);
    }

    // init dancers
    for (int i = 0; i < NR_OF_DANCERS; i++) {
        dancers[i] = new Dancer(danceFloor);
    }

    // init spotlights
    spotlights[0] = new SpotLight(glm::vec3(1.5f, 4, 1.5f));
    spotlights[1] = new SpotLight(glm::vec3(1.5f, 4, 5.5f));
    spotlights[2] = new SpotLight(glm::vec3(5.5f, 4, 1.5f));
    spotlights[3] = new SpotLight(glm::vec3(5.5f, 4, 5.5f));
    for (int i = 0; i < SPOTLIGHTS; i++) {
        spotlightsPositions[i] = spotlights[i]->getPosition();
        spotlightsDirections[i] = spotlights[i]->getDirection();
        spotlightsColors[i] = spotlights[i]->getColor();
    }

    // init ceil
    ceil = new Ceil();

    // init globe
    mapTextures["globeTexture"] = CreateRandomTexture(16, 16);
    globe = new Globe(mapTextures["globeTexture"]);
}

void Tema3::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds) {
    srand(time(NULL));

    //// test dancer
    //{
    //    special->updateLightingTiles(danceFloor);
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix *= Translate(special->getPosition().x, special->getPosition().y, special->getPosition().z);
    //    modelMatrix *= Scale(DANCER_WIDTH, DANCER_HEIGHT, DANCER_WIDTH);
    //    RenderDancer(meshes["wall"], shaders["DancerShader"], modelMatrix, special);
    //}

    // render danceFloor
    for (int i = 0; i < NR_TILES * NR_TILES; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(danceFloor[i]->getCenterPosition().x, danceFloor[i]->getCenterPosition().y, danceFloor[i]->getCenterPosition().z);
        modelMatrix *= RotateOX(M_PI / 2);
        modelMatrix *= Scale(1, 1, 0.001f);
        RenderFloorTile(meshes["wall"], shaders["FloorShader"], modelMatrix, danceFloor[i]);
    }

    // render left walls
    for (int i = 0; i < NR_TILES; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(walls[i]->getCenterPosition().x, walls[i]->getCenterPosition().y, walls[i]->getCenterPosition().z);
        modelMatrix *= Scale(WALL_WIDTH, WALL_HEIGHT, 0.001f);
        RenderWall(meshes["wall"], shaders["WallShader"], modelMatrix, walls[i]);
    }

    // render back walls
    for (int i = NR_TILES; i < NR_TILES * 2; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(walls[i]->getCenterPosition().x, walls[i]->getCenterPosition().y, walls[i]->getCenterPosition().z);
        modelMatrix *= RotateOY(M_PI / 2);
        modelMatrix *= Scale(WALL_WIDTH, WALL_HEIGHT, 0.001f);
        RenderWall(meshes["wall"], shaders["WallShader"], modelMatrix, walls[i]);
    }

    // render right walls
    for (int i = 2 * NR_TILES; i < NR_TILES * 3; i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(walls[i]->getCenterPosition().x, walls[i]->getCenterPosition().y, walls[i]->getCenterPosition().z);
        modelMatrix *= Scale(WALL_WIDTH, WALL_HEIGHT, 0.001f);
        RenderWall(meshes["wall"], shaders["WallShader"], modelMatrix, walls[i]);
    }

    // render dancers
    for (int i = 0; i < NR_OF_DANCERS; i++) {
        dancers[i]->updateLightingTiles(danceFloor);
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(dancers[i]->getPosition().x, dancers[i]->getPosition().y, dancers[i]->getPosition().z);
        modelMatrix *= Scale(DANCER_WIDTH, DANCER_HEIGHT, DANCER_WIDTH);
        RenderDancer(meshes["wall"], shaders["DancerShader"], modelMatrix, dancers[i]);
        dancers[i]->move(deltaTimeSeconds);
    }

    // render ceil
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(ceil->getPosition().x, ceil->getPosition().y, ceil->getPosition().z);
        modelMatrix *= Scale(CEIL_LENGTH, 0.001f, CEIL_LENGTH);
        RenderCeil(meshes["wall"], shaders["CeilShader"], modelMatrix);
    }

    // render globe
    if (lightFromGlobe == 1) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(globe->getPosition().x, globe->getPosition().y, globe->getPosition().z);
        modelMatrix *= Scale(GLOBE_SCALE_FACTOR, GLOBE_SCALE_FACTOR, GLOBE_SCALE_FACTOR);
        RenderGlobe(meshes["sphere"], shaders["GlobeShader"], modelMatrix);
    }

    // render spotlight cones
    if (lightFromSpotLight == 1) {
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            glDepthMask(GL_FALSE);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            for (int i = 0; i < SPOTLIGHTS; i++) {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix *= Translate(spotlights[i]->getPosition().x, spotlights[i]->getPosition().y, spotlights[i]->getPosition().z);

                spotlights[i]->updateDirection(deltaTimeSeconds);
                spotlightsDirections[i] = spotlights[i]->getDirection();
                modelMatrix *= RotateOX(RADIANS(spotlights[i]->getOxAngle()));
                modelMatrix *= RotateOY(RADIANS(spotlights[i]->getOyAngle()));
                modelMatrix *= RotateOZ(RADIANS(spotlights[i]->getOzAngle()));

                modelMatrix *= Scale(tan(RADIANS(spotlights[i]->getCutoffAngle())) * WALL_HEIGHT, WALL_HEIGHT, tan(RADIANS(spotlights[i]->getCutoffAngle())) * WALL_HEIGHT);
                RenderCone(meshes["cone"], shaders["ConeShader"], modelMatrix, spotlights[i]->getColor());
            }

            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
            glDisable(GL_CULL_FACE);
        }
    }

    deltaTime += deltaTimeSeconds / 18;
}

void tema3::Tema3::RenderFloorTile(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix, FloorTile* floorTile) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Bind lightFromDanceFloor
    int df_loc = glGetUniformLocation(shader->program, "light_from_danceFloor");
    glUniform1i(df_loc, lightFromDanceFloor);

    // Bind lightFromSpotlight
    int sl_loc = glGetUniformLocation(shader->program, "is_light_from_spotlight");
    glUniform1i(sl_loc, lightFromSpotLight);

    // Bind is_light_from_globe
    int gl_loc = glGetUniformLocation(shader->program, "is_light_from_globe");
    glUniform1i(gl_loc, lightFromGlobe);

    // Set eye position uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Bind Ke
    int loc_Ke = glGetUniformLocation(shader->program, "floor_Ke");
    glUniform3f(loc_Ke, floorTile->getEmittingColor().r, floorTile->getEmittingColor().g, floorTile->getEmittingColor().b);

    // Bind Kd
    int loc_Kd = glGetUniformLocation(shader->program, "floor_Kd");
    glUniform1f(loc_Kd, floorTile->getKd());

    // Bind Ks
    int loc_Ks = glGetUniformLocation(shader->program, "floor_Ks");
    glUniform1f(loc_Ks, floorTile->getKs());

    // Bind shininess
    int loc_shininess = glGetUniformLocation(shader->program, "shininess");
    glUniform1f(loc_shininess, floorTile->getShininess());

    // Bind spotlights positions
    int loc_sl_pos = glGetUniformLocation(shader->program, "spotlight_position");
    glUniform3fv(loc_sl_pos, SPOTLIGHTS, glm::value_ptr(spotlightsPositions[0]));

    // Bind spotlights directions
    int loc_sl_dir = glGetUniformLocation(shader->program, "spotlight_direction");
    glUniform3fv(loc_sl_dir, SPOTLIGHTS, glm::value_ptr(spotlightsDirections[0]));

    // Bind spotlights colors
    int loc_sl_col = glGetUniformLocation(shader->program, "spotlight_color");
    glUniform3f(loc_sl_col, spotlights[0]->getColor().r, spotlights[0]->getColor().g, spotlights[0]->getColor().b);
    glUniform3fv(loc_sl_col, SPOTLIGHTS, glm::value_ptr(spotlightsColors[0]));

    // Bind spotlights cutoffAngle
    int loc_sp_cut = glGetUniformLocation(shader->program, "cutoff_angle");
    glUniform1f(loc_sp_cut, spotlights[0]->getCutoffAngle());

    // Bind spotlights intensity
    int loc_sp_int = glGetUniformLocation(shader->program, "spotlight_intensity");
    glUniform1f(loc_sp_int, spotlights[0]->getIntensity());

    // activate texture channel
    glActiveTexture(GL_TEXTURE0);

    // bind the texture ID
    glBindTexture(GL_TEXTURE_2D, globe->getTexture()->GetTextureID());

    // send the uniform value
    glUniform1i(glGetUniformLocation(shader->program, "globe_texture"), 0);

    // bind disco_ball_position
    int loc_disco = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(loc_disco, globe->getPosition().x, globe->getPosition().y, globe->getPosition().z);

    // bind globe_intensity
    int disco_int_loc = glGetUniformLocation(shader->program, "globe_intensity");
    glUniform1f(disco_int_loc, globe->getIntensity());

    // bind time
    int loc_time = glGetUniformLocation(shader->program, "delta_time");
    glUniform1f(loc_time, deltaTime);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void tema3::Tema3::RenderWall(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix, Wall* wall) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Bind lightFromDanceFloor
    int y_loc = glGetUniformLocation(shader->program, "light_from_danceFloor");
    glUniform1i(y_loc, lightFromDanceFloor);

    // Bind lightFromSpotlight
    int sl_loc = glGetUniformLocation(shader->program, "is_light_from_spotlight");
    glUniform1i(sl_loc, lightFromSpotLight);

    // Bind is_light_from_globe
    int gl_loc = glGetUniformLocation(shader->program, "is_light_from_globe");
    glUniform1i(gl_loc, lightFromGlobe);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Bind wall_Kd
    int loc_kd = glGetUniformLocation(shader->program, "wall_Kd");
    glUniform1f(loc_kd, wall->getKd());

    // Bind wall_Ks
    int loc_ks = glGetUniformLocation(shader->program, "wall_Ks");
    glUniform1f(loc_ks, wall->getKs());

    // Bind material_shininess
    int ms_loc = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(ms_loc, wall->getShininess());

    // Bind danceFloor_light_position
    int light_position = glGetUniformLocation(shader->program, "danceFloor_light_position");
    glUniform3fv(light_position, NR_OF_LIGHT_FT, glm::value_ptr(wall->getLightingFTPosition()[0]));

    // Bind floorTileLightRadius
    int lr_loc = glGetUniformLocation(shader->program, "floorTileLightRadius");
    glUniform1f(lr_loc, danceFloor[0]->getLightRadius());

    // Bind danceFloor_light_color
    int li_loc = glGetUniformLocation(shader->program, "danceFloor_light_color");
    glUniform3fv(li_loc, NR_OF_LIGHT_FT, glm::value_ptr(wall->getLightingFTColor()[0]));

    // Bind spotlights positions
    int loc_sl_pos = glGetUniformLocation(shader->program, "spotlight_position");
    glUniform3fv(loc_sl_pos, SPOTLIGHTS, glm::value_ptr(spotlightsPositions[0]));

    // Bind spotlights directions
    int loc_sl_dir = glGetUniformLocation(shader->program, "spotlight_direction");
    glUniform3fv(loc_sl_dir, SPOTLIGHTS, glm::value_ptr(spotlightsDirections[0]));

    // Bind spotlights colors
    int loc_sl_col = glGetUniformLocation(shader->program, "spotlight_color");
    glUniform3f(loc_sl_col, spotlights[0]->getColor().r, spotlights[0]->getColor().g, spotlights[0]->getColor().b);
    glUniform3fv(loc_sl_col, SPOTLIGHTS, glm::value_ptr(spotlightsColors[0]));

    // Bind spotlights cutoffAngle
    int loc_sp_cut = glGetUniformLocation(shader->program, "cutoff_angle");
    glUniform1f(loc_sp_cut, spotlights[0]->getCutoffAngle());

    // Bind spotlights intensity
    int loc_sp_int = glGetUniformLocation(shader->program, "spotlight_intensity");
    glUniform1f(loc_sp_int, spotlights[0]->getIntensity());

    // activate texture channel
    glActiveTexture(GL_TEXTURE0);

    // bind the texture ID
    glBindTexture(GL_TEXTURE_2D, globe->getTexture()->GetTextureID());

    // send the uniform value
    glUniform1i(glGetUniformLocation(shader->program, "globe_texture"), 0);

    // bind disco_ball_position
    int loc_disco = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(loc_disco, globe->getPosition().x, globe->getPosition().y, globe->getPosition().z);

    // bind globe_intensity
    int disco_int_loc = glGetUniformLocation(shader->program, "globe_intensity");
    glUniform1f(disco_int_loc, globe->getIntensity());

    // bind time
    int loc_time = glGetUniformLocation(shader->program, "delta_time");
    glUniform1f(loc_time, deltaTime);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void tema3::Tema3::RenderDancer(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix, Dancer* dancer) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Bind lightFromDanceFloor
    int y_loc = glGetUniformLocation(shader->program, "light_from_danceFloor");
    glUniform1i(y_loc, lightFromDanceFloor);

    // Bind lightFromSpotlight
    int sl_loc = glGetUniformLocation(shader->program, "is_light_from_spotlight");
    glUniform1i(sl_loc, lightFromSpotLight);

    // Bind is_light_from_globe
    int gl_loc = glGetUniformLocation(shader->program, "is_light_from_globe");
    glUniform1i(gl_loc, lightFromGlobe);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Bind dancer_Kd
    int loc_kd = glGetUniformLocation(shader->program, "dancer_Kd");
    glUniform1f(loc_kd, dancer->getKd());

    // Bind dancer_Ks
    int loc_ks = glGetUniformLocation(shader->program, "dancer_Ks");
    glUniform1f(loc_ks, dancer->getKs());

    // Bind material_shininess
    int ms_loc = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(ms_loc, dancer->getShininess());

    // Bind danceFloor_light_position
    int light_position = glGetUniformLocation(shader->program, "danceFloor_light_position");
    glUniform3fv(light_position, NR_OF_LIGHT_TILES, glm::value_ptr(dancer->getLightingFTPosition()[0]));

    // Bind danceFloorLightRadius
    int lr_loc = glGetUniformLocation(shader->program, "floorTileLightRadius");
    glUniform1f(lr_loc, danceFloor[0]->getLightRadius());

    // Bind danceFloor_light_color
    int li_loc = glGetUniformLocation(shader->program, "danceFloor_light_color");
    glUniform3fv(li_loc, NR_OF_LIGHT_TILES, glm::value_ptr(dancer->getLightingFTColors()[0]));

    // Bind spotlights positions
    int loc_sl_pos = glGetUniformLocation(shader->program, "spotlight_position");
    glUniform3fv(loc_sl_pos, SPOTLIGHTS, glm::value_ptr(spotlightsPositions[0]));

    // Bind spotlights directions
    int loc_sl_dir = glGetUniformLocation(shader->program, "spotlight_direction");
    glUniform3fv(loc_sl_dir, SPOTLIGHTS, glm::value_ptr(spotlightsDirections[0]));

    // Bind spotlights colors
    int loc_sl_col = glGetUniformLocation(shader->program, "spotlight_color");
    glUniform3f(loc_sl_col, spotlights[0]->getColor().r, spotlights[0]->getColor().g, spotlights[0]->getColor().b);
    glUniform3fv(loc_sl_col, SPOTLIGHTS, glm::value_ptr(spotlightsColors[0]));

    // Bind spotlights cutoffAngle
    int loc_sp_cut = glGetUniformLocation(shader->program, "cutoff_angle");
    glUniform1f(loc_sp_cut, spotlights[0]->getCutoffAngle());

    // Bind spotlights intensity
    int loc_sp_int = glGetUniformLocation(shader->program, "spotlight_intensity");
    glUniform1f(loc_sp_int, spotlights[0]->getIntensity());

    // activate texture channel
    glActiveTexture(GL_TEXTURE0);

    // bind the texture ID
    glBindTexture(GL_TEXTURE_2D, globe->getTexture()->GetTextureID());

    // send the uniform value
    glUniform1i(glGetUniformLocation(shader->program, "globe_texture"), 0);

    // bind disco_ball_position
    int loc_disco = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(loc_disco, globe->getPosition().x, globe->getPosition().y, globe->getPosition().z);

    // bind globe_intensity
    int disco_int_loc = glGetUniformLocation(shader->program, "globe_intensity");
    glUniform1f(disco_int_loc, globe->getIntensity());

    // bind time
    int loc_time = glGetUniformLocation(shader->program, "delta_time");
    glUniform1f(loc_time, deltaTime);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void tema3::Tema3::RenderCone(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix, glm::vec3 color) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Bind color
    int color_loc = glGetUniformLocation(shader->program, "color");
    glUniform3f(color_loc, color.r, color.g, color.b);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void tema3::Tema3::RenderCeil(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Bind is_light_from_globe
    int gl_loc = glGetUniformLocation(shader->program, "is_light_from_globe");
    glUniform1i(gl_loc, lightFromGlobe);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Bind ceil_Kd
    int loc_kd = glGetUniformLocation(shader->program, "ceil_Kd");
    glUniform1f(loc_kd, ceil->getKd());

    // Bind ceil_Ks
    int loc_ks = glGetUniformLocation(shader->program, "ceil_Ks");
    glUniform1f(loc_ks, ceil->getKs());

    // Bind material_shininess
    int ms_loc = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(ms_loc, ceil->getShininess());

    // activate texture channel
    glActiveTexture(GL_TEXTURE0);

    // bind the texture ID
    glBindTexture(GL_TEXTURE_2D, globe->getTexture()->GetTextureID());

    // send the uniform value
    glUniform1i(glGetUniformLocation(shader->program, "globe_texture"), 0);

    // bind disco_ball_position
    int loc_disco = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(loc_disco, globe->getPosition().x, globe->getPosition().y, globe->getPosition().z);

    // bind globe_intensity
    int disco_int_loc = glGetUniformLocation(shader->program, "globe_intensity");
    glUniform1f(disco_int_loc, globe->getIntensity());

    // bind time
    int loc_time = glGetUniformLocation(shader->program, "delta_time");
    glUniform1f(loc_time, deltaTime);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void tema3::Tema3::RenderGlobe(Mesh* mesh, Shader* shader, glm::mat4& modelMatrix) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // activate texture channel
    glActiveTexture(GL_TEXTURE0);

    // bind the texture ID
    glBindTexture(GL_TEXTURE_2D, globe->getTexture()->GetTextureID());

    // send the uniform value
    glUniform1i(glGetUniformLocation(shader->program, "globe_texture"), 0);

    // bind disco_ball_position
    int loc_disco = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(loc_disco, globe->getPosition().x, globe->getPosition().y, globe->getPosition().z);

    // bind time
    int loc_time = glGetUniformLocation(shader->program, "delta_time");
    glUniform1f(loc_time, deltaTime);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

Texture2D* tema3::Tema3::CreateRandomTexture(unsigned int width, unsigned int height) {
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // generate random texture data
    for (unsigned int i = 0; i < size; i++) {
        data[i] = rand() % UCHAR_MAX;
    }

    // generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // u glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

void Tema3::OnInputUpdate(float deltaTime, int mods) {
    /*if (window->KeyHold(GLFW_KEY_LEFT_CONTROL)) {
        if (window->KeyHold(GLFW_KEY_W)) {
            special->moveOX(deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            special->moveOX(-deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            special->moveOZ(deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            special->moveOZ(-deltaTime);
        }
    }*/
}

void Tema3::OnKeyPress(int key, int mods) {
    if (key == GLFW_KEY_1) {
        if (lightFromDanceFloor == 1) {
            lightFromDanceFloor = 0;
        }
        else {
            lightFromDanceFloor = 1;
        }
    }

    if (key == GLFW_KEY_2) {
        if (lightFromSpotLight == 1) {
            lightFromSpotLight = 0;
        }
        else {
            lightFromSpotLight = 1;
        }
    }

    if (key == GLFW_KEY_3) {
        if (lightFromGlobe == 1) {
            lightFromGlobe = 0;
        }
        else {
            lightFromGlobe = 1;
            deltaTime = 0;
        }
    }
}
