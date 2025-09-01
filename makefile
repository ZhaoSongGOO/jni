build/libnative.dylib: cpp/Platform.cc cpp/Platform.h
	clang++ -fpic -I"${JAVA_HOME}/include" -I"${JAVA_HOME}/include/darwin" -shared -o build/libnative.dylib cpp/Platform.cc

platform/Platform.class:platform/Platform.java
	javac platform/Platform.java

run: build/libnative.dylib platform/Platform.class
	java -Djava.library.path=build platform.Platform 
