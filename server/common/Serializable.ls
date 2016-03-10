require! {
  \../../../flatbuffers/js/flatbuffers : {flatbuffers}
  \../../common/generated_include/chunk_generated : {{fbs}:blocks}
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

    @_fbs_type = type
    _fbs_type: type

    (args) ->
      if fbs.AType[name]?
        types[fbs.AType[name]] = @
      props |> map ~> @[it] = args[it]

    Serialize: (builder) ->
      it = builder
      if type is \table
        res = props
          |> map ~>
            | @[it].Serialize? and @[it]._fbs_type is \table => [it, @[it].Serialize builder]
            | @[it].Serialize? and @[it]._fbs_type is \struct => [it, @[it]~Serialize]
            | _               =>  [it, @[it]]

        fbsObj[\start + name] it
        for prop in res
          if is-type \Function prop.1
            prop.1 = prop.1 it
          fbsObj[\add + capitalize prop.0] it, prop.1
        fbsObj[\end + name] it
      else if type is \struct
        args = props |> map ~> @[it]
        args.unshift it
        fbsObj[\create + name].apply fbsObj, args

    @Deserialize = (obj) ->
      serie = {}
      props |> each ->
        if it is options?.union
          serie[it] = common.Message.types_classes[obj[it + \Type]!].Deserialize obj[it] new types[obj[it + \Type]!]
        else if options?.classes?[it]?
          serie[it] = options.classes[it].Deserialize obj[it]!
        else
          serie[it] = obj[it]!

      new @ serie

require! {\./ : common}
