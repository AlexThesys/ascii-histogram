#define GL_GLEXT_PROTOTYPES

#include <cstdio>
#include <cstring>
#include <memory>
#include "../include/renderer.h"
#include "cuda.h"
#include "cuda_gl_interop.h"

static constexpr int num_threads = 256;

__global__ void histo_kernel(unsigned char* buffer,
                     const size_t size, unsigned int* histo)
{
    __shared__ unsigned int cache[num_threads];
    cache[threadIdx.x] = 0u;
    __syncthreads();

    int i = threadIdx.x + blockIdx.x * blockDim.x;
    const int offset = blockDim.x * gridDim.x;
  
    while (i < size) {
        atomicAdd(&(cache[buffer[i]]), 1u);
        i += offset;
    }
    __syncthreads();
    atomicAdd(&(histo[threadIdx.x]), cache[threadIdx.x]);
}

size_t readFromFile(unsigned char**, const char*);

template <typename T>
static void getBufferData(const GLuint Buf, T* buffer, const size_t size);

int main(void)
{
    unsigned char* buffer = nullptr;
    const size_t size = readFromFile(&buffer, "../resource/example.xml");
    
    cudaDeviceProp prop;
    int dev;
    std::memset(&prop, 0, sizeof(cudaDeviceProp));
    prop.major = 1;
    prop.minor = 0;
    cudaChooseDevice(&dev, &prop);
    cudaGLSetGLDevice(dev);
    //cudaSetDevice(0);
    cudaGetDeviceProperties(&prop, dev);
    const int blocks = prop.multiProcessorCount;
    
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    unsigned char* dev_buffer;
    cudaMalloc((void**)&dev_buffer, size);
    cudaMemcpy(dev_buffer, buffer, size, cudaMemcpyHostToDevice);
    cudaFreeHost(buffer);

    std::unique_ptr<GraphicsData> gdata = std::make_unique<GraphicsData>();
    initGL(*gdata, size);
    unsigned int* dev_histo = nullptr;
    cudaGraphicsResource *resource;
    cudaGraphicsGLRegisterBuffer(&resource, gdata->VBO, cudaGraphicsMapFlagsNone);
    cudaGraphicsMapResources(1, &resource, NULL);
    size_t b_size;
    cudaGraphicsResourceGetMappedPointer((void**)&dev_histo, &b_size, resource);

    histo_kernel<<<blocks*2, num_threads>>>(dev_buffer, size, dev_histo);

    cudaFree(dev_buffer);
    cudaGraphicsUnmapResources(1, &resource, NULL);

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);
    printf("Time to calculate: %3.1f ms\n", elapsedTime);

    unsigned int* histo;
    histo  = (unsigned int*)malloc(sizeof(unsigned int)* buf_size);
    //cudaMemcpy(histo, dev_histo, sizeof(unsigned int)*buf_size, 
   //                                 cudaMemcpyDeviceToHost);
    getBufferData(gdata->VBO, histo, buf_size);
    puts("Histogramm data:");
    for (auto i = 0u; i < buf_size; i++)
        printf("%d\t", histo[i]);
    puts("--------------");
    free(histo);

    renderGL(*gdata);

    cudaGraphicsUnregisterResource(resource);
    cleanupGL(*gdata);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return 0;
}

size_t readFromFile(unsigned char** buff, const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == nullptr) {
        puts("Error opening file");
    }
    size_t sz = 0u;
    fseek(file, 0l, SEEK_END);
    sz = ftell(file);
    rewind(file);

    cudaHostAlloc((void**)buff, sz, cudaHostAllocDefault);
    //cudaDeviceSynchronize();
   
    constexpr auto stride = 16u;  
    auto sz1 = sz & (stride - 1u);
    auto sz0 = sz - sz1;

    alignas(stride) char symbol[stride];
    for (auto i = 0lu; i < sz0; i += stride) {
        fread(symbol, 1, stride, file);
        std::memcpy(((*buff)+i), symbol,  stride);
    }
    fread(symbol, 1, sz1, file);
    std::memcpy(((*buff)+sz0), symbol,  sz1);

    fclose(file);

    return sz;
}

template <typename T>
static void getBufferData(const GLuint buf, T* buffer, const size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    T* device_data = (T*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
    memcpy(buffer, device_data, size * sizeof (T));
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
