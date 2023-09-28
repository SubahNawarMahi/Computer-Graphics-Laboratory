//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 45.0;
float rotateAngle_Y = 45.0;
float rotateAngle_Z = 45.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = -0.5;
float translate_Y = 0.0;
float translate_Z = -0.7;
float scale_X = 0.2;
float scale_Y = 0.2;
float scale_Z = 0.2;

// camera
Camera camera(glm::vec3(3.0f, 4.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 3.0, eyeY = 4.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

bool fan_on = false;
float rotateAngle = 0.0;
void ac_black_color(float sx, float sy, float sz, float tx, float ty, float tz, float angle)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f,
       0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f,
       0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.05f,
       0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.05f,
       0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.05f,
       0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.05f,
       0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.05f,
       0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.05f,
    };
    unsigned int cube_indices[] = {
         0, 3, 2,
         2, 1, 0,

         1, 2, 6,
         6, 5, 1,

         5, 6, 7,
         7 ,4, 5,

         4, 7, 3,
         3, 0, 4,

         6, 2, 3,
         3, 7, 6,

         1, 5, 4,
         4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
   // glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    //ourShader.use();
    //// pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);




    //// camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    ////glm::mat4 view = basic_camera.createViewMatrix();
    //ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + tx, translate_Y + ty, translate_Z + tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + angle), glm::vec3(0.0f, 0.0f, 1.0f));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 10, scale_Y, scale_Z*10));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * sx, scale_Y * sy, scale_Z * sz));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

//void black_color(float x, float z)
//{
//    float cube_vertices[] = {
//       0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f,
//       0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f,
//       0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.05f,
//       0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.05f,
//       0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.05f,
//       0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.05f,
//       0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.05f,
//       0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.05f,
//    };
//    unsigned int cube_indices[] = {
//         0, 3, 2,
//         2, 1, 0,
//
//         1, 2, 6,
//         6, 5, 1,
//
//         5, 6, 7,
//         7 ,4, 5,
//
//         4, 7, 3,
//         3, 0, 4,
//
//         6, 2, 3,
//         3, 7, 6,
//
//         1, 5, 4,
//         4, 0, 1
//    };
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);
//    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
//
//    unsigned int VBO, VAO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
//
//    // position attribute
//   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    //glEnableVertexAttribArray(0);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//    // activate shader
//    ourShader.use();
//    // pass projection matrix to shader (note that in this case it could change every frame)
//    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
//    ourShader.setMat4("projection", projection);
//
//
//
//
//    // camera/view transformation
//   // glm::mat4 view = camera.GetViewMatrix();
//    glm::mat4 view = basic_camera.createViewMatrix();
//    ourShader.setMat4("view", view);
//    // Modelling Transformation
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//
//    //floor_black_color_cube
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + x, translate_Y, translate_Z + z));
//    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 10, scale_Y, scale_Z*10));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 89.9, scale_Y, scale_Z * 89));
//    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    ourShader.setMat4("model", model);
//
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//}


//void white_color(float x, float z)
//{
//    float cube_vertices[] = {
//       0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//       0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//       0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
//       0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
//       0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
//       0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
//       0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
//       0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
//    };
//    unsigned int cube_indices[] = {
//         0, 3, 2,
//         2, 1, 0,
//
//         1, 2, 6,
//         6, 5, 1,
//
//         5, 6, 7,
//         7 ,4, 5,
//
//         4, 7, 3,
//         3, 0, 4,
//
//         6, 2, 3,
//         3, 7, 6,
//
//         1, 5, 4,
//         4, 0, 1
//    };
//    // configure global opengl state
//    // -----------------------------
//    glEnable(GL_DEPTH_TEST);
//    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
//
//    unsigned int VBO, VAO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
//
//    // position attribute
//   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    //glEnableVertexAttribArray(0);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//    // activate shader
//    ourShader.use();
//    // pass projection matrix to shader (note that in this case it could change every frame)
//    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
//    ourShader.setMat4("projection", projection);
//
//
//
//
//    // camera/view transformation
//    glm::mat4 view = camera.GetViewMatrix();
//    //glm::mat4 view = basic_camera.createViewMatrix();
//    ourShader.setMat4("view", view);
//    // Modelling Transformation
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//
//
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + x, translate_Y, translate_Z + z));
//    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 10, scale_Y, scale_Z * 10));
//    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    ourShader.setMat4("model", model);
//
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//
//}
void black_color(float sx, float sy, float sz, float tx, float ty, float tz)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f,
       0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.05f,
       0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.05f,
       0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.05f,
       0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.05f,
       0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.05f,
       0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.05f,
       0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.05f,
    };
    unsigned int cube_indices[] = {
         0, 3, 2,
         2, 1, 0,

         1, 2, 6,
         6, 5, 1,

         5, 6, 7,
         7 ,4, 5,

         4, 7, 3,
         3, 0, 4,

         6, 2, 3,
         3, 7, 6,

         1, 5, 4,
         4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
   // glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    //ourShader.use();
    //// pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);




    //// camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    ////glm::mat4 view = basic_camera.createViewMatrix();
    //ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + tx, translate_Y + ty, translate_Z + tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 10, scale_Y, scale_Z*10));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * sx, scale_Y * sy, scale_Z * sz));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void white_color(float sx, float sy, float sz, float tx, float ty, float tz)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
       0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
    };
    unsigned int cube_indices[] = {
         0, 3, 2,
         2, 1, 0,

         1, 2, 6,
         6, 5, 1,

         5, 6, 7,
         7 ,4, 5,

         4, 7, 3,
         3, 0, 4,

         6, 2, 3,
         3, 7, 6,

         1, 5, 4,
         4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
   // glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    //ourShader.use();
    //// pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);




    //// camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    ////glm::mat4 view = basic_camera.createViewMatrix();
    //ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + tx, translate_Y + ty, translate_Z + tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * sx, scale_Y * sy, scale_Z * sz));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
//chair layers
void chair_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
   0.0f, 0.0f, 0.0f, 0.99207f,0.99207f,0.588f,
   0.5f, 0.0f, 0.0f, 0.99207f,0.99207f,0.588f,
   0.5f, 0.5f, 0.0f, 0.99207f,0.99207f,0.588f,
   0.0f, 0.5f, 0.0f, 0.99207f,0.99207f,0.588f,
   0.0f, 0.0f, 0.5f, 0.99207f,0.99207f,0.588f,
   0.5f, 0.0f, 0.5f, 0.99207f,0.99207f,0.588f,
   0.5f, 0.5f, 0.5f, 0.99207f,0.99207f,0.588f,
   0.0f, 0.5f, 0.5f, 0.99207f,0.99207f,0.588f,
    };

    unsigned int cube_indices[] = {
         0, 3, 2,
         2, 1, 0,

         1, 2, 6,
         6, 5, 1,

         5, 6, 7,
         7 ,4, 5,

         4, 7, 3,
         3, 0, 4,

         6, 2, 3,
         3, 7, 6,

         1, 5, 4,
         4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    //floor_lemon_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

//ceiling
void lemon_color_ceiling(float x, float y, float z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.980f, 0.9490f, 0.73305f,
       0.5f, 0.0f, 0.0f, 0.980f, 0.9490f, 0.733f,
       0.5f, 0.5f, 0.0f, 0.980f, 0.9490f, 0.733f,
       0.0f, 0.5f, 0.0f, 0.980f, 0.9490f, 0.733f,
       0.0f, 0.0f, 0.5f, 0.980f, 0.9490f, 0.733f,
       0.5f, 0.0f, 0.5f, 0.980f, 0.9490f, 0.733f,
       0.5f, 0.5f, 0.5f, 0.980f, 0.9490f, 0.733f,
       0.0f, 0.5f, 0.5f, 0.980f, 0.9490f, 0.733f,
    };
    unsigned int cube_indices[] = {
         0, 3, 2,
         2, 1, 0,

         1, 2, 6,
         6, 5, 1,

         5, 6, 7,
         7 ,4, 5,

         4, 7, 3,
         3, 0, 4,

         6, 2, 3,
         3, 7, 6,

         1, 5, 4,
         4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_lemon_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + x, translate_Y + y, translate_Z + z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 10, scale_Y, scale_Z*10));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * 89.9, scale_Y, scale_Z * 89));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void wall_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.741f, 0.549f, 0.898f, //purple wall
        0.5f, 0.0f, 0.0f,  0.741f, 0.549f, 0.898f,
        0.5f, 0.5f, 0.0f,  0.741f, 0.549f, 0.898f,
        0.0f, 0.5f, 0.0f,  0.741f, 0.549f, 0.898f,

        0.5f, 0.0f, 0.0f, 0.741f, 0.549f, 0.898f,
        0.5f, 0.5f, 0.0f,  0.741f, 0.549f, 0.898f,
        0.5f, 0.0f, 0.5f,  0.741f, 0.549f, 0.898f,
        0.5f, 0.5f, 0.5f, 0.741f, 0.549f, 0.898f,

        0.0f, 0.0f, 0.5f,  0.741f, 0.549f, 0.898f,
        0.5f, 0.0f, 0.5f, 0.741f, 0.549f, 0.898f,
        0.5f, 0.5f, 0.5f,  0.741f, 0.549f, 0.898f,
        0.0f, 0.5f, 0.5f, 0.741f, 0.549f, 0.898f,

        0.0f, 0.0f, 0.5f,  0.741f, 0.549f, 0.898f,
        0.0f, 0.5f, 0.5f, 0.741f, 0.549f, 0.898f,
        0.0f, 0.5f, 0.0f,  0.741f, 0.549f, 0.898f,
        0.0f, 0.0f, 0.0f,  0.741f, 0.549f, 0.898f,

        0.5f, 0.5f, 0.5f, 0.741f, 0.549f, 0.898f,
        0.5f, 0.5f, 0.0f,  0.741f, 0.549f, 0.898f,
        0.0f, 0.5f, 0.0f, 0.741f, 0.549f, 0.898f,
        0.0f, 0.5f, 0.5f, 0.741f, 0.549f, 0.898f,

        0.0f, 0.0f, 0.0f, 0.741f, 0.549f, 0.898f,
        0.5f, 0.0f, 0.0f,  0.741f, 0.549f, 0.898f,
        0.5f, 0.0f, 0.5f,  0.741f, 0.549f, 0.898f,
        0.0f, 0.0f, 0.5f,  0.741f, 0.549f, 0.898f,
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;



    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}

