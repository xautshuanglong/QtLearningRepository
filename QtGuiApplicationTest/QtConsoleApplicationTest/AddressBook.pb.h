// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AddressBook.proto

#ifndef PROTOBUF_INCLUDED_AddressBook_2eproto
#define PROTOBUF_INCLUDED_AddressBook_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include <google/protobuf/timestamp.pb.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_AddressBook_2eproto 

namespace protobuf_AddressBook_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[3];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_AddressBook_2eproto
namespace com {
namespace genomics {
namespace protobuf {
class AddressBook;
class AddressBookDefaultTypeInternal;
extern AddressBookDefaultTypeInternal _AddressBook_default_instance_;
class Person;
class PersonDefaultTypeInternal;
extern PersonDefaultTypeInternal _Person_default_instance_;
class Person_PhoneNumber;
class Person_PhoneNumberDefaultTypeInternal;
extern Person_PhoneNumberDefaultTypeInternal _Person_PhoneNumber_default_instance_;
}  // namespace protobuf
}  // namespace genomics
}  // namespace com
namespace google {
namespace protobuf {
template<> ::com::genomics::protobuf::AddressBook* Arena::CreateMaybeMessage<::com::genomics::protobuf::AddressBook>(Arena*);
template<> ::com::genomics::protobuf::Person* Arena::CreateMaybeMessage<::com::genomics::protobuf::Person>(Arena*);
template<> ::com::genomics::protobuf::Person_PhoneNumber* Arena::CreateMaybeMessage<::com::genomics::protobuf::Person_PhoneNumber>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace com {
namespace genomics {
namespace protobuf {

enum Person_PhoneType {
  Person_PhoneType_MOBILE = 0,
  Person_PhoneType_HOME = 1,
  Person_PhoneType_WORK = 2,
  Person_PhoneType_Person_PhoneType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  Person_PhoneType_Person_PhoneType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool Person_PhoneType_IsValid(int value);
const Person_PhoneType Person_PhoneType_PhoneType_MIN = Person_PhoneType_MOBILE;
const Person_PhoneType Person_PhoneType_PhoneType_MAX = Person_PhoneType_WORK;
const int Person_PhoneType_PhoneType_ARRAYSIZE = Person_PhoneType_PhoneType_MAX + 1;

const ::google::protobuf::EnumDescriptor* Person_PhoneType_descriptor();
inline const ::std::string& Person_PhoneType_Name(Person_PhoneType value) {
  return ::google::protobuf::internal::NameOfEnum(
    Person_PhoneType_descriptor(), value);
}
inline bool Person_PhoneType_Parse(
    const ::std::string& name, Person_PhoneType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Person_PhoneType>(
    Person_PhoneType_descriptor(), name, value);
}
// ===================================================================

class Person_PhoneNumber : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:com.genomics.protobuf.Person.PhoneNumber) */ {
 public:
  Person_PhoneNumber();
  virtual ~Person_PhoneNumber();

  Person_PhoneNumber(const Person_PhoneNumber& from);

  inline Person_PhoneNumber& operator=(const Person_PhoneNumber& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Person_PhoneNumber(Person_PhoneNumber&& from) noexcept
    : Person_PhoneNumber() {
    *this = ::std::move(from);
  }

  inline Person_PhoneNumber& operator=(Person_PhoneNumber&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Person_PhoneNumber& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Person_PhoneNumber* internal_default_instance() {
    return reinterpret_cast<const Person_PhoneNumber*>(
               &_Person_PhoneNumber_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(Person_PhoneNumber* other);
  friend void swap(Person_PhoneNumber& a, Person_PhoneNumber& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Person_PhoneNumber* New() const final {
    return CreateMaybeMessage<Person_PhoneNumber>(NULL);
  }

  Person_PhoneNumber* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Person_PhoneNumber>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Person_PhoneNumber& from);
  void MergeFrom(const Person_PhoneNumber& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Person_PhoneNumber* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string number = 1;
  void clear_number();
  static const int kNumberFieldNumber = 1;
  const ::std::string& number() const;
  void set_number(const ::std::string& value);
  #if LANG_CXX11
  void set_number(::std::string&& value);
  #endif
  void set_number(const char* value);
  void set_number(const char* value, size_t size);
  ::std::string* mutable_number();
  ::std::string* release_number();
  void set_allocated_number(::std::string* number);

  // .com.genomics.protobuf.Person.PhoneType type = 2;
  void clear_type();
  static const int kTypeFieldNumber = 2;
  ::com::genomics::protobuf::Person_PhoneType type() const;
  void set_type(::com::genomics::protobuf::Person_PhoneType value);

  // @@protoc_insertion_point(class_scope:com.genomics.protobuf.Person.PhoneNumber)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr number_;
  int type_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_AddressBook_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Person : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:com.genomics.protobuf.Person) */ {
 public:
  Person();
  virtual ~Person();

  Person(const Person& from);

  inline Person& operator=(const Person& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Person(Person&& from) noexcept
    : Person() {
    *this = ::std::move(from);
  }

  inline Person& operator=(Person&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const Person& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Person* internal_default_instance() {
    return reinterpret_cast<const Person*>(
               &_Person_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Person* other);
  friend void swap(Person& a, Person& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Person* New() const final {
    return CreateMaybeMessage<Person>(NULL);
  }

  Person* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Person>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Person& from);
  void MergeFrom(const Person& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Person* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  typedef Person_PhoneNumber PhoneNumber;

  typedef Person_PhoneType PhoneType;
  static const PhoneType MOBILE =
    Person_PhoneType_MOBILE;
  static const PhoneType HOME =
    Person_PhoneType_HOME;
  static const PhoneType WORK =
    Person_PhoneType_WORK;
  static inline bool PhoneType_IsValid(int value) {
    return Person_PhoneType_IsValid(value);
  }
  static const PhoneType PhoneType_MIN =
    Person_PhoneType_PhoneType_MIN;
  static const PhoneType PhoneType_MAX =
    Person_PhoneType_PhoneType_MAX;
  static const int PhoneType_ARRAYSIZE =
    Person_PhoneType_PhoneType_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  PhoneType_descriptor() {
    return Person_PhoneType_descriptor();
  }
  static inline const ::std::string& PhoneType_Name(PhoneType value) {
    return Person_PhoneType_Name(value);
  }
  static inline bool PhoneType_Parse(const ::std::string& name,
      PhoneType* value) {
    return Person_PhoneType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // repeated .com.genomics.protobuf.Person.PhoneNumber phones = 4;
  int phones_size() const;
  void clear_phones();
  static const int kPhonesFieldNumber = 4;
  ::com::genomics::protobuf::Person_PhoneNumber* mutable_phones(int index);
  ::google::protobuf::RepeatedPtrField< ::com::genomics::protobuf::Person_PhoneNumber >*
      mutable_phones();
  const ::com::genomics::protobuf::Person_PhoneNumber& phones(int index) const;
  ::com::genomics::protobuf::Person_PhoneNumber* add_phones();
  const ::google::protobuf::RepeatedPtrField< ::com::genomics::protobuf::Person_PhoneNumber >&
      phones() const;

  // string name = 1;
  void clear_name();
  static const int kNameFieldNumber = 1;
  const ::std::string& name() const;
  void set_name(const ::std::string& value);
  #if LANG_CXX11
  void set_name(::std::string&& value);
  #endif
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  ::std::string* mutable_name();
  ::std::string* release_name();
  void set_allocated_name(::std::string* name);

  // string email = 3;
  void clear_email();
  static const int kEmailFieldNumber = 3;
  const ::std::string& email() const;
  void set_email(const ::std::string& value);
  #if LANG_CXX11
  void set_email(::std::string&& value);
  #endif
  void set_email(const char* value);
  void set_email(const char* value, size_t size);
  ::std::string* mutable_email();
  ::std::string* release_email();
  void set_allocated_email(::std::string* email);

  // .google.protobuf.Timestamp last_updated = 5;
  bool has_last_updated() const;
  void clear_last_updated();
  static const int kLastUpdatedFieldNumber = 5;
  private:
  const ::google::protobuf::Timestamp& _internal_last_updated() const;
  public:
  const ::google::protobuf::Timestamp& last_updated() const;
  ::google::protobuf::Timestamp* release_last_updated();
  ::google::protobuf::Timestamp* mutable_last_updated();
  void set_allocated_last_updated(::google::protobuf::Timestamp* last_updated);

  // int32 id = 2;
  void clear_id();
  static const int kIdFieldNumber = 2;
  ::google::protobuf::int32 id() const;
  void set_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:com.genomics.protobuf.Person)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::com::genomics::protobuf::Person_PhoneNumber > phones_;
  ::google::protobuf::internal::ArenaStringPtr name_;
  ::google::protobuf::internal::ArenaStringPtr email_;
  ::google::protobuf::Timestamp* last_updated_;
  ::google::protobuf::int32 id_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_AddressBook_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class AddressBook : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:com.genomics.protobuf.AddressBook) */ {
 public:
  AddressBook();
  virtual ~AddressBook();

  AddressBook(const AddressBook& from);

  inline AddressBook& operator=(const AddressBook& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  AddressBook(AddressBook&& from) noexcept
    : AddressBook() {
    *this = ::std::move(from);
  }

  inline AddressBook& operator=(AddressBook&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const AddressBook& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const AddressBook* internal_default_instance() {
    return reinterpret_cast<const AddressBook*>(
               &_AddressBook_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  void Swap(AddressBook* other);
  friend void swap(AddressBook& a, AddressBook& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline AddressBook* New() const final {
    return CreateMaybeMessage<AddressBook>(NULL);
  }

  AddressBook* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<AddressBook>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const AddressBook& from);
  void MergeFrom(const AddressBook& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(AddressBook* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .com.genomics.protobuf.Person people = 1;
  int people_size() const;
  void clear_people();
  static const int kPeopleFieldNumber = 1;
  ::com::genomics::protobuf::Person* mutable_people(int index);
  ::google::protobuf::RepeatedPtrField< ::com::genomics::protobuf::Person >*
      mutable_people();
  const ::com::genomics::protobuf::Person& people(int index) const;
  ::com::genomics::protobuf::Person* add_people();
  const ::google::protobuf::RepeatedPtrField< ::com::genomics::protobuf::Person >&
      people() const;

  // @@protoc_insertion_point(class_scope:com.genomics.protobuf.AddressBook)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::com::genomics::protobuf::Person > people_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_AddressBook_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Person_PhoneNumber

// string number = 1;
inline void Person_PhoneNumber::clear_number() {
  number_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Person_PhoneNumber::number() const {
  // @@protoc_insertion_point(field_get:com.genomics.protobuf.Person.PhoneNumber.number)
  return number_.GetNoArena();
}
inline void Person_PhoneNumber::set_number(const ::std::string& value) {
  
  number_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:com.genomics.protobuf.Person.PhoneNumber.number)
}
#if LANG_CXX11
inline void Person_PhoneNumber::set_number(::std::string&& value) {
  
  number_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:com.genomics.protobuf.Person.PhoneNumber.number)
}
#endif
inline void Person_PhoneNumber::set_number(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  number_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:com.genomics.protobuf.Person.PhoneNumber.number)
}
inline void Person_PhoneNumber::set_number(const char* value, size_t size) {
  
  number_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:com.genomics.protobuf.Person.PhoneNumber.number)
}
inline ::std::string* Person_PhoneNumber::mutable_number() {
  
  // @@protoc_insertion_point(field_mutable:com.genomics.protobuf.Person.PhoneNumber.number)
  return number_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Person_PhoneNumber::release_number() {
  // @@protoc_insertion_point(field_release:com.genomics.protobuf.Person.PhoneNumber.number)
  
  return number_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Person_PhoneNumber::set_allocated_number(::std::string* number) {
  if (number != NULL) {
    
  } else {
    
  }
  number_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), number);
  // @@protoc_insertion_point(field_set_allocated:com.genomics.protobuf.Person.PhoneNumber.number)
}

// .com.genomics.protobuf.Person.PhoneType type = 2;
inline void Person_PhoneNumber::clear_type() {
  type_ = 0;
}
inline ::com::genomics::protobuf::Person_PhoneType Person_PhoneNumber::type() const {
  // @@protoc_insertion_point(field_get:com.genomics.protobuf.Person.PhoneNumber.type)
  return static_cast< ::com::genomics::protobuf::Person_PhoneType >(type_);
}
inline void Person_PhoneNumber::set_type(::com::genomics::protobuf::Person_PhoneType value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:com.genomics.protobuf.Person.PhoneNumber.type)
}

// -------------------------------------------------------------------

// Person

// string name = 1;
inline void Person::clear_name() {
  name_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Person::name() const {
  // @@protoc_insertion_point(field_get:com.genomics.protobuf.Person.name)
  return name_.GetNoArena();
}
inline void Person::set_name(const ::std::string& value) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:com.genomics.protobuf.Person.name)
}
#if LANG_CXX11
inline void Person::set_name(::std::string&& value) {
  
  name_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:com.genomics.protobuf.Person.name)
}
#endif
inline void Person::set_name(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:com.genomics.protobuf.Person.name)
}
inline void Person::set_name(const char* value, size_t size) {
  
  name_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:com.genomics.protobuf.Person.name)
}
inline ::std::string* Person::mutable_name() {
  
  // @@protoc_insertion_point(field_mutable:com.genomics.protobuf.Person.name)
  return name_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Person::release_name() {
  // @@protoc_insertion_point(field_release:com.genomics.protobuf.Person.name)
  
  return name_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Person::set_allocated_name(::std::string* name) {
  if (name != NULL) {
    
  } else {
    
  }
  name_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), name);
  // @@protoc_insertion_point(field_set_allocated:com.genomics.protobuf.Person.name)
}

// int32 id = 2;
inline void Person::clear_id() {
  id_ = 0;
}
inline ::google::protobuf::int32 Person::id() const {
  // @@protoc_insertion_point(field_get:com.genomics.protobuf.Person.id)
  return id_;
}
inline void Person::set_id(::google::protobuf::int32 value) {
  
  id_ = value;
  // @@protoc_insertion_point(field_set:com.genomics.protobuf.Person.id)
}

// string email = 3;
inline void Person::clear_email() {
  email_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& Person::email() const {
  // @@protoc_insertion_point(field_get:com.genomics.protobuf.Person.email)
  return email_.GetNoArena();
}
inline void Person::set_email(const ::std::string& value) {
  
  email_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:com.genomics.protobuf.Person.email)
}
#if LANG_CXX11
inline void Person::set_email(::std::string&& value) {
  
  email_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:com.genomics.protobuf.Person.email)
}
#endif
inline void Person::set_email(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  email_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:com.genomics.protobuf.Person.email)
}
inline void Person::set_email(const char* value, size_t size) {
  
  email_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:com.genomics.protobuf.Person.email)
}
inline ::std::string* Person::mutable_email() {
  
  // @@protoc_insertion_point(field_mutable:com.genomics.protobuf.Person.email)
  return email_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Person::release_email() {
  // @@protoc_insertion_point(field_release:com.genomics.protobuf.Person.email)
  
  return email_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Person::set_allocated_email(::std::string* email) {
  if (email != NULL) {
    
  } else {
    
  }
  email_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), email);
  // @@protoc_insertion_point(field_set_allocated:com.genomics.protobuf.Person.email)
}

// repeated .com.genomics.protobuf.Person.PhoneNumber phones = 4;
inline int Person::phones_size() const {
  return phones_.size();
}
inline void Person::clear_phones() {
  phones_.Clear();
}
inline ::com::genomics::protobuf::Person_PhoneNumber* Person::mutable_phones(int index) {
  // @@protoc_insertion_point(field_mutable:com.genomics.protobuf.Person.phones)
  return phones_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::com::genomics::protobuf::Person_PhoneNumber >*
Person::mutable_phones() {
  // @@protoc_insertion_point(field_mutable_list:com.genomics.protobuf.Person.phones)
  return &phones_;
}
inline const ::com::genomics::protobuf::Person_PhoneNumber& Person::phones(int index) const {
  // @@protoc_insertion_point(field_get:com.genomics.protobuf.Person.phones)
  return phones_.Get(index);
}
inline ::com::genomics::protobuf::Person_PhoneNumber* Person::add_phones() {
  // @@protoc_insertion_point(field_add:com.genomics.protobuf.Person.phones)
  return phones_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::com::genomics::protobuf::Person_PhoneNumber >&
Person::phones() const {
  // @@protoc_insertion_point(field_list:com.genomics.protobuf.Person.phones)
  return phones_;
}

// .google.protobuf.Timestamp last_updated = 5;
inline bool Person::has_last_updated() const {
  return this != internal_default_instance() && last_updated_ != NULL;
}
inline const ::google::protobuf::Timestamp& Person::_internal_last_updated() const {
  return *last_updated_;
}
inline const ::google::protobuf::Timestamp& Person::last_updated() const {
  const ::google::protobuf::Timestamp* p = last_updated_;
  // @@protoc_insertion_point(field_get:com.genomics.protobuf.Person.last_updated)
  return p != NULL ? *p : *reinterpret_cast<const ::google::protobuf::Timestamp*>(
      &::google::protobuf::_Timestamp_default_instance_);
}
inline ::google::protobuf::Timestamp* Person::release_last_updated() {
  // @@protoc_insertion_point(field_release:com.genomics.protobuf.Person.last_updated)
  
  ::google::protobuf::Timestamp* temp = last_updated_;
  last_updated_ = NULL;
  return temp;
}
inline ::google::protobuf::Timestamp* Person::mutable_last_updated() {
  
  if (last_updated_ == NULL) {
    auto* p = CreateMaybeMessage<::google::protobuf::Timestamp>(GetArenaNoVirtual());
    last_updated_ = p;
  }
  // @@protoc_insertion_point(field_mutable:com.genomics.protobuf.Person.last_updated)
  return last_updated_;
}
inline void Person::set_allocated_last_updated(::google::protobuf::Timestamp* last_updated) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete reinterpret_cast< ::google::protobuf::MessageLite*>(last_updated_);
  }
  if (last_updated) {
    ::google::protobuf::Arena* submessage_arena =
      reinterpret_cast<::google::protobuf::MessageLite*>(last_updated)->GetArena();
    if (message_arena != submessage_arena) {
      last_updated = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, last_updated, submessage_arena);
    }
    
  } else {
    
  }
  last_updated_ = last_updated;
  // @@protoc_insertion_point(field_set_allocated:com.genomics.protobuf.Person.last_updated)
}

// -------------------------------------------------------------------

// AddressBook

// repeated .com.genomics.protobuf.Person people = 1;
inline int AddressBook::people_size() const {
  return people_.size();
}
inline void AddressBook::clear_people() {
  people_.Clear();
}
inline ::com::genomics::protobuf::Person* AddressBook::mutable_people(int index) {
  // @@protoc_insertion_point(field_mutable:com.genomics.protobuf.AddressBook.people)
  return people_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::com::genomics::protobuf::Person >*
AddressBook::mutable_people() {
  // @@protoc_insertion_point(field_mutable_list:com.genomics.protobuf.AddressBook.people)
  return &people_;
}
inline const ::com::genomics::protobuf::Person& AddressBook::people(int index) const {
  // @@protoc_insertion_point(field_get:com.genomics.protobuf.AddressBook.people)
  return people_.Get(index);
}
inline ::com::genomics::protobuf::Person* AddressBook::add_people() {
  // @@protoc_insertion_point(field_add:com.genomics.protobuf.AddressBook.people)
  return people_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::com::genomics::protobuf::Person >&
AddressBook::people() const {
  // @@protoc_insertion_point(field_list:com.genomics.protobuf.AddressBook.people)
  return people_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf
}  // namespace genomics
}  // namespace com

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::com::genomics::protobuf::Person_PhoneType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::com::genomics::protobuf::Person_PhoneType>() {
  return ::com::genomics::protobuf::Person_PhoneType_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_AddressBook_2eproto
