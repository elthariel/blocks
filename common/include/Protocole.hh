#pragma once

#include "chunk_generated.h"

namespace blocks
{
    class Protocole
    {
      public:
        typedef std::tuple<uint8_t *, size_t> Message;

        template <class T>
        static Message create_message(std::string sender, fbs::Action action,
                                      fbs::AType type,
                                      T body)
        {
          flatbuffers::FlatBufferBuilder *builder = new flatbuffers::FlatBufferBuilder();
          // auto sender_id = builder->CreateString(sender);
          auto message = fbs::CreateMessage(*builder,
                                            atoi(sender.c_str()),
                                            action,
                                            type,
                                            body->serialize(*builder).Union());

          std::cout << "Create message " << atoi(sender.c_str()) << " " << action << " " << type << std::endl;
          builder->Finish(message);
          return std::make_tuple(builder->GetBufferPointer(), builder->GetSize());
        }

        template <class T>
        static Message create_rpc(fbs::AType type,
                                  T body)
        {
          flatbuffers::FlatBufferBuilder *builder = new flatbuffers::FlatBufferBuilder();

          auto message = fbs::CreateRPC(*builder,
                                        type,
                                        body->serialize(*builder).Union());

          std::cout << "Create RPC " << type << std::endl;
          builder->Finish(message);
          return std::make_tuple(builder->GetBufferPointer(), builder->GetSize());
        }

    };
}
