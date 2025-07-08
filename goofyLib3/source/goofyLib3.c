// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include "goofyLib3.h"
#include "shaderloader.h"

// stuff
#define VERTEX_MEMORY_SIZE sizeof(GOOFY_VERTICE)
#define STB_IMAGE_IMPLEMENTATION
#define INCREASE 2048
#define MAX_TRASH 64
#include "stb_image.h"

/*
sizeof(char) = 1
sizeof(short) = 2
sizeof(int) = 4
sizeof(long) = 4
sizeof(long long) = 8
sizeof(float) = 4
sizeof(double) = 8
sizeof(long double) = 16
*/
GLuint textureArray; // Declare the texture array globally or pass as a parameter
GOOFY_TRASH_BATCH* goofy_trashRegistry[MAX_TRASH];
short goofy_trashRegistryCount = 0;

// skibidi Functions
void APIENTRY glDebugOutput(GLenum source,GLenum type, unsigned int id,GLenum severity,GLsizei length,const char *message,const void *userParam){
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    printf("---------------\n");
    printf("Debug message (%u): %s\n", id, message);

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             printf("Source: API\n"); break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("Source: Window System\n"); break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler\n"); break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("Source: Third Party\n"); break;
    case GL_DEBUG_SOURCE_APPLICATION:     printf("Source: Application\n"); break;
    case GL_DEBUG_SOURCE_OTHER:           printf("Source: Other\n"); break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               printf("Type: Error\n"); break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour\n"); break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("Type: Undefined Behaviour\n"); break;
    case GL_DEBUG_TYPE_PORTABILITY:         printf("Type: Portability\n"); break;
    case GL_DEBUG_TYPE_PERFORMANCE:         printf("Type: Performance\n"); break;
    case GL_DEBUG_TYPE_MARKER:              printf("Type: Marker\n"); break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          printf("Type: Push Group\n"); break;
    case GL_DEBUG_TYPE_POP_GROUP:           printf("Type: Pop Group\n"); break;
    case GL_DEBUG_TYPE_OTHER:               printf("Type: Other\n"); break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         printf("Severity: high\n"); break;
    case GL_DEBUG_SEVERITY_MEDIUM:       printf("Severity: medium\n"); break;
    case GL_DEBUG_SEVERITY_LOW:          printf("Severity: low\n"); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification\n"); break;
    }

    printf("\n");
}
void error_callback(int error, const char* description){
    fprintf(stderr, "[GOOFYLIB3] ERROR : %s\n", description);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glfwMakeContextCurrent(window); // 👈 Ensure the correct context
    glViewport(0, 0, width, height);
}


