require! {
  events: {EventEmitter}
  \rabbit.js : rabbit
  \../../common/generated_include/chunk_generated : {{fbs}:blocks}
  \./ : common
}

export class RPCEmitter extends EventEmitter

  @ <<< fbs.Action

  ->
    @cbs = []
    @ready = false
    @context = rabbit.createContext 'amqp://localhost'
    @context.on \ready @~_init
    @_emit = EventEmitter::emit

  _init: ->
    @req = @context.socket \REQ

    @req.on \data ~>
      msg = common.RPC.Deserialize it
      console.log msg.body._type_name
      if msg.body._type_name is \Error
        @cbs.shift! msg.body
      else
        @cbs.shift! null, msg.body

    @req.connect \rpc_queue ~>
      @ready = true
      @_emit \ready

  @_Wait = (event, cb) ->
    (...args) ->
      if not @[event] then @once event, ~> cb.apply @, args
      else cb.apply @, args

  ask: @_Wait \ready (data, cb) ->
    @cbs.push cb
    msg = common.RPC.Create data
    @req.write msg.Serialize!

export class RPCReceiver

  @ <<< fbs.Action

  (@handler) ->
    @res = []
    @ready = false
    @context = rabbit.createContext 'amqp://localhost'
    @context.on \ready @~_init

  _init: ->
    @rep = @context.socket \REP

    @rep.on \data ~>
      msg = common.RPC.Deserialize it
      if not @handler[msg.bodyType]?
        return @rep.write common.RPC.Create(new Error err: "No handler for type #{msg.bodyType}").Serialize!
      question = {msg, answer: null}
      @res.push question
      @handler[msg.bodyType].call @handler, msg.body, (err, res) ~>
        question.answer = common.RPC.Create res
        question.answer .= Serialize!
        @_SendAnswers!

    @rep.connect \rpc_queue

  _SendAnswers: ->
    i = 0
    for question in @res
      console.log 'Question ?' question
      if not question.answer?
        break
      console.log 'Write !'
      @rep.write question.answer
      i++
    @res = @res[i to]
    console.log 'After' @res

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
