# xpathed

> High performance Node.js library to extract and parse XML into JS values.

- 🧭 Only extract the data you need, defined by a schema.
- 🧪 Automatic Typescript type inference.
- 🔧 Written in C++, based on [pugixml](http://pugixml.org/).

## Contents

- [Installation](#installation)
- [Usage](#usage)
- [Reference](#reference)
  - [Primatives](#primatives)
  - [Objects](#objects)
  - [Nodes](#nodes)

## Installation

```bash
npm install xpathed
```

## Usage

```js
import { compile, parse } from "xpathed";

// compile schema to a function
const search = compile({
  type: "nodes",
  xpath: "//customers/customer",
  data: {
    type: "object",
    data: {
      id: { type: "number", xpath: "@id" },
      name: { type: "string", xpath: "name/text()" },
    },
  },
});

// parse xml string
const doc = parse(string);

// search document with schema
const data = search(doc);
console.log(data[0].name);
```

## Reference

### Primatives

To evaluate an XPath expression and cast it to a primative type.

```js
const doc = parse(`<node id="1" name="first" />`);

compile({ type: "number", xpath: "@id" })(doc); // => 1
compile({ type: "string", xpath: "@name" })(doc); // => "first"
compile({ type: "boolean", xpath: "@name = 'first'" })(doc); // => true
```

### Objects

To apply multiple schemas and structure the results as an object.

```js
const doc = parse(`<node id="1" name="first" />`);

compile({
  type: "object",
  data: {
    id: { type: "number", xpath: "@id" },
    name: { type: "string", xpath: "@first" },
  },
})(doc); // => { id: 1, name: "first" }
```

### Nodes

To evaluate an XPath expression and apply a schema to _each node_ or the _first node_.

```js
const doc = parse(`<nodes>
        <node id="1" name="first" />
        <node id="2" name="second" />
    </nodes>`);

// each node
compile({
  type: "nodes",
  xpath: "//node",
  data: { type: "number", xpath: "@id" },
})(doc); // => [ 1, 2 ]

// first node
compile({
  type: "node",
  xpath: "//node",
  data: { type: "number", xpath: "@id" },
})(doc); // => 1
```
