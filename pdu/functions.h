#ifndef functions_h
#define functions_h

#include "types.h"

namespace misc
{
   template <int16_t N1, int16_t N2>
   bool is_equal(auto(&lhs)[N1], auto(&rhs)[N2])
   {
      bool b = true;

      for (int16_t i = 0; i < N1; ++i)
      {
         if (lhs[i] != rhs[i])
         {
            b = false;
            break;
         }
      }
      return b;
   }

   bool is_equal(concepts::uchar_t lhs, int16_t N1, concepts::uchar_t rhs)
   {
      bool b = true;

      for (int16_t i = 0; i < N1; ++i)
      {
         if (lhs[i] != rhs[i])
         {
            b = false;
            break;
         }
      }
      return b;
   }

   bool is_equal(concepts::uchar_t lhs, int16_t N1, concepts::uchar_t rhs, int16_t N2)
   {
      bool b = true;

      for (int16_t i = 0; i < N1 && i < N2; ++i)
      {
         if (lhs[i] != rhs[i])
         {
            b = false;
            break;
         }
      }
      return b;
   }

   template <class T>
   void set_null(T* data, size_t b, size_t e)
   {
      for (;b < e; ++b)
         data[b] = '\0';
   }

   template <typename T>
   void set_null(T* data, size_t SZ)
   {
      memset(data, 0, SZ);
   }

   template <typename T, size_t...N>
   void set_null(T(&...data)[N])
   {
      (memset(data, 0, N), ...);
   }

   constexpr size_t strlen(const auto *s)
   {
      size_t i = 0;
      for (; s[i]; ++i)
         ;
      return i;
   }

}

namespace cuap
{
	using namespace pdu;
   namespace functions
   {
      using namespace misc;
      using namespace pdu;

      template <typename T>
      T get_from_header(auto& hbuf, Header hfields)
      {
         T ret;
         memcpy(&ret, &hbuf[hfields], sizeof(T));
         return ret;
      }

      void get_body_str(auto& hbuf, uchar* dest, auto hfields, const size_t SZ)
      {
         memcpy(dest, &hbuf[hfields], SZ);
      }

      template <typename T>
      T get_from_body(auto& hbuf, BeginBody hfields)
      {
         T ret;
         memcpy(&ret, &hbuf[hfields], sizeof(T));
         return ret;
      }
   }

}

namespace cuap
{
	using namespace pdu;
   using concepts::uchar_t;
   /** Erases the given range in buffer, then copy values in @src to the range in @buffer
       @param b: start offset in buffer, assgnment starts here
       @param e: end offset in buffer, assgnment ends one element b4 e.
       @param val_sz: val size/len to be assigned to the buffer
   */
   void set_field(uchar_t buffer, size_t b, size_t e, auto* val, size_t src_sz)
   {
      misc::set_null(buffer, b, e);
      for (size_t i = 0; ((i < src_sz) && (b < e)); ++b, ++i)
         buffer[b] = val[i];
   }

   void set_field(uchar_t buffer, size_t b, size_t e, auto* val)
   {
      misc::set_null(buffer, b, e);
      for (size_t i = 0; val[i] && (b < e); ++b, ++i)
         buffer[b] = val[i];
   }

   /** Overload for uint8_t type, rarely needed!*/
   constexpr void set_field_u8(uchar_t buffer, uint8_t val, size_t offset) { buffer[offset] = val; }

   /** Overload for uint16_t type*/
   constexpr void set_field_u16(uchar_t buffer, uint16_t val, size_t offset){
      memcpy(&buffer[offset], &val, sizeof(uint16_t));
   }

   /** Overload for uint32_t type*/
   constexpr void set_field_u32(uchar_t buffer, uint32_t val, size_t offset){
      memcpy(&buffer[offset], &val, sizeof(uint32_t));
   }

   /// Generic function to copy string-based body fields to the destination
   /// @param dest: destination buffer, where the field data is copied
   /// @param field_len: length/size of the field
   template <class T2 = uint8_t, uint N>
   constexpr void get_field(uchar_t buffer, T2(&dest)[N], size_t offset , size_t field_len)
   {
      misc::set_null(dest);
      memcpy(dest, &buffer[offset], field_len);
   }

   constexpr void get_field(uchar_t buffer, std::string &dest, size_t offset , size_t field_len)
   {
      for (; offset < (offset + field_len); ++offset)
         dest.append(1, buffer[offset]);
   }

   /// Generic function for all integer-based body fields
   template <class T>
   constexpr T get_field(size_t offset, uchar_t buffer)
   {
      uint32_t ret = 0;
      memcpy(&ret, &buffer[offset], sizeof(T));
      return ret;
   }

