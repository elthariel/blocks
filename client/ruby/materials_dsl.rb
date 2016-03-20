# Idea borrowed from ActiveSupport
module Concern
  def included(base = nil, &block)
    if base.nil?
      @_included_block = block
    else
      super
    end
  end

  def append_features(base)
    super
    base.extend const_get("ClassMethods") if const_defined?("ClassMethods")
    base.class_eval(&@_included_block) if instance_variable_defined?("@_included_block")
  end
end

module Materials
  module DSL
    module Base
      extend Concern

      attr_reader :data
      def initialize
        @data = self.class.defaults.dup
      end

      module ClassMethods
        def defaults
          unless self.singleton_class.class_variable_defined? :@@defaults
            self.singleton_class.class_variable_set :@@defaults, {}
          end
          self.singleton_class.class_variable_get :@@defaults
        end

        def dsl_attr name, default_value = nil
          unless default_value.nil?
            self.defaults[name] = default_value
          end

          define_method name.to_sym do |value|
            @data[name] = value
          end
        end
      end
    end

    class Root
      include Base

      def material name, &proc
        mat = Materiel.new name

        mat.instance_exec(&proc)

        @data['materials'] ||= {}
        @data['materials'] = mat.data
      end

      def block id, name, &proc
        block = Block.new id, name

        block.instance_exec(&proc)

        @data['blocks'] ||= {}
        @data['blocks'][id] = block.data
      end
    end

    class Material
      include Base

      def initialize name
        super()
        data['name'] = name
      end

      dsl_attr :alpha, false
      dsl_attr :texture
      dsl_attr :texture_mode, 'default'
      dsl_attr :scale, 1
    end

    class Block < Material
      dsl_attr :air, false

      def initialize id, name
        super name
        data['id'] = id
      end
    end
  end

  def self.define &proc
    root = DSL::Root.new
    root.instance_exec(&proc)
    root.data
  end
end
