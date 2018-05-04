#ifndef session_sa_h
#define session_sa_h

#include "login.h"

//! @brief session_sa: Session that the Service Application (SA) initiates
/** Total Size (Header + Body): 247 <br>
    We use 248 here such that 248 + 8(i.e buffer sizeof(offset)) = 256 <br>
    Hence sizeof(basic_begin) = 256
*/
namespace cuap::pdu
{
   struct basic_begin : public basic_header<uint8_t, 248>
   {
         using basic_header<uint8_t, 248>::basic_header;
      //enum Op_Type { USSR = 0x01, USSN = 0x02, USSDCResp = 0x03, ENDRelease = 0x04};

      public: /// Being, Continue, End Related methods
         template <uint32_t N>
         void msisdn(uint8_t(&dest)[N])
         {
            misc::set_null(dest);
            memcpy(dest, &buffer[BeginBody::MsIsdn], 21); /** MSISDN is 21  byte octet string */
         }

         void set_msisdn(const uint8_t* _msisdn, size_t sz)
         {
            assign_n(_msisdn, sz, BeginBody::MsIsdn , BeginBody::Service_Code);
         }

         template <uint32_t N>
         void service_code(uint8_t(&dest)[N])
         {
            misc::set_null(dest);
            memcpy(dest, &buffer[BeginBody::Service_Code], 21); /** Service_Code is 21 byte octet string */
         }

         void set_service_code(const uint8_t* srv_code, size_t sz)
         {
            assign_n(srv_code, sz, BeginBody::Service_Code, BeginBody::Code_Scheme);
         }

         template <uint32_t N>
         void ussd_content (uint8_t(&dest)[N])
         {
            misc::set_null(dest);
            memcpy(dest, &buffer[BeginBody::Ussd_Content], command_len() - BeginBody::Ussd_Content);
         }

         void ussd_content (uint8_t* dest, size_t sz)
         {
            misc::set_null(dest, sz);
            memcpy(dest, &buffer[BeginBody::Ussd_Content], command_len() - BeginBody::Ussd_Content);
         }

         void set_ussd_content(const uint8_t* srv_code, size_t sz)
         {
            assign_n(srv_code, sz, BeginBody::Ussd_Content, BeginBody::Ussd_Content + 182);
         }

         inline  uint8_t ussd_ver() { return buffer[BeginBody::Ussd_Version]; }
         void    set_ussd_ver(uint8_t ussd)
         {
            buffer[BeginBody::Ussd_Version] = ussd;
         }

         inline  uint8_t ussd_op_type() { return buffer[BeginBody::Ussd_Op_Type]; }
         void    set_ussd_op_type(uint8_t op_type)
         {
            buffer[BeginBody::Ussd_Op_Type] = op_type;
         }

         inline  uint8_t code_scheme() { return buffer[BeginBody::Code_Scheme]; }
         void    set_code_scheme(uint8_t csch)
         {
            buffer[BeginBody::Ussd_Op_Type] = csch;
         }

      public: /// Methods to test fot the operation type
         bool op_type_USSR() { return ussd_op_type() == 0x01; }
         bool op_type_USSN() { return ussd_op_type() == 0x02; }
         bool op_type_USSDCResp() { return ussd_op_type() == 0x03; }
         bool op_type_ENDRelese() { return ussd_op_type() == 0x04; }
   };
}

namespace cuap::pdu
{
   using namespace pdu;
   /// Represents a BEGIN message
   using begin_msg    = basic_begin;

   /// Represents CONTINUE(request), CONTINUE(response), CONTINUE(notify)
   using continue_msg = basic_begin;

   /// Represents an END message
   using end_msg      = basic_begin;
}

namespace concepts
{
   template<typename T>
   using Ref = T&;

   template<typename T>
   using ptr = T*;

   template<typename T>
   using const_ptr = const T*;

   /*template <class T>
   concept bool begin_family = is_convertible_v<T, pdu::begin_msg> && is_convertible_v<T, pdu::continue_msg>
                               || MutableChar<T>;*/
   template <class T>
   concept bool begin_t = requires(T)
   {
      typename cuap::pdu::begin_msg;
      !std::is_pointer<T>::value;
   };
}

#endif//session_sa_h
