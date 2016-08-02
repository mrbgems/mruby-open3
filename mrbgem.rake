MRuby::Gem::Specification.new('mruby-open3') do |spec|
  spec.license = 'MIT'
  spec.authors = 'Takashi Kokubun'
  spec.add_dependency 'mruby-io'
  spec.add_dependency 'mruby-process'
  spec.add_dependency 'mruby-thread'
end
