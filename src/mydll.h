#ifndef MYDLL_H
#define MYDLL_H

#include <napi.h>

typedef int (*FnAdd)(int, int);

class MyDll : public Napi::ObjectWrap<MyDll>
{
private:
    Napi::Value add(const Napi::CallbackInfo &info);
    void *module_;
    FnAdd s_add;

public:
    MyDll(const Napi::CallbackInfo &info);
    ~MyDll();
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);
};

#endif