// MAIN MODULE
GLFWwindow* goofy_initWindow(const char* windowName, short width, short height, char major_version, char minor_version) {
    glfwSetErrorCallback(error_callback);
    printf("[GOOFYLIB3] Initializing window %s\n",windowName); 
    if (!glfwInit()) {
        printf("[GOOFYLIB3] Failed to initialize GLFW for window %s\n",windowName);
        return NULL; 
    } else {
        printf("[GOOFYLIB3] Succesfully initialized GLFW for window %s\n",windowName); 
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_FALSE);  
    
    // WINDOW MAKE
    GLFWwindow* window = glfwCreateWindow(width, height, windowName, NULL, NULL);
    if (!window) {
        printf("[GOOFYLIB3] Failed to create a window for window %s\n",windowName);
        glfwTerminate();
        return NULL;
    } else {
        printf("[GOOFYLIB3] Succesfully initialized a window for window %s\n",windowName); 
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("[GOOFYLIB3] Failed to initialize GLAD for window %s\n",windowName);
        glfwDestroyWindow(window);
        glfwTerminate();
        return NULL;  
    } else {
        printf("[GOOFYLIB3] Succesfully initialized GLAD for window %s\n",windowName); 
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(glDebugOutput, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    } 
    printf("[GOOFYLIB3] Succesfully made window %s\n",windowName); 
    return window;
}
GLuint goofy_initShaders(const char* vertexPath,const char* fragmentPath) {
    GLuint shaderProgram = create_shader_program(vertexPath,fragmentPath);
    if (shaderProgram == 0) {
        printf("Failed to create shader program\n");
        exit(1);
    }
    return shaderProgram;
}
GOOFY_BUFFER goofy_initBuffer(GLuint maxVertices, GLuint maxIndices, size_t maxMeshes) {
    GOOFY_BUFFER buffer;
    buffer.MAX_MESHES = maxMeshes;
    buffer.meshCount = 0;
    buffer.currentVerticeOffset = 0;
    buffer.currentIndiceOffset = 0;

    // alloc arrays
    buffer.vertexOffsets = (size_t*)malloc(sizeof(size_t) * maxMeshes);
    buffer.indexOffsets = (size_t*)malloc(sizeof(size_t) * maxMeshes);
    buffer.vertexCounts = (size_t*)malloc(sizeof(size_t) * maxMeshes);
    buffer.indexCounts = (size_t*)malloc(sizeof(size_t) * maxMeshes);
    printf("[GOOFYLIB3] Succesfully allocated buffers\n");
    if (!buffer.vertexOffsets || !buffer.indexOffsets || !buffer.vertexCounts || !buffer.indexCounts) {
        printf("[GOOFYLIB3] Failed to allocate memory for mesh data arrays\n");
    }

    glGenVertexArrays(1, &buffer.VAO);
    glBindVertexArray(buffer.VAO);
    if (buffer.VAO == 0) {
        printf("[GOOFYLIB3] Failed to generate VAO\n");
    } else {
        printf("[GOOFYLIB3] Successfully initialized VAO\n");
    }

    glGenBuffers(1, &buffer.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
    if (buffer.VBO == 0) {
        printf("[GOOFYLIB3] Failed to generate VBO\n");
    } else {
        printf("[GOOFYLIB3] Allocating VBO with size: %zu\n", maxVertices * sizeof(GOOFY_VERTICE));
        glBufferData(GL_ARRAY_BUFFER, maxVertices * sizeof(GOOFY_VERTICE), NULL, GL_DYNAMIC_DRAW);
        printf("[GOOFYLIB3] Successfully initialized VBO\n");
    }

    glGenBuffers(1, &buffer.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
    if (buffer.EBO == 0) {
        printf("[GOOFYLIB3] Failed to generate EBO.\n");
    } else {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * maxIndices, NULL, GL_DYNAMIC_DRAW);
        printf("[GOOFYLIB3] Successfully initialized EBO\n");
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_MEMORY_SIZE, (void*)offsetof(GOOFY_VERTICE, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_MEMORY_SIZE, (void*)offsetof(GOOFY_VERTICE, colors));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_MEMORY_SIZE, (void*)offsetof(GOOFY_VERTICE, normals));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_MEMORY_SIZE, (void*)offsetof(GOOFY_VERTICE, texCoords));
    glEnableVertexAttribArray(3);

    glVertexAttribIPointer(4, 1, GL_INT, VERTEX_MEMORY_SIZE, (void*)offsetof(GOOFY_VERTICE, texIndex));
    glEnableVertexAttribArray(4);

    glVertexAttribIPointer(5, 1, GL_UNSIGNED_INT, VERTEX_MEMORY_SIZE, (void*)offsetof(GOOFY_VERTICE, is3d));
    glEnableVertexAttribArray(5);

    printf("[GOOFYLIB3] Initialized all vertex attributes for VAO\n");

    glBindVertexArray(0);

    return buffer;
}
void goofy_renderMesh(GOOFY_BUFFER* buffer, GOOFY_MESH* mesh, GLuint shaderProgram) {
    if (buffer->meshCount >= buffer->MAX_MESHES) {
        fprintf(stderr, "Exceeded max mesh count!\n");
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, buffer->currentVerticeOffset * sizeof(GOOFY_VERTICE), mesh->vertexCount * sizeof(GOOFY_VERTICE), mesh->vertices);

    unsigned int* adjustedIndices = malloc(sizeof(unsigned int) * mesh->indexCount);
    for (size_t i = 0; i < mesh->indexCount; ++i) {
        adjustedIndices[i] = mesh->indices[i] + buffer->currentVerticeOffset;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, buffer->currentIndiceOffset * sizeof(unsigned int), mesh->indexCount * sizeof(unsigned int), adjustedIndices);
    free(adjustedIndices);

    size_t idx = buffer->meshCount++;
    buffer->vertexOffsets[idx] = buffer->currentVerticeOffset;
    buffer->indexOffsets[idx] = buffer->currentIndiceOffset;
    buffer->currentIndiceOffset += mesh->indexCount;
    buffer->currentVerticeOffset += mesh->vertexCount;
    buffer->vertexCounts[idx] = mesh->vertexCount;
    buffer->indexCounts[idx] = mesh->indexCount;
}
void goofy_drawAllMeshes(GOOFY_BUFFER* buffer, GLuint shaderProgram) {
    if (buffer->meshCount == 0) return;

    glBindVertexArray(buffer->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->EBO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureArray"), 0);

    GLsizei* countArray = malloc(sizeof(GLsizei) * buffer->meshCount);
    const void** indexOffsetArray = malloc(sizeof(void*) * buffer->meshCount);
    GLint* baseVertexArray = malloc(sizeof(GLint) * buffer->meshCount);

    for (size_t i = 0; i < buffer->meshCount; ++i) {
        countArray[i] = (GLsizei)buffer->indexCounts[i];
        indexOffsetArray[i] = (const void*)(buffer->indexOffsets[i] * sizeof(unsigned int));
        baseVertexArray[i] = 0; // indices already adjusted
    }

    glMultiDrawElementsBaseVertex(
        GL_TRIANGLES,
        countArray,
        GL_UNSIGNED_INT,
        indexOffsetArray,
        (GLsizei)buffer->meshCount,
        baseVertexArray
    );
    buffer->currentIndiceOffset = 0;
    buffer->currentVerticeOffset = 0;
    buffer->meshCount = 0;
    free(countArray);
    free(indexOffsetArray);
    free(baseVertexArray);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
void goofy_terminate() {
    for (short i = 0; i < goofy_trashRegistryCount; ++i) {
        if (goofy_trashRegistry[i]) {
            goofy_freeTrashBatch(goofy_trashRegistry[i]);
            goofy_trashRegistry[i] = NULL;
        }
    }
    goofy_trashRegistryCount = 0;
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glBindVertexArray(0);
}

// FILE-LOADING functions
GOOFY_MESH goofy_objMesh(const char* filepath) {
    GOOFY_MESH result = {0}; 
    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "[GOOFYLIB3] Failed to open OBJ file: %s\n", filepath);
        return result;  
    }

    char line[2048];
    int bufferAmount = 2048;

    float *x = (float *)malloc(sizeof(float) * bufferAmount);
    float *y = (float *)malloc(sizeof(float) * bufferAmount);
    float *z = (float *)malloc(sizeof(float) * bufferAmount);
    int vertexCap = bufferAmount;
    int vertex = 0;

    float *vtx = (float *)malloc(sizeof(float) * bufferAmount);
    float *vty = (float *)malloc(sizeof(float) * bufferAmount);
    int texCap = bufferAmount;
    int texCoord = 0;

    float *nx = (float *)malloc(sizeof(float) * bufferAmount);
    float *ny = (float *)malloc(sizeof(float) * bufferAmount);
    float *nz = (float *)malloc(sizeof(float) * bufferAmount);
    int normalCap = bufferAmount;
    int normal = 0;


    int faces = 0;

    printf("[GOOFYLIB3] Parsing OBJ file: %s\n", filepath);
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;
    
        if (strncmp(line, "vt ", 3) == 0) {
            if (texCoord >= vertexCap) {
                bufferAmount += INCREASE;
                vtx = realloc(vtx, sizeof(float) * bufferAmount);
                vty = realloc(vty, sizeof(float) * bufferAmount);
                vertexCap = bufferAmount;
            }
            sscanf(line, "vt %f %f", &vtx[texCoord], &vty[texCoord]);
            texCoord++;
        } else if (strncmp(line, "vn ", 3) == 0) {
            if (normal >= vertexCap) {
                bufferAmount += INCREASE;
                nx = realloc(nx, sizeof(float) * bufferAmount);
                ny = realloc(ny, sizeof(float) * bufferAmount);
                nz = realloc(nz, sizeof(float) * bufferAmount);
                vertexCap = bufferAmount;
            }
            sscanf(line, "vn %f %f %f", &nx[normal], &ny[normal], &nz[normal]);
            normal++;
        } else if (strncmp(line, "v ", 2) == 0) {
            if (vertex >= vertexCap) {
                bufferAmount += INCREASE;
                x = realloc(x, sizeof(float) * bufferAmount);
                y = realloc(y, sizeof(float) * bufferAmount);
                z = realloc(z, sizeof(float) * bufferAmount);
                vertexCap = bufferAmount;
            }
            sscanf(line, "v %f %f %f", &x[vertex], &y[vertex], &z[vertex]);
            vertex++;
        } else if (strncmp(line, "f ", 2) == 0) {
            faces++;
        }
    }

    printf("[GOOFYLIB3] Parsed %d vertices, %d texture coords, %d normals, %d faces\n", vertex, texCoord, normal, faces);

    result.vertexCount = faces * 3;  
    result.indexCount = faces * 3;
    result.vertices = malloc(sizeof(GOOFY_VERTICE) * result.indexCount);
    result.indices = malloc(sizeof(unsigned int) * result.indexCount);

    if (!result.vertices || !result.indices) {
        fprintf(stderr, "[GOOFYLIB3] Memory allocation failed\n");
        free(x); free(y); free(z); free(vtx); free(vty); free(nx); free(ny); free(nz);
        fclose(file);
        return result;
    }

    int indexCount = 0;
    int vertexCount = 0;
    rewind(file);  
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "f ", 2) == 0) {
            int vIndex[3], tIndex[3], nIndex[3];
            int parsedCount = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                                      &vIndex[0], &tIndex[0], &nIndex[0], 
                                      &vIndex[1], &tIndex[1], &nIndex[1], 
                                      &vIndex[2], &tIndex[2], &nIndex[2]);

            if (parsedCount == 9) {
                /*
                printf("  Parsed face: v/t/n %d/%d/%d %d/%d/%d %d/%d/%d\n", 
                        vIndex[0], tIndex[0], nIndex[0],
                        vIndex[1], tIndex[1], nIndex[1],
                        vIndex[2], tIndex[2], nIndex[2]);
                */

                GOOFY_VERTICE v[3];
                for (int i = 0; i < 3; ++i) {
                    int vi = vIndex[i] - 1;
                    int ti = tIndex[i] - 1;
                    int ni = nIndex[i] - 1;

                    v[i].position[0] = x[vi];
                    v[i].position[1] = y[vi];
                    v[i].position[2] = z[vi];
                    v[i].texCoords[0] = vtx[ti];
                    v[i].texCoords[1] = vty[ti];
                    v[i].normals[0] = nx[ni];
                    v[i].normals[1] = ny[ni];
                    v[i].normals[2] = nz[ni];
                    v[i].colors[0] = 1.0f;
                    v[i].colors[1] = 1.0f;
                    v[i].colors[2] = 1.0f;
                    v[i].texIndex = 0;
                    v[i].is3d = 1;
                    //CPE1704TKS
                    if (vIndex[i] <= 0 || vIndex[i] > vertex ||
                        tIndex[i] <= 0 || tIndex[i] > texCoord ||
                        nIndex[i] <= 0 || nIndex[i] > normal) {
                        fprintf(stderr, "[GOOFYLIB3] Error: Face index out of bounds on line: %s", line);
                        continue;
                    }
                    /*
                    printf("    Vertex %d: Pos(%f, %f, %f) Tex(%f, %f) Norm(%f, %f, %f)\n",
                        i, v[i].position[0], v[i].position[1], v[i].position[2],
                        v[i].texCoords[0], v[i].texCoords[1],
                        v[i].normals[0], v[i].normals[1], v[i].normals[2]);
                    */
                }
                
                for (int i = 0; i < 3; ++i)
                    result.vertices[vertexCount++] = v[i];

                result.indices[indexCount++] = vertexCount - 3;
                result.indices[indexCount++] = vertexCount - 2;
                result.indices[indexCount++] = vertexCount - 1;
            } else {
                fprintf(stderr, "[GOOFYLIB3] Warning: Malformed face line: %s", line);
            }
        }
    }

    printf("[GOOFYLIB3] Final mesh: %d vertices, %d indices\n", vertexCount, indexCount);

    free(x); free(y); free(z);
    free(vtx); free(vty);
    free(nx); free(ny); free(nz);
    fclose(file);
    return result;
}

