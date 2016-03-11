require! {
  \flatbuffers/src/flatbuffers : {flatbuffers}
  \../../common/generated_include/chunk_generated : {{fbs}:blocks}
}

module.exports = Serializable = (name, options) ->
  fbsObj = fbs[name]

  type = switch
    | fbsObj[\getRootAs + name]?  => \table
    | fbsObj[\create + name]?     => \struct
    | _                           => throw new Error "Fbs object is not a struct nor a table"

  props = keys fbsObj:: .[1 to]

  types = map (fbs.), (keys fbs.AType)
  class _Serializable

    @_fbs_type = type
    @_type_name = name
    @_type = fbs[name]
    @_atype = fbs.AType[name]

    _fbs_type: type
    _type_name: name
    _type: fbs[name]
    _atype: fbs.AType[name]

    (args) ->
      if fbs.AType[name]?
        types[fbs.AType[name]] = @
      props |> map ~> @[it] = args[it]

    Serialize: (builder) ->
      it = builder
      if type is \table
        res = props
          |> filter ~> @[it]?
          |> map ~>
            | is-type \String @[it] => [it, builder.createString @[it]]
            | is-type \Array @[it] =>
              a = (@[it] |> map (.Serialize builder))
              o = fbsObj[\create + capitalize(it) + \Vector] builder, a
              [it, o]
            | @[it].Serialize? and @[it]._fbs_type is \table => [it, @[it].Serialize builder]
            | @[it].Serialize? and @[it]._fbs_type is \struct => [it, @[it]~Serialize]
            | _                                               =>  [it, @[it]]

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
      console.log 'Deserialize' name
      props |> each ->
        if it is options?.union
          console.log it, obj[it + \Type]!
          serie[it] = common.Message.types_classes[obj[it + \Type]!].Deserialize obj[it] new types[obj[it + \Type]!]
        else if options?.classes?[it]?
          serie[it] = options.classes[it].Deserialize obj[it]!
        else
          serie[it] = obj[it]!

        if obj[it + \Length]?
          len = obj[it + \Length]!
          serie[it] = []
          for i from 0 til len
            serie[it].push options.vectors[it].Deserialize obj[it] i
        console.log serie[it]

      new @ serie

require! {\./ : common}
