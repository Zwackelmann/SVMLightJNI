.PHONY=all
cc=gcc
cflagslib=-c -Wno-write-strings -fPIC -o
jniflags=-c -fPIC -Wno-write-strings -I/usr/lib/jvm/java-6-openjdk-amd64/include/ -o
jnilibflags=-Wno-write-strings  -fPIC -I/usr/lib/jvm/java-6-openjdk-amd64/include/ -shared -o

svm_jni: svm_classify svm_learn_main svm_hideo 
	${cc} ${jniflags} svm_jni.o svm_jni.cpp
	${cc} ${jnilibflags} libsvm_jni.so *.o
svm_classify: svm_common
	${cc} ${cflagslib} svm_classify.o svm_classify.cpp
svm_common:
	${cc} ${cflagslib} svm_common.o svm_common.cpp
svm_hideo: svm_common
	${cc} ${cflagslib} svm_hideo.o svm_hideo.cpp
svm_learn: svm_common
	${cc} ${cflagslib} svm_learn.o svm_learn.cpp
svm_learn_main: svm_common svm_learn
	${cc} ${cflagslib} svm_learn_main.o svm_learn_main.cpp
svm_loqo: svm_common
	${cc} ${cflagslib} svm_loqo.o svm_loqo.cpp