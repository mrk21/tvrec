#= require ./video_model

$$.module 'Model', (self) ->
  class ReservationModel extends Backbone.RelationalModel
    url: -> $$.Router.path('epgs','reservation', id: @id).suffix('json')
    
    create: ->
      @isNew = -> true
      @save()
    
    destroy: ->
      @isNew = -> false
      super()
  
  class self.EpgModel extends Backbone.RelationalModel
    @today: -> moment().subtract(4,'hours')
    @format_date: (date) -> moment(date || @today()).format('YYYY-MM-DD')
    
    initialize: -> @reservation = new ReservationModel(id: @id)
    startTime: -> moment @get('start_time')
    stopTime: -> moment @get('stop_time')
    airtime: -> (@stopTime() - @startTime()) / 60 / 1000
    
    timeValue: ->
      t = @startTime()
      hour = t.hour()
      hour += 24 if hour < 4
      hour*60 + t.minutes()
    
    reserve: (start_offset=0, stop_offset=0) ->
      @reservation.set(start_offset: start_offset, stop_offset: stop_offset)
      @reservation.create().done (d) => @set(d)
    
    unreserve: ->
      @reservation.destroy().done (d) => @set(d)
    
    isBroadcast: -> moment().isAfter(@startTime()) && moment().isBefore(@stopTime())
    isReserved: -> _.isNumber(@get('video_id'))
    canReserve: -> !@isReserved() && moment().isBefore(@stopTime())
    canUnreserve: -> @isReserved() && moment().isBefore(@startTime())
  
  class self.EpgCollection extends Backbone.Collection
    model: self.EpgModel
    
    url: -> $$.Router.path('epgs','list', date: @options.date).suffix('json')
    
    initialize: (models, options) ->
      @options = options || {}
      @options.date = moment(@options.date)
      super(models, @options)
