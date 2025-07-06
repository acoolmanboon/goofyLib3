#ifndef GOOFYLIB3_H
#define GOOFYLIB3_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


typedef struct {
    GLuint VAO, VBO, EBO;
    size_t MAX_MESHES;

    size_t meshCount;
    size_t* vertexOffsets;
    size_t* indexOffsets;
    size_t* vertexCounts;
    size_t* indexCounts;

    size_t currentVerticeOffset;
    size_t currentIndiceOffset;
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

GLFWwindow* goofy_initWindow(const char* windowName, int width, int height, int major_version, int minor_version); // Use this to create an OpenGL context then create a new window
GLuint goofy_initShaders(const char* vertexPath,const char* fragmentPath); // Compiles a vertex and fragment shader
GOOFY_BUFFER goofy_initBuffer(GLuint maxVertices, GLuint maxIndices, size_t maxMeshes); // Returns a GOOFY_BUFFER object
void goofy_renderMesh(GOOFY_BUFFER* buffer, GOOFY_MESH* mesh, GLuint shaderProgram); // Renders a mesh object
void goofy_drawAllMeshes(GOOFY_BUFFER* buffer, GLuint shaderProgram); // Draws all meshes in a buffer instance
void goofy_reallocateMesh(GOOFY_MESH* mesh, size_t newVertexCapacity, size_t newIndexCapacity); // reallocates a mesh's size
void goofy_printMesh(GOOFY_MESH* mesh); // prints a mesh data (useful for debugging)
GOOFY_MESH goofy_appendMesh(const GOOFY_MESH* mesh1,const GOOFY_MESH* mesh2); // appends meshes to other meshes (might get deprecated)
void goofy_deleteBuffer(GOOFY_BUFFER* buffer); // you need to run this at the end or the buffers will memory leak or smth bad
void goofy_initTextures(int textureWidth, int textureHeight, int numLayers); // initializes global texture array only run this oonce
void goofy_freeMesh(GOOFY_MESH* mesh); // delete a mesh you need to run this at the end
void goofy_transformMesh(GOOFY_MESH* mesh,float x,float y,float z); // moves a mesh
GOOFY_MESH goofy_objMesh(const char* filepath); // loads a mesh from an obj file
int goofy_loadTexture(const char* path, int layerIndex); // Loads a texture from file
void goofy_terminate(); // THIS DOES NOT DO CLEANUP FOR STUFF ILL ADD IT LATER

#endif
