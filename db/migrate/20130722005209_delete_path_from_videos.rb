class DeletePathFromVideos < ActiveRecord::Migration
  def change
    remove_column :videos, :path
  end
end
