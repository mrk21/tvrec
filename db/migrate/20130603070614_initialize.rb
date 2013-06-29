class Initialize < ActiveRecord::Migration
  def change
    create_table :videos do |t|
      t.column :title         , :string  , :nil=>false
      t.column :description   , :text    
      t.column :channel_id    , :integer , :nil=>false
      t.column :start_time    , :datetime, :nil=>false
      t.column :stop_time     , :datetime, :nil=>false
      t.column :path          , :string  
      t.column :status        , :integer , :nil=>false, :default=>0
      t.column :job_id        , :integer
    end
    
    create_table :channels do |t|
      t.column :name       , :string
      t.column :physical_no, :integer, :nil=>false
      t.column :logical_no , :integer
    end
    
    create_table :epgs do |t|
      t.column :channel_id , :integer , :nil=>false
      t.column :title      , :string  , :nil=>false
      t.column :description, :text
      t.column :start_time , :datetime, :nil=>false
      t.column :stop_time  , :datetime, :nil=>false
      t.column :video_id   , :integer
    end
  end
end
