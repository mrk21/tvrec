$$.module 'View.Epgs', (self) ->
  class self.TableItemView extends Backbone.Marionette.ItemView
    template: JST['templates/epgs/table_item_view']
    templateHelpers: $$.Helper.use('epgs')
    serializeData: -> model: @model
    tagName: 'td'
    className: 'epgs'
    id: -> "epgs_#{@model.id}"
    attributes: -> rowspan: @templateHelpers.getRowSpan(@model)
  
  class self.TableView extends Backbone.Marionette.CompositeView
    template: JST['templates/epgs/table_view']
    templateHelpers: $$.Helper.use('epgs')
    itemView: self.TableItemView
    tagName: 'table'
    className: 'epgs'
    
    serializeData: ->
      collection: @options.collection
      channels: @options.channels
    
    appendHtml: (collectionView, itemView, index) ->
      collectionView.$('#'+ itemView.model.cid).replaceWith(itemView.el)
