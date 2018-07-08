//
// Created by junior on 18-7-8.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "../../include/Shader.h"
using namespace std;
const std::string shader_root = "../../resource/shader/";

// window_size_call_back
void frameBufferSizeCallBack(GLFWwindow * window,int w,int h){
    glViewport(0,0,w,h);
}
void processInput(GLFWwindow*window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
int main(){
    // glfw profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    // create window point
    GLFWwindow * window = glfwCreateWindow(800,600,"HelloWindow", nullptr, nullptr);
    if (window== nullptr){
        std::cout<<"create window fail!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"init glad fail!\n";
        return -1;
    }

    glViewport(0,0,800,600);// render view port

    // register call_back_func
    glfwSetFramebufferSizeCallback(window,frameBufferSizeCallBack);

    Shader shader1("../../resource/shader/hello_world/vertex.vs",
                   "../../resource/shader/hello_world/fragment1.fs");
    Shader shader2("../../resource/shader/hello_world/vertex.vs",
                   "../../resource/shader/hello_world/fragment2.fs");

    float vertices [] ={
            -0.5,0.5,0.0,
            -0.5,-0.5,0.0,
            0.0,0.0,0.0
    };
    float vertices1 [] ={
            0.0,0.0,0.0,
            0.5,0.5,0.0,
            0.5,-0.5,0.0
    };
    unsigned int VAO[2];
    unsigned int VBO[2];
    glGenVertexArrays(2,VAO);
    glGenBuffers(2,VBO);
    for (int i=0;i<2;i++){
        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER,VBO[i]);
        if (i==0)
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);
        else
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1),vertices1, GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3* sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
    }

    while (!glfwWindowShouldClose(window)){
        processInput(window);

        // render
        glClearColor(0.2,0.3,0.3,1.0); // RGBA
        glClear(GL_COLOR_BUFFER_BIT);

        shader1.use();
        float currentTime = glfwGetTime();
        float v=(sin(currentTime)*0.5)+0.5;
        shader1.setFloat4("uniform_color",0.0,v,0.0,1.0);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES,0,3);
        shader2.use();
        shader2.setFloat4("uniform_color",v,0.0,0.0,1.0);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES,0,3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

