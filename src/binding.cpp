#include <napi.h>

auto compile(const Napi::CallbackInfo &info)
{
	auto env = info.Env();
	return env.Undefined();
}

auto parse(const Napi::CallbackInfo &info)
{
	auto env = info.Env();
	return env.Undefined();
}

auto Init(Napi::Env env, Napi::Object exports)
{
	exports.Set(
		Napi::String::New(env, "compile"),
		Napi::Function::New(env, compile, "compile"));
	exports.Set(
		Napi::String::New(env, "parse"),
		Napi::Function::New(env, parse, "parse"));
	return exports;
}

NODE_API_MODULE(xpathed, Init);