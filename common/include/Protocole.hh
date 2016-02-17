#pragma once

#include "chunk_generated.h"

namespace blocks
{
    class Protocole
    {
      public:
        static std::tuple<uint8_t *, size_t> create_message(fbs::Action action)
        {
          flatbuffers::FlatBufferBuilder builder;

          auto message = fbs::CreateMessage(builder, action);
          builder.Finish(message);
          return std::make_tuple(builder.GetBufferPointer(), builder.GetSize());
        }

        template <class T>
        static std::tuple<uint8_t *, size_t> create_message(fbs::Action action,
                                                            fbs::AType type,
                                                            T body)
        {
          flatbuffers::FlatBufferBuilder builder;

          auto message = fbs::CreateMessage(builder,
                                            action,
                                            type,
                                            body->serialize(builder).Union());
          builder.Finish(message);
          return std::make_tuple(builder.GetBufferPointer(), builder.GetSize());
        }

    };
}
