global import require \prelude-ls
require! {
  \./common : {Events, PosObj, Pos, Player, Block, Chunk}
}

blocks = for i from 0 til 35000
  new Block id: i, variant: 1, air: true, transparent: true, light: 0
chunk = new Chunk version: 0, size: 16, blocks: blocks, cid: new Pos x: 1 y: 2 z: 3

socket = new Events
  ..subscribe \foo
  ..on \data -> console.log it.toString!
  ..emit2 \foo \lol
  #
  # ..on Events.MOVE, (player) ->
  #   console.log 'MOVE' player
  #
  # ..on Events.INITIAL_POS, (pos) ->
  #   console.log 'INITIAL_POS' pos
  #
  # ..on Events.CHUNK, (chunk) ->
  #   console.log 'CHUNK' chunk
  #
  # ..emit      \foo Events.CHUNK, chunk
  # ..emit      \foo Events.MOVE, new Player id: 42, pos: new Pos x: 1 y: 2 z: 3
  # ..emit      \foo Events.INITIAL_POS, new PosObj pos: new Pos x: 1 y: 2 z: 3

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
