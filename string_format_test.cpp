#include <sstream>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <fmt/format.h>
#define THREAD_COUNT 10
#define REPEAT_TIMES 1000

typedef void* (*func_t)(void*);

void* test_stringstream(void* args){
    for(int i = 0; i < REPEAT_TIMES; i ++) {
        std::string s;
        std::stringstream ss;
        ss<< 1.234 << 42 << 3.13 << "str" << (void*) 1000 << (int)'X';
        ss.str(s);
    }
}

void* test_snprintf(void* args){
    for(int i = 0; i < REPEAT_TIMES; i ++){
    	std::string s;
    	char buffer[50];
    	snprintf(buffer, sizeof(buffer), "%0.10f:%04d:%+g:%s:%p:%c:%%\n",1.234, 42, 3.13, "str", (void*)1000, (int)'X');
    	s = buffer;
        //std::cout << "s="<<s<<std::endl;
    }
}

void* test_append(void* args){
    for (int i =0; i < REPEAT_TIMES; i ++) {
        std::string s;
        s.append("1.234");  
        s.append("42");
        s.append("3.13");
        s.append("str");
        s.append("0x3e8");
        s.append(std::to_string((int)'X'));
    }
}

void* test_fmt(void* args) {
    for(int i = 0; i < REPEAT_TIMES; i ++) {
       std::string s;
       s=fmt::format("{:.10f}:{:04}:{:+}:{}:{}:{}:%\n",1.234, 42, 3.13, "str", (void*)1000, 'X');
    }
}

void* test_add(void* agrs) {
    for(int i = 0;i < REPEAT_TIMES; i ++) {
       std::string s;
       s +="1.234";
       s += "42";
       s += "3.13";
       s += "str";
       s += "0x3e8";
       s += "X";
    }
}

void multi_thread_test_format_string(func_t func){
    timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    pthread_t t[THREAD_COUNT];
    for(int i = 0; i < THREAD_COUNT; i++) {
        int ret = pthread_create(&t[i], NULL, func, NULL);
        if (ret != 0)
        {
        	std::cout << "pthread_create error: error_code=" << ret << std::endl;
        } 
    }
    for(int i=0; i < THREAD_COUNT; i ++) {
        pthread_join(t[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &t2); 
    if (func == test_stringstream) {
        std::cout << "stringstream cost "<< ((t2.tv_sec - t1.tv_sec) * 10^9 + t2.tv_nsec - t1.tv_nsec) << std::endl;
    }else if(func == test_snprintf) {
        std::cout << "snprintf     cost "<< ((t2.tv_sec - t1.tv_sec) * 10^9 + t2.tv_nsec - t1.tv_nsec) << std::endl;
    }else if(func == test_append) {
        std::cout << "append       cost "<< ((t2.tv_sec - t1.tv_sec) * 10^9 + t2.tv_nsec - t1.tv_nsec) << std::endl;
    }else if(func == test_fmt) {
        std::cout << "fmt          cost " << ((t2.tv_sec - t1.tv_sec) * 10^9 + t2.tv_nsec - t1.tv_nsec) << std::endl;
    }else if (func == test_add) {
        std::cout << "add          cost " << ((t2.tv_sec - t1.tv_sec) * 10^9 + t2.tv_nsec - t1.tv_nsec) << std::endl;
    }
}

int main() {
    multi_thread_test_format_string(test_stringstream);
    multi_thread_test_format_string(test_snprintf);
    multi_thread_test_format_string(test_append);
    multi_thread_test_format_string(test_fmt);
    multi_thread_test_format_string(test_add);
}
