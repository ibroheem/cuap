#ifndef bind_family_h
#define bind_family_h

#include "types.h"
#include "functions.h"

namespace cuap::pdu
{
   using namespace misc;
   using namespace concepts;

   struct basic_bind_resp : public basic_header<uint8_t, 57>
   {
      using basic_header<uint8_t, 57>::basic_header;

      template <class T, uint16_t N>
      void  set_system_id(T(&id)[N])
      {
         assign_n(id, N, BindBody::System_ID, BindBody::Password);
      }

      void  set_system_id(const uchar_t id, size_t sz)
      {
         assign_n(id, sz, BindBody::System_ID, BindBody::Password);
      }

      /// @brief Gets System_ID from Message
      /// sets @id to null, then assign the target string to it.
      template <class T2, uint N>
      void system_id(T2(&id)[N])
      {
         misc::set_null(id);
         functions::get_body_str(*this, id, BindBody::System_ID, 11);
      }
   };

   struct basic_bind_msg : public basic_bind_resp
   {
      using basic_bind_resp::basic_bind_resp;

      /// @brief Gets Password from Message
      /// sets @newpass to null, then assign the target string to it.
      void password(char_t newpass)
      {
         set_null(newpass);
         functions::get_body_str(*this, newpass, BindBody::Password, 11);
      }

      void  set_password(uchar_t newpass, size_t sz)
      {
         assign_n(newpass, sz, BindBody::Password, BindBody::System_Type);
      }

      void system_type(char_t sys_type)
      {
         set_null(sys_type);
         functions::get_body_str(*this, sys_type, BindBody::System_Type, 11);
      }

      void  set_system_type(uchar_t tp, size_t sz)
      {
         this->assign_n(tp, sz, BindBody::System_Type, BindBody::Interface_Version);
      }

      uint32_t interface_ver() { return get_u32(BindBody::Interface_Version); }
      void     set_interface_ver(uint32_t iv)
      {
         memcpy(&buffer[BindBody::Interface_Version], &iv, sizeof(uint32_t));
      }

   };

}

namespace cuap::pdu
{
   using bind_msg  = basic_bind_msg;
   using bind_resp = basic_bind_resp;

   /// Unbind don't have body, hence it's just alias of basic_header
   using unbind_msg = basic_header<uint8_t, 20>;

   /// Unbind_resp don't have body, hence it's just alias of basic_header
   using unbind_resp = unbind_msg;

   /// Shake don't have body, hence it's just alias of basic_header
   using shake_msg = unbind_msg;

   /// Shake_resp don't have body, hence it's just alias of basic_header
   using shake_resp  = unbind_msg;
}

#endif//bind_family_h
