require! {
  \./Serializable
  \../../../flatbuffers/js/flatbuffers : {flatbuffers}
  \../../common/generated_include/chunk_generated : {{fbs}:blocks}
}

Events = require \./Events
export Events.Events
export class Pos extends Serializable \Pos
export class PosObj extends Serializable \PosObj classes: pos: Pos
export class Block extends Serializable \Block
export class BlockPos extends Serializable \BlockPos classes: pos: Pos, block: Block
export class Chunk extends Serializable \Chunk classes: cid: Pos
export class Player extends Serializable \Player classes: pos: Pos

export class Message extends Serializable \Message, union: \body

  @types_actions =
    (fbs.Action.INITIAL_POS): fbs.AType.PosObj
    (fbs.Action.MOVE): fbs.AType.Player
    (fbs.Action.ASK_CHUNK): fbs.AType.Chunk
    (fbs.Action.CHUNK): fbs.AType.Chunk
    (fbs.Action.PLACE_BLOCK): fbs.AType.BlockPos
    (fbs.Action.BREAK_BLOCK): fbs.AType.PosObj
    (fbs.Action.UPDATE_BLOCK): fbs.AType.BlockPos
    (fbs.Action.PLAYER_CONNECT): fbs.AType.Player
    (fbs.Action.PLAYER_DISCONNECT): fbs.AType.Player


  @types_classes =
    (fbs.AType.PosObj): PosObj
    (fbs.AType.BlockPos): BlockPos
    (fbs.AType.Chunk): Chunk
    (fbs.AType.Player): Player

  Serialize: ->
    builder = new flatbuffers.Builder 0
    res = super builder
    builder.finish res
    new Buffer builder.asUint8Array!

  @Deserialize = (buffer) ->
    arr = new Uint8Array buffer
    buf = new flatbuffers.ByteBuffer arr
    msg = fbs.Message.getRootAsMessage buf
    msg = super msg

  @Create = (action, body) ->
    bodyType = @types_actions[action]
    new Message {action, bodyType, body}
