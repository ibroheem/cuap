module buffer;

import std.stdio;
import type_alias;
import std.experimental.allocator;

@nogc @trusted struct buffer (T = ubyte, size_t SIZE)
{
   public:
      this(T[]) { }

      ref buffer append(const byte val)   { /*this += val; return this; */return memcpy_n_incr(val); }
      ref buffer append(const ushort val) { return memcpy_n_incr(val); }
      ref buffer append(const uint val)   { return memcpy_n_incr(val); }

      void  opIndexAssign(ref const T val, int i) { data[i] = val; offset += val.sizeof; }
      //void  opIndexAssign(ref T val, int i) { data[i] = val; }

      ref T opIndex(size_t i)  { return data[i]; }

      /// Returns Maximum size of the buffer
      size_t max_size() { return SIZE; }

      /// Returns the number of elements in the buffer
      size_t size()     { return offset; }

      /// Returns the number of elements in the buffer
      size_t length()   { return offset; }

      void append_values(T[] lst)
      {
         foreach (v; lst)
         memcpy_n_incr(v);
      }

      T[SIZE] data;
      size_t offset = 0;

   public:
      ref buffer memcpy_n_incr(T)(T val)
      {
         import core.stdc.string : memcpy;

         memcpy(&buffer[offset], &val, val.sizeof);
         offset += val.sizeof;
         return this;
      }
};

public
{
   void test()
   {
      buffer!(ubyte, 5) buf;
      buf[0] = 50;
      buf.append(cast(uint8_t)60);
      buf.append(cast(uint8_t)23);

      buf.append_values([7, 2]);

      for (int i = 0; i < buf.size(); ++i)
         writef("Value: %d\n", buf[i]);

      writefln("\nBuffer Element Count: %d", buf.size());
      //static_buffer!(ubyte, 16) = static_buffer();
   }

   void exec()
   {
      test();
   }
}
