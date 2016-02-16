// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_CHUNK_BLOCKS_FBS_H_
#define FLATBUFFERS_GENERATED_CHUNK_BLOCKS_FBS_H_

#include "flatbuffers/flatbuffers.h"


namespace blocks {
namespace fbs {

struct ChunkAction;
struct PlayerAction;
struct Pos;
struct Block;
struct Chunk;
struct Message;

enum Type {
  Type_PLAYER = 0,
  Type_CHUNK = 1,
  Type_MIN = Type_PLAYER,
  Type_MAX = Type_CHUNK
};

inline const char **EnumNamesType() {
  static const char *names[] = { "PLAYER", "CHUNK", nullptr };
  return names;
}

inline const char *EnumNameType(Type e) { return EnumNamesType()[static_cast<int>(e)]; }

enum ChunkActionEnum {
  ChunkActionEnum_ASK_CHUNK = 0,
  ChunkActionEnum_CHUNK = 1,
  ChunkActionEnum_NEW_BLOCK = 2,
  ChunkActionEnum_DELETE_BLOCK = 3,
  ChunkActionEnum_MIN = ChunkActionEnum_ASK_CHUNK,
  ChunkActionEnum_MAX = ChunkActionEnum_DELETE_BLOCK
};

inline const char **EnumNamesChunkActionEnum() {
  static const char *names[] = { "ASK_CHUNK", "CHUNK", "NEW_BLOCK", "DELETE_BLOCK", nullptr };
  return names;
}

inline const char *EnumNameChunkActionEnum(ChunkActionEnum e) { return EnumNamesChunkActionEnum()[static_cast<int>(e)]; }

enum PlayerActionEnum {
  PlayerActionEnum_CONNECT = 0,
  PlayerActionEnum_MOVE = 1,
  PlayerActionEnum_MIN = PlayerActionEnum_CONNECT,
  PlayerActionEnum_MAX = PlayerActionEnum_MOVE
};

inline const char **EnumNamesPlayerActionEnum() {
  static const char *names[] = { "CONNECT", "MOVE", nullptr };
  return names;
}

inline const char *EnumNamePlayerActionEnum(PlayerActionEnum e) { return EnumNamesPlayerActionEnum()[static_cast<int>(e)]; }

enum Action {
  Action_NONE = 0,
  Action_ChunkAction = 1,
  Action_PlayerAction = 2,
  Action_MIN = Action_NONE,
  Action_MAX = Action_PlayerAction
};

inline const char **EnumNamesAction() {
  static const char *names[] = { "NONE", "ChunkAction", "PlayerAction", nullptr };
  return names;
}

inline const char *EnumNameAction(Action e) { return EnumNamesAction()[static_cast<int>(e)]; }

inline bool VerifyAction(flatbuffers::Verifier &verifier, const void *union_obj, Action type);

enum AType {
  AType_NONE = 0,
  AType_Chunk = 1,
  AType_MIN = AType_NONE,
  AType_MAX = AType_Chunk
};

inline const char **EnumNamesAType() {
  static const char *names[] = { "NONE", "Chunk", nullptr };
  return names;
}

inline const char *EnumNameAType(AType e) { return EnumNamesAType()[static_cast<int>(e)]; }

inline bool VerifyAType(flatbuffers::Verifier &verifier, const void *union_obj, AType type);

MANUALLY_ALIGNED_STRUCT(8) Pos FLATBUFFERS_FINAL_CLASS {
 private:
  int64_t x_;
  int64_t y_;
  int64_t z_;

 public:
  Pos(int64_t _x, int64_t _y, int64_t _z)
    : x_(flatbuffers::EndianScalar(_x)), y_(flatbuffers::EndianScalar(_y)), z_(flatbuffers::EndianScalar(_z)) { }

  int64_t x() const { return flatbuffers::EndianScalar(x_); }
  void mutate_x(int64_t _x) { flatbuffers::WriteScalar(&x_, _x); }
  int64_t y() const { return flatbuffers::EndianScalar(y_); }
  void mutate_y(int64_t _y) { flatbuffers::WriteScalar(&y_, _y); }
  int64_t z() const { return flatbuffers::EndianScalar(z_); }
  void mutate_z(int64_t _z) { flatbuffers::WriteScalar(&z_, _z); }
};
STRUCT_END(Pos, 24);

MANUALLY_ALIGNED_STRUCT(2) Block FLATBUFFERS_FINAL_CLASS {
 private:
  uint16_t id_;
  uint16_t variant_;
  uint8_t air_;
  uint8_t transparent_;

 public:
  Block(uint16_t _id, uint16_t _variant, bool _air, bool _transparent)
    : id_(flatbuffers::EndianScalar(_id)), variant_(flatbuffers::EndianScalar(_variant)), air_(flatbuffers::EndianScalar(static_cast<uint8_t>(_air))), transparent_(flatbuffers::EndianScalar(static_cast<uint8_t>(_transparent))) { }

  uint16_t id() const { return flatbuffers::EndianScalar(id_); }
  void mutate_id(uint16_t _id) { flatbuffers::WriteScalar(&id_, _id); }
  uint16_t variant() const { return flatbuffers::EndianScalar(variant_); }
  void mutate_variant(uint16_t _variant) { flatbuffers::WriteScalar(&variant_, _variant); }
  bool air() const { return flatbuffers::EndianScalar(air_) != 0; }
  void mutate_air(bool _air) { flatbuffers::WriteScalar(&air_, static_cast<uint8_t>(_air)); }
  bool transparent() const { return flatbuffers::EndianScalar(transparent_) != 0; }
  void mutate_transparent(bool _transparent) { flatbuffers::WriteScalar(&transparent_, static_cast<uint8_t>(_transparent)); }
};
STRUCT_END(Block, 6);

struct ChunkAction FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_CHUNK = 4
  };
  ChunkActionEnum chunk() const { return static_cast<ChunkActionEnum>(GetField<int8_t>(VT_CHUNK, 0)); }
  bool mutate_chunk(ChunkActionEnum _chunk) { return SetField(VT_CHUNK, static_cast<int8_t>(_chunk)); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_CHUNK) &&
           verifier.EndTable();
  }
};

