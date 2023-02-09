const { suite } = require("uvu");
const assert = require("uvu/assert");
const xpathed = require("..");
const { xml } = require("./fixtures");

const test = suite("parse");

test("import", () => {
	assert.type(xpathed.parse, "function");
});

test("parse", () => {
	assert.type(xpathed.parse(xml), "object");
});

test.run();
