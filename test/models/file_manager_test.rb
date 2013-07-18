require 'test_helper'

class FileManagerTest < ActiveSupport::TestCase
  test "ファイル名に適したものに変換できること" do
    targets = {
      '!:;/\|,*?<>()$~\'"[]#♯&%' => '！：；／＼｜，＊？＜＞（）＄〜’”［］＃＃＆％',
      'ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ' => 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ',
      '０１２３４５６７８９' => '0123456789',
      
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
      
      'a b   c　　　d
      
      e	f	　 	g' => 'a_b_c_d_e_f_g',
    }
    targets.each do |from,to|
      converted = FileManager.to_filename(from)
      assert_equal(converted, to)
    end
  end
end
