bool EmitFieldNonDefaultCondition(io::Printer* printer,
                                  const std::string& prefix,
                                  const FieldDescriptor* field) {
  GOOGLE_CHECK(!HasHasbit(field));
  Formatter format(printer);
  format.Set("prefix", prefix);
  format.Set("name", FieldName(field));
  // Merge and serialize semantics: primitive fields are merged/serialized only
  // if non-zero (numeric) or non-empty (string).
  if (!field->is_repeated() && !field->containing_oneof()) {
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_STRING) {
      format("if ($prefix$$name$().size() > 0) {\n");
    } else if (field->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE) {
      // Message fields still have has_$name$() methods.
      format("if ($prefix$has_$name$()) {\n");
    } else if (field->cpp_type() == FieldDescriptor::CPPTYPE_DOUBLE ||
               field->cpp_type() == FieldDescriptor::CPPTYPE_FLOAT) {
      // Handle float comparison to prevent -Wfloat-equal warnings
      format("if (!($prefix$$name$() <= 0 && $prefix$$name$() >= 0)) {\n");
    } else {
      format("if ($prefix$$name$() != 0) {\n");
    }
    format.Indent();
    return true;
  } else if (field->real_containing_oneof()) {
    format("if (_internal_has_$name$()) {\n");
    format.Indent();
    return true;
  }
  return false;
}

// Does the given field have a has_$name$() method?
bool HasHasMethod(const FieldDescriptor* field) {
  if (HasFieldPresence(field->file())) {
    // In proto1/proto2, every field has a has_$name$() method.
    return true;
  }
  // For message types without true field presence, only fields with a message
  // type have a has_$name$() method.
  return field->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE ||
         field->has_optional_keyword();
}

// Collects map entry message type information.
void CollectMapInfo(const Options& options, const Descriptor* descriptor,
                    std::map<std::string, std::string>* variables) {
  GOOGLE_CHECK(IsMapEntryMessage(descriptor));
  std::map<std::string, std::string>& vars = *variables;
  const FieldDescriptor* key = descriptor->FindFieldByName("key");
  const FieldDescriptor* val = descriptor->FindFieldByName("value");
  vars["key_cpp"] = PrimitiveTypeName(options, key->cpp_type());
  switch (val->cpp_type()) {
    case FieldDescriptor::CPPTYPE_MESSAGE:
      vars["val_cpp"] = FieldMessageTypeName(val, options);
      break;
    case FieldDescriptor::CPPTYPE_ENUM:
      vars["val_cpp"] = ClassName(val->enum_type(), true);
      break;
    default:
      vars["val_cpp"] = PrimitiveTypeName(options, val->cpp_type());
  }
  vars["key_wire_type"] =
      "TYPE_" + ToUpper(DeclaredTypeMethodName(key->type()));
  vars["val_wire_type"] =
      "TYPE_" + ToUpper(DeclaredTypeMethodName(val->type()));
}

// Does the given field have a private (internal helper only) has_$name$()
// method?
bool HasPrivateHasMethod(const FieldDescriptor* field) {
  // Only for oneofs in message types with no field presence. has_$name$(),
  // based on the oneof case, is still useful internally for generated code.
  return (!HasFieldPresence(field->file()) && field->real_containing_oneof());
}

// TODO(ckennelly):  Cull these exclusions if/when these protos do not have
// their methods overridden by subclasses.

bool ShouldMarkClassAsFinal(const Descriptor* descriptor,
                            const Options& options) {
  return true;
}

bool ShouldMarkClearAsFinal(const Descriptor* descriptor,
                            const Options& options) {
  static std::set<std::string> exclusions{
  };

  const std::string name = ClassName(descriptor, true);
  return exclusions.find(name) == exclusions.end() ||
         options.opensource_runtime;
}

bool ShouldMarkIsInitializedAsFinal(const Descriptor* descriptor,
                                    const Options& options) {
  static std::set<std::string> exclusions{
  };

  const std::string name = ClassName(descriptor, true);
  return exclusions.find(name) == exclusions.end() ||
         options.opensource_runtime;
}

bool ShouldMarkNewAsFinal(const Descriptor* descriptor,
                          const Options& options) {
  return true;
}

int main() { }

inf f(){return 0;}
