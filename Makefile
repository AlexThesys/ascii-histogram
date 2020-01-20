LDFLAGS = -lGLEW -lGL -lGLU -lglfw 
histogramm: histogramm.o 
	g++ -o build/$@ src/Shader.cpp src/Window.cpp src/renderer.cpp \
		obj/$^ \
	-L/usr/local/cuda-10.1/lib64 -lcuda -lcudart \
	-I/usr/local/cuda-10.1/include \
	-I/usr/include \
	-Iinclude \
	${LDFLAGS} \
	-O3 -Wall -Wextra -std=c++14

histogramm.o:
	nvcc -o obj/$@ -std=c++14 -c -arch=sm_30 src/histogramm.cu

.PHONY: clean
