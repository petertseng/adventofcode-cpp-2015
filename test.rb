require_relative '../adventofcode-common/test'

test_and_exit { |daypad|
  cpp_file = Dir.glob("#{__dir__}/#{daypad}*.cpp")
  next if cpp_file.empty?
  raise "Need exactly one file not #{cpp_file}" if cpp_file.size != 1
  "#{__dir__}/out/#{File.basename(cpp_file[0], '.cpp')}"
}
