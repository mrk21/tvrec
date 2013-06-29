class Channel < ActiveRecord::Base
  has_many :epgs, dependent: :destroy
  
  scope :ordered, -> { order :logical_no, :physical_no }
end
