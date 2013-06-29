$$.module 'Model.Widget', (self) ->
  class self.NavModel extends Backbone.Model
    defaults: ->
      uri: $$.Router.path('root')
    
    uri: ->
      URI(@get('uri'))
    
    isCurrent: (uri) ->
      myUri = (@uri()+'/').replace('//','/')
      uri = (uri+'/').replace('//','/')
      URI.commonPath(myUri, uri) == uri
  
  class self.NavCollection extends Backbone.Collection
    model: self.NavModel
