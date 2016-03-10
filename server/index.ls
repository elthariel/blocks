require! {
  \prelude-ls
  \rabbit.js : rabbit
  events: {EventEmitter}
  \../flatbuffers/js/flatbuffers : {flatbuffers}
  \./blocks_generated : {{fbs}:blocks}
  \./server/common
}

class Socket extends EventEmitter
  (@eventQueue = \events) ->
    @waitingSubs = []
    @ready = false
    @inited = false
    @drain = false
    @context = rabbit.createContext 'amqp://localhost'
    @context.on \ready @~Init
    @_emit = EventEmitter::emit
    @On = EventEmitter::on

  Init: ->
    @pub = @context.socket \PUB routing: \topic
    @sub = @context.socket \SUB routing: \topic

    @sub.on \data ~>
      msg = common.Message.Deserialize it
      @_emit msg.action, msg.body
      #emit event

    @On \drain ~>
      @pub.connect @eventQueue, ~>
        @ready = true
        @_emit \ready

    @inited = true
    @_emit \inited

  @_Wait = (event, cb) ->
    (...args) ->
      if not @[event] then @once event, ~> cb.apply @, args
      else cb.apply @, args

  Subscribe: @_Wait \inited (topic) ->
    @drain = false
    @waitingSubs.push topic
    @sub.connect @eventQueue, topic, ~>
      @waitingSubs.splice (find-index (is topic), @waitingSubs)
      if not @waitingSubs.length
        @drain = true
        @_emit \drain

  Emit: @_Wait \ready @_Wait \drain (topic, event, obj) ->
    msg = common.Message.Create event, obj
    serie = msg.Serialize!
    @pub.publish topic, serie

socket = new Socket
  # ..On        \data -> console.log it.toString!
  ..Subscribe \foo
  ..On fbs.Action.MOVE, (pos) ->
    console.log 'MOOVE' pos
  ..Emit      \foo fbs.Action.MOVE, new common.PosObj pos: new common.Pos x: 1 y: 2 z: 3

# world.players
# world.players.[ID]
# world.chunks
# world.chunks.[XYZ]
#
# client RPC        world.players       AUTH             [\test, \test] => [1 \test \test pos]
# client subscribe  world.players.1
# auth   send       world.players       PLAYER_CONNECTED [1 \test pos]
# chunk  emit       world.players.1     CHUNK            [...]
# chunk  emit       world.players.1     INVENTORY        [...]
# client receive    world.players.1     CHUNK            [...]
# client subscribe  world.chunks.[XYZ]
# client receive    world.chunks.[XYZ]  PLAYER_MOVE      [...]
# client receive    world.chunks.[XYZ]  UPDATE_BLOCK     [...]
#
# global import prelude-ls
#
# # class BiDirect
# #   () ->
# #
# # class BiDirectClient extends BiDirect
# # class BiDirectServer extends BiDirect
# #
# # Client
# clientSocketEmit = new thrift.Client \localhost 9001
# clientEmitter = new thrift.Emitter Server, clientSocketEmit, (client) ->
#   client.auth \test \test (err, res) -> console.log err, res
#
# # Server
# serverSocketRecv = new thrift.Server 9001
#
# users = [
#   login: \test password: \test
# ]
#
# handler =
#   auth: (login, pass, done) ->
#     user = find (.login is login), users
#     if user? and pass is user.password
#       return done null, true
#
#     done null, false
#
# serverReceiver = new thrift.Receiver Server, serverSocketRecv, handler
