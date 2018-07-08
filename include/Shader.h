//
// Created by junior on 18-1-19.
//

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    unsigned int program_id;
private:
    void checkErrors(unsigned int ID, const std::string&type){
        int success;
        char log[1024];
        if (type!="PROGRAM"){
            glGetShaderiv(ID,GL_COMPILE_STATUS,&success);
            if (!success){
                glGetShaderInfoLog(ID,1024, nullptr,log);
                std::cout<<"ERROR::SHADER_COMPILATION_ERROR of type:"<<type<<" "<<log<<std::endl;
            }
        } else {
            glGetProgramiv(ID,GL_LINK_STATUS,&success);
            if (!success){
                glGetProgramInfoLog(ID,1024, nullptr,log);
                std::cout<<"ERROR::PROGRAM_LINK_ERROR of type:"<<type<<" "<<log<<std::endl;
            }
        }
    }
public:

    Shader(const char* vertex_shader_path, const char* fragment_shader_path){
        std::ifstream vertex_file(vertex_shader_path);
        std::ifstream fragment_file(fragment_shader_path);
        std::stringstream vertex_stream,fragment_stream;
        vertex_stream<<vertex_file.rdbuf();
        fragment_stream<<fragment_file.rdbuf();
	vertex_file.close();
	fragment_file.close();
        std::string vertexCode(vertex_stream.str());
        std::string fragmentCode(fragment_stream.str());
        const char * vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();

        unsigned int vertex_shader_id=glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragment_shader_id=glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vertex_shader_id,1,&vShaderCode, nullptr);
        glCompileShader(vertex_shader_id);
        checkErrors(vertex_shader_id,"VERTEX");
        glShaderSource(fragment_shader_id,1,&fShaderCode, nullptr);
        glCompileShader(fragment_shader_id);
        checkErrors(fragment_shader_id,"FRAGMENT");
        program_id=glCreateProgram();
        glAttachShader(program_id,vertex_shader_id);
        glAttachShader(program_id,fragment_shader_id);
        glLinkProgram(program_id);
        checkErrors(program_id,"PROGRAM");
        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);
    }

    void use(){
        glUseProgram(program_id);
    }
    void setBool(const std::string& name,bool value) const {
        glUniform1i(glGetUniformLocation(program_id,name.c_str()),value);
    }
    void setInt(const std::string& name,int value)const {
        glUniform1i(glGetUniformLocation(program_id,name.c_str()),value);
    }
    void setFloat(const std::string&name, float value)const {
        glUniform1f(glGetUniformLocation(program_id,name.c_str()),value);
    }
    void setFloat4(const std::string&name, float a,float b, float c, float d)const {
        glUniform4f(glGetUniformLocation(program_id,name.c_str()),a,b,c,d);
    }
    ~Shader(){
        glDeleteProgram(program_id);
    }
};
#endif //SHADER_H
