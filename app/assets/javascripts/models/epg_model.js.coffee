$$.module 'Model', (self) ->
  class self.EpgModel extends Backbone.RelationalModel
    @today: -> moment().subtract(4,'hours')
    
    @format_date: (date) ->
      date ||= @today()
      moment(date).format('YYYY-MM-DD')
    
    start_time: -> moment @get('start_time')
    stop_time: -> moment @get('stop_time')
    
    airtime: ->
      (@stop_time() - @start_time()) / 60 / 1000
    
    timeValue: ->
      t = @start_time()
      hour = t.hour()
      hour += 24 if hour < 4
      hour*60 + t.minutes()
  
  class self.EpgCollection extends Backbone.Collection
    model: self.EpgModel
    
    url: -> $$.Router.path('epgs','list', date: @options.date).suffix('json')
    
    initialize: (models, options) ->
      @options = options || {}
      @options.date = moment(@options.date)
      super(models, @options)
