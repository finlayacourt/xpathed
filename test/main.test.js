const { suite } = require("uvu");
const assert = require("uvu/assert");
const xpathed = require("..");

const test = suite("main");

const schema = {
	type: "object",
	data: {
		string: {
			type: "string",
			xpath: "//string/text()",
		},
		nodes: {
			type: "nodes",
			xpath: "//item",
			data: {
				type: "number",
				xpath: "number/@value",
			},
		},
		node: {
			type: "node",
			xpath: "//one",
			data: {
				type: "boolean",
				xpath: "text() = 'test2'",
			},
		},
	},
};

test("compile", () => {
	assert.type(xpathed.compile, "function");
	assert.type(xpathed.compile(schema), "function");
});

test("parse", () => {
	assert.type(xpathed.parse, "function");
	assert.type(
		xpathed.parse(
			`<doc>
				<string>test1</string>
				<item><number value="1" /></item>
				<item><number value="2" /></item>
				<item><number value="3" /></item>
				<one>test2</one>
			</doc>`
		),
		"object"
	);
});

test("search", () => {
	const doc = xpathed.parse(
		`<doc>
		<string>test1</string>
		<item><number value="1" /></item>
		<item><number value="2" /></item>
		<item><number value="3" /></item>
		<one>test2</one>
	</doc>`
	);
	const search = xpathed.compile(schema);
	assert.equal(search(doc), {
		string: "test1",
		nodes: [1, 2, 3],
		node: true,
	});
});

test.run();