void almirah(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
       0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void laptop_low(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.9019f,0.9058f,0.9098f,
        0.5f, 0.0f, 0.0f,  0.9019f,0.9058f,0.9098f,
        0.5f, 0.5f, 0.0f,  0.9019f,0.9058f,0.9098f,
        0.0f, 0.5f, 0.0f,  0.9019f,0.9058f,0.9098f,

        0.5f, 0.0f, 0.0f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,

        /*0.0f, 0.0f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,*/
        0.0f, 0.0f, 0.5f,  0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, //keypad
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.0f, 0.0f,  0.796f, 0.674f, 0.533f,

        0.5f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,

        0.0f, 0.0f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.0f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.0f, 0.0f, 0.5f, 0.796f, 0.674f, 0.533f,
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void laptop_keypad(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.9019f,0.9058f,0.9098f,
        0.5f, 0.0f, 0.0f,   0.9019f,0.9058f,0.9098f,
        0.5f, 0.5f, 0.0f,   0.9019f,0.9058f,0.9098f,
        0.0f, 0.5f, 0.0f,  0.9019f,0.9058f,0.9098f,

        0.5f, 0.0f, 0.0f, 0.9019f,0.9058f,0.9098f,
        0.5f, 0.5f, 0.0f,  0.9019f,0.9058f,0.9098f,
        0.5f, 0.0f, 0.5f, 0.9019f,0.9058f,0.9098f,
        0.5f, 0.5f, 0.5f,0.9019f,0.9058f,0.9098f,

        /*0.0f, 0.0f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,*/
        0.0f, 0.0f, 0.5f,  0.9019f,0.9058f,0.9098f,
        0.5f, 0.0f, 0.5f,0.9019f,0.9058f,0.9098f,
        0.5f, 0.5f, 0.5f, 0.9019f,0.9058f,0.9098f, //keypad
        0.0f, 0.5f, 0.5f,0.9019f,0.9058f,0.9098f,

        0.0f, 0.0f, 0.5f, 0.9019f,0.9058f,0.9098f,
        0.0f, 0.5f, 0.5f,  0.9019f,0.9058f,0.9098f,
        0.0f, 0.5f, 0.0f,  0.9019f,0.9058f,0.9098f,
        0.0f, 0.0f, 0.0f, 0.9019f,0.9058f,0.9098f,

        0.5f, 0.5f, 0.5f, 0.9019f,0.9058f,0.9098f,
        0.5f, 0.5f, 0.0f,  0.9019f,0.9058f,0.9098f,
        0.0f, 0.5f, 0.0f, 0.9019f,0.9058f,0.9098f,
        0.0f, 0.5f, 0.5f,0.9019f,0.9058f,0.9098f,

        0.0f, 0.0f, 0.0f, 0.9019f,0.9058f,0.9098f,
        0.5f, 0.0f, 0.0f, 0.9019f,0.9058f,0.9098f,
        0.5f, 0.0f, 0.5f,0.9019f,0.9058f,0.9098f,
        0.0f, 0.0f, 0.5f, 0.9019f,0.9058f,0.9098f,
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void laptop_up(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    //0.9019f,0.9058f,0.9098f,
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f,1.0f,1.0f,
        0.5f, 0.0f, 0.0f,  1.0f,1.0f,1.0f,
        0.5f, 0.5f, 0.0f,  1.0f,1.0f,1.0f,
        0.0f, 0.5f, 0.0f,  1.0f,1.0f,1.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f,  0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, //laptop screen

        /*0.0f, 0.0f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,*/
        0.0f, 0.0f, 0.5f,  0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.5f,  0.9019f,0.9058f,0.9098f,
        0.0f, 0.5f, 0.5f,  0.9019f,0.9058f,0.9098f,
        0.0f, 0.5f, 0.0f,  0.9019f,0.9058f,0.9098f,
        0.0f, 0.0f, 0.0f,  0.9019f,0.9058f,0.9098f,

        0.5f, 0.5f, 0.5f, 0.9019f,0.9058f,0.9098f,
        0.5f, 0.5f, 0.0f,  0.9019f,0.9058f,0.9098f,
        0.0f, 0.5f, 0.0f,  0.9019f,0.9058f,0.9098f,
        0.0f, 0.5f, 0.5f, 0.9019f,0.9058f,0.9098f,

        0.0f, 0.0f, 0.0f,  0.9019f,0.9058f,0.9098f,
        0.5f, 0.0f, 0.0f, 0.9019f,0.9058f,0.9098f,
        0.5f, 0.0f, 0.5f, 0.9019f,0.9058f,0.9098f,
        0.0f, 0.0f, 0.5f, 0.9019f,0.9058f,0.9098f,
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void laptop_screen(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    //0.9019f,0.9058f,0.9098f,
    //0.04313f,0.4078f,0.698f
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.04313f,0.4078f,0.698f,
        0.5f, 0.0f, 0.0f,  0.04313f,0.4078f,0.698f,
        0.5f, 0.5f, 0.0f,  0.04313f,0.4078f,0.698f,
        0.0f, 0.5f, 0.0f,  0.04313f,0.4078f,0.698f,

        0.5f, 0.0f, 0.0f, 0.04313f,0.4078f,0.698f,
        0.5f, 0.5f, 0.0f,  0.04313f,0.4078f,0.698f,
        0.5f, 0.0f, 0.5f,  0.04313f,0.4078f,0.698f,
        0.5f, 0.5f, 0.5f, 0.04313f,0.4078f,0.698f, //laptop screen

        /*0.0f, 0.0f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,*/
        0.0f, 0.0f, 0.5f,  0.04313f,0.4078f,0.698f,
        0.5f, 0.0f, 0.5f, 0.04313f,0.4078f,0.698f,
        0.5f, 0.5f, 0.5f, 0.04313f,0.4078f,0.698f,
        0.0f, 0.5f, 0.5f, 0.04313f,0.4078f,0.698f,

        0.0f, 0.0f, 0.5f,  0.04313f,0.4078f,0.698f,
        0.0f, 0.5f, 0.5f,  0.04313f,0.4078f,0.698f,
        0.0f, 0.5f, 0.0f,  0.04313f,0.4078f,0.698f,
        0.0f, 0.0f, 0.0f,  0.04313f,0.4078f,0.698f,

        0.5f, 0.5f, 0.5f, 0.04313f,0.4078f,0.698f,
        0.5f, 0.5f, 0.0f, 0.04313f,0.4078f,0.698f,
        0.0f, 0.5f, 0.0f,  0.04313f,0.4078f,0.698f,
        0.0f, 0.5f, 0.5f, 0.04313f,0.4078f,0.698f,

        0.0f, 0.0f, 0.0f,  0.04313f,0.4078f,0.698f,
        0.5f, 0.0f, 0.0f,0.04313f,0.4078f,0.698f,
        0.5f, 0.0f, 0.5f, 0.04313f,0.4078f,0.698f,
        0.0f, 0.0f, 0.5f, 0.04313f,0.4078f,0.698f,
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-270.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}


void almirah_shelf(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.254f, 0.0078f, 0.0f,
       0.5f, 0.0f, 0.0f, 0.254f, 0.0078f, 0.0f,
       0.5f, 0.5f, 0.0f, 0.254f, 0.0078f, 0.0f,
       0.0f, 0.5f, 0.0f, 0.254f, 0.0078f, 0.0f,
       0.0f, 0.0f, 0.5f, 0.254f, 0.0078f, 0.0f,
       0.5f, 0.0f, 0.5f, 0.254f, 0.0078f, 0.0f,
       0.5f, 0.5f, 0.5f, 0.254f, 0.0078f, 0.0f,
       0.0f, 0.5f, 0.5f, 0.254f, 0.0078f, 0.0f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}

void bed_layer_1_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.0f,  0.796f, 0.674f, 0.533f,

        0.5f, 0.0f, 0.0f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,

        0.0f, 0.0f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,

        0.0f, 0.0f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.5f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.0f, 0.0f,  0.796f, 0.674f, 0.533f,

        0.5f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.5f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.0f, 0.5f, 0.5f, 0.796f, 0.674f, 0.533f,

        0.0f, 0.0f, 0.0f,  0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.0f, 0.796f, 0.674f, 0.533f,
        0.5f, 0.0f, 0.5f, 0.796f, 0.674f, 0.533f,
        0.0f, 0.0f, 0.5f, 0.796f, 0.674f, 0.533f,
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void bed_layer_2_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.411f, 0.403f, 0.423f,
        0.5f, 0.0f, 0.0f,  0.411f, 0.403f, 0.423f,
        0.5f, 0.5f, 0.0f,  0.411f, 0.403f, 0.423f,
        0.0f, 0.5f, 0.0f,  0.411f, 0.403f, 0.423f,

        0.5f, 0.0f, 0.0f,  0.411f, 0.403f, 0.423f,
        0.5f, 0.5f, 0.0f,  0.411f, 0.403f, 0.423f,
        0.5f, 0.0f, 0.5f,  0.411f, 0.403f, 0.423f,
        0.5f, 0.5f, 0.5f,  0.411f, 0.403f, 0.423f,

        0.0f, 0.0f, 0.5f,   0.411f, 0.403f, 0.423f,
        0.5f, 0.0f, 0.5f,  0.411f, 0.403f, 0.423f,
        0.5f, 0.5f, 0.5f,  0.411f, 0.403f, 0.423f,
        0.0f, 0.5f, 0.5f,  0.411f, 0.403f, 0.423f,

        0.0f, 0.0f, 0.5f,   0.411f, 0.403f, 0.423f,
        0.0f, 0.5f, 0.5f,  0.411f, 0.403f, 0.423f,
        0.0f, 0.5f, 0.0f,   0.411f, 0.403f, 0.423f,
        0.0f, 0.0f, 0.0f,   0.411f, 0.403f, 0.423f,

        0.5f, 0.5f, 0.5f,  0.411f, 0.403f, 0.423f,
        0.5f, 0.5f, 0.0f,  0.411f, 0.403f, 0.423f,
        0.0f, 0.5f, 0.0f,   0.411f, 0.403f, 0.423f,
        0.0f, 0.5f, 0.5f,  0.411f, 0.403f, 0.423f,

        0.0f, 0.0f, 0.0f,  0.411f, 0.403f, 0.423f,
        0.5f, 0.0f, 0.0f,  0.411f, 0.403f, 0.423f,
        0.5f, 0.0f, 0.5f,  0.411f, 0.403f, 0.423f,
        0.0f, 0.0f, 0.5f,  0.411f, 0.403f, 0.423f,
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void bed_sheet_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.956f, 0.94f, 0.92f,
 0.5f, 0.0f, 0.0f,  0.956f, 0.94f, 0.92f,
 0.5f, 0.5f, 0.0f,  0.956f, 0.94f, 0.92f,
 0.0f, 0.5f, 0.0f,  0.956f, 0.94f, 0.92f,

 0.5f, 0.0f, 0.0f,  0.956f, 0.94f, 0.92f,
 0.5f, 0.5f, 0.0f,  0.956f, 0.94f, 0.92f,
 0.5f, 0.0f, 0.5f,  0.956f, 0.94f, 0.92f,
 0.5f, 0.5f, 0.5f,  0.956f, 0.94f, 0.92f,

 0.0f, 0.0f, 0.5f,   0.956f, 0.94f, 0.92f,
 0.5f, 0.0f, 0.5f,  0.956f, 0.94f, 0.92f,
 0.5f, 0.5f, 0.5f,  0.956f, 0.94f, 0.92f,
 0.0f, 0.5f, 0.5f,  0.956f, 0.94f, 0.92f,

 0.0f, 0.0f, 0.5f,   0.956f, 0.94f, 0.92f,
 0.0f, 0.5f, 0.5f,  0.956f, 0.94f, 0.92f,
 0.0f, 0.5f, 0.0f,   0.956f, 0.94f, 0.92f,
 0.0f, 0.0f, 0.0f,   0.956f, 0.94f, 0.92f,

 0.5f, 0.5f, 0.5f,  0.956f, 0.94f, 0.92f,
 0.5f, 0.5f, 0.0f,  0.956f, 0.94f, 0.92f,
 0.0f, 0.5f, 0.0f,   0.956f, 0.94f, 0.92f,
 0.0f, 0.5f, 0.5f,  0.956f, 0.94f, 0.92f,

 0.0f, 0.0f, 0.0f,  0.956f, 0.94f, 0.92f,
 0.5f, 0.0f, 0.0f,  0.956f, 0.94f, 0.92f,
 0.5f, 0.0f, 0.5f,  0.956f, 0.94f, 0.92f,
 0.0f, 0.0f, 0.5f,  0.956f, 0.94f, 0.92f,


    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void bed_pillow_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
      0.0f, 0.0f, 0.0f, 1.0f,0.812f,0.8588f,
0.5f, 0.0f, 0.0f, 1.0f,0.812f,0.8588f,
0.5f, 0.5f, 0.0f,  1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.0f, 1.0f,0.812f,0.8588f,

0.5f, 0.0f, 0.0f,  1.0f,0.812f,0.8588f,
0.5f, 0.5f, 0.0f,  1.0f,0.812f,0.8588f,
0.5f, 0.0f, 0.5f,  1.0f,0.812f,0.8588f,
0.5f, 0.5f, 0.5f,  1.0f,0.812f,0.8588f,

0.0f, 0.0f, 0.5f,  1.0f,0.812f,0.8588f,
0.5f, 0.0f, 0.5f, 1.0f,0.812f,0.8588f,
0.5f, 0.5f, 0.5f, 1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.5f, 1.0f,0.812f,0.8588f,

0.0f, 0.0f, 0.5f,   1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.5f,  1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.0f,   1.0f,0.812f,0.8588f,
0.0f, 0.0f, 0.0f,   1.0f,0.812f,0.8588f,

0.5f, 0.5f, 0.5f,  1.0f,0.812f,0.8588f,
0.5f, 0.5f, 0.0f,  1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.0f,  1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.5f,  1.0f,0.812f,0.8588f,

0.0f, 0.0f, 0.0f,  1.0f,0.812f,0.8588f,
0.5f, 0.0f, 0.0f,  1.0f,0.812f,0.8588f,
0.5f, 0.0f, 0.5f,  1.0f,0.812f,0.8588f,
0.0f, 0.0f, 0.5f,  1.0f,0.812f,0.8588f,



    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void carpet(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
      0.0f, 0.0f, 0.0f, 1.0f,0.812f,0.8588f,
0.5f, 0.0f, 0.0f, 1.0f,0.812f,0.8588f,
0.5f, 0.5f, 0.0f,  1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.0f, 1.0f,0.812f,0.8588f,

0.5f, 0.0f, 0.0f,  1.0f,0.812f,0.8588f,
0.5f, 0.5f, 0.0f,  1.0f,0.812f,0.8588f,
0.5f, 0.0f, 0.5f,  1.0f,0.812f,0.8588f,
0.5f, 0.5f, 0.5f,  1.0f,0.812f,0.8588f,

0.0f, 0.0f, 0.5f,  1.0f,0.812f,0.8588f,
0.5f, 0.0f, 0.5f, 1.0f,0.812f,0.8588f,
0.5f, 0.5f, 0.5f, 1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.5f, 1.0f,0.812f,0.8588f,

0.0f, 0.0f, 0.5f,   1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.5f,  1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.0f,   1.0f,0.812f,0.8588f,
0.0f, 0.0f, 0.0f,   1.0f,0.812f,0.8588f,

0.5f, 0.5f, 0.5f,  1.0f,0.812f,0.8588f,
0.5f, 0.5f, 0.0f,  1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.0f,  1.0f,0.812f,0.8588f,
0.0f, 0.5f, 0.5f,  1.0f,0.812f,0.8588f,

0.0f, 0.0f, 0.0f,  1.0f,0.812f,0.8588f,
0.5f, 0.0f, 0.0f,  1.0f,0.812f,0.8588f,
0.5f, 0.0f, 0.5f,  1.0f,0.812f,0.8588f,
0.0f, 0.0f, 0.5f,  1.0f,0.812f,0.8588f,



    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void carpet2(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.996f,0.2039f,0.494f,
 0.5f, 0.0f, 0.0f, 0.996f,0.2039f,0.494f,
 0.5f, 0.5f, 0.0f,  0.996f,0.2039f,0.494f,
 0.0f, 0.5f, 0.0f, 0.996f,0.2039f,0.494f,

 0.5f, 0.0f, 0.0f,  0.996f,0.2039f,0.494f,
 0.5f, 0.5f, 0.0f,  0.996f,0.2039f,0.494f,
 0.5f, 0.0f, 0.5f,  0.996f,0.2039f,0.494f,
 0.5f, 0.5f, 0.5f,  0.996f,0.2039f,0.494f,

 0.0f, 0.0f, 0.5f,  0.996f,0.2039f,0.494f,
 0.5f, 0.0f, 0.5f, 0.996f,0.2039f,0.494f,
 0.5f, 0.5f, 0.5f, 0.996f,0.2039f,0.494f,
 0.0f, 0.5f, 0.5f, 0.996f,0.2039f,0.494f,

 0.0f, 0.0f, 0.5f,   0.996f,0.2039f,0.494f,
 0.0f, 0.5f, 0.5f,  0.996f,0.2039f,0.494f,
 0.0f, 0.5f, 0.0f,   0.996f,0.2039f,0.494f,
 0.0f, 0.0f, 0.0f,   0.996f,0.2039f,0.494f,

 0.5f, 0.5f, 0.5f,  0.996f,0.2039f,0.494f,
 0.5f, 0.5f, 0.0f,  0.996f,0.2039f,0.494f,
 0.0f, 0.5f, 0.0f,  0.996f,0.2039f,0.494f,
 0.0f, 0.5f, 0.5f,  0.996f,0.2039f,0.494f,

 0.0f, 0.0f, 0.0f,  0.996f,0.2039f,0.494f,
 0.5f, 0.0f, 0.0f,  0.996f,0.2039f,0.494f,
 0.5f, 0.0f, 0.5f,  0.996f,0.2039f,0.494f,
 0.0f, 0.0f, 0.5f,  0.996f,0.2039f,0.494f,



    };

    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void book_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
       0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
       0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
       0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void book_color_1(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.24f, 0.58f, 0.50f,
       0.5f, 0.0f, 0.0f,  0.24f, 0.58f, 0.50f,
       0.5f, 0.5f, 0.0f,  0.24f, 0.58f, 0.50f,
       0.0f, 0.5f, 0.0f,  0.24f, 0.58f, 0.50f,
       0.0f, 0.0f, 0.5f,  0.24f, 0.58f, 0.50f,
       0.5f, 0.0f, 0.5f, 0.24f, 0.58f, 0.50f,
       0.5f, 0.5f, 0.5f,  0.24f, 0.58f, 0.50f,
       0.0f, 0.5f, 0.5f,  0.24f, 0.58f, 0.50f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}

void book_color_2(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.494f, 0.505f, 0.16f,
       0.5f, 0.0f, 0.0f,  0.494f, 0.505f, 0.16f,
       0.5f, 0.5f, 0.0f, 0.494f, 0.505f, 0.16f,
       0.0f, 0.5f, 0.0f,  0.494f, 0.505f, 0.16f,
       0.0f, 0.0f, 0.5f, 0.494f, 0.505f, 0.16f,
       0.5f, 0.0f, 0.5f, 0.494f, 0.505f, 0.16f,
       0.5f, 0.5f, 0.5f,  0.494f, 0.505f, 0.16f,
       0.0f, 0.5f, 0.5f,  0.494f, 0.505f, 0.16f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}

void book_color_3(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.294f, 0.68f, 0.235f,
       0.5f, 0.0f, 0.0f,   0.294f, 0.68f, 0.235f,
       0.5f, 0.5f, 0.0f,  0.294f, 0.68f, 0.235f,
       0.0f, 0.5f, 0.0f,   0.294f, 0.68f, 0.235f,
       0.0f, 0.0f, 0.5f,  0.294f, 0.68f, 0.235f,
       0.5f, 0.0f, 0.5f,  0.294f, 0.68f, 0.235f,
       0.5f, 0.5f, 0.5f,  0.294f, 0.68f, 0.235f,
       0.0f, 0.5f, 0.5f,   0.294f, 0.68f, 0.235f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}
void book_color_4(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.239f, 1.0f, 0.87f,
       0.5f, 0.0f, 0.0f,  0.239f, 1.0f, 0.87f,
       0.5f, 0.5f, 0.0f,  0.239f, 1.0f, 0.87f,
       0.0f, 0.5f, 0.0f,  0.239f, 1.0f, 0.87f,
       0.0f, 0.0f, 0.5f,  0.239f, 1.0f, 0.87f,
       0.5f, 0.0f, 0.5f,  0.239f, 1.0f, 0.87f,
       0.5f, 0.5f, 0.5f,  0.239f, 1.0f, 0.87f,
       0.0f, 0.5f, 0.5f,  0.239f, 1.0f, 0.87f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}

void book1(float tx, float ty, float tz)
{
    book_color(1, 3, 3, 3 + tx, 2 + ty, 2 + tz);
    book_color_1(0.2, 3, 3, 2.98 + tx, 1.99 + ty, 2 + tz);
    book_color_1(0.2, 3, 3.15, 3.1 + tx, 2.035 + ty, 2 + tz);
    book_color_1(1.48, 3, 0.2, 2.98 + tx, 1.99 + ty, 2.3 + tz);
}

void book2(float tx, float ty, float tz)
{
    book_color(1, 3, 3, 3 + tx, 2 + ty, 2 + tz);
    book_color_2(0.2, 3, 3, 2.98 + tx, 1.99 + ty, 2 + tz);
    book_color_2(0.2, 3, 3.15, 3.1 + tx, 2.035 + ty, 2 + tz);
    book_color_2(1.48, 3, 0.2, 2.98 + tx, 1.99 + ty, 2.3 + tz);
}

void book3(float tx, float ty, float tz)
{
    book_color(1, 3, 3, 3 + tx, 2 + ty, 2 + tz);
    book_color_3(0.2, 3, 3, 2.98 + tx, 1.99 + ty, 2 + tz);
    book_color_3(0.2, 3, 3.15, 3.1 + tx, 2.035 + ty, 2 + tz);
    book_color_3(1.48, 3, 0.2, 2.98 + tx, 1.99 + ty, 2.3 + tz);
}

void book4(float tx, float ty, float tz)
{
    book_color(1, 3, 3, 3 + tx, 2 + ty, 2 + tz);
    book_color_4(0.2, 3, 3, 2.98 + tx, 1.99 + ty, 2 + tz);
    book_color_4(0.2, 3, 3.15, 3.1 + tx, 2.035 + ty, 2 + tz);
    book_color_4(1.48, 3, 0.2, 2.98 + tx, 1.99 + ty, 2.3 + tz);
}


void wall_shelf_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
       0.0f, 0.0f, 0.0f, 0.964f, 0.89f, 0.819f,
       0.5f, 0.0f, 0.0f, 0.964f, 0.89f, 0.819f,
       0.5f, 0.5f, 0.0f,  0.964f, 0.89f, 0.819f,
       0.0f, 0.5f, 0.0f,0.964f, 0.89f, 0.819f,
       0.0f, 0.0f, 0.5f, 0.964f, 0.89f, 0.819f,
       0.5f, 0.0f, 0.5f, 0.964f, 0.89f, 0.819f,
       0.5f, 0.5f, 0.5f,  0.964f, 0.89f, 0.819f,
       0.0f, 0.5f, 0.5f, 0.964f, 0.89f, 0.819f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);




    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}

void wall_shelf_1(float tx, float ty, float tz)
{
    wall_shelf_color(8, 0.3, -5, 5 + tx, 2.3 + ty, 8.9 + tz);
    wall_shelf_color(0.3, 8, -5, 5 + tx, 2.3 + ty, 8.9 + tz);
    wall_shelf_color(0.3, 8, -5, 5.8 + tx, 2.3 + ty, 8.9 + tz);
    wall_shelf_color(8.3, 0.3, -5, 5 + tx, 3.1 + ty, 8.9 + tz);
}
void window_glass_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
         0.0f, 0.0f, 0.0f,   0.615f, 0.866f, 0.964f,
         0.5f, 0.0f, 0.0f,   0.615f, 0.866f, 0.964f,
         0.5f, 0.5f, 0.0f,   0.615f, 0.866f, 0.964f,
         0.0f, 0.5f, 0.0f,   0.615f, 0.866f, 0.964f,
         0.0f, 0.0f, 0.5f,   0.615f, 0.866f, 0.964f,
         0.5f, 0.0f, 0.5f,   0.615f, 0.866f, 0.964f,
         0.5f, 0.5f, 0.5f,   0.615f, 0.866f, 0.964f,
         0.0f, 0.5f, 0.5f,   0.615f, 0.866f, 0.964f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    //glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //// activate shader
    //ourShader.use();
    //// pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);




    //// camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    ////glm::mat4 view = basic_camera.createViewMatrix();
    //ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void white_color2(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
         0.0f, 0.0f, 0.0f,   0.607f, 0.811f, 0.901f,
         0.5f, 0.0f, 0.0f,   0.607f, 0.811f, 0.901f,
         0.5f, 0.5f, 0.0f,   0.607f, 0.811f, 0.901f,
         0.0f, 0.5f, 0.0f,   0.607f, 0.811f, 0.901f,
         0.0f, 0.0f, 0.5f,   0.607f, 0.811f, 0.901f,
         0.5f, 0.0f, 0.5f,   0.607f, 0.811f, 0.901f,
         0.5f, 0.5f, 0.5f,   0.607f, 0.811f, 0.901f,
         0.0f, 0.5f, 0.5f,   0.607f, 0.811f, 0.901f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    //glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //// activate shader
    //ourShader.use();
    //// pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);




    //// camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    ////glm::mat4 view = basic_camera.createViewMatrix();
    //ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void pyramid_draw(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float pyramid_vertices[] = {
    -0.5f,0.0f,0.5f,  0.83f,0.70f,0.44f,
    -0.5f,0.0f,-0.5f,  0.83f,0.70f,0.44f,
    0.5f,0.0f,-0.5f,  0.83f,0.70f,0.44f,
    0.5f,0.0f,0.5f,  0.83f,0.70f,0.44f,
    0.0f,0.8f,0.0f,  0.92f,0.86f,0.76f,
    };
    unsigned int pyramid_indices[] = {
        0, 1, 2,
        0, 2, 3,
        0,1,4,
        1,2,4,
        2,3,4,
        3,0,4
    };
    //glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //   glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    // activate shader
    ourShader.use();

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    //  glm::mat4 view = basic_camera.createViewMatrix();
    ourShader.setMat4("view", view);

    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, rotationPyramid;
    //new portion pyramid
//
//
//
// pass projection matrix to shader (note that in this case it could change every frame)
//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        //ourShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
       // glm::mat4 view = basic_camera.createViewMatrix();
        //ourShader.setMat4("view", view);

        // Modelling Transformation
       // glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
       // glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    //  rotationPyramid = glm::rotate(identityMatrix, glm::radians(rotationP), glm::vec3(0.0f, 1.0f, 0.0f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    //ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(pyramid_indices) / sizeof(int), GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //////


}
void bed_side_wardrobe(float tx, float ty, float tz)
{
    white_color2(-6, 2.5, 10, 7.85 + tx, 0.1 + ty, 6.1 + tz);
    white_color(-6, 2.5, 10, 7.85 + tx, 0.35 + ty, 6.1 + tz);
    bed_pillow_color(-6.5, 1, 11, 7.85 + tx, 0.6 + ty, 6.05 + tz);

    black_color(0.2, 0.2, 2, 7.2 + tx, 0.43 + ty, 6.5 + tz);
    black_color(0.2, 0.2, 2, 7.2 + tx, 0.25 + ty, 6.5 + tz);
}
void dressing_table_color_1(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
         0.0f, 0.0f, 0.0f,   0.494f, 0.207f, 0.133f,
         0.5f, 0.0f, 0.0f,   0.494f, 0.207f, 0.133f,
         0.5f, 0.5f, 0.0f,   0.494f, 0.207f, 0.133f,
         0.0f, 0.5f, 0.0f,   0.494f, 0.207f, 0.133f,
         0.0f, 0.0f, 0.5f,   0.494f, 0.207f, 0.133f,
         0.5f, 0.0f, 0.5f,   0.494f, 0.207f, 0.133f,
         0.5f, 0.5f, 0.5f,   0.494f, 0.207f, 0.133f,
         0.0f, 0.5f, 0.5f,   0.494f, 0.207f, 0.133f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    //glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //// activate shader
    //ourShader.use();
    //// pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);




    //// camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    ////glm::mat4 view = basic_camera.createViewMatrix();
    //ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void dressing_table_color_2(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
         0.0f, 0.0f, 0.0f,   0.152f, 0.086f, 0.054f,
         0.5f, 0.0f, 0.0f,   0.152f, 0.086f, 0.054f,
         0.5f, 0.5f, 0.0f,   0.152f, 0.086f, 0.054f,
         0.0f, 0.5f, 0.0f,   0.152f, 0.086f, 0.054f,
         0.0f, 0.0f, 0.5f,   0.152f, 0.086f, 0.054f,
         0.5f, 0.0f, 0.5f,   0.152f, 0.086f, 0.054f,
         0.5f, 0.5f, 0.5f,   0.152f, 0.086f, 0.054f,
         0.0f, 0.5f, 0.5f,   0.152f, 0.086f, 0.054f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    //glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //// activate shader
    //ourShader.use();
    //// pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);




    //// camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    ////glm::mat4 view = basic_camera.createViewMatrix();
    //ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
//fan blade
void chocolate_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    /*float cube_vertices[] = {
         0.0f, 0.0f, 0.0f,   0.874f, 0.827f, 0.654f,
         0.5f, 0.0f, 0.0f,   0.874f, 0.827f, 0.654f,
         0.5f, 0.5f, 0.0f,   0.874f, 0.827f, 0.654f,
         0.0f, 0.5f, 0.0f,   0.874f, 0.827f, 0.654f,
         0.0f, 0.0f, 0.5f,   0.874f, 0.827f, 0.654f,
         0.5f, 0.0f, 0.5f,   0.874f, 0.827f, 0.654f,
         0.5f, 0.5f, 0.5f,   0.874f, 0.827f, 0.654f,
         0.0f, 0.5f, 0.5f,   0.874f, 0.827f, 0.654f,
    };*/
    float cube_vertices[] = {
           0.0f, 0.0f, 0.0f,   0.3529f, 0.24313f, 0.24313f,
           0.5f, 0.0f, 0.0f,   0.3529f, 0.24313f, 0.24313f,
           0.5f, 0.5f, 0.0f,   0.3529f, 0.24313f, 0.24313f,
           0.0f, 0.5f, 0.0f,   0.3529f, 0.24313f, 0.24313f,
           0.0f, 0.0f, 0.5f,   0.3529f, 0.24313f, 0.24313f,
           0.5f, 0.0f, 0.5f,   0.3529f, 0.24313f, 0.24313f,
           0.5f, 0.5f, 0.5f,   0.3529f, 0.24313f, 0.24313f,
           0.0f, 0.5f, 0.5f,   0.3529f, 0.24313f, 0.24313f,
    };

    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //// activate shader
    //ourShader.use();
    //// pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);




    //// camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    ////glm::mat4 view = basic_camera.createViewMatrix();
    //ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
//fan rod
void chocolate_color2(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
     0.0f, 0.0f, 0.0f,   0.23529f, 0.14902f, 0.14902f,
     0.5f, 0.0f, 0.0f,   0.23529f, 0.14902f, 0.14902f,
     0.5f, 0.5f, 0.0f,   0.23529f, 0.14902f, 0.14902f,
     0.0f, 0.5f, 0.0f,   0.23529f, 0.14902f, 0.14902f,
     0.0f, 0.0f, 0.5f,   0.23529f, 0.14902f, 0.14902f,
     0.5f, 0.0f, 0.5f,   0.23529f, 0.14902f, 0.14902f,
     0.5f, 0.5f, 0.5f,   0.23529f, 0.14902f, 0.14902f,
     0.0f, 0.5f, 0.5f,   0.23529f, 0.14902f, 0.14902f,
    };

    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //// activate shader
    //ourShader.use();
    //// pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);




    //// camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    ////glm::mat4 view = basic_camera.createViewMatrix();
    //ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void chocolate_color2_2(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z)
{
    float cube_vertices[] = {
         0.0f, 0.0f, 0.0f,   0.878f, 0.682f, 0.596f,
         0.5f, 0.0f, 0.0f,   0.878f, 0.682f, 0.596f,
         0.5f, 0.5f, 0.0f,   0.878f, 0.682f, 0.596f,
         0.0f, 0.5f, 0.0f,   0.878f, 0.682f, 0.596f,
         0.0f, 0.0f, 0.5f,   0.878f, 0.682f, 0.596f,
         0.5f, 0.0f, 0.5f,   0.878f, 0.682f, 0.596f,
         0.5f, 0.5f, 0.5f,   0.878f, 0.682f, 0.596f,
         0.0f, 0.5f, 0.5f,   0.878f, 0.682f, 0.596f,
    };
    unsigned int cube_indices[] = {
      0, 3, 2,
      2, 1, 0,

      1, 2, 6,
      6, 5, 1,

      5, 6, 7,
      7 ,4, 5,

      4, 7, 3,
      3, 0, 4,

      6, 2, 3,
      3, 7, 6,

      1, 5, 4,
      4, 0, 1
    };
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);
    //// activate shader
    //ourShader.use();
    //// pass projection matrix to shader (note that in this case it could change every frame)
    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    //ourShader.setMat4("projection", projection);




    //// camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    ////glm::mat4 view = basic_camera.createViewMatrix();
    //ourShader.setMat4("view", view);
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.3f, 0.8f, 0.5f,
        0.5f, 0.0f, 0.0f, 0.5f, 0.4f, 0.3f,
        0.5f, 0.5f, 0.0f, 0.2f, 0.7f, 0.3f,
        0.0f, 0.5f, 0.0f, 0.6f, 0.2f, 0.8f,
        0.0f, 0.0f, 0.5f, 0.8f, 0.3f, 0.6f,
        0.5f, 0.0f, 0.5f, 0.4f, 0.4f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.2f, 0.3f, 0.6f,
        0.0f, 0.5f, 0.5f, 0.7f, 0.5f, 0.4f
    };*/
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f
    };*/
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f,

        0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.5f,  1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.5f,  1.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.5f,  1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.5f, 0.0f, 0.5f,  1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.5f,  1.0f, 1.0f, 1.0f
    };



    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    /*unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7 ,4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };*/
    /* float cube_vertices[] = {
         -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
         -0.5f,  0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,

         -0.5f, -0.5f,  0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,
         -0.5f, -0.5f,  0.5f,

         -0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,

          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,

         -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
          0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f, -0.5f,

         -0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f,  0.5f,
          0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f, -0.5f,
     };*/
     // world space positions of our cubes
     /*glm::vec3 cubePositions[] = {
         glm::vec3(0.0f,  0.0f,  0.0f),
         glm::vec3(2.0f,  5.0f, -15.0f),
         glm::vec3(-1.5f, -2.2f, -2.5f),
         glm::vec3(-3.8f, -2.0f, -12.3f),
         glm::vec3(2.4f, -0.4f, -3.5f),
         glm::vec3(-1.7f,  3.0f, -7.5f),
         glm::vec3(1.3f, -2.0f, -2.5f),
         glm::vec3(1.5f,  2.0f, -2.5f),
         glm::vec3(1.5f,  0.2f, -1.5f),
         glm::vec3(-1.3f,  1.0f, -1.5f)
     };*/
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.847f, 0.94f, 0.949f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        /* // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);




        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        ourShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


        //floor_white_color_cube
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X*15 , scale_Y, scale_Z*15 ));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        ourShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/



        //for (int i = 0; i <= 9; i=i+1)
        // {
        //     for (int j = 0; j <= 4; j=j+1)
        //     {
        //         if ((i + j) % 2 == 0)
        //         {
        //             black_color(i, j);
        //         }
        //         else
        //         {
        //             white_color(i, j);
        //         }

        //     }

        // }
         //black_color(0.1, 0.02);
       //pyramid_draw(2, 4, 2, 2.5, 2.5, 3.5);
        lemon_color_ceiling(0.1, 0, 0.02);
        lemon_color_ceiling(0.1, 4, 0.02); //ceiling


        //  wall_color(90, 40, 1, 0, 0, -0.1);
        wall_color(1, 40, 90, 9, 0, 0);
        wall_color(90, 40, 1, 0, 0, 8.9);

        //window portion starts
        //wall + window
        wall_color(50, 40, 1, 0, 0, -0.1);//window left wall
        wall_color(15, 40, 1, 7.5, 0, -0.1);//window right wall

        wall_color(25, 15, 1, 5, 0, -0.1);//window below wall

        wall_color(25, 5, 1, 5, 3.5, -0.1);//window up wall


        white_color2(0.5, 20, 0.3, 6.0, 1.5, -0.1);

        white_color2(0.5, 20, 0.3, 6.85, 1.5, -0.1);


        white_color(30, 0.7, 2, 5, 3.6, 0);
        white_color(25, 1, 1, 5, 1.5, -0.1);
        //porda
        bed_pillow_color(3, -28, 0.1, 5, 3.6, 0.1);
        bed_pillow_color(3, -28, 0.1, 5.3, 3.6, 0.1);
        bed_pillow_color(3, -28, 0.1, 5.6, 3.6, 0.1);
        bed_pillow_color(8, -28, 0.1, 7.1, 3.6, 0.1);
        //almirah
       //almirah(15,25,1,0.2,0,0);

        //laptop
        //laptop

        laptop_up(0.5, 10.20408, 6, 2.5, 1.19, 0.3);
        laptop_screen(0.001, 9.20408, 3.5, 2.5 + 0.045, 1.19 + 0.13 + 0.05, 0.35);
        laptop_low(6, 10.20408, 0.5, 2.5, 1.19, 0.3);
        // laptop_keypad(3, 8.20408, 0.1 + 0.1, 0.2 + 0.7, 0.3, 0.2);
        laptop_keypad(3, 8.20408, 0.1, 2.5 + 0.1, 1.19 + 0.045, 0.3 + 0.1);

        //tube light
        white_color(17, 0.5, 0.5, 2.5, 3.7, 0.01);
        //AC
        white_color(-3, 5, 13, 8.9, 3, 3.5);
        ac_black_color(0.2, 0.5, 12, 8.58, 3.05, 3.55, 0);
        // black_color(0.2, 0.5, 5, 8.58, 3.2, 3.85);

         //Clock

        ac_black_color(5, 6, 0.1, 3.1, 3, 0, 0);
        white_color(4.5, 5.5, 0.1, 3.125, 3.025, 0.01);

        ac_black_color(0.1, 2, 0.1, 3.34, 3.3, 0.02, 0);
        ac_black_color(0.1, 2, 0.1, 3.35, 3.3, 0.02, 150);
        ac_black_color(0.1, 2, 0.1, 3.34, 3.3, 0.02, 270);

        // laptop_up(0.5, 10.20408, 6, 2.5, 1.19+0.05, 0.3);
        // laptop_screen(0.001, 9.20408, 3.5, 2.5+0.045, 1.19+0.13+0.05, 0.35);
        // laptop_low(6, 10.20408, 0.5, 2.5, 1.19+0.05, 0.3);
        //// laptop_keypad(3, 8.20408, 0.1 + 0.1, 0.2 + 0.7, 0.3, 0.2);
        // laptop_keypad(3, 8.20408, 0.1 , 2.5+0.1, 1.19+0.045+0.05, 0.3+0.1);

         /*bookshelf*/

       // almirah(14.7, 25, 1, 0.2, 0.1, 0); //inside wood
        bed_layer_1_color(14.7, 25, 1, 0.2, 0.1, 0); //inside wood
        bed_layer_1_color(1, 25, 10, 0.2, 0.1, 0);
        bed_layer_1_color(1, 25, 10, 1.6, 0.1, 0);
        bed_layer_1_color(15, 1, 10, 0.2, 2.5, 0);

        ////bookshelf shelves
        almirah_shelf(13, 1, 10, 0.3, 2.0, 0);
        almirah_shelf(13, 1, 10, 0.3, 1.5, 0);
        almirah_shelf(13, 1, 10, 0.3, 1, 0);
        almirah_shelf(13, 1, 10, 0.3, 0.5, 0);
        //bedside wardrobe
        //bed side wardrobe
       // bed_side_wardrobe(0, 0, -1);
        bed_side_wardrobe(0, 0, -4);

        //dressing table
        dressing_table_color_1(-6, 8, 20, 8, 0.1, 5.35);
        dressing_table_color_1(-0.2, 15, 17, 7.9, 0.9, 5.5);
        white_color(-0.2, 13, 15, 7.85, 1.0, 5.6);
        dressing_table_color_2(0.5, 3, 12, 7.35, 0.5, 5.40);
        dressing_table_color_2(0.5, 3, 12, 7.35, 0.15, 5.40);

        dressing_table_color_2(0.5, 7, 4, 7.35, 0.15, 6.8);
        white_color(0.2, 0.2, 1.8, 7.34, 0.5, 6.9);

        white_color(0.2, 0.2, 1.8, 7.34, 0.65, 5.9);
        white_color(0.2, 0.2, 1.8, 7.34, 0.3, 5.9);

        ////almirah wardrobe


        bed_layer_1_color(10, 25.5, -10, 0.88, 0.1, 8.50); //body
        dressing_table_color_2(9, 24, 0.5, 0.94, 0.17, 7.49); // door
        white_color(0.5, 2, 0.5, 1.7, 1.2, 7.45); // door
        //almirah(10, 25, -15, 6.90, 0.1 , 7.50); //body
        //bed_layer_1_color(2, 25, -15, 6.88, 0.1, 7.50); //door
        //bed
        bed_layer_1_color(-31, 2 + 3, -15, 7.99, 0.1, 4.9);
        bed_layer_1_color(1, 8, -15, 7.90, 0.1 + 0.3, 4.90);
        bed_sheet_color(-30, 0.5, -15, 7.90, 0.6, 4.90);

        bed_pillow_color(-4, 1, -5, 7.90, 0.35 + 0.3, 4.80);
        bed_pillow_color(-4, 1, -5, 7.90, 0.35 + 0.3, 4.0);
        //blanket

        bed_pillow_color(19, 0.5, 13, 5.2, 0.33 + 0.3, 3.5);
        //carpet
        //         //column0
        // carpet2(5, 0.5, 5, 2.2, 0.12 , 3.0);
        carpet(5, 0.5, 5, 2.2 + 0.5, 0.12, 3.0);
        carpet2(5, 0.5, 5, 2.2 + 1, 0.12, 3.0);
        carpet(5, 0.5, 5, 2.2 + 1.5, 0.12, 3.0);
        carpet2(5, 0.5, 5, 2.2 + 2.0, 0.12, 3.0);
        //column1
       // carpet(5, 0.5, 5, 2.2, 0.12 , 3.5);
        carpet2(5, 0.5, 5, 2.2 + 0.5, 0.12, 3.5);
        carpet(5, 0.5, 5, 2.2 + 1, 0.12, 3.5);
        carpet2(5, 0.5, 5, 2.2 + 1.5, 0.12, 3.5);
        carpet(5, 0.5, 5, 2.2 + 2.0, 0.12, 3.5);

        //row,column2
      //  carpet2(5, 0.5, 5, 2.2 , 0.12, 3.5 + 0.5);
        carpet(5, 0.5, 5, 2.2 + 0.5, 0.12, 4.0);
        carpet2(5, 0.5, 5, 2.2 + 1, 0.12, 4.0);
        carpet(5, 0.5, 5, 2.2 + 1.5, 0.12, 4.0);
        carpet2(5, 0.5, 5, 2.2 + 2.0, 0.12, 4.0);
        //column3
      //  carpet(5, 0.5, 5, 2.2, 0.12, 4.5);
        carpet2(5, 0.5, 5, 2.2 + 0.5, 0.12, 4.5);
        carpet(5, 0.5, 5, 2.2 + 1, 0.12, 4.5);
        carpet2(5, 0.5, 5, 2.2 + 1.5, 0.12, 4.5);
        carpet(5, 0.5, 5, 2.2 + 2.0, 0.12, 4.5);
        //column4