struct ChunkActionBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_chunk(ChunkActionEnum chunk) { fbb_.AddElement<int8_t>(ChunkAction::VT_CHUNK, static_cast<int8_t>(chunk), 0); }
  ChunkActionBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  ChunkActionBuilder &operator=(const ChunkActionBuilder &);
  flatbuffers::Offset<ChunkAction> Finish() {
    auto o = flatbuffers::Offset<ChunkAction>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<ChunkAction> CreateChunkAction(flatbuffers::FlatBufferBuilder &_fbb,
   ChunkActionEnum chunk = ChunkActionEnum_ASK_CHUNK) {
  ChunkActionBuilder builder_(_fbb);
  builder_.add_chunk(chunk);
  return builder_.Finish();
}

struct PlayerAction FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_PLAYER = 4
  };
  PlayerActionEnum player() const { return static_cast<PlayerActionEnum>(GetField<int8_t>(VT_PLAYER, 0)); }
  bool mutate_player(PlayerActionEnum _player) { return SetField(VT_PLAYER, static_cast<int8_t>(_player)); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_PLAYER) &&
           verifier.EndTable();
  }
};

struct PlayerActionBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_player(PlayerActionEnum player) { fbb_.AddElement<int8_t>(PlayerAction::VT_PLAYER, static_cast<int8_t>(player), 0); }
  PlayerActionBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  PlayerActionBuilder &operator=(const PlayerActionBuilder &);
  flatbuffers::Offset<PlayerAction> Finish() {
    auto o = flatbuffers::Offset<PlayerAction>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<PlayerAction> CreatePlayerAction(flatbuffers::FlatBufferBuilder &_fbb,
   PlayerActionEnum player = PlayerActionEnum_CONNECT) {
  PlayerActionBuilder builder_(_fbb);
  builder_.add_player(player);
  return builder_.Finish();
}

struct Chunk FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_VERSION = 4,
    VT_SIZE = 6,
    VT_CID = 8,
    VT_BLOCKS = 10
  };
  uint8_t version() const { return GetField<uint8_t>(VT_VERSION, 0); }
  bool mutate_version(uint8_t _version) { return SetField(VT_VERSION, _version); }
  uint8_t size() const { return GetField<uint8_t>(VT_SIZE, 0); }
  bool mutate_size(uint8_t _size) { return SetField(VT_SIZE, _size); }
  const Pos *cid() const { return GetStruct<const Pos *>(VT_CID); }
  Pos *mutable_cid() { return GetStruct<Pos *>(VT_CID); }
  const flatbuffers::Vector<const Block *> *blocks() const { return GetPointer<const flatbuffers::Vector<const Block *> *>(VT_BLOCKS); }
  flatbuffers::Vector<const Block *> *mutable_blocks() { return GetPointer<flatbuffers::Vector<const Block *> *>(VT_BLOCKS); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_VERSION) &&
           VerifyField<uint8_t>(verifier, VT_SIZE) &&
           VerifyFieldRequired<Pos>(verifier, VT_CID) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_BLOCKS) &&
           verifier.Verify(blocks()) &&
           verifier.EndTable();
  }
};

