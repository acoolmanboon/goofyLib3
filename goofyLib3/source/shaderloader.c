#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>

char* load_shader_source(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open shader: %s\n", filepath);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = (char*)malloc(length + 1);
    if (source == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for shader source\n");
        fclose(file);
        return NULL;
    }
    
    fread(source, 1, length, file);
    source[length] = '\0';
    
    fclose(file);
    return source;
}

GLuint compile_shader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        fprintf(stderr, "Shader Compilation Error: %s\n", log);
    }

    return shader;
}

GLuint create_shader_program(const char* vertexPath, const char* fragmentPath) {
    char* vertexSource = load_shader_source(vertexPath);
    char* fragmentSource = load_shader_source(fragmentPath);

    if (vertexSource == NULL || fragmentSource == NULL) {
        return 0;
    }

    GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        fprintf(stderr, "Shader Linking Error: %s\n", log);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Free shader sources
    free(vertexSource);
    free(fragmentSource);
    
    return shaderProgram;
}
