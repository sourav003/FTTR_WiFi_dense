//
// Generated file, do not edit! Created by opp_msgtool 6.0 from trigger_bsr.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "trigger_bsr_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(trigger_bsr)

trigger_bsr::trigger_bsr(const char *name, short kind) : ::omnetpp::cPacket(name, kind)
{
}

trigger_bsr::trigger_bsr(const trigger_bsr& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

trigger_bsr::~trigger_bsr()
{
}

trigger_bsr& trigger_bsr::operator=(const trigger_bsr& other)
{
    if (this == &other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void trigger_bsr::copy(const trigger_bsr& other)
{
    this->Throughput = other.Throughput;
    this->BW_alloc = other.BW_alloc;
    this->TX_time = other.TX_time;
    this->STA_band_pref = other.STA_band_pref;
    this->STA_buffer_size = other.STA_buffer_size;
}

void trigger_bsr::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->Throughput);
    doParsimPacking(b,this->BW_alloc);
    doParsimPacking(b,this->TX_time);
    doParsimPacking(b,this->STA_band_pref);
    doParsimPacking(b,this->STA_buffer_size);
}

void trigger_bsr::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->Throughput);
    doParsimUnpacking(b,this->BW_alloc);
    doParsimUnpacking(b,this->TX_time);
    doParsimUnpacking(b,this->STA_band_pref);
    doParsimUnpacking(b,this->STA_buffer_size);
}

double trigger_bsr::getThroughput() const
{
    return this->Throughput;
}

void trigger_bsr::setThroughput(double Throughput)
{
    this->Throughput = Throughput;
}

double trigger_bsr::getBW_alloc() const
{
    return this->BW_alloc;
}

void trigger_bsr::setBW_alloc(double BW_alloc)
{
    this->BW_alloc = BW_alloc;
}

double trigger_bsr::getTX_time() const
{
    return this->TX_time;
}

void trigger_bsr::setTX_time(double TX_time)
{
    this->TX_time = TX_time;
}

const char * trigger_bsr::getSTA_band_pref() const
{
    return this->STA_band_pref.c_str();
}

void trigger_bsr::setSTA_band_pref(const char * STA_band_pref)
{
    this->STA_band_pref = STA_band_pref;
}

double trigger_bsr::getSTA_buffer_size() const
{
    return this->STA_buffer_size;
}

void trigger_bsr::setSTA_buffer_size(double STA_buffer_size)
{
    this->STA_buffer_size = STA_buffer_size;
}

class trigger_bsrDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_Throughput,
        FIELD_BW_alloc,
        FIELD_TX_time,
        FIELD_STA_band_pref,
        FIELD_STA_buffer_size,
    };
  public:
    trigger_bsrDescriptor();
    virtual ~trigger_bsrDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(trigger_bsrDescriptor)

trigger_bsrDescriptor::trigger_bsrDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(trigger_bsr)), "omnetpp::cPacket")
{
    propertyNames = nullptr;
}

trigger_bsrDescriptor::~trigger_bsrDescriptor()
{
    delete[] propertyNames;
}

bool trigger_bsrDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<trigger_bsr *>(obj)!=nullptr;
}

const char **trigger_bsrDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *trigger_bsrDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int trigger_bsrDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int trigger_bsrDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_Throughput
        FD_ISEDITABLE,    // FIELD_BW_alloc
        FD_ISEDITABLE,    // FIELD_TX_time
        FD_ISEDITABLE,    // FIELD_STA_band_pref
        FD_ISEDITABLE,    // FIELD_STA_buffer_size
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *trigger_bsrDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "Throughput",
        "BW_alloc",
        "TX_time",
        "STA_band_pref",
        "STA_buffer_size",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int trigger_bsrDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "Throughput") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "BW_alloc") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "TX_time") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "STA_band_pref") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "STA_buffer_size") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *trigger_bsrDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "double",    // FIELD_Throughput
        "double",    // FIELD_BW_alloc
        "double",    // FIELD_TX_time
        "string",    // FIELD_STA_band_pref
        "double",    // FIELD_STA_buffer_size
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **trigger_bsrDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *trigger_bsrDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int trigger_bsrDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    trigger_bsr *pp = omnetpp::fromAnyPtr<trigger_bsr>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void trigger_bsrDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    trigger_bsr *pp = omnetpp::fromAnyPtr<trigger_bsr>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'trigger_bsr'", field);
    }
}

const char *trigger_bsrDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    trigger_bsr *pp = omnetpp::fromAnyPtr<trigger_bsr>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string trigger_bsrDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    trigger_bsr *pp = omnetpp::fromAnyPtr<trigger_bsr>(object); (void)pp;
    switch (field) {
        case FIELD_Throughput: return double2string(pp->getThroughput());
        case FIELD_BW_alloc: return double2string(pp->getBW_alloc());
        case FIELD_TX_time: return double2string(pp->getTX_time());
        case FIELD_STA_band_pref: return oppstring2string(pp->getSTA_band_pref());
        case FIELD_STA_buffer_size: return double2string(pp->getSTA_buffer_size());
        default: return "";
    }
}

void trigger_bsrDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    trigger_bsr *pp = omnetpp::fromAnyPtr<trigger_bsr>(object); (void)pp;
    switch (field) {
        case FIELD_Throughput: pp->setThroughput(string2double(value)); break;
        case FIELD_BW_alloc: pp->setBW_alloc(string2double(value)); break;
        case FIELD_TX_time: pp->setTX_time(string2double(value)); break;
        case FIELD_STA_band_pref: pp->setSTA_band_pref((value)); break;
        case FIELD_STA_buffer_size: pp->setSTA_buffer_size(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'trigger_bsr'", field);
    }
}

omnetpp::cValue trigger_bsrDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    trigger_bsr *pp = omnetpp::fromAnyPtr<trigger_bsr>(object); (void)pp;
    switch (field) {
        case FIELD_Throughput: return pp->getThroughput();
        case FIELD_BW_alloc: return pp->getBW_alloc();
        case FIELD_TX_time: return pp->getTX_time();
        case FIELD_STA_band_pref: return pp->getSTA_band_pref();
        case FIELD_STA_buffer_size: return pp->getSTA_buffer_size();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'trigger_bsr' as cValue -- field index out of range?", field);
    }
}

void trigger_bsrDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    trigger_bsr *pp = omnetpp::fromAnyPtr<trigger_bsr>(object); (void)pp;
    switch (field) {
        case FIELD_Throughput: pp->setThroughput(value.doubleValue()); break;
        case FIELD_BW_alloc: pp->setBW_alloc(value.doubleValue()); break;
        case FIELD_TX_time: pp->setTX_time(value.doubleValue()); break;
        case FIELD_STA_band_pref: pp->setSTA_band_pref(value.stringValue()); break;
        case FIELD_STA_buffer_size: pp->setSTA_buffer_size(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'trigger_bsr'", field);
    }
}

const char *trigger_bsrDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr trigger_bsrDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    trigger_bsr *pp = omnetpp::fromAnyPtr<trigger_bsr>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void trigger_bsrDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    trigger_bsr *pp = omnetpp::fromAnyPtr<trigger_bsr>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'trigger_bsr'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

