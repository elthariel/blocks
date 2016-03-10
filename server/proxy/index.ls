require! {
  \prelude-ls
}

users = [
  login: \test password: \test
]

global import prelude-ls

# clients = new thrift.BiDirectServer do
#   receiver:
#     processor: Proxy
#     port: 9001
#   emitter:
#     processor: Client
#     port: 9002
#   handler:
#     auth: (login, pass, done) ->
#       user = find (.login is login), users
#       if user? and pass is user.password
#         return done null, true
#
#         done null, false
#
#
# mdsClient = new thrift.Client \localhost 9001
# mds = new thrift.Emitter MDS, mdsClient, (client) ->

# mds = new thrift.BiDirectClient do
#   receiver:
#     processor: Proxy
#     host: \localhost
#     port: 9002
#   emitter:
#     processor: MDS
#     host: \localhost
#     port: 9001
#   handler:
#     subscribe: (listener, watched, event) ->
#     emit: (listener, watched, event) ->
#

# # Listen to clients
# emitSocket = new thrift.Server 9002
# recvSocket = new thrift.Server 9001
#
# emitter = new thrift.Emitter Client, emitSocket, (client) ->
#   pos = new Types.Pos
#   pos <<< x: 1 y: 1 z: 1
#   client.set_position pos#, (err, res) -> console.log err, res
#
# receiver = new thrift.Receiver Proxy, recvSocket, do
#   auth: (login, pass, done) ->
#     user = find (.login is login), users
#     if user? and pass is user.password
#       return done null, true
#
#     done null, false
