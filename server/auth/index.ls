global import require \prelude-ls
require! {
  \../common : {Bus, Player, Pos, PlayerAuth, BoolAnswer, Error}
  nodulator: N
}

NPlayer = N \player
  ..Create login: \test pass: \test #pos: JSON.stringify x: 1 y: 2 z: 3

export class Auth
  ->
    @bus = new Bus \events_server, \rpc_queue_server, @
      ..subscribe_rpc \auth

  (PlayerAuth._atype): (playerAuth, env, done) ->
    NPlayer.Fetch login: playerAuth.login
      .Then ~>
        if it.pass is playerAuth.pass
          it <<< pos: new Pos x: 1 y: 2 z: 3
          return done null new Player it
        done null new BoolAnswer answer: false
      .Catch ~>
        console.log 'Error ?' it
        done null new Error err: it.status

new Auth
