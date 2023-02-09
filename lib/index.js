const addon = require("node-gyp-build")(require("path").dirname(__dirname));
exports.compile = addon.compile;
exports.parse = addon.parse;