// TEXTURE-RELATED functions
void goofy_initTextures(int textureWidth, int textureHeight, int numLayers) {
    glGenTextures(1, &textureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    // default
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Allocate the texture array with the same size for all textures (im too lazy to add dynamic layers DONT BE LAZY!)
    // actually nvm i may add reallocating it
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, textureWidth, textureHeight, numLayers);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    printf("[GOOFYLIB3] Successfully initialized texture array!\n");
}
int goofy_loadTexture(const char* path, int layerIndex) { // taken from goofylib 1
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(1);  
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (!data) {
        printf("[GOOFYLIB3] Failed to load texture: %s\n", path);
        return -1; 
    }
    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layerIndex, width, height, 1, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY); 
    stbi_image_free(data);

    printf("[GOOFYLIB3] Loaded texture: %s into layer %d\n", path, layerIndex);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    return layerIndex; 
}

// MESH-MODIFICATION functions
void goofy_transformMesh(GOOFY_MESH* mesh, float x, float y, float z) {
    for (int i = 0; i < mesh->vertexCount; i++) {
        mesh->vertices[i].position[0] += x;
        mesh->vertices[i].position[1] += y;
        mesh->vertices[i].position[2] += z;
    }
}

// MEMORY-RELATED functions
void goofy_reallocateMesh(GOOFY_MESH* mesh, size_t newVertexCapacity, size_t newIndexCapacity) {
    mesh->vertices = realloc(mesh->vertices, sizeof(GOOFY_VERTICE) * newVertexCapacity);
    mesh->indices = realloc(mesh->indices, sizeof(unsigned int) * newIndexCapacity);
    mesh->vertexCount += newVertexCapacity;
    mesh->indexCount += newIndexCapacity;
    if (!mesh->vertices || !mesh->indices) {
        fprintf(stderr, "[GOOFYLIB3] Failed to reallocate mesh memory\n");
        exit(1);
    }
}
GOOFY_MESH goofy_appendMesh(const GOOFY_MESH* a,const GOOFY_MESH* b) {
    GOOFY_MESH result;
    result.vertexCount = a->vertexCount + b->vertexCount;
    result.indexCount = a->indexCount + b->indexCount;
    result.vertices = malloc(sizeof(GOOFY_VERTICE) * result.vertexCount);
    result.indices = malloc(sizeof(unsigned int) * result.indexCount);
    if (!result.vertices || !result.indices) {
        fprintf(stderr, "[GOOFYLIB3] Failed to allocate memory for mesh append.\n");
        exit(1);
    }
    memcpy(result.vertices, a->vertices, sizeof(GOOFY_VERTICE) * a->vertexCount);
    memcpy(result.vertices + a->vertexCount, b->vertices, sizeof(GOOFY_VERTICE) * b->vertexCount);
    memcpy(result.indices, a->indices, sizeof(unsigned int) * a->indexCount);
    for (size_t i = 0; i < b->indexCount; ++i) {
        result.indices[a->indexCount + i] = b->indices[i] + a->vertexCount;
    }
    return result;
}
void goofy_freeBuffer(GOOFY_BUFFER* buffer) {
    if (!buffer) return;

    if (buffer->vertexOffsets) free(buffer->vertexOffsets);
    if (buffer->indexOffsets) free(buffer->indexOffsets);
    if (buffer->vertexCounts) free(buffer->vertexCounts);
    if (buffer->indexCounts) free(buffer->indexCounts);

    glDeleteBuffers(1, &buffer->VBO);
    glDeleteBuffers(1, &buffer->EBO);
    glDeleteVertexArrays(1, &buffer->VAO);

    buffer->vertexOffsets = NULL;
    buffer->indexOffsets = NULL;
    buffer->vertexCounts = NULL;
    buffer->indexCounts = NULL;
}
void goofy_freeMesh(GOOFY_MESH* mesh) {
    if (!mesh) return;
    if (mesh->vertices) {
        free(mesh->vertices);
        mesh->vertices = NULL;
    }
    if (mesh->indices) {
        free(mesh->indices);
        mesh->indices = NULL;
    }
    mesh->vertexCount = 0;
    mesh->indexCount = 0;
}
void goofy_initTrashBatch(GOOFY_TRASH_BATCH* batch,int maxSize,char autoClean) {
    batch->maxSize = maxSize;
    batch->trashCount = 0;
    batch->items = malloc(sizeof(GOOFY_TRASH_ITEM) * batch->maxSize);
    if (autoClean == 1) {
        if (MAX_TRASH>goofy_trashRegistryCount) {
            goofy_trashRegistry[goofy_trashRegistryCount++] = batch;
        } else {
            printf("[GOOFYLIB3] Max trash batches reached (%d). Cannot track more batches.\n", MAX_TRASH);
            return;
        }
    }
}
void goofy_addToTrash(GOOFY_TRASH_BATCH* batch, GOOFY_TRASH type, void* item) {
    if (batch->trashCount >= batch->maxSize) {
        printf("[GOOFYLIB3] Tried to add trash item to full trash batch with capacity %d. Increasing to %d items \n",batch->maxSize,batch->maxSize+256); 
        batch->maxSize += 256;
        batch->items = realloc(batch->items, sizeof(GOOFY_TRASH_ITEM) * batch->maxSize);
    }

    batch->items[batch->trashCount++] = (GOOFY_TRASH_ITEM){type, item};
}
void goofy_clearTrash(GOOFY_TRASH_BATCH* batch) {
     for (int i = 0; i < batch->trashCount; i++) {
        GOOFY_TRASH_ITEM* item = &batch->items[i];
        switch (item->type) {
            case GOOFY_TRASH_MESH:
                goofy_freeMesh((GOOFY_MESH*)item->item);
                break;
            case GOOFY_TRASH_BUFFER:
                goofy_freeBuffer((GOOFY_BUFFER*)item->item);
                break;
            default:
                printf("[GOOFYLIB3] Unknown trash item being cleared in trash batch. Type : %d.\n",item->type); 
                break;
        }
    }
    batch->trashCount = 0;
}
void goofy_freeTrashBatch(GOOFY_TRASH_BATCH* batch) {
    goofy_clearTrash(batch);
    free(batch->items);
    batch->items = NULL;
    batch->maxSize = 0;
}