struct ChunkBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_version(uint8_t version) { fbb_.AddElement<uint8_t>(Chunk::VT_VERSION, version, 0); }
  void add_size(uint8_t size) { fbb_.AddElement<uint8_t>(Chunk::VT_SIZE, size, 0); }
  void add_cid(const Pos *cid) { fbb_.AddStruct(Chunk::VT_CID, cid); }
  void add_blocks(flatbuffers::Offset<flatbuffers::Vector<const Block *>> blocks) { fbb_.AddOffset(Chunk::VT_BLOCKS, blocks); }
  ChunkBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  ChunkBuilder &operator=(const ChunkBuilder &);
  flatbuffers::Offset<Chunk> Finish() {
    auto o = flatbuffers::Offset<Chunk>(fbb_.EndTable(start_, 4));
    fbb_.Required(o, Chunk::VT_CID);  // cid
    return o;
  }
};

inline flatbuffers::Offset<Chunk> CreateChunk(flatbuffers::FlatBufferBuilder &_fbb,
   uint8_t version = 0,
   uint8_t size = 0,
   const Pos *cid = 0,
   flatbuffers::Offset<flatbuffers::Vector<const Block *>> blocks = 0) {
  ChunkBuilder builder_(_fbb);
  builder_.add_blocks(blocks);
  builder_.add_cid(cid);
  builder_.add_size(size);
  builder_.add_version(version);
  return builder_.Finish();
}

struct Message FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_TYPE = 6,
    VT_ACTION_TYPE = 8,
    VT_ACTION = 10,
    VT_BODY_TYPE = 12,
    VT_BODY = 14
  };
  int64_t id() const { return GetField<int64_t>(VT_ID, 0); }
  bool mutate_id(int64_t _id) { return SetField(VT_ID, _id); }
  Type type() const { return static_cast<Type>(GetField<int8_t>(VT_TYPE, 0)); }
  bool mutate_type(Type _type) { return SetField(VT_TYPE, static_cast<int8_t>(_type)); }
  Action action_type() const { return static_cast<Action>(GetField<uint8_t>(VT_ACTION_TYPE, 0)); }
  bool mutate_action_type(Action _action_type) { return SetField(VT_ACTION_TYPE, static_cast<uint8_t>(_action_type)); }
  const void *action() const { return GetPointer<const void *>(VT_ACTION); }
  void *mutable_action() { return GetPointer<void *>(VT_ACTION); }
  AType body_type() const { return static_cast<AType>(GetField<uint8_t>(VT_BODY_TYPE, 0)); }
  bool mutate_body_type(AType _body_type) { return SetField(VT_BODY_TYPE, static_cast<uint8_t>(_body_type)); }
  const void *body() const { return GetPointer<const void *>(VT_BODY); }
  void *mutable_body() { return GetPointer<void *>(VT_BODY); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int64_t>(verifier, VT_ID) &&
           VerifyField<int8_t>(verifier, VT_TYPE) &&
           VerifyField<uint8_t>(verifier, VT_ACTION_TYPE) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_ACTION) &&
           VerifyAction(verifier, action(), action_type()) &&
           VerifyField<uint8_t>(verifier, VT_BODY_TYPE) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_BODY) &&
           VerifyAType(verifier, body(), body_type()) &&
           verifier.EndTable();
  }
};

