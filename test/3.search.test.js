const { suite } = require("uvu");
const assert = require("uvu/assert");
const xpathed = require("..");
const { schema, xml } = require("./fixtures");

const test = suite("search");

const doc = xpathed.parse(xml);
const search = xpathed.compile(schema);

test("search", () => {
	assert.equal(search(doc), {
		string: "test1",
		nodes: [1, 2, 3],
		node: true,
	});
});

test.run();
