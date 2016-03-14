global import require \prelude-ls
require! {
  \./common : {Events, RPCReceiver, RPCEmitter, PosObj, Pos, Player, Block, Chunk, PlayerAuth, BoolAnswer, Error}
  nodulator: N
}

NPlayer = N \player
  ..Create login: \test pass: \test #pos: JSON.stringify x: 1 y: 2 z: 3

class Auth
  ->
    @rpc = new RPCReceiver @

  (PlayerAuth._atype): (playerAuth, done) ->
    console.log 'Auth rpc !' playerAuth
    NPlayer.Fetch login: playerAuth.login
      .Then ~>
        if it.pass is playerAuth.pass
          it <<< pos: new Pos x: 1 y: 2 z: 3
          return done null new Player it
        done null new BoolAnswer answer: false
      .Catch ~>
        done null new Error err: it.status

class Game
  ->
    # @socket = new Events

class ServerTest
  ->
    @socket = new Events
      ..subscribe \auth
      ..on Events.AUTH, ~>
        console.log \AUTH it
        @socket.emit \world.players.1, Events.AUTH, new BoolAnswer answer: true

      # ..emit      \test Events.MOVE, new Player id: 42, login: \tonton pos: new Pos x: 1 y: 2 z: 3

    # @rpc = new RPCEmitter
    # @rpc.ask new PlayerAuth(login: \test pass: \test), (err, res) ~>
    #   console.log "Client AUTH answer" err, res

new Auth
# new Game
new ServerTest

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
