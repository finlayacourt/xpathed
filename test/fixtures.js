exports.schema = {
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

exports.xml = `<doc>
	<string>test1</string>
	<item><number value="1" /></item>
	<item><number value="2" /></item>
	<item><number value="3" /></item>
	<one>test2</one>
</doc>`;