//  carpet2(5, 0.5, 5, 2.2, 0.12, 4.5+0.5);
        carpet(5, 0.5, 5, 2.2 + 0.5, 0.12, 4.5 + 0.5);
        carpet2(5, 0.5, 5, 2.2 + 1, 0.12, 4.5 + 0.5);
        carpet(5, 0.5, 5, 2.2 + 1.5, 0.12, 4.5 + 0.5);
        carpet2(5, 0.5, 5, 2.2 + 2.0, 0.12, 4.5 + 0.5);


        //table //0.207f,0.207f,0.588f,
        chair_color(-31, 2, 16, 5.1, 1, 0.2);
        chair_color(1, 10, 1, 5, 0.1, 0.2);
        chair_color(1, 10, 1, 2, 0.1, 0.2);
        chair_color(1, 10, 1, 2, 0.1, 1.7);
        chair_color(1, 10, 1, 5, 0.1, 1.7);

        //chair
        bed_layer_2_color(1, 6, 1, 3, 0.1, 1.3);
        bed_layer_2_color(10, 1, 8, 3, 0.7, 1.3);
        // bed_layer_1_color(1, 1, 1, 3, 0.1, 1.3);
        bed_layer_2_color(1, 6, 1, 3.90, 0.1, 1.3);
        bed_layer_2_color(1, 15, 1, 3, 0.1, 2);
        bed_layer_2_color(1, 15, 1, 3.90, 0.1, 2);
        bed_layer_2_color(10, 1, 1, 3, 0.9, 2);
        bed_layer_2_color(10, 1, 1, 3, 1.1, 2);
        bed_layer_2_color(10, 1, 1, 3, 1.3, 2);
        bed_layer_2_color(10, 1, 1, 3, 1.5, 2);
        // bed_layer_1_color(1, 1, 1, 3, 0.1, 2);
        // bed_layer_1_color(1, 1, 1, 3, 0.1, 2);
         //bed_layer_1_color(1, 1, 1, 3, 0.1, 2);


        //books
        //shelf_top_1
        book1(-2.58, 0.1, -1.3);
        book2(-2.43, 0.1, -1.3);
        //book3(-2.28, 0.1, -1.3);
        //book4(-2.13, 0.1, -1.3);
        //book1(-1.98, 0.1, -1.3);
        //book2(-1.83, 0.1, -1.3);
        //book3(-1.68, 0.1, -1.3);
        //book4(-1.53, 0.1, -1.3);

        ////shelf_top_2
        book2(-2.58, -0.4, -1.3);
        book3(-2.43, -0.4, -1.3);
        //book4(-2.28, -0.4, -1.3);
        //book1(-2.13, -0.4, -1.3);
        //book2(-1.98, -0.4, -1.3);
        //book3(-1.83, -0.4, -1.3);
        //book4(-1.68, -0.4, -1.3);
        //book1(-1.53, -0.4, -1.3);
        ////shelf_top_3
        book3(-2.58, -0.9, -1.3);
        book4(-2.43, -0.9, -1.3);
        //book1(-2.28, -0.9, -1.3);
        //book2(-2.13, -0.9, -1.3);
        //book3(-1.98, -0.9, -1.3);
        //book4(-1.83, -0.9, -1.3);
        //book1(-1.68, -0.9, -1.3);
        //book2(-1.53, -0.9, -1.3);

        ////shelf_top_4
        //book4(-2.58, -1.4, -1.3);
        //book1(-2.43, -1.4, -1.3);
        //book2(-2.28, -1.4, -1.3);
        //book3(-2.13, -1.4, -1.3);
        //book4(-1.98, -1.4, -1.3);
        //book1(-1.83, -1.4, -1.3);
        //book2(-1.68, -1.4, -1.3);
        //book3(-1.53, -1.4, -1.3);





        //wall_shelf
        wall_shelf_1(0, 0, 0);
        wall_shelf_1(0.8, 0.5, 0);
        wall_shelf_1(-0.8, 0.5, 0);

        //wall_shelf_color(20, 1, -6, 4, 3, 8.9);

        //fan starts here

        chocolate_color2(1, -5, 1, 4, 4, 4);

        chair_color(1.5, 1.5, 1.4, 4.05, 3.45, 4.05);
        chair_color(-1.5, 1.5, 1.4, 4.05, 3.45, 4.05);

        chair_color(1.5, 1.5, -1.4, 4.05, 3.45, 4.05);
        chair_color(-1.5, 1.5, -1.4, 4.05, 3.45, 4.05);
        //fan blade
        chocolate_color(-10, 0.1, 1.5, 4.05, 3.5, 4.05);
        chocolate_color(-10, 0.1, -1.5, 4.05, 3.5, 4.05);

        chocolate_color(10, 0.1, 1.5, 4.05, 3.5, 4.05);
        chocolate_color(10, 0.1, -1.5, 4.05, 3.5, 4.05);

        chocolate_color(1.5, 0.1, 10, 4.05, 3.5, 4.05);
        chocolate_color(-1.5, 0.1, 10, 4.05, 3.5, 4.05);

        chocolate_color(1.5, 0.1, -10, 4.05, 3.5, 4.05);
        chocolate_color(-1.5, 0.1, -10, 4.05, 3.5, 4.05);
        
        int coun = 0;
        if (fan_on)
        {
            int coun = 0;
            if (coun < 5)
            {
                rotateAngle += 15;
                coun+=1;
            }
            else if (coun>5 && coun < 11)
            {
                rotateAngle += 17;
                coun+=1;
            }
            else if (coun > 11 )
            {
                rotateAngle += 20;
                coun += 1;
            }
            


        }



        /*
        // render boxes
        for (unsigned int i = 0; i < 10; i++)
        {
          // calculate the model matrix for each object and pass it to shader before drawing
           glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
           model = glm::translate(model, cubePositions[i]);
           float angle = 20.0f * i;
           model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
           ourShader.setMat4("model", model);

           glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);*/

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (!fan_on)
        {
            fan_on = true;
        }
        else
        {
            fan_on = false;
        }

    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
        /*if (fan_on)
        {
            fan_on = false;
            int coun1 = 0;
            if (coun1 < 5)
            {
                rotateAngle += 20;
                coun1 += 1;
            }
            else if (coun1 > 5 && coun1 < 11)
            {
                rotateAngle += 15;
                coun1 += 1;
            }
            else if (coun1 > 11)
            {
                rotateAngle += 25;
                coun1 += 1;
            }
        }*/
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 1;
        else if (rotateAxis_Y) rotateAngle_Y -= 1;
        else rotateAngle_Z -= 1;
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        /* lookAtX += 2.5 * deltaTime;
         basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);*/
        camera.ProcessKeyboard(YAW_LEFT, deltaTime);
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        /*lookAtX -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);*/
        camera.ProcessKeyboard(YAW_RIGHT, deltaTime);
        
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        /*lookAtY += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);*/
        camera.ProcessKeyboard(PITCH_R, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {/*
        lookAtY -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);*/
        camera.ProcessKeyboard(PITCH_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        /*lookAtZ += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);*/
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        /*  lookAtZ -= 2.5 * deltaTime;
          basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);*/
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 0.0f, 1.0f));
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
