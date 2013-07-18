class FileManager
  INVALID_CHARS_FROM = '!:;/\\|,*?<>()$~\'"[]#♯&%'.split(//)
  INVALID_CHARS_TO = '！：；／＼｜，＊？＜＞（）＄〜’”［］＃＃＆％'.split(//)
  
  def self.to_filename(name)
    name = NKF.nkf('-m0Z1 -w', name.to_s).strip
    name = name.gsub(/^\.\.$/,'‥')
    name = name.gsub(/^\.$/,'・')
    name = name.gsub(/\s+/,'_')
    
    INVALID_CHARS_FROM.each_with_index do |c,i|
      name = name.gsub(c, INVALID_CHARS_TO[i])
    end
    name
  end
end
