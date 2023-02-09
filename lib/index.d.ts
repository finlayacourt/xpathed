export type SchemaString = {
	type: "string";
	xpath: string;
};

export type SchemaNumber = {
	type: "number";
	xpath: string;
};

export type SchemaBoolean = {
	type: "boolean";
	xpath: string;
};

export type SchemaObject = {
	type: "object";
	data: Record<string, Schema>;
};

export type SchemaNodes = {
	type: "nodes";
	xpath: string;
	data: Schema;
};

export type SchemaNode = {
	type: "node";
	xpath: string;
	data: Schema;
};

export type Schema =
	| SchemaString
	| SchemaNumber
	| SchemaBoolean
	| SchemaObject
	| SchemaNodes
	| SchemaNode;

export type SchemaToData<S extends Schema> = S extends SchemaString
	? string
	: S extends SchemaNumber
	? number
	: S extends SchemaBoolean
	? boolean
	: S extends SchemaObject
	? { [K in keyof S["data"]]: SchemaToData<S["data"][K]> }
	: S extends SchemaNode
	? SchemaToData<S["data"]> | null
	: S extends SchemaNodes
	? SchemaToData<S["data"]>[]
	: never;

export interface Document extends Object {}

export function compile<S extends Schema>(
	schema: S
): (document: Document) => SchemaToData<S>;

export function parse(xml: string): Document;
