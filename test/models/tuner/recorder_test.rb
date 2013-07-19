require 'test_helper'

class Tuner::RecorderTest < ActiveSupport::TestCase
  if ENV['USE_RECPT1'].to_i == 1 then
    Tuner::Recorder
    
    test "録画上限に達してても、録画開始まで待つこと" do
      sleep 1
      results = []
      threads = []
      threads.push Thread.new{ results.push Tuner::Recorder.record(16, 2, nil, 10) }
      threads.push Thread.new{ results.push Tuner::Recorder.record(25, 2, nil, 10) }
      threads.push Thread.new{ results.push Tuner::Recorder.record(22, 4, nil, 10) }
      threads.push Thread.new{ results.push Tuner::Recorder.record(26, 4, nil, 10) }
      threads.each{|t| t.join}
      results.each do |r|
        assert_equal r[1].exitstatus, 0
      end
    end
    
    test "録画上限に達してても、録画開始まで待つがタイムアウト数上限になると失敗すること" do
      sleep 1
      t1_result = []
      t2_result = []
      t3_result = []
      threads = []
      threads.push Thread.new{ t1_result = Tuner::Recorder.record(16, 12, nil, 10) }
      threads.push Thread.new{ t2_result = Tuner::Recorder.record(22, 12, nil, 10) }
      sleep 1
      threads.push Thread.new{ t3_result = Tuner::Recorder.record(25, 14, nil, 10) }
      threads.each{|t| t.join}
      assert_equal t1_result[1].exitstatus, 0
      assert_equal t2_result[1].exitstatus, 0
      assert_equal t3_result[1].exitstatus, 4
    end
  end
end
