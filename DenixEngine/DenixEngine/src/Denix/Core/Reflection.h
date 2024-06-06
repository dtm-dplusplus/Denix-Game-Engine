//#pragma once
//
//#include <cstddef>
//#include <string>
//#include <vector>
//
//struct YamlStruct
//{
//    int id;
//    std::string name;
//    std::vector<std::string> tags;
//};
//
//struct Type {
//    char const* name;
//    size_t size;
//};
//
//struct Field {
//    Type* type;
//    char const* name;
//    size_t offset;
//};
//
//struct Function {
//    Function(int numParam)
//    {}
//    Field returnValue;
//    Field* parameters;
//    char const* name;
//};
//
//struct Class : public Type {
//    Field* fields;
//    Function* functions;
//};
//
//template<>
//Type const*
//GetType<int>()
//{
//	static Type type{"int", sizeof(int)};
//	return &type;
//}
//
//template<>
//Type const*
//GetType<char>()
//{
//    static Type type{ "char", sizeof(char) };
//    return &type;
//}
//
//template<class T>
//Type const*
//GetType()
//{
//    return std::detail::GetTypeImpl(TypeTag<T>{});
//}
//
//template<class T>
//Type const* 
//GetTypeImpl(TypeTag<T>)
//{
//	static Type type{ "T", sizeof(T) };
//	return &type;
//}
//
//Class const* GetClassImpl();