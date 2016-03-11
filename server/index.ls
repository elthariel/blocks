global import require \prelude-ls
require! {
  \./common : {Events, RPCReceiver, RPCEmitter, PosObj, Pos, Player, Block, Chunk, PlayerAuth}
  nodulator: N
}

NPlayer = N \player

class Auth
  ->
    @rpc = new RPCReceiver do
      (RPCReceiver.AUTH): @~Auth

  Auth: (player, done) ->
    NPlayer.Fetch login: player.login
      .Then ~>
        if it.pass is player.pass
          return done null true
        done null false
      .Catch ~>
        done null false

class Game
  ->
    @socket = new Events

class Client
  ->
    @socket = new Events
    @rpc = new RPCEmitter
    @rpc.ask RPCEmitter.AUTH, new PlayerAuth(login: \test pass: \test), (err, res) ~>
      console.log "Client AUTH answer" err, res

new Auth
new Game
new Client

# block = new Block id: 1 variant: 0, air: true, transparent: true, light: 0
#
# socket = new Events
#   ..subscribe \foo
#
#   ..on Events.MOVE, (player) ->
#     console.log 'MOVE' player
#
#   ..on Events.INITIAL_POS, (pos) ->
#     console.log 'INITIAL_POS' pos
#
#   ..on Events.CHUNK, (chunk) ->
#     console.log 'CHUNK' chunk
#
#   # ..emit      \foo Events.MOVE, new Chunk version: 0, size: 16, blocks: [block], cid: new Pos x: 1 y: 2 z: 3
#   ..emit      \foo Events.MOVE, new Player id: 42, pos: new Pos x: 1 y: 2 z: 3
#   ..emit      \foo Events.INITIAL_POS, new PosObj pos: new Pos x: 1 y: 2 z: 3

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
