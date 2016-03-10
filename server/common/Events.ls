require! {
  events: {EventEmitter}
  \rabbit.js : rabbit
  \../../common/generated_include/chunk_generated : {{fbs}:blocks}
  \./ : common
}

export class Events extends EventEmitter

  @ <<< fbs.Action

  (@eventQueue = \events) ->
    @waitingSubs = []
    @ready = false
    @inited = false
    @drain = false
    @context = rabbit.createContext 'amqp://localhost'
    @context.on \ready @~_init
    @_emit = EventEmitter::emit

  _init: ->
    @pub = @context.socket \PUB routing: \topic
    @sub = @context.socket \SUB routing: \topic

    @sub.on \data ~>
      msg = common.Message.Deserialize it
      @_emit msg.action, msg.body

    @on \drain ~>
      @pub.connect @eventQueue, ~>
        @ready = true
        @_emit \ready

    @inited = true
    @_emit \inited

  @_Wait = (event, cb) ->
    (...args) ->
      if not @[event] then @once event, ~> cb.apply @, args
      else cb.apply @, args

  subscribe: @_Wait \inited (topic) ->
    @drain = false
    @waitingSubs.push topic
    @sub.connect @eventQueue, topic, ~>
      @waitingSubs.splice (find-index (is topic), @waitingSubs)
      if not @waitingSubs.length
        @drain = true
        @_emit \drain

  emit: @_Wait \ready @_Wait \drain (topic, event, obj) ->
    msg = common.Message.Create event, obj
    serie = msg.Serialize!
    @pub.publish topic, serie
