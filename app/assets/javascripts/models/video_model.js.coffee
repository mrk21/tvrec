$$.module 'Model', (self) ->
  class self.VideoModel extends Backbone.RelationalModel
    url: -> $$.Router.path('videos','show', id: @id).suffix('json')
  
  class self.VideoCollection extends Backbone.Collection
    model: self.VideoModel
    url: -> $$.Router.path('videos','index').suffix('json')
