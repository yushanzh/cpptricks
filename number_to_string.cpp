#include <sstream>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <fmt/format.h>
#include <boost/lexical_cast.hpp>

#define THREAD_COUNT 4
#define REPEAT_TIMES 100000

typedef void* (*func_t)(void*);

void* test_lexical_cast(void* args){
    for(int i = 0; i < REPEAT_TIMES; i ++) {
        std::string s = boost::lexical_cast<::std::string>(12345);
    }
}

void* test_snprintf(void* args){
    for(int i = 0; i < REPEAT_TIMES; i ++){
    	std::string s;
    	char buffer[50];
    	snprintf(buffer, sizeof(buffer), "%d",12345);
    	s = buffer;
        //std::cout << "s="<<s<<std::endl;
    }
}

void* test_to_string(void* args){
    for (int i =0; i < REPEAT_TIMES; i ++) {
        std::string s = std::to_string(12345);
    }
}

void* test_fmt(void* args) {
    for(int i = 0; i < REPEAT_TIMES; i ++) {
       std::string s;
       s=fmt::format("{}",12345);
    }
}


void multi_thread_test_number_to_string(func_t func){
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
    if (func == test_lexical_cast) {
        std::cout << "lexical_cast cost "<< ((t2.tv_sec - t1.tv_sec) * 10^9 + t2.tv_nsec - t1.tv_nsec) << std::endl;
    }else if(func == test_snprintf) {
        std::cout << "snprintf     cost "<< ((t2.tv_sec - t1.tv_sec) * 10^9 + t2.tv_nsec - t1.tv_nsec) << std::endl;
    }else if(func == test_to_string) {
        std::cout << "to_string    cost "<< ((t2.tv_sec - t1.tv_sec) * 10^9 + t2.tv_nsec - t1.tv_nsec) << std::endl;
    }else if(func == test_fmt) {
        std::cout << "fmt          cost " << ((t2.tv_sec - t1.tv_sec) * 10^9 + t2.tv_nsec - t1.tv_nsec) << std::endl;
    }
}

int main() {
    multi_thread_test_number_to_string(test_lexical_cast);
    multi_thread_test_number_to_string(test_to_string);
    multi_thread_test_number_to_string(test_fmt);
    multi_thread_test_number_to_string(test_snprintf);
}
