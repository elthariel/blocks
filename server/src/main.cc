#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

#include "Server.hh"
// #include "TcpConnection.hh"

#include "chunk_generated.h"

// class Serializable
// {
//
// public:
//     uint8_t *serialize() {
//
//     }
//
//     void deserialize(uint8_t *buff) {
//
//     }
//
// private:
//     std::map<char *, >
// };
//
// template <typename T>
// class Test
// {
//     void T() {std::cout << "test" << std::endl;}
// };

int main()
{

    // Test<> test;

  // flatbuffers::FlatBufferBuilder builder;
  //
  // auto pos = blocks::fbs::Pos(0, 0, 0);
  // std::vector<blocks::fbs::Block> blocks_vector;
  // blocks::fbs::Block block1(2, 1, false, false);
  // blocks::fbs::Block block2(1, 1, false, false);
  // blocks_vector.push_back(block1);
  // blocks_vector.push_back(block2);
  // auto blocks = builder.CreateVectorOfStructs(blocks_vector);
  // auto chunk = blocks::fbs::CreateChunk(builder, 10, 1, &pos, blocks);
  // builder.Finish(chunk);
  //
  // auto buff = builder.GetBufferPointer();
  //
  // auto test = blocks::fbs::GetChunk(buff);
  //
  // auto test2 = static_cast<const <> *>(test);

  // std::cout << test2->get_version() << std::endl;
  // test2->test();
  // test2->test2();

  blocks::Server server;

  return 0;
}
