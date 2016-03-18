global import require \prelude-ls
require! {
  \../common/generated_include/chunk_generated : {{fbs}:blocks}
  \./common : {Bus, PosObj, Pos, Player, Block, Chunk, PlayerAuth, BoolAnswer, Error, RPC, Message}
  nodulator: N
}

NPlayer = N \player
  ..Create login: \test pass: \test #pos: JSON.stringify x: 1 y: 2 z: 3

class Auth
  ->
    @bus = new Bus \events, \rpc_queue, @
      ..subscribe_rpc \auth

  (PlayerAuth._atype): (playerAuth, env, done) ->
    console.log 'Auth rpc !' playerAuth
    NPlayer.Fetch login: playerAuth.login
      .Then ~>
        if it.pass is playerAuth.pass
          it <<< pos: new Pos x: 1 y: 2 z: 3
          return done null new Player it
        done null new BoolAnswer answer: false
      .Catch ~>
        done null new Error err: it.status

new Auth

# class Auth
#   ->
#     @rpc = new RPCReceiver @, \rpc_queue_server
#
#   (PlayerAuth._atype): (playerAuth, done) ->
#     console.log 'Auth rpc !' playerAuth
#     NPlayer.Fetch login: playerAuth.login
#       .Then ~>
#         if it.pass is playerAuth.pass
#           it <<< pos: new Pos x: 1 y: 2 z: 3
#           return done null new Player it
#         done null new BoolAnswer answer: false
#       .Catch ~>
#         done null new Error err: it.status
#
#
# class Proxy
#   ->
#     @waitingLogin = {}
#     @logged = {}
#     @clients = new Events \string
#     @servers = new Events \string \events_server
#     @rpcProxy = new RPCProxy
#     @rpcProxy.req.on \data ~>
#       msg = RPC.Deserialize it
#       if msg.bodyType is fbs.AType.Player
#         @logged[@waitingLogin[msg.body.login]] = msg.body
#         console.log 'Logged: ' msg.body.login, @waitingLogin[msg.body.login]
#         delete @waitingLogin[msg.body.login]
#       @rpcProxy.rep.write it
#
#     @rpcProxy.rep.on \data ~>
#       msg = RPC.Deserialize it
#       sender = @rpcProxy.rep.requests.0.properties.replyTo
#
#       if msg.bodyType is fbs.AType.PlayerAuth
#         @waitingLogin[msg.body.login] = sender
#         @rpcProxy.req.write it
#       else if @logged[sender]?
#         @rpcProxy.req.write it
#
#     @servers.subscribe \world.players.*
#     @servers.on \data ~>
#       msg = Message.Deserialize it
#       @clients.emit msg.routing, it
#
#     @clients.subscribe \world.chunks
#     @clients.on \data ~>
#       msg = Message.Deserialize it
#       @servers.emit msg.routing, it
#
# new Proxy
# new Auth
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