struct MessageBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int64_t id) { fbb_.AddElement<int64_t>(Message::VT_ID, id, 0); }
  void add_type(Type type) { fbb_.AddElement<int8_t>(Message::VT_TYPE, static_cast<int8_t>(type), 0); }
  void add_action_type(Action action_type) { fbb_.AddElement<uint8_t>(Message::VT_ACTION_TYPE, static_cast<uint8_t>(action_type), 0); }
  void add_action(flatbuffers::Offset<void> action) { fbb_.AddOffset(Message::VT_ACTION, action); }
  void add_body_type(AType body_type) { fbb_.AddElement<uint8_t>(Message::VT_BODY_TYPE, static_cast<uint8_t>(body_type), 0); }
  void add_body(flatbuffers::Offset<void> body) { fbb_.AddOffset(Message::VT_BODY, body); }
  MessageBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  MessageBuilder &operator=(const MessageBuilder &);
  flatbuffers::Offset<Message> Finish() {
    auto o = flatbuffers::Offset<Message>(fbb_.EndTable(start_, 6));
    return o;
  }
};

inline flatbuffers::Offset<Message> CreateMessage(flatbuffers::FlatBufferBuilder &_fbb,
   int64_t id = 0,
   Type type = Type_PLAYER,
   Action action_type = Action_NONE,
   flatbuffers::Offset<void> action = 0,
   AType body_type = AType_NONE,
   flatbuffers::Offset<void> body = 0) {
  MessageBuilder builder_(_fbb);
  builder_.add_id(id);
  builder_.add_body(body);
  builder_.add_action(action);
  builder_.add_body_type(body_type);
  builder_.add_action_type(action_type);
  builder_.add_type(type);
  return builder_.Finish();
}

inline bool VerifyAction(flatbuffers::Verifier &verifier, const void *union_obj, Action type) {
  switch (type) {
    case Action_NONE: return true;
    case Action_ChunkAction: return verifier.VerifyTable(reinterpret_cast<const ChunkAction *>(union_obj));
    case Action_PlayerAction: return verifier.VerifyTable(reinterpret_cast<const PlayerAction *>(union_obj));
    default: return false;
  }
}

inline bool VerifyAType(flatbuffers::Verifier &verifier, const void *union_obj, AType type) {
  switch (type) {
    case AType_NONE: return true;
    case AType_Chunk: return verifier.VerifyTable(reinterpret_cast<const Chunk *>(union_obj));
    default: return false;
  }
}

inline const blocks::fbs::Message *GetMessage(const void *buf) { return flatbuffers::GetRoot<blocks::fbs::Message>(buf); }

inline Message *GetMutableMessage(void *buf) { return flatbuffers::GetMutableRoot<Message>(buf); }

inline bool VerifyMessageBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<blocks::fbs::Message>(); }

inline void FinishMessageBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<blocks::fbs::Message> root) { fbb.Finish(root); }

}  // namespace fbs
}  // namespace blocks

#endif  // FLATBUFFERS_GENERATED_CHUNK_BLOCKS_FBS_H_
