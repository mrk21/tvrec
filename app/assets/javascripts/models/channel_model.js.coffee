$$.module 'Model', (self) ->
  class self.ChannelModel extends Backbone.RelationalModel
    url: -> $$.Router.path('channels','show', id: @id).suffix('json')
    relations: [
      type: 'HasMany'
      key: 'epgs'
      relatedModel: '$$.Model.EpgModel'
      collectionType: '$$.Model.EpgCollection'
      includeInJSON: false
    ]
  
  class self.ChannelCollection extends Backbone.Collection
    model: self.ChannelModel
    url: -> $$.Router.path('channels','index').suffix('json')
