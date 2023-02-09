const { suite } = require("uvu");
const assert = require("uvu/assert");
const xpathed = require("..");
const { schema } = require("./fixtures");

const test = suite("compile");

test("import", () => {
	assert.type(xpathed.compile, "function");
});

[
	{
		expect: "$ should be object",
		schema: "",
	},
	{
		expect: "$.type should be string",
		schema: { type: 1 },
	},
	{
		expect: "$.type has unknown value",
		schema: { type: "invalid" },
	},
	{
		expect: "$.data should be object",
		schema: { type: "object", data: "" },
	},
	{
		expect: "$.xpath should be string",
		schema: { type: "string", xpath: 1 },
	},
	{
		expect: "$.xpath is invalid",
		schema: { type: "string", xpath: "%" },
	},
	{
		expect: "$.xpath should return node set",
		schema: {
			type: "nodes",
			xpath: "count(*)",
			data: { type: "string", xpath: "*" },
		},
	},
].forEach(({ expect, schema }) => {
	test(expect, () => {
		assert.throws(
			() => xpathed.compile(schema),
			(err) => err.toString().includes(expect)
		);
	});
});

test("compile", () => {
	assert.type(xpathed.compile(schema), "function");
});

test.run();
