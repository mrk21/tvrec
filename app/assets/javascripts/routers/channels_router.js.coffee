$$.module 'Router', (self) ->
  self.on 'before:start', ->
    $$.routers.channels = new self.ChannelsRouter
  
  class self.ChannelsRouter extends self.ApplicationRouter
    routes:
      'channels': 'index'
      'channels/:id': 'show'
    
    index: ->
      channels = new $$.Model.ChannelCollection()
      channels.fetch().done ->
        $$.contentRegion.show(new $$.View.Channels.ListView(collection: channels))
    
    show: ->
      
