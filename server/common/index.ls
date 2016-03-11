require! {
  \./Serializable
  \flatbuffers/src/flatbuffers : {flatbuffers}
  \../../common/generated_include/chunk_generated : {{fbs}:blocks}
}

Events = require \./Events
export Events.Events
export Events.RPCReceiver
export Events.RPCEmitter
export class Pos extends Serializable \Pos
export class PosObj extends Serializable \PosObj classes: pos: Pos
export class Block extends Serializable \Block
export class BlockPos extends Serializable \BlockPos classes: pos: Pos
export class Chunk extends Serializable \Chunk vectors: {blocks: Block}, classes: cid: Pos
export class Player extends Serializable \Player classes: pos: Pos
export class PlayerAuth extends Serializable \PlayerAuth
export class BoolAnswer extends Serializable \BoolAnswer
export class Error extends Serializable \Error

export class Message extends Serializable \Message, union: \body

  @types_actions =
    (fbs.Action.MOVE): fbs.AType.Player
    (fbs.Action.ASK_CHUNK): fbs.AType.Chunk
    (fbs.Action.CHUNK): fbs.AType.Chunk
    (fbs.Action.PLACE_BLOCK): fbs.AType.BlockPos
    (fbs.Action.BREAK_BLOCK): fbs.AType.PosObj
    (fbs.Action.UPDATE_BLOCK): fbs.AType.BlockPos
    (fbs.Action.PLAYER_CONNECT): fbs.AType.Player
    (fbs.Action.PLAYER_DISCONNECT): fbs.AType.Player
    (fbs.Action.AUTH): fbs.AType.PlayerAuth

  @types_classes =
    (fbs.AType.PosObj): PosObj
    (fbs.AType.BlockPos): BlockPos
    (fbs.AType.Chunk): Chunk
    (fbs.AType.Player): Player
    (fbs.AType.PlayerAuth): PlayerAuth
    (fbs.AType.BoolAnswer): BoolAnswer
    (fbs.AType.Error): Error

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

export class RPC extends Serializable \RPC, union: \body

  Serialize: ->
    builder = new flatbuffers.Builder 0
    res = super builder
    builder.finish res
    new Buffer builder.asUint8Array!

  @Deserialize = (buffer) ->
    arr = new Uint8Array buffer
    buf = new flatbuffers.ByteBuffer arr
    msg = fbs.RPC.getRootAsRPC buf
    msg = super msg

  @Create = (body) ->
    new RPC {bodyType: body._atype, body}
