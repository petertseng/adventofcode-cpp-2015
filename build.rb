day_libs = {
  4 => 'md5.cpp',
}.each_value(&:freeze).freeze
day_stds = {
  # variant
  7 => 'c++17',
}.each_value(&:freeze).freeze

stds = %w(c++11 c++14 c++17).map(&:freeze).freeze

multistd = ARGV.delete('--multistd')
days = ARGV.empty? ? 1..25 : ARGV.map(&method(:Integer))

succeeded = []
failed_any = []
failed_all = []

width = 9

header = '  ' + stds.map { |std| " %#{width}s" % std }.join
puts header if multistd

lines = days.map { |day|
  daypad = '%02d' % day

  srcs = Dir.glob("#{daypad}_*.cpp")
  if srcs.empty?
    puts "#{day} not implemented"
    next
  end
  raise "too many #{day} #{srcs}" if srcs.size > 1

  stds_to_use = multistd ? stds : (ds = day_stds[day]) ? [ds] : [nil]

  system('mkdir -p out')
  outname = File.basename(srcs[0], '.cpp')

  failed_day = []
  succeeded_any = false

  s = daypad + stds_to_use.map { |std|
    std_flag = std && "-std=#{std}"
    start = Time.now
    if system("g++ -Wall -Wextra -Wpedantic -O2 #{std_flag} #{day_libs[day]} #{srcs[0]} -o out/#{outname}")
      succeeded_any = true
      # subtract 1, the s
      " \e[1;32m%#{width - 1}.5fs\e[0m" % (Time.now - start)
    else
      failed_day << "#{day}-#{std}"
      " \e[1;31m%#{width}s\e[0m" % 'fail'
    end
  }.join

  s << " (#{stds_to_use.join(' ')})" if !multistd && !stds_to_use.compact.empty?

  if succeeded_any
    succeeded << day
  else
    failed_all << day
  end
  failed_any.concat(failed_day)

  puts s

  s
}

# If multistd, there may have been errors,
# so show the table again just so it's all there.
if multistd
  puts header
  puts lines.compact
end

puts "Succeeded: #{succeeded}"
puts "Failed any: #{failed_any}"
puts "Failed all: #{failed_all}"
exit(1) unless failed_all.empty?
