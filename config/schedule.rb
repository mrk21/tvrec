set :environment, ENV['RAILS_ENV'] || :development
set :job_template, ". /etc/environment && /bin/bash -l -c ':job'"
set :output, 'log/crontab.log'

every :day, at: '03:30', :roles => [:app] do
  rake 'tvrec:update_epg'
end
