#pragma once

struct Type{
    char const* name;
    size_t size;
}

struct Field{
    Type* type;
    char const* name;
    size_t offset;
}

struct Function{
    Field returnValue;
    Field parameters[N];
    char const* name;
}

struct Class: public Type {
    Field fields[N];
    Function functions[N];
}

