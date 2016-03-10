require! {
  \prelude-ls
}

global import prelude-ls

# users = [
#   login: \test password: \test
# ]

# class MDSHandler extends EventEmitter
#   ->
#     @watched = {}
#
#   subscribe: (listener, watched, event, done) ->
#     resource = @watched[watched]
#     if not resource?
#       @watched[watched] = []
#       resource = @watched[watched]
#
#     resource.push {listener, event}
#     done!
#
#   emit: (watched, event, data, done) ->
#     @watched[watched]
#     super.call @, event, data
#     done!
#
# mdsServer = new thrift.Server 9001
# mds = new thrift.Receiver MDS, mdsServer, do
#   subscribe: (listener, watched, event) ->
#
#   emit: (watched, event) ->
