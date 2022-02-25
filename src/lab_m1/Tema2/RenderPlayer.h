#pragma once

#include "Tema2.h"

using namespace tema2;

void Tema2::RenderPlayer(bool isThirdPerson) {
    // head
    {
        if (isThirdPerson) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(player->getPosition().x, player->getPosition().y + 2, player->getPosition().z);
            modelMatrix *= transform3D::RotateOY(player->getAngleOY());
            RenderMesh(meshes["box"], modelMatrix, glm::vec3(1, 0.89f, 0.8f));
        }
    }
    // body
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(player->getPosition().x, player->getPosition().y, player->getPosition().z);
        modelMatrix *= transform3D::RotateOY(player->getAngleOY());
        modelMatrix *= transform3D::Scale(0.95f, 2.95f, 1.95f);
        RenderMesh(meshes["box"], modelMatrix, glm::vec3(0, 0.6f, 0.6f));
    }
    // leftArm
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(player->getPosition().x, player->getPosition().y + 0.5f, player->getPosition().z - 1.5f);
        modelMatrix *= transform3D::Translate(0, -0.5f, 1.5f);
        modelMatrix *= transform3D::RotateOY(player->getAngleOY());
        modelMatrix *= transform3D::Translate(0, 0.5f, -1.5f);
        modelMatrix *= transform3D::Scale(1, 1.95f, 0.95f);
        RenderMesh(meshes["box"], modelMatrix, glm::vec3(0, 0.6f, 0.6f));
    }
    // rightArm
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(player->getPosition().x, player->getPosition().y + 0.5f, player->getPosition().z + 1.5f);
        modelMatrix *= transform3D::Translate(0, -0.5f, -1.5f);
        modelMatrix *= transform3D::RotateOY(player->getAngleOY());
        modelMatrix *= transform3D::Translate(0, 0.5f, 1.5f);
        modelMatrix *= transform3D::Scale(1, 1.95f, 0.95f);
        RenderMesh(meshes["box"], modelMatrix, glm::vec3(0, 0.6f, 0.6f));
    }
    // leftHand
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(player->getPosition().x, player->getPosition().y - 1, player->getPosition().z - 1.5f);
        modelMatrix *= transform3D::Translate(0, 1, 1.5f);
        modelMatrix *= transform3D::RotateOY(player->getAngleOY());
        modelMatrix *= transform3D::Translate(0, -1, -1.5f);
        modelMatrix *= transform3D::Scale(1, 0.95f, 0.95f);
        RenderMesh(meshes["box"], modelMatrix, glm::vec3(1, 0.89f, 0.8f));
    }
    // rightHand
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(player->getPosition().x, player->getPosition().y - 1, player->getPosition().z + 1.5f);
        modelMatrix *= transform3D::Translate(0, 1, -1.5f);
        modelMatrix *= transform3D::RotateOY(player->getAngleOY());
        modelMatrix *= transform3D::Translate(0, -1, 1.5f);
        modelMatrix *= transform3D::Scale(1, 0.95f, 0.95f);
        RenderMesh(meshes["box"], modelMatrix, glm::vec3(1, 0.89f, 0.8f));
    }
    // leftLeg
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(player->getPosition().x, player->getPosition().y - 2.5f, player->getPosition().z - 0.5f);
        modelMatrix *= transform3D::Translate(0, 2.5f, 0.5f);
        modelMatrix *= transform3D::RotateOY(player->getAngleOY());
        modelMatrix *= transform3D::Translate(0, -2.5f, -0.5f);
        modelMatrix *= transform3D::Scale(1, 2, 0.95f);
        RenderMesh(meshes["box"], modelMatrix, glm::vec3(0, 0, 1));
    }
    // rightLeg
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(player->getPosition().x, player->getPosition().y - 2.5f, player->getPosition().z + 0.5f);
        modelMatrix *= transform3D::Translate(0, 2.5f, -0.5f);
        modelMatrix *= transform3D::RotateOY(player->getAngleOY());
        modelMatrix *= transform3D::Translate(0, -2.5f, 0.5f);
        modelMatrix *= transform3D::Scale(1, 2, 0.95f);
        RenderMesh(meshes["box"], modelMatrix, glm::vec3(0, 0, 1));
    }
}
