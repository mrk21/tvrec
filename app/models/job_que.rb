class JobQue
  class Item
    include ActiveModel::Model
    attr_accessor :id, :user, :status, :datetime
    
    def remove
      JobQue.remove self.id
    end
  end
  
  def self.get(id=nil)
    responce = `atq`
    list = responce.split(/\n/).map do |row|
      data = row.split(/\s+/)
      Item.new(
        id: data.shift.to_i,
        user: data.pop,
        status: data.pop,
        datetime: Time.zone.parse(data.join ' ')
      )
    end
    return list if id.nil?
    list.find{|v| v.id == id}
  end
  
  def self.remove(id)
    `atrm #{id}`
  end
  
  def self.push(datetime, code)
    code = Shellwords.escape code
    shell = "#{Rails.root}/bin/rails r #{code} &>> #{Rails.root}/log/job_que.log"
    if datetime <= Time.zone.now then
      spawn shell
      return -1
    end
    
    at = Shellwords.escape datetime.strftime('%H:%M %d.%m.%y')
    shell = Shellwords.escape shell
    responce = `echo #{shell} | at #{at} 2>&1`
    matches = responce.match(/job (\d+)/)
    return nil if matches.nil?
    matches[1].to_i
  end
end
