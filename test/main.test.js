const { suite } = require("uvu");
const assert = require("uvu/assert");
const xpathed = require("..");

const test = suite("main");

test("compile", () => {
	assert.type(xpathed.compile, "function");
	assert.equal(xpathed.compile(), undefined);
});

test("parse", () => {
	assert.type(xpathed.parse, "function");
	assert.equal(xpathed.parse(), undefined);
});

test.run();
