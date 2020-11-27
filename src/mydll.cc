#include "mydll.h"
#include <codecvt>
#include <iostream>

#ifdef _WIN32
#include "win32-dlfcn.h"
#else
#include <dlfcn.h>
#endif

typedef int (*CAC_FUNC)(int, int);

Napi::Object MyDll::Init(Napi::Env env, Napi::Object exports)
{

    Napi::Function func = DefineClass(
        env, "MyDll", {InstanceMethod("add", &MyDll::add)});

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("MyDll", func);
    return exports;
}

MyDll::MyDll(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<MyDll>(info)
{
    this->module_ = dlopen(info[0].ToString().Utf8Value().c_str(), RTLD_LAZY);
    if (this->module_ == NULL)
    {
        printf("Failed to load dylib \n");
        exit(EXIT_FAILURE);
    }
};

MyDll::~MyDll()
{
    dlclose(this->module_);
}

Napi::Object MyDll::NewInstance(Napi::Env env, Napi::Value arg)
{
    Napi::EscapableHandleScope scope(env);
    Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New({arg});
    return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Value MyDll::add(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    CAC_FUNC cac_func = NULL;
    char *error;

    if (info.Length() < 2)
    {
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber() || !info[1].IsNumber())
    {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    int arg0 = info[0].As<Napi::Number>().Int32Value();
    int arg1 = info[1].As<Napi::Number>().Int32Value();

    *(void **)(&cac_func) = dlsym(this->module_, "add");

    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    // typedef int (*pAdd)(int, int);
    // pAdd box = (pAdd)GetProcAddress(this->module_, "add");

    int result = (*cac_func)(arg0, arg1);

    return Napi::Number::New(env, result);
}
