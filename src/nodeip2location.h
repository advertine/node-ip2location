#ifndef HAVE_NODEIP2LOCATION_H
#define HAVE_NODEIP2LOCATION_H

#include <nan.h>
#include <v8.h>
#include <node.h>
#include <string.h>

extern "C" {
#  include "ip2location.h"
}

#include "nodeip2ldictionary.h"

using v8::Local;
using v8::Object;
using v8::Array;
using v8::String;
using v8::Function;
using Nan::Persistent;
using Nan::ObjectWrap;

#define LOCATION_DBMODE_FILE   "file"
#define LOCATION_DBMODE_MMAP   "mmap"
#define LOCATION_DBMODE_SHARED "shared"
#define LOCATION_DBMODE_CACHE  "cache"
#define LOCATION_DBMODE_CLOSED "closed"
#define LOCATION_MSG_NOT_SUPPORTED \
  "This method is not applicable for current IP2Location binary data file." \
  " Please upgrade your subscription package to install new data file."


#define LOCATION_ALL ( (uint32_t)(IP2L_MASK(IP2L_INDEX_MAX + 1) - 1) )

class Location: public ObjectWrap {
  public:
    IP2Location *iplocdb;
    const char *dbmode;
    static Persistent<Function> constructor;

    static NAN_MODULE_INIT(Init);
    Location( char *locdbpath, IP2LOCATION_ACCESS_TYPE mtype, char *shared = NULL );
    ~Location();
    void Close();
    static NAN_METHOD(New);
    static NAN_GETTER(GetDbMode);
    static NAN_GETTER(GetIsOpen);
    static NAN_GETTER(HasIpv6);
    static NAN_METHOD(CloseDatabase);
    static NAN_METHOD(DeleteShared);
    static NAN_METHOD(CreateDictionary);
    static NAN_METHOD(GetDbInfo);
    static NAN_METHOD(Query);
    static NAN_METHOD(GetAll);

  private:
    static void SetResultErrorStatus( Local<Object> &result,
                                      const char * const status,
                                      bool setIP = true );
    static void BuildDictionary( IP2LDictionary &dict,
                                 IP2Location *loc,
                                 uint32_t mask );
    static void BuildDictionaryItem( IP2Location *loc,
                                     uint32_t rowoffset,
                                     uint32_t mask,
                                     IP2LDictionary &dict );
    template <IP2LOCATION_DATA_INDEX branch_index,
              IP2L_DICT_TYPE branch_type,
              IP2LOCATION_DATA_INDEX leaf_index>
    NAN_INLINE static void BuildDictionaryBranchItem(
                                                IP2Location *loc,
                                                uint32_t rowoffset,
                                                uint32_t mask,
                                                IP2LDictionaryCountry *country,
                                                char * const name);
    static Local<Object> CreateDictionaryResult( const IP2LDictionary &dict,
                                                 const uint32_t mask );
    static Local<Array> CreateArrayResult(
                            const ::Map<IP2LDictionaryElement>::type &dict_map);
    NAN_INLINE static void CreateDictionaryResultBranch(
                            const uint32_t mask,
                            const uint32_t branch_mask,
                            const uint32_t leaf_mask,
                            const ::Map<IP2LDictionaryElement>::type &branch_map,
                            const Local<String> &indexLabel,
                            const Local<String> &label,
                            Local<Object> &result);
    NAN_INLINE static void CreateDictionaryResultElement(
                              const uint32_t mask,
                              const uint32_t leaf_mask,
                              const ::Map<IP2LDictionaryElement>::type &leaf_map,
                              const Local<String> &label,
                              Local<Object> &result);
};

#endif /* HAVE_NODEIP2LOCATION_H */
