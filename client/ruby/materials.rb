# Definitions of blocks textures for

puts "Loading materials definition"

puts Materials.define do
  block 0, 'Void' do
  end

  block 1, 'Air' do
    air 1
  end

  block 2, 'Dirt' do
    texture 'blocks/dirt.png'
  end
end