   namespace header
   {
      constexpr uint32_t header_field(Header hld, const uchar_t buffer)
      {
         uint32_t ret = 0;
         memcpy(&ret, &buffer[hld], sizeof(uint32_t));
         return ret;
      }

      constexpr uint32_t command_len(uchar_t buffer)
      {
         return header_field(Header::CommandLength, buffer);
      }

      constexpr uint32_t command_id(const uchar_t buffer)
      {
         return header_field(Header::CommandID, buffer);
      }

      constexpr uint32_t command_status(uchar_t buffer)
      {
         return header_field(Header::CommandStatus, buffer);
      }

      constexpr uint32_t sender_id(concepts::uchar_t buffer)
      {
         return header_field(Header::SenderID, buffer);
      }

      constexpr uint32_t receiver_id(uchar_t buffer)
      {
         return header_field(Header::ReceiverID, buffer);
      }

      constexpr void set_command_len(uchar_t buffer, uint32_t val)
      {
         set_field_u32(buffer, val, Header::CommandLength);
      }

      constexpr void set_command_id(uchar_t buffer, uint32_t val)
      {
         set_field_u32(buffer, val, Header::CommandID);
      }

      constexpr void set_command_status(uchar_t buffer, uint32_t val)
      {
         set_field_u32(buffer, val, Header::CommandStatus);
      }

      constexpr void set_sender_id(uchar_t buffer, uint32_t val)
      {
         set_field_u32(buffer, val, Header::SenderID);
      }

      constexpr void set_receiver_id(uchar_t buffer, uint32_t val)
      {
         set_field_u32(buffer, val, Header::ReceiverID);
      }

		constexpr bool is_Bind(uchar_t buffer)       { return command_id(buffer) == 0x65; }
      constexpr bool is_UnBind(uchar_t buffer)     { return command_id(buffer) == 0x66; }

      constexpr bool is_BindResp(uchar_t buffer)   { return command_id(buffer) == 0x67; }
      constexpr bool is_UnBindResp(uchar_t buffer) { return command_id(buffer) == 0x68; }

      constexpr bool is_Begin(uchar_t buffer)      { return command_id(buffer) == 0x6f; }
      constexpr bool is_Continue(uchar_t buffer)   { return command_id(buffer) == 0x70; }

      constexpr bool is_End(uchar_t buffer)        { return command_id(buffer) == 0x71; }
      constexpr bool is_Abort(uchar_t buffer)      { return command_id(buffer) == 0x72; }

      constexpr bool is_Switch(uchar_t buffer)     { return command_id(buffer) == 0x74; }
      constexpr bool is_SwitchBegin(uchar_t buffer){ return command_id(buffer) == 0x77; }

      constexpr bool is_ChargeInd(uchar_t buffer)  { return command_id(buffer) == 0x75; }
      constexpr bool is_ChargeIndResp(uchar_t buffer)  { return command_id(buffer) == 0x76; }

      constexpr bool is_Shake(uchar_t buffer)      { return command_id(buffer) == 0x83; }
      constexpr bool is_ShakeResp(uchar_t buffer)  { return command_id(buffer) == 0x84; }

      constexpr uint8_t cuap_pdu_arr[]
              = {0x65, 0x66, 0x67, 0x68, 0x6f, 0x70, 0x71, 0x72, 0x74, 0x77, 0x75, 0x76, 0x83, 0x84};
      constexpr bool is_cuap_msg(uchar_t buffer)
		{
			uint8_t cmd = command_id(buffer);
			bool ret = false;
			for (uint8_t i = 0; i < sizeof(cuap_pdu_arr); ++i)
			{
				if (cuap_pdu_arr[i] == cmd)
				{
					ret = true;
					break;
				}
			}
			return ret;
		}
   }

   /// Body Fields
   namespace body
   {
      /// Bind body of related fields operations
      namespace bind_msg
      {
         template <class T2, uint N>
         constexpr void system_id(uchar_t buffer, T2(&dest)[N])
         {
            get_field(buffer, dest, BindBody::System_ID, 11);
         }

			template <uchar_t CT>
         void set_system_id(uchar_t buffer, CT val)
         {
            cuap::set_field(buffer, BindBody::System_ID, BindBody::Password, val, 11);
         }

         template <class T2, uint N>
         constexpr void password(uchar_t buffer, T2(&dest)[N])
         {
            cuap::get_field(buffer, dest, BindBody::Password, 9);
         }

         void set_password(uchar_t buffer, auto* val)
         {
            cuap::set_field(buffer,
                            BindBody::Password, BindBody::System_Type,
                            val, 9);
         }

