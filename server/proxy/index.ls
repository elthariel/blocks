global import require \prelude-ls
require! {
  \../../common/generated_include/chunk_generated : {{fbs}:blocks}
  \../common : {BusProxy, Player, PlayerAuth, Error, RPC, Message}
}

export class Proxy

  ->
    @logged = {}

    @clients = new BusProxy \events \rpc_queue {}
    @servers = new BusProxy \events_server \rpc_queue_server

    @clients.on \_rpc (env) ~>
      sender = env.properties.replyTo
      if @logged[sender]?
        @servers.ask env
      else
        msg = RPC.Deserialize env.content
        if msg.bodyType is fbs.AType.PlayerAuth
          @servers.ask env, (env_) ~>
            msg = RPC.Deserialize env_.content
            if msg.bodyType is fbs.AType.Player
              @logged[sender] = msg.body
              console.log 'Logged: ' msg.body.login
            else
              console.log 'auth failed' msg

    @servers.on \_event (env) ~>
      @clients.emit env


new Proxy
