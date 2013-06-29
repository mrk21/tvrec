$$.module 'View.Widget', (self) ->
  class self.ListItemView extends Backbone.Marionette.ItemView
    templateHelpers: $$.Helper.use()
    tagName: 'li'
    
  class self.ListView extends Backbone.Marionette.CollectionView
    itemView: self.ListItemView
    tagName: 'ul'