// PRIMITIVE-CREATION functions
GOOFY_MESH goofy_cubeMesh(float x,float y,float z,unsigned int texture,float width,float height,float length,float r,float g,float b,float tX,float tY)  {
    GOOFY_MESH mesh;
    GOOFY_VERTICE cubeVertices[] = {
        // Front face (Z = -size)
        {{x+width, y+height, z+length}, {r,g,b}, {0,0,1}, {0.0f, tY}, texture,1},
        {{x-width, y+height, z+length}, {r,g,b}, {0,0,1}, {tX, tY}, texture,1},
        {{x-width, y-height, z+length}, {r,g,b}, {0,0,1}, {tX, 0.0f}, texture,1},
        {{x+width, y-height, z+length}, {r,g,b}, {0,0,1}, {0.0f, 0.0f}, texture,1},
    
        // Back face (Z = +size)
        {{x-width, y+height, z-length}, {r,g,b}, {0,0,-1},  {0.0f, tY}, texture,1},
        {{x+width, y+height, z-length}, {r,g,b}, {0,0,-1},  {tX, tY}, texture,1},
        {{x+width, y-height, z-length}, {r,g,b}, {0,0,-1},  {tX, 0.0f}, texture,1},
        {{x-width, y-height, z-length}, {r,g,b}, {0,0,-1},  {0.0f, 0.0f}, texture,1},
    
        // Left face (X = -size)
        {{x+width, y+height, z-length}, {r,g,b}, {1,0,0}, {0.0f, tY}, texture,1},
        {{x+width, y+height, z+length}, {r,g,b}, {1,0,0}, {tX, tY}, texture,1},
        {{x+width, y-height, z+length}, {r,g,b}, {1,0,0}, {tX, 0.0f}, texture,1},
        {{x+width, y-height, z-length}, {r,g,b}, {1,0,0}, {0.0f, 0.0f}, texture,1},
    
        // Right face (X = +size)
        {{x-width, y+height, z+length}, {r,g,b}, {-1,0,0},  {0.0f, tY}, texture,1},
        {{x-width, y+height, z-length}, {r,g,b}, {-1,0,0},  {tX, tY}, texture,1},
        {{x-width, y-height, z-length}, {r,g,b}, {-1,0,0},  {tX, 0.0f}, texture,1},
        {{x-width, y-height, z+length}, {r,g,b}, {-1,0,0},  {0.0f, 0.0f}, texture,1},
    
        // Top face (Y = +size)
        {{x+width, y-height, z+length}, {r,g,b}, {0,-1,0},  {0.0f, tY}, texture,1},
        {{x-width, y-height, z+length}, {r,g,b}, {0,-1,0},  {tX, tY}, texture,1},
        {{x-width, y-height, z-length}, {r,g,b}, {0,-1,0},  {tX, 0.0f}, texture,1},
        {{x+width, y-height, z-length}, {r,g,b}, {0,-1,0},  {0.0f, 0.0f}, texture,1},
    
        // Bottom face (Y = -size)
        {{x+width, y+height, z-length}, {r,g,b}, {0,1,0}, {0.0f, tY}, texture,1},
        {{x-width, y+height, z-length}, {r,g,b}, {0,1,0}, {tX, tY}, texture,1},
        {{x-width, y+height, z+length}, {r,g,b}, {0,1,0}, {tX, 0.0f}, texture,1},
        {{x+width, y+height, z+length}, {r,g,b}, {0,1,0}, {0.0f, 0.0f}, texture,1},
    };
    
    unsigned int cubeIndices[] = {
        // Front
        0, 1, 2, 2, 3, 0,
        // Back
        4, 5, 6, 6, 7, 4,
        // Left
        8, 9,10,10,11, 8,
        // Right
       12,13,14,14,15,12,
        // Top
       16,17,18,18,19,16,
        // Bottom
       20,21,22,22,23,20
    };
    mesh.indexCount = 36;
    mesh.vertexCount = 24;
    mesh.vertices = (GOOFY_VERTICE*)malloc(sizeof(GOOFY_VERTICE) * mesh.vertexCount);
    mesh.indices = (unsigned int*)malloc(sizeof(unsigned int) * mesh.indexCount);
    if (!mesh.vertices || !mesh.indices) {
        fprintf(stderr, "Failed to allocate mesh data\n");
        exit(1);
    }
    memcpy(mesh.vertices, cubeVertices, sizeof(cubeVertices));
    memcpy(mesh.indices, cubeIndices, sizeof(cubeIndices));
    return mesh;
}

// MISCELLANEOUS functions
void goofy_printMesh(GOOFY_MESH* mesh) {
    if (!mesh) {
        printf("[GOOFY_PRINT] Null mesh pointer.\n");
        return;
    }

    printf("[GOOFY_PRINT] Mesh has %zu vertices and %zu indices\n", mesh->vertexCount, mesh->indexCount);

    for (size_t i = 0; i < mesh->vertexCount; ++i) {
        GOOFY_VERTICE v = mesh->vertices[i];
        printf("  Vertex %zu:\n", i);
        printf("    Position: (%.6f, %.6f, %.6f)\n", v.position[0], v.position[1], v.position[2]);
        printf("    Normal:   (%.6f, %.6f, %.6f)\n", v.normals[0], v.normals[1], v.normals[2]);
        printf("    Texcoord: (%.6f, %.6f)\n", v.texCoords[0], v.texCoords[1]);
        printf("    TexIndex: (%d)\n", v.texIndex);
        printf("    is3d: %u\n", v.is3d);
    }

    printf("  Indices:\n    ");
    for (size_t i = 0; i < mesh->indexCount; ++i) {
        printf("%u ", mesh->indices[i]);
        if ((i + 1) % 12 == 0) printf("\n    ");
    }
    printf("\n");
}