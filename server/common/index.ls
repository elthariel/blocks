require! {
  \./Serializable
  \../../../flatbuffers/js/flatbuffers : {flatbuffers}
  \../blocks_generated : {{fbs}:blocks}
}

export class Pos extends Serializable \Pos
export class PosObj extends Serializable \PosObj classes: pos: Pos
export class Block extends Serializable \Block
export class BlockPos extends Serializable \BlockPos classes: pos: Pos, block: Block
export class Chunk extends Serializable \Chunk
export class Player extends Serializable \Player classes: pos: Pos

export class Message extends Serializable \Message, union: \body

  @types_actions = [
    fbs.AType.PosObj
    fbs.AType.PosObj
  ]

  @types_classes =
    (fbs.AType.PosObj): PosObj

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
