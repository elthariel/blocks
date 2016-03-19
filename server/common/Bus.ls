require! {
  events: {EventEmitter}
  \amqplib/channel_api : amqp
  \../../common/generated_include/chunk_generated : {{fbs}:blocks}
  \./ : common
}

export class Bus extends EventEmitter

  (@eventQueue = \events, @rpcQueue = \rpc_queue, @rpcHandler) ->
    @rpcAnswers = {}
    @correlationId = 0
    @eventReady = false
    @rpcReady = false
    @_emit = EventEmitter::emit
    amqp.connect!
      .then (conn) ~>
        conn.createChannel!then (@eventChannel) ~> @_declare_event!
        if @rpcHandler?
          conn.createChannel!then (@rpcChannel) ~> @_declare_rpc!

  _declare_event: ->
    @eventChannel.assertExchange @eventQueue, \topic, durable: false
    @eventChannel.assertQueue '', exclusive: true
      .then ~>
        @eventPrivateQueue = it.queue
        @eventReady = true
        @_emit \eventReady
        @eventChannel.consume @eventPrivateQueue, @~_receive_event, {noAck: true}

  _declare_rpc: ->
    @rpcChannel.assertExchange @rpcQueue, \topic, durable: false
    @rpcChannel.assertQueue '', exclusive: true
      .then ~>
        @rpcPrivateQueue = it.queue
        @rpcReady = true
        @_emit \rpcReady
        @rpcChannel.consume @rpcPrivateQueue, @~_receive_rpc, {noAck: true}

  _receive_event: ->
    if it.properties.correlationId.length
      @rpcAnswers[it.properties.correlationId] common.RPC.Deserialize it.content
      delete @rpcAnswers[it.properties.correlationId]
    else
      msg = common.Message.Deserialize it.content
      @_emit msg.action, msg.body, it
      @_emit \_event, msg, it

  _receive_rpc: (env) ->
    msg = common.RPC.Deserialize env.content
    if not @rpcHandler[msg.bodyType]?
      return @eventChannel.sendToQueue env.properties.replyTo, common.RPC.Create(new Error err: "No handler for type #{msg.bodyType}").Serialize!, do
        deliveryMode: true
        expiration: 3600
        correlationId: env.properties.correlationId

    @_emit \_rpc, msg, env
    @rpcHandler[msg.bodyType].call @rpcHandler, msg.body, env, (err, res) ~>
      answer = common.RPC.Create res .Serialize!
      @eventChannel.sendToQueue env.properties.replyTo, answer, do
        deliveryMode: true
        expiration: 3600
        correlationId: env.properties.correlationId

  @_Wait = (event, cb) ->
    (...args) ->
      if not @[event] then @once event, ~> cb.apply @, args
      else cb.apply @, args

  subscribe_events: @_Wait \eventReady (topic) ->
    @eventChannel.bindQueue @eventPrivateQueue, @eventQueue, topic

  subscribe_rpc: @_Wait \rpcReady (topic) ->
    @rpcChannel.bindQueue @rpcPrivateQueue, @rpcQueue, topic

  emit: @_Wait \eventReady (topic, event, obj) ->
    msg = common.Message.Create event, obj
    @eventChannel.publish @eventQueue, topic, msg.Serialize!

  ask: @_Wait \eventReady (topic, data, cb) ->
    msg = common.RPC.Create data
    @eventChannel.publish @rpcQueue, topic, msg.Serialize!, do
      replyTo: @eventPrivateQueue
      correlationId: @correlationId + ''
    @rpcAnswers[@correlationId++] = cb

export class BusProxy extends Bus

  ->
    super ...
    @subscribe_events \*
    @subscribe_rpc \* if @rpcHandler?

  _receive_event: ->
    if it.properties.correlationId?.length
      @rpcAnswers[+it.properties.correlationId] it
      delete @rpcAnswers[+it.properties.correlationId]
    else
      @_emit \_event, it

  _receive_rpc: (env) ->
    @_emit \_rpc, env

  emit: @_Wait \eventReady (env) ->
    @eventChannel.publish @eventQueue, env.fields.routingKey, env.content

  ask: @_Wait \eventReady (env_, cb) ->
    env = {content: env_.content}
    env.properties = {} <<< env_.properties
    env.fields = {} <<< env_.fields
    if cb?
      env.properties.replyTo = @eventPrivateQueue
      env.properties.correlationId = @correlationId + ''
      @rpcAnswers[@correlationId++] = (recvEnv) ~>
        cb recvEnv
        @eventChannel.sendToQueue env_.properties.replyTo, recvEnv.content, do
          deliveryMode: true
          expiration: 3600
          correlationId: env_.properties.correlationId


    @eventChannel.publish @rpcQueue, env.fields.routingKey, env.content, do
      replyTo: env.properties.replyTo
      correlationId: env.properties.correlationId
