#ifndef GOOFYLIB3_H
#define GOOFYLIB3_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


typedef struct { // this is created by a function which returns your rendering buffer
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
} GOOFY_BUFFER;
typedef struct {
    float position[3];
    float colors[3];
    float normals[3];
    float texCoords[2];
    int texIndex; 
    unsigned int is3d;
} GOOFY_VERTICE;

typedef struct {
    GOOFY_VERTICE* vertices;
    size_t vertexCount;
    unsigned int* indices;
    size_t indexCount;
} GOOFY_MESH;

// MAIN MODULE

GLFWwindow* goofy_initWindow(const char* windowName, int width, int height); // Use this to create an OpenGL context then create a new window
GLuint goofy_initShaders(const char* vertexPath,const char* fragmentPath); // Compiles a vertex and fragment shader
GOOFY_BUFFER goofy_initBuffer(GLuint maxVertices, GLuint maxIndices); // Returns a GOOFY_BUFFER object
void goofy_renderMesh(GOOFY_BUFFER* buffer, GOOFY_MESH* mesh, size_t vertexOffset, size_t indexOffset,GLuint shaderProgram); // Renders a mesh object (offsets are not necesarilly needed) you can set them to 0 and it will work 
void goofy_reallocateMesh(GOOFY_MESH* mesh, size_t newVertexCapacity, size_t newIndexCapacity);
void goofy_printMesh(GOOFY_MESH* mesh);
GOOFY_MESH goofy_appendMesh(const GOOFY_MESH* mesh1,const GOOFY_MESH* mesh2);
void goofy_deleteBuffer(GOOFY_BUFFER* buffer); // Run this at the end to delete a buffer
void goofy_initTextures(int textureWidth, int textureHeight, int numLayers); // Initializes a texture array
void goofy_freeMesh(GOOFY_MESH* mesh); // delete a mesh
void goofy_transformMesh(GOOFY_MESH* mesh,float x,float y,float z);
GOOFY_MESH goofy_objMesh(const char* filepath);
int goofy_loadTexture(const char* path, int layerIndex); // Loads a texture from file
void goofy_terminate(); // ONLY UNBINDS YOUR STUFF! DELETE EVERYTHING ELSE MANUALLY
#endif
