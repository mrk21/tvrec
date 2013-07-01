require 'test_helper'

class Tuner::RecorderTest < ActiveSupport::TestCase
=begin
  Tuner::Recorder
  test "録画上限に達してても、録画開始まで待つこと" do
    sleep 1
    results = []
    threads = []
    threads.push Thread.new{ results.push Tuner::Recorder.record(16, 2) }
    threads.push Thread.new{ results.push Tuner::Recorder.record(25, 2) }
    threads.push Thread.new{ results.push Tuner::Recorder.record(22, 4) }
    threads.push Thread.new{ results.push Tuner::Recorder.record(26, 4) }
    threads.each{|t| t.join}
    results.each do |r|
      assert_equal r[1].to_i, 0
    end
  end
  
  test "録画上限に達してても、録画開始まで待つがタイムアウト数上限になると失敗すること" do
    sleep 1
    t1_result = []
    t2_result = []
    t3_result = []
    threads = []
    threads.push Thread.new{ t1_result = Tuner::Recorder.record(16, 12) }
    threads.push Thread.new{ t2_result = Tuner::Recorder.record(22, 12) }
    sleep 1
    threads.push Thread.new{ t3_result = Tuner::Recorder.record(25, 14) }
    threads.each{|t| t.join}
    assert_equal t1_result[1].to_i, 0
    assert_equal t2_result[1].to_i, 0
    assert_equal t3_result[1].to_i, 256
  end
=end
end
