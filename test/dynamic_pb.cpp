#include <iostream>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>

#include "dynamic_pb.pb.h"

using namespace std;
using namespace google::protobuf;

const string serialize()
{
    Vector vector;

    FileDescriptorProto file_proto;
    file_proto.set_name("foo.proto");

    FieldDescriptorProto *field_proto = NULL;

    // create dynamic message proto names "Struct"
    DescriptorProto *struct_proto = file_proto.add_message_type();
    struct_proto->set_name("Struct");

    cout << struct_proto->DebugString();

    field_proto = struct_proto->add_field();
    field_proto->set_label(FieldDescriptorProto::LABEL_REQUIRED);
    field_proto->set_type(FieldDescriptorProto::TYPE_STRING);
    field_proto->set_name("first");
    field_proto->set_number(1);

    field_proto = struct_proto->add_field();
    field_proto->set_label(FieldDescriptorProto::LABEL_REQUIRED);
    field_proto->set_type(FieldDescriptorProto::TYPE_UINT32);
    field_proto->set_name("second");
    field_proto->set_number(2);

    string proto_str;
    file_proto.SerializeToString(&proto_str);
    vector.set_proto(proto_str);

    // add the "Struct" message proto to file proto and build it
    DescriptorPool pool;
    const FileDescriptor *file_descriptor = pool.BuildFile(file_proto);

    const Descriptor *descriptor = file_descriptor->FindMessageTypeByName("Struct");

    // build a dynamic message by "Struct" proto
    DynamicMessageFactory factory(&pool);
    const Message *message = factory.GetPrototype(descriptor);

    // create a real instance of "Struct"
    Message *s = message->New();

    // write the "Struct" instance by reflection
    const Reflection *reflection = s->GetReflection();

    const FieldDescriptor *field = NULL;
    field = descriptor->FindFieldByName("first");
    reflection->SetString(s, field, "my key");
    field = descriptor->FindFieldByName("second");
    reflection->SetUInt32(s, field, 1234);

    string value_str;
    s->SerializeToString(&value_str);
    vector.set_value(value_str);

    delete s;

    string ret;
    vector.SerializeToString(&ret);
    return ret;
}

void deserialize(const string &str)
{
    Vector vector;
    vector.ParseFromString(str);

    string proto = vector.proto();
    string value = vector.value();

    FileDescriptorProto file_proto;
    file_proto.ParseFromString(proto);

    cout << file_proto.DebugString();

    DescriptorPool pool;
    const FileDescriptor *file_descriptor = pool.BuildFile(file_proto);

    const Descriptor *descriptor = file_descriptor->FindMessageTypeByName("Struct");

    // build a dynamic message by "Struct" proto
    DynamicMessageFactory factory(&pool);
    const Message *message = factory.GetPrototype(descriptor);

    // create a real instance of "Struct"
    Message *s = message->New();

    // write the "Struct" instance by reflection
    const Reflection *reflection = s->GetReflection();

    s->ParseFromString(value);

    cout << s->DebugString();
}

int main(int argc, const char *argv[])
{
    string str = serialize();
    deserialize(str);

    return 0;
}
