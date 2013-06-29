namespace :tvrec do
  desc 'Init'
  task :init => ['db:drop','db:create','db:migrate'] do
    Tuner.search_channel
  end
  
  desc 'Update EPG'
  task :update_epg => :environment do
    Tuner.update_epg
  end
end
