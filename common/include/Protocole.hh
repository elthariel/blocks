#pragma once

#include "chunk_generated.h"

namespace blocks
{
    class Protocole
    {
      public:
        typedef std::tuple<uint8_t *, size_t> Message;

        template <class T>
        static Message create_message(std::string routing, fbs::Action action, T body)
        {
          flatbuffers::FlatBufferBuilder *builder = new flatbuffers::FlatBufferBuilder();
          auto type = body->atype();
          auto routing_ = builder->CreateString(routing);
          auto message = fbs::CreateMessage(*builder,
                                            routing_,
                                            action,
                                            type,
                                            body->serialize(*builder).Union());

          // std::cout << "Create message " << routing << " " << type << " " << type << std::endl;
          builder->Finish(message);
          return std::make_tuple(builder->GetBufferPointer(), builder->GetSize());
        }

        template <class T>
        static Message create_rpc(T body)
        {
          flatbuffers::FlatBufferBuilder *builder = new flatbuffers::FlatBufferBuilder();

          auto type = body->atype();
          auto message = fbs::CreateRPC(*builder,
                                        type,
                                        body->serialize(*builder).Union());

          std::cout << "Create RPC " << type << std::endl;
          builder->Finish(message);
          return std::make_tuple(builder->GetBufferPointer(), builder->GetSize());
        }

    };
}
