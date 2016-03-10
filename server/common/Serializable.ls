require! {
  \../../../flatbuffers/js/flatbuffers : {flatbuffers}
  \../../blocks_generated : {{fbs}:blocks}
}

module.exports = Serializable = (name, options) ->
  fbsObj = fbs[name]

  type = switch
    | fbsObj[\getRootAs + name]?  => \table
    | fbsObj[\create + name]?     => \struct
    | _                           => throw new Error "Fbs object is not a struct nor a table"

  props = keys fbsObj:: .[1 to]

  types = {0: void, 1: fbs.Player, 2: fbs.Chunk, 3: fbs.PosObj, 4: fbs.BlockPos}


  class _Serializable
    (args) ->
      if fbs.AType[name]?
        types[fbs.AType[name]] = @
      props |> map ~> @[it] = args[it]

    Serialize: (builder) ->
      it = builder
      if type is \table
        res = props
          |> map ~>
            switch
            | @[it].Serialize? => [it, @[it].Serialize builder]
            | _               =>  [it, @[it]]

        fbsObj[\start + name] it
        for prop in res
          console.log (\add + capitalize prop.0), prop.1
          fbsObj[\add + capitalize prop.0] it, prop.1
        return fbsObj[\end + name] it
      else if type is \struct
        args = props |> map ~> @[it]
        args.unshift it
        # console.log (\create + name), args
        return fbsObj[\create + name].apply fbsObj, args

    @Deserialize = (obj) ->
      console.log "Deserialize"
      serie = {}
      props |> each ->
        console.log 'options?' options
        if it is options?.union
          console.log 'UNION !' common.Message.types_classes[obj[it + \Type]!]#, obj[it] types[obj[it + \Type]!]
          serie[it] = common.Message.types_classes[obj[it + \Type]!].Deserialize obj[it] new types[obj[it + \Type]!]
          # serie[it] =  obj[it] types[obj[it + \Type]!]
        else if options?.classes?[it]?
          console.log 'Classes !' options.classes[it]
          serie[it] = options.classes[it].Deserialize obj[it]!
        else
          console.log 'normal' obj[it]
          serie[it] = obj[it]!

      new @ serie

require! {\./ : common}