         template <class T2, uint N>
         constexpr void system_type(uchar_t buffer, T2(&dest)[N])
         {
            cuap::get_field(buffer, dest, BindBody::System_Type, 13);
         }

         void set_system_type(uchar_t buffer, auto* val)
         {
            cuap::set_field(buffer,
                            BindBody::System_Type, BindBody::Interface_Version,
                            val, 13);
         }

         constexpr uint32_t interface_version(uchar_t buffer)
         {
            return cuap::get_field<uint32_t>(BindBody::Interface_Version, buffer);
         }

         constexpr void set_interface_version(uchar_t buffer, uint32_t val)
         {
            set_field_u32(buffer, val, BindBody::Interface_Version);
         }
      }

      namespace bind_resp_msg = cuap::body::bind_msg;

      /// Begin-family body (including Continue, End) fields operations
      namespace begin_msg
      {
         constexpr uint8_t ussd_version (uchar_t buffer)
         {
            return buffer[BeginBody::Ussd_Version];
         }

         constexpr uint8_t ussd_op_type(uchar_t buffer)
         {
            return buffer[BeginBody::Ussd_Op_Type];
         }

         constexpr uint8_t code_scheme(uchar_t buffer)
         {
            return buffer[BeginBody::Code_Scheme];
         }

         /// Get MSISDN from src and copy to dest
         template <class T, class T2, uint N>
         void msisdn(T* src, T2(&dest)[N])
         {
            get_field(src, dest, BeginBody::MsIsdn, 21);
         }

         /// Get MSISDN from src and copy to dest
         template <class T>
         void msisdn(T* src, std::string& dest)
         {
            get_field(src, dest, BeginBody::MsIsdn, 21);
         }

         /// Get Service Code from src and copy to dest
         template <class T, class T2, uint N>
         void service_code(T* buffer, T2(&dest)[N])
         {
            get_field(buffer, dest, BeginBody::Service_Code, 21);
         }

         template <class T, class T2, uint N>
         void ussd_content(T* buffer, T2(&dest)[N])
         {
            get_field(buffer, dest,
                      BeginBody::Ussd_Content,
                      header::command_len(buffer) - BeginBody::Ussd_Content);
         }

         template <class T>
         T* ussd_content(T* buffer)
         {
            return buffer + BeginBody::Ussd_Content;
         }

         constexpr void set_code_scheme(uchar_t buffer, uint8_t val)
         {
            buffer[BeginBody::Code_Scheme] = val;
         }

         void set_msisdn(uchar_t buffer, auto* val)
         {
            cuap::set_field(buffer,
                            BeginBody::MsIsdn, BeginBody::Service_Code,
                            val, 21);
         }

         void set_service_code(uchar_t buffer, auto* val)
         {
            cuap::set_field(buffer,
                            BeginBody::Service_Code, BeginBody::Code_Scheme,
                            val, 21);
         }

         void set_ussd_content(uchar_t buffer, auto* val)
         {
            cuap::set_field(buffer,
                            BeginBody::Ussd_Content, BeginBody::Ussd_Content + 182,
                            val, header::command_len(buffer) - BeginBody::Ussd_Content);
         }

         constexpr void set_ussd_op_type(uchar_t buffer, uint8_t val)
         {
            buffer[BeginBody::Ussd_Op_Type] = val;
         }

         constexpr void set_ussd_version(uchar_t buffer, uint8_t val)
         {
            buffer[BeginBody::Ussd_Version] = val;
         }

      }

      namespace continue_msg = body::begin_msg;
      namespace end_msg      = body::begin_msg;

      namespace switch_msg
      {
         constexpr uint8_t switchmode (uchar_t buffer)
         {
            return get_field<uint8_t>(static_cast<uint8_t>(Switch::SwitchMode), buffer);
         }

         template <class T2, uint N>
         void msisdn(auto* src, T2(&dest)[N])
         {
            get_field(src, dest, static_cast<uint8_t>(Switch::MsIsdn), 21);
         }

         template <class T2, uint N>
         void org_service_code (auto* src, T2(&dest)[N])
         {
            get_field(src, dest, static_cast<uint8_t>(Switch::Org_Service_Code ), 21);
         }

         template <class T2, uint N>
         void dest_service_code (auto* src, T2(&dest)[N])
         {
            get_field(src, dest, static_cast<uint8_t>(Switch::Dest_Service_Code), 21);
         }

         template <class T2, uint N>
         void ussd_content(auto* src, T2(&dest)[N])
         {
            get_field(src, dest,
                       static_cast<uint8_t>(BeginBody::Ussd_Content),
                       header::command_len(src) - static_cast<uint8_t>(BeginBody::Ussd_Content));
         }
      }

   }

}

#endif//functions_h
