#= require_self
#= require_tree ./widget
#= require_tree .

$$.module 'View', (self) ->
  self.on 'before:start', ->
    $$.appView = new self.ApplicationView
  
  class self.ApplicationView extends Backbone.View
    el: 'body'
    
    events:
      'click a:not([href^=#]):not([data-fetch=false])': 'onPageChange'
    
    onPageChange: (event) =>
      return unless Backbone.History.started
      event.preventDefault()
      url = $(event.currentTarget).attr('href')
      $$.router.navigate(url, {trigger: true})
      false
