require 'test_helper'

class FileManagerTest < ActiveSupport::TestCase
  test "ファイル名に適したものに変換できること" do
    targets = {
      ':;/\|,*?"<>()$~' => '：；／＼｜，＊？”＜＞（）＄〜',
      'ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ' => 'ABCDEFGHIJKLMNOPQRSTUVWXYZ',
      '０１２３４５６７８９' => '0123456789',
      
      '【】' => '【】',
      '［］' => '[]',
      '（）' => '（）',
      '()' => '（）',
      '＃' => '#',
      
      '.' => '・',
      '..' => '‥',
      '...' => '...',
      
      'a.' => 'a.',
      'a..' => 'a..',
      'a...' => 'a...',
      
      'a.b' => 'a.b',
      'a..b' => 'a..b',
      'a...b' => 'a...b',
      
      '.a' => '.a',
      '..a' => '..a',
      '...a' => '...a',
      
      ' . ' => '・',
      ' ..　 ' => '‥',
      ' 　... ' => '...',
    }
    targets.each do |from,to|
      converted = FileManager.to_filename(from)
      assert_equal(converted, to)
    end
  end
end
