#include <napi.h>
#include "pugixml.hpp"

enum Type
{
	string_schema,
	number_schema,
	boolean_schema,
	node_schema,
	nodes_schema,
	object_schema,
};

struct Schema
{
	Type type;
	pugi::xpath_query query;
	std::string key;
	Schema *sibling = nullptr;
	Schema *child = nullptr;

	Schema(const Napi::Env &env, const Napi::Value &input)
	{
		if (!input.IsObject())
		{
			throw Napi::TypeError::New(env, "should be object");
		}
		auto input_object = input.As<Napi::Object>();
		auto type_value = input_object.Get("type");
		if (!type_value.IsString())
		{
			throw Napi::TypeError::New(env, "type should be string");
		}

		auto type_string = type_value.As<Napi::String>().Utf8Value();

		if (type_string == "string")
			type = string_schema;
		else if (type_string == "number")
			type = number_schema;
		else if (type_string == "boolean")
			type = boolean_schema;
		else if (type_string == "object")
			type = object_schema;
		else if (type_string == "node")
			type = node_schema;
		else if (type_string == "nodes")
			type = nodes_schema;
		else
			throw Napi::TypeError::New(env, "type has unknown value");

		if (type == object_schema)
		{
			auto data_value = input_object.Get("data");
			if (!data_value.IsObject())
			{
				throw Napi::TypeError::New(env, "data should be object");
			}
			auto data_object = data_value.As<Napi::Object>();

			auto first = true;
			auto current = this;
			for (const auto &e : data_object)
			{
				auto next_key = e.first.ToString().Utf8Value();
				auto next_schema = new Schema(env, e.second);
				next_schema->key = next_key;
				if (first)
				{
					current->child = next_schema;
					first = false;
				}
				else
				{
					current->sibling = next_schema;
				}
				current = next_schema;
			}
		}
		else
		{
			auto path_value = input_object.Get("xpath");
			if (!path_value.IsString())
			{
				throw Napi::TypeError::New(env, "xpath should be string");
			}
			auto path_string = path_value.As<Napi::String>().Utf8Value();

			try
			{
				query = pugi::xpath_query(path_string.c_str());
				if (
					type == node_schema ||
					type == nodes_schema)
				{
					if (query.return_type() != pugi::xpath_type_node_set)
					{
						throw Napi::TypeError::New(env, "xpath should return node set");
					}

					auto data_value = input_object.Get("data");
					child = new Schema(env, data_value);
				}
			}
			catch (pugi::xpath_exception error)
			{
				throw Napi::Error::New(env, std::string("xpath is invalid: ") + error.what());
			}
		}
	}

	~Schema()
	{
		delete sibling;
		delete child;
	}

	Napi::Value parse(const Napi::Env &env, const pugi::xml_node &node)
	{
		switch (type)
		{
		case string_schema:
			return Napi::String::New(env, query.evaluate_string(node));
		case number_schema:
			return Napi::Number::New(env, query.evaluate_number(node));
		case boolean_schema:
			return Napi::Boolean::New(env, query.evaluate_boolean(node));
		case object_schema:
		{
			auto object = Napi::Object::New(env);
			for (auto current = child; current; current = current->sibling)
			{
				auto value = current->parse(env, node);
				object.Set(current->key, value);
			}
			return object;
		}
		case node_schema:
		{
			auto next = query.evaluate_node(node).node();
			return next ? child->parse(env, next) : env.Null();
		}
		case nodes_schema:
		{
			auto array = Napi::Array::New(env);
			auto results = query.evaluate_node_set(node);
			for (size_t i = 0; i < results.size(); i++)
			{
				auto next = results[i].node();
				auto value = child->parse(env, next);
				array.Set(i, value);
			}
			return array;
		}
		}
	}
};

template <typename T>
void finalize(Napi::Env env, T *data)
{
	delete data;
}

auto search(const Napi::CallbackInfo &info)
{
	auto env = info.Env();
	auto schema = static_cast<Schema *>(info.Data());

	auto input_value = info[0];
	if (!input_value.IsExternal())
	{
		throw Napi::TypeError::New(env, "First argument should be compiled xml");
	}
	auto input_external = input_value.As<Napi::External<pugi::xml_document>>();

	auto document = input_external.Data();
	return schema->parse(env, *document);
};

auto compile(const Napi::CallbackInfo &info)
{
	auto env = info.Env();
	auto input_value = info[0];
	auto schema = new Schema(env, input_value);
	auto searcher = Napi::Function::New(env, search, "search", schema);
	searcher.AddFinalizer(finalize<Schema>, schema);
	return searcher;
}

auto parse(const Napi::CallbackInfo &info)
{
	auto env = info.Env();
	auto input_value = info[0];
	if (!input_value.IsString())
	{
		throw Napi::TypeError::New(env, "First argument should be string");
	}
	auto input_string = input_value.As<Napi::String>().Utf8Value();

	auto document = new pugi::xml_document;
	auto result = document->load_string(input_string.c_str());
	if (!result)
	{
		delete document;
		throw Napi::Error::New(env, std::string("Parse error: ") + result.description());
	}

	return Napi::External<pugi::xml_document>::New(env, document, finalize<pugi::xml_document>);
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